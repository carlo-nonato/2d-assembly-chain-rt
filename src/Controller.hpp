#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "MainWindow.hpp"
#include "Simulation.hpp"

#include <QImage>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>



class Controller
{

public:      

    //Controller(MainWindow *mw);

    Controller(Simulation *simulation);

    void start();
    void *itemSpawnerThread(void *arg);
    void *anomalyThread(void *arg); //doRecogThread    
    void *stackingThread(void *arg);

    //avoid segmentation fault by passing context
    static void *anomalyThread_helper(void *context);
    static void *stackingThread_helper(void *context);
    static void *itemSpawnerThread_helper(void *context);

    //item spawner
    void spawnItem(Controller *context);

    //anomaly robot actions
    bool doRecog(Controller *context);
    void grabAndTrash(Controller *context);

    //stacking robot actions
    void mountItem(Controller *context);

    //common actions
    QImage getFrameFromCamera(int left, int top, Controller *c, int robot);
    bool isItemInPos(QImage image, int robot, bool catching);

private:
    MainWindow *m_mw;

    Simulation *m_simulation;

    sem_t m_mutex;

    bool anomaly_robot_busy; //anomaly robot busy or free

    sem_t m_sem_camera; //camera busy or free
    sem_t m_itemspawner; 

    //sem_t m_aRobot_item_in_pos; // if item under anomalyR, do recog
    //sem_t m_sRobot_item_in_pos; // if item under stackingR, stack item

    pthread_attr_t m_attr;    
    pthread_t m_itemspwaner_thread;
    pthread_t m_anomaly_thread;
    pthread_t m_stacking_thread;
//    pthread_t m_camera_thread[2]; //check every x ms if item in pos for anomalyR or stackingR

    void millisleep(int ms);
};

#endif // CONTROLLER_HPP
