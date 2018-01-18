/**
  * this file includes the main method to start
  * up the whole system
  * Author: Charlie Street
  */

#include "../../include/interface/mainwindow.h"
#include <QApplication>

/**
 * @param argc number of arguments
 * @param argv the arguments themselves
 * @return exit code
 */
int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWindow window;

    window.show();

    return a.exec();
}
