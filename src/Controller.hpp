#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <semaphore.h>

class Simulation;
class QImage;

/**
 * The Controller class is responsible for driving the two robots in response
 * of the camera inputs. It uses different tasks for object recognition,
 * anomaly detection and for sending commands to the robots simultaneously.
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    /** Create a controller and attach it to a simulation. */ 
    Controller(Simulation *simulation);

    void anomalyThread();
    void stackingThread();
    void updateFrameThread();

    static void *anomalyThreadHelper(void *arg);
    static void *stackingThreadHelper(void *arg);
    static void *updateFrameThreadHelper(void *arg);

public slots:
    /** Starts the controller. */
    void start();

private:
    /** The simulation that this controller is attached to. */
    Simulation *m_simulation;
    QImage *m_frame;
    sem_t m_anomalySem;
    sem_t m_stackingSem;
    sem_t m_anomalySynch;
    sem_t m_stackingSynch;
};

#endif // CONTROLLER_HPP
