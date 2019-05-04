#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QImage>
#include <QObject>

#include <opencv4/opencv2/imgproc/imgproc.hpp>

#include <semaphore.h>

class Simulation;

/**
 * The Controller class is responsible for driving the two robots in response
 * of the camera inputs. It uses different tasks for object recognition,
 * anomaly detection and for sending commands to the robots simultaneously. */
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(Simulation *simulation);

    QImage anomalyFrame() const { return m_anomalyFrame; }
    QImage stackingFrame() const { return m_stackingFrame; };

public slots:
    void start();

signals:
    void anomalyFrameUpdated();
    void stackingFrameUpdated();

private:
    static void* anomalyThreadHelper(void *arg);
    static void* stackingThreadHelper(void *arg);
    static void* updateFrameThreadHelper(void *arg);

    QImage m_anomalyFrame;
    QImage m_stackingFrame;
    Simulation *m_simulation;
    sem_t m_anomalySem;
    sem_t m_stackingSem;
    sem_t m_anomalySynch;
    sem_t m_stackingSynch;

    bool isIgnorable(std::vector<cv::Point> contour, double maxGrabY) const;

    void anomalyThread();
    void stackingThread();
    void updateFrameThread();
};

#endif // CONTROLLER_HPP