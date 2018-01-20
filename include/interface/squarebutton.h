/**
  * file represents class which inherits from push button to have some desirable behaviour
  * Author: Charlie Street
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
    void paintEvent(QPaintEvent *event) override;

public:
    SquareButton(QWidget *parent, QString iconPath);
    ~SquareButton() override;
};


#endif // SQUAREBUTTON_H
