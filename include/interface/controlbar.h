/**
  * file contains class for control bar
  * the control bar contains info about
  * the currently playing backing track
  * and allows the user to start/stop etc.
  * Author: Charlie Street
  */

#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPainter>

#include "nowplayingbar.h"

#include <string>

#define PLAY_PATH "images/play-button.png"
#define STOP_PATH "images/stop.png"
#define FILE_PATH "images/folders.png"

/**
 * class represents the control bar of the system
 * with buttons and a now playing bar
 */
class ControlBar: public QWidget {

private:

    NowPlayingBar *playing;

    QPushButton *fileBrowser;
    QPushButton *playButton;
    QPushButton *stopButton;

    QHBoxLayout *layout;

    std::string nowPlaying; //anticipate needing later on

public:

    //constructor and destructor
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar() override;

    //simple getters
    QPushButton *getFileButton();
    QPushButton *getPlayButton();
    QPushButton *getStopButton();

};


#endif // CONTROLBAR_H
