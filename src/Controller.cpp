#include "Controller.hpp"

#include "Robot.hpp"
#include "Simulation.hpp"

#include <QDebug>

// #include <iostream>

Controller::Controller(Simulation *simulation) {
    m_simulation = simulation;
    // sem_init(&m_sem_camera, 0, 1);
}

void Controller::start() {
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, &anomalyThreadHelper, this);
    pthread_create(&thread, &attr, &stackingThreadHelper, this);

    pthread_attr_destroy(&attr);
}

void Controller::anomalyThread() {
    m_simulation->anomalyRobot()->rotateToEnd();
    // while (true) {
    //     if (doRecog())
    //         grabAndTrash();
    //     millisleep(700);
    // }
}

void Controller::stackingThread() {
    m_simulation->stackingRobot()->rotateToEnd();
    // while (true) {
    //     mountItem(context);
    // }
}

// void Controller::millisleep(int ms)
// {
// 	struct timespec t;
// 	t.tv_sec = 0;
// 	t.tv_nsec = ms * 1000000;
// 	nanosleep(&t, NULL);
// }

// //COMMONS
// QImage Controller::getFrameFromCamera(int x, int y, int width, int height, int robot) {
//     std::cout << "Robot " << robot << ": trying to get frame\n";
//     sem_wait(&m_sem_camera);

//     std::cout << "Robot " << robot << ": camera access granted\n";

//     QImage img = m_simulation->frameFromCamera(x, y, width, height);

//     std::cout << "Robot " << robot << ": frame aquired, releasing camera\n";

//     sem_post(&m_sem_camera);

//     return img;
// }

// bool Controller::isItemInPos(QImage image, int robot, bool catching) {
//     //TODO RECOG FOR OBJECT
//     if (robot == 0 && catching == false) {
//         QColor color(image.pixel(image.size().width() / 2, image.size().height() / 2));
//         if (color.red() > 200)
//             return true;
//     }
//     else if (robot == 0 && catching) {
//         QColor color(image.pixel(image.size().width() / 2 + 10, image.size().height() - 4));
//         QColor color2(image.pixel(image.size().width() / 2 + 10, image.size().height() - 35));
//         if (color.red() > 200 && color2.red() < 100)
//             return true;
//     }
//     else if (robot == 1) {
//         QColor color(image.pixel(image.size().width() / 2 - 10, image.size().height() - 4));
//         QColor color2(image.pixel(image.size().width() / 2 - 10, image.size().height() - 20));
//         if (color.red() > 200 && color2.red() < 100)
//             return true;
//     }

//     return true;
//   }

//ANOMALY ROBOT
// bool Controller::doRecog() {
//     std::cout << "Robot 0: waiting for item\n";

//     int robot = 0;

//     QImage img = getFrameFromCamera(10, 30, 200, 250, robot);
//     while (!isItemInPos(img, robot, false)) {
//         img = getFrameFromCamera(10, 30, 200, 250, robot);
//         millisleep(100);
//     }

//     std::cout << "Robot 0: item in position, scanning for anomalies\n";

//     //do Recog stuff

//     std::cout << "Robot 0: scanning completed\n";

//     return true;
// }

// void Controller::grabAndTrash() {
//     std::cout << "Robot 0: wait for grabbing\n";
//     int robot = 0;
//     QImage img = getFrameFromCamera(10, 30, 200, 250, robot);
//     while (!isItemInPos(img, robot, true)) {
//         img = getFrameFromCamera(10, 30, 200, 250, robot);
//         millisleep(100);
//     }
//     std::cout << "Robot 0: item in position, start grabbing\n";

//     m_simulation->anomalyRobot()->grab();
//     m_simulation->anomalyRobot()->rotateToEnd();
//     m_simulation->anomalyRobot()->release();
//     m_simulation->anomalyRobot()->rotateToStart();
//     std::cout << "Robot 0: returned to start\n";
// }

// //STACKING ROBOT
// void Controller::mountItem(Controller *c) {
//     std::cout << "Robot 1: wait for mounting\n";
//     int robot = 1;
//     QImage img = getFrameFromCamera(10, 230, c, robot);
//     while (!isItemInPos(img, robot, true)) {
//         img = getFrameFromCamera(10, 230, c, robot);
//         millisleep(100);
//     }
//     std::cout << "Robot 1: item in position, start mounting\n";

//     c->m_simulation->stackingRobot()->release();
//     c->m_simulation->stackingRobot()->rotateToStart();
//     c->m_simulation->stackingRobot()->grab();
//     c->m_simulation->stackingRobot()->rotateToEnd();
// }

// Actual pthread threads. They need to be static methods.

void *Controller::anomalyThreadHelper(void *arg) {
    ((Controller *) arg)->anomalyThread();
    return NULL;
}

void *Controller::stackingThreadHelper(void *arg) {
    ((Controller *) arg)->stackingThread();
    return NULL;
}