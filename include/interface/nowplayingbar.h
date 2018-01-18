/**
  * file contains class for widget describing
  * now playing bar
  * Author: Charlie Street
  */
#ifndef NOWPLAYINGBAR_H
#define NOWPLAYINGBAR_H

#include <QWidget>
#include <QString>
#include <QPainter>

#define OUTSIDE_COLOUR "#c8c8c8"
#define INSIDE_COLOUR "#98aed1"
#define TEXT_COLOUR "#000000"

/**
 * class represents a widget which displays a now displaying bar
 */
class NowPlayingBar : public QWidget {
private:
    QString currentTrack;
    int currentIndex; //for rolling text
    void paintBar();

protected:
    void paintEvent(QPaintEvent *event);

public:
    NowPlayingBar(QWidget *parent = 0);
    void switchTrack(QString newTrack);
};


#endif // NOWPLAYINGBAR_H
