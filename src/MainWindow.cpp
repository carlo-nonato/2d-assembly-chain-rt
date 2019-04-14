#include "MainWindow.hpp"

#include "Controller.hpp"
#include "CVUtils.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"

#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow() {
    m_simulation = new Simulation();
    
    m_controller = new Controller(m_simulation);
    
    QGraphicsView *view = new QGraphicsView(m_simulation);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding);

    m_anomalyCamera = new QLabel();
    m_stackingCamera = new QLabel();

    // QDoubleSpinBox *speedSpinBox = new QDoubleSpinBox();
    // speedSpinBox->setValue(m_simulation->conveyorBelt()->speed());
    // connect(speedSpinBox, SIGNAL(valueChanged(double)),
    //         m_simulation->conveyorBelt(), SLOT(setSpeed(double)));

    QPushButton *startButton = new QPushButton("Start");
    connect(startButton, &QPushButton::clicked,
            m_controller, &Controller::start);

    // QPushButton *createItemButton = new QPushButton("Create item");
    // connect(createItemButton, &QPushButton::clicked,
    //         m_simulation, &Simulation::createItem);

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
    arControls->setLayout(arControlsLayout);

    QPushButton *srGrabButton = new QPushButton("Grab");
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
    QGroupBox *srControls = new QGroupBox("Stacking Robot");

    QVBoxLayout *srControlsLayout = new QVBoxLayout();
    srControlsLayout->addWidget(srGrabButton);
    srControlsLayout->addWidget(srRotateToEndButton);
    srControlsLayout->addWidget(srRotateToStartButton);
    srControlsLayout->addWidget(srReleaseButton);
    srControls->setLayout(srControlsLayout);

    QGridLayout *controlLayout = new QGridLayout();
    // controlLayout->addWidget(new QLabel("Conveyor belt speed"), 0, 0);
    // controlLayout->addWidget(speedSpinBox, 0, 1);
    controlLayout->addWidget(startButton, 1, 0, 1, 2);
    // controlLayout->addWidget(createItemButton, 1, 0, 1, 2);
    // controlLayout->addWidget(arControls, 3, 0, 1, 2);
    // controlLayout->addWidget(srControls, 4, 0, 1, 2);
    // controlLayout->setRowStretch(4, 1);*/

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(view, 0, 0, 3, 1);
    layout->addLayout(controlLayout, 0, 1);
    layout->addWidget(m_anomalyCamera, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_stackingCamera, 2, 1, Qt::AlignCenter);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCameras);
    timer->start(100);
}

void MainWindow::updateCameras() {
    QImage *frame = m_simulation->frameFromCamera(10, 0, 280, 600);
    m_anomalyCamera->setPixmap(QPixmap::fromImage(frame->copy(0, 90, 280, 280)));
    delete frame;

    // QImage frame2 = m_simulation->frameFromCamera(10, 240, 300, 250);
    // m_stackingCamera->setPixmap(QPixmap::fromImage(frame2));    
}