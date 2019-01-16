#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <QGraphicsScene>

class ConveyorBelt;
class Robot;

class Simulation : public QGraphicsScene 
{
    Q_OBJECT

public:
    Simulation();

    ConveyorBelt * conveyorBelt() const { return m_conveyorBelt; }
    Robot * anomalyRobot() const { return m_anomalyRobot; }
    Robot * stackingRobot() const { return m_stackingRobot; }
    QImage frameFromCamera();
    void start();

public slots:
    void createItem();

private:
    ConveyorBelt *m_conveyorBelt;
    Robot *m_anomalyRobot;
    Robot *m_stackingRobot;
};

#endif // SIMULATION_HPP