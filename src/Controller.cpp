#include "Controller.hpp"

#include "ConveyorBelt.hpp"
#include "Simulation.hpp"

#include <iostream>
// #include <cstdlib>
// #include <time.h>

Controller::Controller(Simulation *simulation) {
    m_simulation = simulation;

    /* Link commands to actual actions in the simulation.
       Needed because of Qt multi thread timers management. */
    connect(this, &Controller::createItem,
            m_simulation, &Simulation::createItem);

    // sem_init(&m_sem_camera, 0, 1);
    // sem_init(&m_itemspawner, 0, 0);
}

void Controller::start() {
    m_simulation->conveyorBelt()->turnOn();

    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread, &attr, &itemCreatorThreadHelper, this);
    // pthread_create(&thread, &attr, &anomalyThreadHelper, this);
    // pthread_create(&thread, &attr, &stackingThreadHelper, this);

    pthread_attr_destroy(&attr);
}

void Controller::itemCreatorThread() {
    // sem_wait(&m_itemspawner);

    emit createItem();
    // m_simulation->anomalyRobot()->rotateToEnd();
}

void Controller::anomalyThread() {
    // Controller *context = (Controller *)arg;
    // while (true) {
    //     if (doRecog(context))
    //         grabAndTrash(context);
    // }
}

void Controller::stackingThread()
{
    // Controller *context = (Controller *)arg;
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

//COMMONS
// QImage Controller::getFrameFromCamera(int left, int top, Controller *c, int robot) {
//     std::cout << "Robot " << robot << ": trying to get frame\n";
//     sem_wait(&c->m_sem_camera);

//     std::cout << "Robot " << robot << ": camera access granted\n";

//     QImage img = m_simulation->frameFromCamera(left, top);

//     std::cout << "Robot " << robot << ": frame aquired, releasing camera\n";

//     sem_post(&c->m_sem_camera);

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

//      return true;
//  }

//ANOMALY ROBOT
// bool Controller::doRecog(Controller *c) {
//     std::cout << "Robot 0: wait for item\n";
//     sem_post(&c->m_itemspawner);

//     int robot = 0;

//     QImage img = getFrameFromCamera(10, 30, c, robot);
//     while (!isItemInPos(img, robot, false)) {
//         img = getFrameFromCamera(10, 30, c, robot);
//         millisleep(100);
//     }

//     std::cout << "Robot 0: item in position, scanning for anomalies\n";

//     //do Recog stuff

//     std::cout << "Robot 0: scanning completed\n";

//     return true;
// }

// void Controller::grabAndTrash(Controller *c) {
//     std::cout << "Robot 0: wait for grabbing\n";
//     int robot = 0;
//     QImage img = getFrameFromCamera(10, 30, c, robot);
//     while (!isItemInPos(img, robot, true)) {
//         img = getFrameFromCamera(10, 30, c, robot);
//         millisleep(100);
//     }
//     std::cout << "Robot 0: item in position, start grabbing\n";

//     c->m_simulation->anomalyRobot()->grab();
//     c->m_simulation->anomalyRobot()->rotateToEnd();
//     c->m_simulation->anomalyRobot()->release();
//     c->m_simulation->anomalyRobot()->rotateToStart();
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
void *Controller::itemCreatorThreadHelper(void *arg) {
    ((Controller *)arg)->itemCreatorThread();
    return NULL;
}

void *Controller::anomalyThreadHelper(void *arg) {
    ((Controller *)arg)->anomalyThread();
    return NULL;
}

void *Controller::stackingThreadHelper(void *arg) {
    ((Controller *)arg)->stackingThread();
    return NULL;
}