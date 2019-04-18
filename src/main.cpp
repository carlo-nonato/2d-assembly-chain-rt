#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";    
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
    QApplication app(argc, argv);

    MainWindow window;
    //TODO: Fix min size for high DPI monitor
    window.setMinimumSize(500, 500);
    window.show();
    
    return app.exec();
} 