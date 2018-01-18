/**
  * file specifies class for a widget representing
  * most of a keyboard/piano
  * Author: Charlie Street
  */

#ifndef PIANO_H
#define PIANO_H

#include <QWidget>
#include <QPainter>
#include <vector>

/**
 * class represents a widget which will appear
 * as a piano on the screen
 */
class Piano : public QWidget {
private:
    int currentlyPlayedNote;
    std::vector<int> whiteNotes;
    std::vector<int> blackNotes;
    void paintPiano();

protected:
    void paintEvent(QPaintEvent *event);

public:

    /**
     * constructor will set up the widget
     * @param parent dealing with inheritance
     */
    Piano(QWidget *parent = 0);

    /**
     * function will set a particular note on the keyboard to 'played'
     * @param note the note to turn on
     */
    void setNoteOn(unsigned int note);

    /**
     * function will set current note on keyboard to 'off'
     */
    void setNoteOff();
};


#endif // PIANO_H
