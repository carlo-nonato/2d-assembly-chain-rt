#include "MainWindow.hpp"

#include "Controller.hpp"
#include "ConveyorBelt.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"

#include <QGraphicsView>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow() {
    m_simulation = new Simulation();
    m_controller = new Controller(m_simulation);
    
    QGraphicsView *view = new QGraphicsView(m_simulation);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumWidth(m_simulation->sceneRect().width()+3);
    view->setMinimumHeight(m_simulation->sceneRect().height()+3);

    m_anomalyCamera = new QLabel();
    m_stackingCamera = new QLabel();

    QPushButton *startButton = new QPushButton("Start the controller");
    connect(startButton, &QPushButton::clicked,
            m_controller, &Controller::start);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(view, 0, 0, 3, 1);
    layout->addWidget(startButton, 0, 2);
    layout->addWidget(m_anomalyCamera, 1, 2);
    layout->addWidget(m_stackingCamera, 2, 2);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(m_controller, &Controller::anomalyFrameUpdated,
            this, &MainWindow::updateAnomalyCamera);
    connect(m_controller, &Controller::stackingFrameUpdated,
            this, &MainWindow::updateStackingCamera);
}

void MainWindow::updateAnomalyCamera() {
    m_anomalyCamera->setPixmap(
        QPixmap::fromImage(m_controller->anomalyFrame()));
}

void MainWindow::updateStackingCamera() {
    m_stackingCamera->setPixmap(
        QPixmap::fromImage(m_controller->stackingFrame()));
}