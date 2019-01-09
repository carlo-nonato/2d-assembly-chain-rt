#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class Simulation;
class QLabel;

class MainWindow : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void changeCameraFrame();

private:
    Simulation *m_simulation;
    QLabel *m_camera;
};

#endif // MAINWINDOW_HPP
