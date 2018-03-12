/**
  * this file includes the main method to start
  * up the whole system
  * Author: Charlie Street
  */

#include "../../include/interface/mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QMessageBox>
#include <QIcon>

/**
 * function starts up and runs the system
 * @param argc number of arguments
 * @param argv the arguments themselves
 * @return exit code
 */
int main(int argc, char **argv) {

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon("images/logo.png"));
    //set up a splash screen while system starts up
    QPixmap pixmap("images/splash.png"); //Splash Screen Image and Logo kindly designed by Tom Nicklin
    pixmap = pixmap.scaledToWidth(800);
    QSplashScreen splash(pixmap);
    splash.show();

    //set up the bridge into the backend of the system
    shared_ptr<Bridge> bridge = std::make_shared<Bridge>();

    //if an error occurs, display an error message prompting the user to exit the system.
    if(bridge->getErr() != noError) {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Initialisation Error", "An error occurred during initialisation.\n "
                "The system cannot be used in this state.\n Please restart the system.");
        messageBox.setFixedSize(500, 200);
        messageBox.show();
    }

    MainWindow window(nullptr, bridge);
    window.show(); //show the main window
    splash.finish(&window); //finish with the splash screen

    return a.exec();


}
