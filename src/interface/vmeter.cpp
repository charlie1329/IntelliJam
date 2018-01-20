/**
 * file implements vmeter functionality
 * from vmeter.h
 * Author: Charlie Street
 */

#include "../../include/interface/vmeter.h"
#include <cmath>

/**
 * constructor calls super constructor and initialises peak value
 * @param parent messing with inheritance
 */
VMeter::VMeter(QWidget *parent) : QWidget(parent), currentValue(0.0) {
    connect(this,SIGNAL(valueChanged(double)),this,SLOT(updateValue(double))); //set up event handling
}

/**
 * overwrites paint event function to paint meter
 * @param event the (unused) event
 */
void VMeter::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    paintVMeter();
}

/**
 * function emits new event for new peak value for volume meter
 * @param newValue the new volume meter value
 */
void VMeter::setNewValue(double newValue) {
    emit valueChanged(fabs(newValue)); //I just want the magnitude, not the sign
}

/**
 * updates the volume meter with a new value
 * @param newValue
 */
void VMeter::updateValue(double newValue) {
    currentValue = newValue;
    update();
}

/**
 * function paints the volume meter on the screen
 * dependent on the peak value currently observed
 */
void VMeter::paintVMeter() {

    //get scalings
    int numBars = NUM_GREEN + NUM_YELLOW + NUM_RED;
    double barHeight = height() / 2.0;
    double barWidth = width() / (numBars + ((numBars-1)/2.0));

    //set up painter object
    QPainter painter(this);

    QPen pen;
    pen.setStyle(Qt::NoPen);

    painter.setPen(pen);

    double opaqueRectangles = (currentValue/MAX_ABS) * numBars; //how many rectangles should we have opaque

    painter.setBrush(QColor(GREEN)); //start with green rectangles

    //actually paint the volume meter
    for(int i = 0; i < numBars; i++) {
        if(i == NUM_GREEN) painter.setBrush(QColor(YELLOW));
        if(i == NUM_GREEN + NUM_YELLOW) painter.setBrush(QColor(RED));
        if(i >= opaqueRectangles) painter.setOpacity(TRANSPARENCY); //if we've surpassed the current peak value

        painter.drawRect((int)(i*(barWidth*1.5)),(int)(barHeight/2.0),(int)(barWidth),(int)(barHeight));
    }

}
