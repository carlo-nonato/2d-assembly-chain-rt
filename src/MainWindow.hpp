#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class Simulation;
class Controller;
class QLabel;

class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void updateCameras();

private:    
    Simulation *m_simulation;
    Controller *m_controller;
    QLabel *m_anomalyCamera;
    QLabel *m_stackingCamera;
};

#endif // MAINWINDOW_HPP
