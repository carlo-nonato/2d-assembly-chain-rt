#include "Controller.hpp"
#include "Robot.hpp"

#include <unistd.h>
#include <time.h>
#include <cstdlib>
#include <iostream>


/*Controller::Controller(MainWindow *mw)
{
    m_mw = mw;

    m_simulation = mw->getSim();    

    sem_init(&m_mutex, 0, 1);
    
    sem_init(&m_sem_camera, 0, 1);
    sem_init(&m_itemspawner, 0, 0);
}*/


Controller::Controller(Simulation *simulation)
{
    m_simulation = simulation;    

    sem_init(&m_mutex, 0, 1);
    
    sem_init(&m_sem_camera, 0, 1);
    sem_init(&m_itemspawner, 0, 0);
}

void Controller::start() {
  

    pthread_attr_init(&m_attr);
  
    pthread_create(&m_itemspwaner_thread, &m_attr, &itemSpawnerThread_helper, this);
    //pthread_create(&m_anomaly_thread, &m_attr, &anomalyThread_helper, this);
    //pthread_create(&m_stacking_thread, &m_attr, &stackingThread_helper, this);
    
    pthread_attr_destroy(&m_attr);

    //pthread_join(m_anomaly_thread, nullptr);
    //pthread_join(m_stacking_thread, nullptr);
}

void Controller::millisleep(int ms)
{
	struct timespec t;
	t.tv_sec = 0;
	t.tv_nsec = ms * 1000000;
	nanosleep(&t, NULL);
}


//COMMONS
QImage Controller::getFrameFromCamera(int left, int top, Controller *c, int robot) {    
    std::cout << "Robot " << robot << ": trying to get frame\n";
    sem_wait(&c->m_sem_camera);    
    
    std::cout << "Robot " << robot << ": camera access granted\n";

    QImage img = m_simulation->frameFromCamera(left, top);    


    std::cout << "Robot " << robot << ": frame aquired, releasing camera\n";

    sem_post(&c->m_sem_camera);        

    return img;
}

 bool Controller::isItemInPos(QImage image, int robot, bool catching) {
    //TODO RECOG FOR OBJECT
    if (robot == 0 && catching == false) {
        QColor color(image.pixel(image.size().width() / 2, image.size().height() / 2));
        if (color.red() > 200)
            return true;
    }
    else if (robot == 0 && catching) {
        QColor color(image.pixel(image.size().width() / 2 + 10, image.size().height() - 4));
        QColor color2(image.pixel(image.size().width() / 2 + 10, image.size().height() - 35));
        if (color.red() > 200 && color2.red() < 100)
            return true;
    }
    else if (robot == 1) {         
        QColor color(image.pixel(image.size().width() / 2 - 10, image.size().height() - 4));
        QColor color2(image.pixel(image.size().width() / 2 - 10, image.size().height() - 20));
        if (color.red() > 200 && color2.red() < 100)        
            return true;
    }

     return true;
 }



//ITEM SPAWNER
void Controller::spawnItem(Controller *c) {
    std::cout << "Item spwaner: trying to spawn item\n";
    sem_post(&m_itemspawner);
    sem_wait(&m_itemspawner);
    std::cout << "Item spwaner: anomaly robot is free, spawning item\n";    

    m_simulation->createItem();
    millisleep(500);
    m_simulation->anomalyRobot()->rotateToEnd();

    std::cout << "Item spwaner: item spwaned\n";
}



//ANOMALY ROBOT
bool Controller::doRecog(Controller *c) {
    std::cout << "Robot 0: wait for item\n";
    sem_post(&c->m_itemspawner);

    int robot = 0;

    QImage img = getFrameFromCamera(10, 30, c, robot);
    while (!isItemInPos(img, robot, false)) {
        img = getFrameFromCamera(10, 30, c, robot);
        millisleep(100); 
    }    

    std::cout << "Robot 0: item in position, scanning for anomalies\n";

    //do Recog stuff

    std::cout << "Robot 0: scanning completed\n";

    return true;
}

void Controller::grabAndTrash(Controller *c) {
    std::cout << "Robot 0: wait for grabbing\n";
    int robot = 0;
    QImage img = getFrameFromCamera(10, 30, c, robot);
    while (!isItemInPos(img, robot, true)) {
        img = getFrameFromCamera(10, 30, c, robot);
        millisleep(100); 
    }  
    std::cout << "Robot 0: item in position, start grabbing\n";

    c->m_simulation->anomalyRobot()->grab();
    c->m_simulation->anomalyRobot()->rotateToEnd();
    c->m_simulation->anomalyRobot()->release();
    c->m_simulation->anomalyRobot()->rotateToStart();
    std::cout << "Robot 0: returned to start\n";
}




//STACKING ROBOT
void Controller::mountItem(Controller *c) {
    std::cout << "Robot 1: wait for mounting\n";
    int robot = 1;
    QImage img = getFrameFromCamera(10, 230, c, robot);
    while (!isItemInPos(img, robot, true)) {
        img = getFrameFromCamera(10, 230, c, robot);
        millisleep(100); 
    }  
    std::cout << "Robot 1: item in position, start mounting\n";

    c->m_simulation->stackingRobot()->release();
    c->m_simulation->stackingRobot()->rotateToStart();
    c->m_simulation->stackingRobot()->grab();
    c->m_simulation->stackingRobot()->rotateToEnd();
}


//THREADS

void *Controller::itemSpawnerThread(void *arg) {
    Controller *context = (Controller *)arg;    
    int i = 0;
    while (i < 3) {        
        spawnItem(context);
        i++;
    }
    pthread_exit(0);
}

void *Controller::anomalyThread(void *arg) {    
    Controller *context = (Controller *)arg;
    while (true) {        
        if (doRecog(context))
            grabAndTrash(context);                
    }
    pthread_exit(0);
}

void *Controller::stackingThread(void *arg) {
    Controller *context = (Controller *)arg;
    while (true) {        
        mountItem(context);
    }
    pthread_exit(0);
}

//avoid segmentation fault by passing context
void *Controller::anomalyThread_helper(void *context) {
    std::cout << "Sono qui anomaly helper\n";
    return ((Controller *)context)->anomalyThread(context);
}

void *Controller::stackingThread_helper(void *context) {
    return ((Controller *)context)->stackingThread(context);
}

void *Controller::itemSpawnerThread_helper(void *context) {
    return ((Controller *)context)->itemSpawnerThread(context);
}