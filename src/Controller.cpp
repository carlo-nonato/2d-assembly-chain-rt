#include "Controller.hpp"

#include "Robot.hpp"
#include "Simulation.hpp"
#include "CVUtils.hpp"

#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>

#include <QDebug>

void millisleep(int ms)
{
	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = ms * 1000000;
	nanosleep(&t, NULL);
}

Controller::Controller(Simulation *simulation) : m_simulation(simulation) {
    m_frame = nullptr;
}

void Controller::start() {
    pthread_t thread;
    pthread_attr_t attr;

    sem_init(&m_anomalySem, 0, 1);
    sem_init(&m_stackingSem, 0, 1);
    sem_init(&m_anomalySynch, 0, 0);
    sem_init(&m_stackingSynch, 0, 0);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, &updateFrameThreadHelper, this);
    pthread_create(&thread, &attr, &anomalyThreadHelper, this);
    // pthread_create(&thread, &attr, &stackingThreadHelper, this);

    pthread_attr_destroy(&attr);
}

void Controller::updateFrameThread() {
    while (true) {
        sem_wait(&m_anomalySem);
        sem_wait(&m_stackingSem);

        delete m_frame;
        m_frame = m_simulation->frameFromCamera(10, 0, 280, 600);

        sem_post(&m_anomalySem);
        sem_post(&m_stackingSem);

        sem_post(&m_anomalySynch);
        sem_post(&m_stackingSynch);
        millisleep(100);
    }
}

void Controller::anomalyThread() {
    // TODO: should not be literal
    int grabPointY = 140;

    while (true) {
        sem_wait(&m_anomalySynch);
        sem_wait(&m_anomalySem);
        // TODO: should not be literal
        QImage frame = m_frame->copy(0, 90, 280, 280);
        sem_post(&m_anomalySem);

        cv::Mat src = QImage2Mat(frame);
        std::vector<std::vector<cv::Point>> contours;

        preprocess(src);
        cv::findContours(src, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < (int) contours.size(); i++) {
            cv::Rect boundingRect = cv::boundingRect(contours[i]);
            
            // TODO: min area should not be literal
            if (std::fabs(cv::contourArea(contours[i])) < 2800
                    || boundingRect.y == 0 || boundingRect.y >= grabPointY)
                continue;
            
            Shape shape = shapeDetection(contours[i]);
            // TODO: check for some kind of anomaly
            if ((shape == Shape::Ellipse || shape == Shape::Circle)
                    && boundingRect.y + boundingRect.height/2 >= grabPointY) {
                m_simulation->anomalyRobot()->grab();
                m_simulation->anomalyRobot()->rotateToEnd();
                m_simulation->anomalyRobot()->release();
                m_simulation->anomalyRobot()->rotateToStart();
            }
        }
    }
}

void Controller::stackingThread() {
    while (true) {
        m_simulation->stackingRobot()->grab();
        m_simulation->stackingRobot()->rotateToEnd();
        m_simulation->stackingRobot()->release();
        m_simulation->stackingRobot()->rotateToStart();
    }
}

void* Controller::anomalyThreadHelper(void *arg) {
    ((Controller *) arg)->anomalyThread();
    return nullptr;
}

void* Controller::stackingThreadHelper(void *arg) {
    ((Controller *) arg)->stackingThread();
    return nullptr;
}

void* Controller::updateFrameThreadHelper(void *arg) {
    ((Controller *) arg)->updateFrameThread();
    return nullptr;
}