#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

// #include <semaphore.h>

class Simulation;

/* The Controller class is responsible for driving the two robots in response
   of the camera inputs. It uses different tasks for object recognition,
   anomaly detection and for sending commands to the robots simultaneously. */
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(Simulation *simulation);

    void anomalyThread();
    void stackingThread();

    static void *anomalyThreadHelper(void *arg);
    static void *stackingThreadHelper(void *arg);

    //anomaly robot actions
    // bool doRecog();
    // void grabAndTrash();

    // //stacking robot actions
    // void mountItem(Controller *context);

    // //common actions
    // QImage getFrameFromCamera(int x, int y, int width, int height, int robot);
    // bool isItemInPos(QImage image, int robot, bool catching);

public slots:
    void start();

private:
    Simulation *m_simulation;

    // sem_t m_sem_camera; //camera busy or free
    
    // sem_t m_aRobot_item_in_pos; // if item under anomalyR, do recog
    // sem_t m_sRobot_item_in_pos; // if item under stackingR, stack item

    // void millisleep(int ms);
};

#endif // CONTROLLER_HPP
