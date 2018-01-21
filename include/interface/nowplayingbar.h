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
#include <QTimer>

#define OUTSIDE_COLOUR "#c8c8c8"
#define INSIDE_COLOUR "#98aed1"

/**
 * class represents a widget which displays a now playing bar
 */
class NowPlayingBar : public QWidget {
    Q_OBJECT
private:
    QString currentTrack;
    QTimer *scrollTimer;
    int currentIndex; //for rolling text
    void paintBar();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void newTrackSlot(QString newTrack);
    void timerUpdateSlot();
public:
    explicit NowPlayingBar(QWidget *parent = nullptr);
    ~NowPlayingBar() override;
    void switchTrack(QString newTrack);
    QString getCurrentTrack();
};


#endif // NOWPLAYINGBAR_H
