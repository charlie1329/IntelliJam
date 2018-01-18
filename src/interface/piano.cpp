/**
 * file implements functions from piano.h
 * Author: Charlie Street
 */

#include "../../include/interface/piano.h"
#define NOTE_ON "#1e73fc"
#define BLACK "#000000"
#define WHITE "#ffffff"
#define START_NOTE 20

/**
 * implemented from piano.h
 * @param parent dealing with inheritance
 */
Piano::Piano(QWidget *parent) : QWidget(parent), currentlyPlayedNote(-1) {

    //fill up the note mapping
    for(int i = 0; i < 5; i++) {
        whiteNotes.push_back(i*12 + 0);
        whiteNotes.push_back(i*12 + 2);
        whiteNotes.push_back(i*12 + 4);
        whiteNotes.push_back(i*12 + 6);
        whiteNotes.push_back(i*12 + 7);
        whiteNotes.push_back(i*12 + 9);
        whiteNotes.push_back(i*12 + 11);

        blackNotes.push_back(i*12 + 1);
        blackNotes.push_back(i*12 + 3);
        blackNotes.push_back(i*12 + 5);
        blackNotes.push_back(i*12 + 8);
        blackNotes.push_back(i*12 + 10);

    }
}

/**
 * overwrites the paintEvent function for this widget
 * @param event the event received
 */
void Piano::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    paintPiano();
}

/**
 * function draws the piano on the screen
 */
void Piano::paintPiano() {

    //calculate which note to set to blue if necessary
    int currentOn = -1;
    bool isWhite = false;

    if(currentlyPlayedNote >= 0) {

        for(unsigned int i = 0; i < whiteNotes.size(); i++) {
            if(whiteNotes.at(i) == currentlyPlayedNote) {
                currentOn = i;
                isWhite = true;
                break;
            }
        }

        if(!isWhite) {
            for(unsigned int i = 0; i < blackNotes.size(); i++) {
                if(blackNotes.at(i) == currentlyPlayedNote) {
                    currentOn = i;
                    break;
                }
            }
        }
    }

    //int h = height();
    double w = width(); //width mainly used for judging size of keyboard
    double relativeW = 820.0;

    //the following values have been calculated by hand
    double whiteNoteBigger = (24.0/relativeW) * w;
    double whiteNoteSmaller = (23.0/relativeW) * w;
    double whiteNoteHeight = (14.0/78.75) * ((double) w);

    double startY = (height() - whiteNoteHeight)/2.0;

    double blackNoteWidth = (14.0/relativeW) * w;
    double blackNoteHeight= (9.0/14.0) * whiteNoteHeight;

    QPainter painter(this);
    //QPen pen;
    //pen.setStyle(Qt::NoPen);
    painter.setPen(QColor(BLACK)); //black

    //draw white keys
    double topX = 0;
    painter.setBrush(QBrush(WHITE));

    for(int i = 0; i < 35; i++) {

        if(currentOn == i && isWhite) painter.setBrush(QColor(NOTE_ON)); //if this note being played

        if(i % 7 == 0 || i % 7 == 3 || i % 7 == 4) {
            painter.drawRect(topX,startY,whiteNoteBigger,whiteNoteHeight);
            topX += whiteNoteBigger;
        } else {
            painter.drawRect(topX,startY,whiteNoteSmaller,whiteNoteHeight);
            topX += whiteNoteSmaller;
        }

        if(currentOn == i && isWhite) painter.setBrush(QColor(WHITE));
    }

    //draw black keys
    topX = 0;
    painter.setBrush(QBrush(BLACK));

    for(int i = 0; i < 25; i++) {

        if(currentOn == i && !isWhite){
            painter.setBrush(QColor(NOTE_ON)); //if this note being played
            painter.setPen(QColor(NOTE_ON)); //set border to that colour as well
        }

        if(i % 5 == 3) {
            painter.drawRect(topX + ((27.0/relativeW) * w), startY, blackNoteWidth, blackNoteHeight);
            topX += (41.0/relativeW) * w;
        } else if (i % 5 == 4) {
            painter.drawRect(topX + ((14.0/relativeW) * w), startY, blackNoteWidth, blackNoteHeight);
            topX += (42.0/relativeW) * w;
        } else {
            painter.drawRect(topX + ((13.0/relativeW) * w), startY, blackNoteWidth, blackNoteHeight);
            topX += (27.0/relativeW) * w;
        }

        if(currentOn == i && !isWhite){
            painter.setBrush(QColor(BLACK));
            painter.setPen(QColor(BLACK));
        }
    }

}

/**
 * function sets new note on keyboard to on
 * @param note the note to be set
 */
void Piano::setNoteOn(unsigned int note) {
    currentlyPlayedNote = note - START_NOTE;
}

/**
 * function sets currentNote to 'off'
 */
void Piano::setNoteOff() {
    currentlyPlayedNote = -1;
}
