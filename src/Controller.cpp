#include "Controller.hpp"

#include "ConveyorBelt.hpp"
#include "cvutils.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"

void millisleep(int ms)
{
	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = ms*1000000;
	nanosleep(&t, NULL);
}

Controller::Controller(Simulation *simulation) : m_simulation(simulation) {}

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
    pthread_create(&thread, &attr, &stackingThreadHelper, this);

    pthread_attr_destroy(&attr);
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

bool Controller::isIgnorable(std::vector<cv::Point> contour,
                             double maxGrabY) const {
    cv::Rect boundingRect = cv::boundingRect(contour);

    if (boundingRect.y > maxGrabY)
        return true;
    
    std::vector<cv::Point> approx;
    cv::approxPolyDP(contour, approx, cv::arcLength(contour, true)*0.012, true);
    double area = std::fabs(cv::contourArea(contour));
    if (area < m_simulation->minItemArea()*0.96
            || boundingRect.y == 0
            || (cv::isContourConvex(approx) && boundingRect.x == 0)) {
        return true;
    }
    return false;
}

void Controller::updateFrameThread() {
    QRectF anomalySBR = m_simulation->anomalyRobot()->sceneBoundingRect();
    QRectF stackingSBR = m_simulation->stackingRobot()->sceneBoundingRect();
    double anomalyGrabY = anomalySBR.center().y();
    double stackingGrabY = stackingSBR.center().y();
    double frameSize = m_simulation->conveyorBelt()->usableWidth();

    while (true) {
        QImage *frame = m_simulation->frameFromCamera();

        sem_wait(&m_anomalySem);

        m_anomalyFrame = frame->copy(0, anomalyGrabY - frameSize/2, frameSize,
                                     frameSize);

        sem_post(&m_anomalySem);
        sem_post(&m_anomalySynch);
        emit anomalyFrameUpdated();

        sem_wait(&m_stackingSem);
        
        m_stackingFrame = frame->copy(0, stackingGrabY - frameSize/2, frameSize,
                                      frameSize);
        
        sem_post(&m_stackingSem);
        sem_post(&m_stackingSynch);
        emit stackingFrameUpdated();

        delete frame;

        millisleep(100);
    }
}

void Controller::anomalyThread() {
    Shape shape = Shape::None;
    double grabY = m_simulation->conveyorBelt()->usableWidth()/2;
    double maxGrabY =
        grabY + m_simulation->anomalyRobot()->armSize().height()/2;

    while (true) {
        sem_wait(&m_anomalySynch);
        sem_wait(&m_anomalySem);
        
        cv::Mat src = QImage2Mat(m_anomalyFrame);
        
        sem_post(&m_anomalySem);
        
        preprocess(src);
        
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(src, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < (int) contours.size(); i++) {
            cv::Rect boundingRect = cv::boundingRect(contours[i]);

            if (isIgnorable(contours[i], maxGrabY)) {
                shape = Shape::None;
                continue;
            }
            
            if (shape == Shape::None) {
                if ((shape = shapeDetection(contours[i])) == Shape::None)
                    continue;
            }
         
            if ((shape != Shape::Rect && shape != Shape::Square)
                    && boundingRect.y + boundingRect.height/2 >= grabY) {
                m_simulation->anomalyRobot()->grab();
                m_simulation->anomalyRobot()->rotateToEnd();
                m_simulation->anomalyRobot()->release();
                m_simulation->anomalyRobot()->rotateToStart();
                shape = Shape::None;
            }
        }
    }
}

void Controller::stackingThread() {
    bool calc = false;
    double grabY = m_simulation->conveyorBelt()->usableWidth()/2;
    double maxGrabY =
        grabY + m_simulation->stackingRobot()->armSize().height() / 2;

    while (true) {
        sem_wait(&m_stackingSynch);
        sem_wait(&m_stackingSem);
        
        cv::Mat src = QImage2Mat(m_stackingFrame);

        sem_post(&m_stackingSem);

        preprocess(src);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(src, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < (int) contours.size(); i++) {
            cv::Rect boundingRect = cv::boundingRect(contours[i]);

            if (isIgnorable(contours[i], maxGrabY)) {
                calc = false;
                continue;
            }

            if (!calc) {
                cv::RotatedRect rot = cv::minAreaRect(contours[i]);
                calc = true;
                m_simulation->stackingRobot()->scheduleItemRotation(rot.angle);
                m_simulation->stackingRobot()->grab();
                m_simulation->stackingRobot()->rotateToEnd();
            }

            if (calc && boundingRect.y + boundingRect.height/2 >= grabY
                    && (int) contours.size() == 1 && boundingRect.x == 0) {
                m_simulation->stackingRobot()->release();
                m_simulation->stackingRobot()->rotateToStart();
                break;
            }
        }
    }
}