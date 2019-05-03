#include "MainWindow.hpp"

#include <QApplication>

#include <cmath>
#include <QScreen>

static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";

int main(int argc, char *argv[]) {
    double scaleFactor;

    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {

        QApplication tmp(argc, argv);
        scaleFactor = floor(
            tmp.primaryScreen()->physicalDotsPerInchX() / 
            tmp.primaryScreen()->logicalDotsPerInchX());

        qputenv("QT_SCALE_FACTOR",
                QByteArray::fromStdString(std::to_string((int) scaleFactor)));
    }
    
    QApplication app(argc, argv);
    MainWindow window;
    // Calculate the DPI scale factor to adjust font size in high DPI monitors
    scaleFactor = window.physicalDpiX()/(double) window.logicalDpiX();
    QFont font;
    font.setPointSizeF(font.pointSizeF()*scaleFactor);
    window.setFont(font);
    window.show();
    
    return app.exec();
}