#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class Simulation;
class Controller;
class QLabel;

class MainWindow : public QMainWindow 
{
    Q_OBJECT

signals:
    void windowShown();
    void windowLoaded();

public:
    MainWindow();
    void show();
    
    void showEvent(QShowEvent* event);

    Simulation *getSim() const {return m_simulation;}

public slots:
    void changeCameraFrame();
    void sigReceived();

private:    
    Simulation *m_simulation;
    Controller *m_controller;
    QLabel *m_camera;
    QLabel *m_camera2;

    bool firstTimeShown;

    
};

#endif // MAINWINDOW_HPP
