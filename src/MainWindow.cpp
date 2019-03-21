#include "MainWindow.hpp"

#include "ConveyorBelt.hpp"
#include "Simulation.hpp"
#include "Controller.hpp"
#include "Robot.hpp"

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QImage>
#include <QLabel>
#include <QTimer>
#include <QApplication>
#include <iostream>

void MainWindow::show()
{
   QMainWindow::show();
   QApplication::processEvents();
   emit windowShown();
   if (firstTimeShown == true)
   {
      emit windowLoaded();
      firstTimeShown = false;
   }
}

void MainWindow::sigReceived() {
    m_controller->start();
}

void MainWindow::showEvent(QShowEvent *){
    std::cout << "Sono nello show event\n";
}

MainWindow::MainWindow() {
    firstTimeShown = true;
    //initialize simulation class
    m_simulation = new Simulation();

    m_simulation->start(2);

    // m_simulation->stackingRobot()->grab();
    // m_simulation->stackingRobot()->rotateToEnd();

    m_controller = new Controller(m_simulation);

    //init all graphics' components
    QGraphicsView *view = new QGraphicsView(m_simulation);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding);

    m_camera = new QLabel();
    m_camera2 = new QLabel();

    /*QDoubleSpinBox *speedSpinBox = new QDoubleSpinBox();
    speedSpinBox->setValue(m_simulation->conveyorBelt()->speed());
    connect(speedSpinBox, SIGNAL(valueChanged(double)),
            m_simulation->conveyorBelt(), SLOT(setSpeed(double)));

    QPushButton *createItemButton = new QPushButton("Create item");
//     connect(createItemButton, &QPushButton::clicked,
//             m_simulation, &Simulation::createItem);
    connect(createItemButton, SIGNAL(clicked()),
            m_simulation, SLOT(createItem()));

    QPushButton *arGrabButton = new QPushButton("Grab");
    connect(arGrabButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::grab);
    QPushButton *arRotateToEndButton = new QPushButton("Rotate to end");
    connect(arRotateToEndButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::rotateToEnd);
    QPushButton *arRotateToStartButton = new QPushButton("Rotate to start");
    connect(arRotateToStartButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::rotateToStart);
    QPushButton *arReleaseButton = new QPushButton("Release");
    connect(arReleaseButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::release);
    QGroupBox *arControls = new QGroupBox("Anomaly Robot");

    QVBoxLayout *arControlsLayout = new QVBoxLayout();
    arControlsLayout->addWidget(arGrabButton);
    arControlsLayout->addWidget(arRotateToEndButton);
    arControlsLayout->addWidget(arRotateToStartButton);
    arControlsLayout->addWidget(arReleaseButton);
    arControls->setLayout(arControlsLayout);*/

    /*QPushButton *srGrabButton = new QPushButton("Grab");
    connect(srGrabButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::grab);
    QPushButton *srRotateToEndButton = new QPushButton("Rotate to end");
    connect(srRotateToEndButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::rotateToEnd);
    QPushButton *srRotateToStartButton = new QPushButton("Rotate to start");
    connect(srRotateToStartButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::rotateToStart);
    QPushButton *srReleaseButton = new QPushButton("Release");
    connect(srReleaseButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::release);
    QGroupBox *srControls = new QGroupBox("Stacking Robot");*/

    /*QVBoxLayout *srControlsLayout = new QVBoxLayout();
    srControlsLayout->addWidget(srGrabButton);
    srControlsLayout->addWidget(srRotateToEndButton);
    srControlsLayout->addWidget(srRotateToStartButton);
    srControlsLayout->addWidget(srReleaseButton);
    srControls->setLayout(srControlsLayout);*/

    //QGridLayout *controlLayout = new QGridLayout();
    //controlLayout->addWidget(new QLabel("Conveyor belt speed"), 0, 0);
    /*controlLayout->addWidget(speedSpinBox, 0, 1);
    controlLayout->addWidget(createItemButton, 1, 0, 1, 2);
    controlLayout->addWidget(arControls, 2, 0, 1, 2);
    controlLayout->addWidget(srControls, 3, 0, 1, 2);*/
    //controlLayout->setRowStretch(4, 1);

    QHBoxLayout *layout = new QHBoxLayout();

    //view hold the entire scene
    layout->addWidget(view);
    //camera window
    layout->addWidget(m_camera);
    layout->addWidget(m_camera2);
    //GRID
    //layout->addLayout(controlLayout);

    //centers the view
    QWidget *centralWidget = new QWidget();    
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    //start simulation at speed "x"
    //m_simulation->start(2);        


    //capture camera frame every 100ms
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::changeCameraFrame, Qt::ConnectionType(Qt::QueuedConnection));
    timer->start(100);

    connect(this, SIGNAL(windowLoaded()), this, SLOT(sigReceived()), Qt::ConnectionType(Qt::QueuedConnection));
}

void MainWindow::changeCameraFrame() {
    QImage frame = m_simulation->frameFromCamera();
    m_camera->setPixmap(QPixmap::fromImage(frame));

    QImage frame2 = m_simulation->frameFromCamera(10, 230);
    m_camera2->setPixmap(QPixmap::fromImage(frame2));
}
