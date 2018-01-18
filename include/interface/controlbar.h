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

#define PLAY_PATH "C:/Users/charl/CLionProjects/FYP/images/003-play-button.png"
#define STOP_PATH "C:/Users/charl/CLionProjects/FYP/images/002-stop.png"
#define FILE_PATH "C:/Users/charl/CLionProjects/FYP/images/001-folders.png"

class ControlBar: public QWidget {

private:

    NowPlayingBar *playing;

    QPushButton *fileBrowser;
    QPushButton *playButton;
    QPushButton *stopButton;

    QHBoxLayout *layout;

    std::string nowPlaying; //anticipate needing later on

public:
    ControlBar(QWidget *parent = 0);
    ~ControlBar();

};


#endif // CONTROLBAR_H
