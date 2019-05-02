#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class Simulation;
class Controller;
class QLabel;

/** Main window. It shows the simulation and the points of view of the two
 *  robots. It gives the ability to the user to start the controller. */ 
class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void updateAnomalyCamera(); 
    void updateStackingCamera();

private:
    Simulation *m_simulation;
    Controller *m_controller;
    QLabel *m_anomalyCamera;
    QLabel *m_stackingCamera;
};

#endif // MAINWINDOW_HPP