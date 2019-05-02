#include "MainWindow.hpp"

#include <QApplication>

static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";

int main(int argc, char *argv[]) {
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
    
    QApplication app(argc, argv);

    MainWindow window;
    // Calculate the DPI scale factor to adjust font size in high DPI monitors
    double scaleFactor = window.physicalDpiX()/double(window.logicalDpiX()); 
    QFont font;
    font.setPointSizeF(font.pointSizeF()*scaleFactor);
    window.setFont(font);
    window.show();
    
    return app.exec();
}