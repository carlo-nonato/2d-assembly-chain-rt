#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

class Simulation;

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(Simulation *simulation);

    void itemCreatorThread();
    void anomalyThread();
    void stackingThread();

    static void *itemCreatorThreadHelper(void *arg);
    static void *anomalyThreadHelper(void *arg);
    static void *stackingThreadHelper(void *arg);

    //anomaly robot actions
    // bool doRecog(Controller *context);
    // void grabAndTrash(Controller *context);

    // //stacking robot actions
    // void mountItem(Controller *context);

    // //common actions
    // QImage getFrameFromCamera(int left, int top, Controller *c, int robot);
    // bool isItemInPos(QImage image, int robot, bool catching);

public slots:
    void start();

signals:
    void createItem();

private:
    Simulation *m_simulation;

    // sem_t m_sem_camera; //camera busy or free
    // sem_t m_itemspawner; 

    // sem_t m_aRobot_item_in_pos; // if item under anomalyR, do recog
    // sem_t m_sRobot_item_in_pos; // if item under stackingR, stack item

    // void millisleep(int ms);
};

#endif // CONTROLLER_HPP
