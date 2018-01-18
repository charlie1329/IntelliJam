/**
  * file represents class which inherits from push button to have some desirable behaviour
  */

#ifndef SQUAREBUTTON_H
#define SQUAREBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QIcon>
#include <QPixmap>
#include <QString>

/**
 * Class represents a square button with a dynamically sized
 * icon
 */
class SquareButton: public QPushButton {
private:
    QIcon *icon;
    QPixmap *pixmap;
    void paintButton();

protected:
    void paintEvent(QPaintEvent *event);

public:
    SquareButton(QWidget *parent, QString iconPath);
    ~SquareButton();
};


#endif // SQUAREBUTTON_H
