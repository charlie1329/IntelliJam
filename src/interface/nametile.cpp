/**
 * file implements member functions from nametile.h
 * Author: Charlie Street
 */

#include "../../include/interface/nametile.h"
#include <QString>
#include <QFontMetrics>
#include <iostream>
#include <QPixmap>

/**
 * constructor just initialises fields
 * @param parent the parent widget
 * @param initActive the initial active state of the tile
 * @param ai is the tile representing the AI?
 */
NameTile::NameTile(QWidget *parent, bool initActive, bool ai):
    QWidget(parent),active(initActive),isAI(ai) {}

/**
 * overwrites superclass function to paint the new widget
 * @param event the (unused) event
 */
void NameTile::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);
    drawTile();
}

/**
 * function switches the active state of the name tile
 */
void NameTile::switchActive() {
    active = !active;
}

/**
 * function draws the name tile on to the interface
 */
void NameTile::drawTile() {

    //get scalings
    double h = height();
    double w = width();

    //set up painter object
    QPainter painter(this);

    QPen noPen; //I don't want borders if possible
    noPen.setStyle(Qt::NoPen);

    //set pen for border around currently active box
    QPen thickPen;
    thickPen.setStyle(Qt::SolidLine);
    thickPen.setWidth(10);
    thickPen.setBrush(QColor(HIGHLIGHT_COLOUR));
    thickPen.setCapStyle(Qt::RoundCap);
    thickPen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(thickPen);

    painter.setBrush(QColor(ACTIVE_COLOUR));

    //no border and translucent for non-active
    if(!active) {
        painter.setPen(noPen);
        painter.setOpacity(0.5);
    }

    painter.drawRect(0,0,w,h);

    /*
    //different text colours depending on whether AI or user
    if(isAI) {
        painter.setPen(QColor(AI_COLOUR));
    } else {
        painter.setPen(QColor(USER_COLOUR));
    }

    //set the font
    QFont font(FONT);
    font.setBold(true);
    font.setPointSize(12);
    font.setStyleHint(QFont::SansSerif);

    //find width of text
    QFontMetrics fm(font);
    int textWidth = fm.width(name);

    //dynamically sized font
    int fontSize = 12;
    while(textWidth < width()/1.5) {
        font.setPointSize(fontSize);
        QFontMetrics fmTmp(font);
        textWidth = fmTmp.width(name);
        fontSize += 2;
    }

    painter.setFont(font);

    //centre text in box
    //painter.translate(QPoint(w/2,h/2));
    //painter.drawText(-textWidth/2,0,name);
    */


    QString whichPic = HUMAN;
    if(isAI) whichPic = ROBOT;

    QPixmap picture(whichPic);
    painter.drawPixmap(0.1*width(),0.1*height(),width() * 0.8, height() * 0.8,picture);

}
