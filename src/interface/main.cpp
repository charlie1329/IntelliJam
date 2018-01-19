/**
  * this file includes the main method to start
  * up the whole system
  * Author: Charlie Street
  */

#include "../../include/interface/mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

/**
 * function starts up and runs the system
 * @param argc number of arguments
 * @param argv the arguments themselves
 * @return exit code
 */
int main(int argc, char **argv)
{

    QApplication a(argc, argv);

    //set up a splash screen while system starts up
    QPixmap pixmap("C:/Users/charl/Pictures/ade.png");
    QSplashScreen splash(pixmap);
    splash.show();

    //set up the bridge into the backend of the system
    shared_ptr<Bridge> bridge = std::make_shared<Bridge>();

    MainWindow window(nullptr, bridge);

    window.show(); //show the main window
    splash.finish(&window); //finish with the splash screen

    return a.exec();
}
