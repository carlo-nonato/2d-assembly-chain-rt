#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <QGraphicsScene>

class ConveyorBelt;
class Robot;

/* This class represents the world and is responsible of everyhing that the
 * controller can interact with such as the conveyor belt, the robots and the
 * camera.
 */
class Simulation : public QGraphicsScene
{
    Q_OBJECT

public:
    Simulation();

    ConveyorBelt *conveyorBelt() { return m_conveyorBelt; }
    Robot *anomalyRobot() { return m_anomalyRobot; }
    Robot *stackingRobot() { return m_stackingRobot; }
    
    /*
     * Return the minimum calculable area with an epsilon error
    */
    int minItemArea(double epsilon = 0) { return (m_minItemArea - m_minItemArea * epsilon); }
    static void rotateAroundCenter(QGraphicsItem *item, qreal angle);

public slots:
    QImage* frameFromCamera(int x, int y, int width, int height);

private:
    ConveyorBelt *m_conveyorBelt;
    Robot *m_anomalyRobot;
    Robot *m_stackingRobot;
    int m_minItemArea = 3600;    

private slots:
    void createItem(int minWidth = 60, int minHeight = 60, int maxWidth = 90, int maxHeight = 90);
    QImage* _frameFromCamera(int x, int y, int width, int height);
};

#endif // SIMULATION_HPP
