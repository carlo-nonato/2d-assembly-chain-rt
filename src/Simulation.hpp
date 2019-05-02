#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <QGraphicsScene>

class ConveyorBelt;
class Robot;

/** The Simulation class represents the world and is responsible of everyhing
 * that the controller can interact with such as the conveyor belt, the robots
 * and the camera. */
class Simulation : public QGraphicsScene
{
    Q_OBJECT

public:
    Simulation();

    ConveyorBelt* conveyorBelt() { return m_conveyorBelt; }
    Robot* anomalyRobot() { return m_anomalyRobot; }
    Robot* stackingRobot() { return m_stackingRobot; }
    
    void setItemSizeRange(int minWidth, int minHeight, int maxWidth,
                          int maxHeight);
    int minItemArea() const;

public slots:
    QImage* frameFromCamera();

private:
    int m_minItemWidth = 60;
    int m_minItemHeight = 60;
    int m_maxItemWidth = 90;
    int m_maxItemHeight = 90;
    ConveyorBelt *m_conveyorBelt;
    Robot *m_anomalyRobot;
    Robot *m_stackingRobot;

private slots:
    void createItem();
    QImage* _frameFromCamera();
};

#endif // SIMULATION_HPP