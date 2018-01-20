/**
 * file implements member functions from nametile.h
 * Author: Charlie Street
 */

#include "../../include/interface/nametile.h"
#include <QPixmap>

/**
 * constructor just initialises fields
 * @param parent the parent widget
 * @param initActive the initial active state of the tile
 * @param ai is the tile representing the AI?
 */
NameTile::NameTile(QWidget *parent, bool initActive, bool ai):
    QWidget(parent),active(initActive),isAI(ai) {

    //set it up to repaint whenever the active status of the object is changed
    connect(this,SIGNAL(activeChanged()),this,SLOT(update()));

}

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
    emit activeChanged();
}

/**
 * function tests whether tile is active or not
 * @return
 */
bool NameTile::getActive() {
    return active;
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

    painter.drawRect(0,0,(int)w,(int)h);

    //display soloist picture
    QString whichPic = HUMAN;
    if(isAI) whichPic = ROBOT;

    QPixmap picture(whichPic);
    painter.drawPixmap((int)(0.1*width()),(int)(0.1*height()),(int)(width() * 0.8), (int)(height() * 0.8),picture);

}
