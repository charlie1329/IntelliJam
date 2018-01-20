/**
 * file implements member functions from sqaurebutton.h
 * Author: Charlie Street
 */

#include "../../include/interface/squarebutton.h"

/**
 * constructor initialises icon
 * @param parent parent widget
 * @param iconPath file path to icon file
 */
SquareButton::SquareButton(QWidget *parent, QString iconPath): QPushButton(parent) {

    setStyleSheet("QPushButton {background:rgba(200,200,200,1);} "
                  "QPushButton:hover {background:rgba(255,255,255,0.5);}");

    pixmap = new QPixmap(iconPath);
    icon = new QIcon(*pixmap);

    paintButton();
}

/**
 * destructor deletes heap allocated fields
 */
SquareButton::~SquareButton() {
    delete icon;
    delete pixmap;
}

/**
 * overwrites paint event function
 * @param event the (unused) event
 */
void SquareButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);
    resize(QSize(height(),height()));
}

/**
 * paints button on screen
 */
void SquareButton::paintButton() {
    setIcon(*icon);
    setIconSize(QSize((int)(1.5*height()),(int)(1.5*height())));
    resize(QSize((int)(1.5*height()),(int)(1.5*height())));
}
