#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <QGraphicsScene>

#include <random>

class ConveyorBelt;
class Robot;

/* The Simulation class is responsible of everyhing that the Controller can
   interact with. */
class Simulation : public QGraphicsScene
{
    Q_OBJECT

public:
    Simulation();

    ConveyorBelt *conveyorBelt() { return m_conveyorBelt; }
    Robot *anomalyRobot() { return m_anomalyRobot; }
    Robot *stackingRobot() { return m_stackingRobot; }
    QImage frameFromCamera(int x, int y, int width, int height);

private:
    ConveyorBelt *m_conveyorBelt;
    Robot *m_anomalyRobot;
    Robot *m_stackingRobot;

    std::random_device dev;
    std::mt19937 rng; // the Mersenne Twister

private slots:
    void createItem();
};

#endif // SIMULATION_HPP
