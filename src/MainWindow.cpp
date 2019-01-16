#include "MainWindow.hpp"

#include "ConveyorBelt.hpp"
#include "Simulation.hpp"
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

MainWindow::MainWindow() {
    m_simulation = new Simulation();

    QGraphicsView *view = new QGraphicsView(m_simulation);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding);

    m_camera = new QLabel();

    QDoubleSpinBox *speedSpinBox = new QDoubleSpinBox();
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
    controlLayout->addWidget(new QLabel("Conveyor belt speed"), 0, 0);
    controlLayout->addWidget(speedSpinBox, 0, 1);
    controlLayout->addWidget(createItemButton, 1, 0, 1, 2);
    controlLayout->addWidget(arControls, 2, 0, 1, 2);
    controlLayout->addWidget(srControls, 3, 0, 1, 2);
    controlLayout->setRowStretch(4, 1);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(view);
    layout->addWidget(m_camera);
    layout->addLayout(controlLayout);
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    m_simulation->start();

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::changeCameraFrame);
    timer->start(100);
}

void MainWindow::changeCameraFrame() {
    QImage frame = m_simulation->frameFromCamera();
    m_camera->setPixmap(QPixmap::fromImage(frame));
}
