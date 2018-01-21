/**
 * implements member functions from controlbar.h
 * Author: Charlie Street
 */

#include "../../include/interface/controlbar.h"
#include "../../include/interface/squarebutton.h"

#include <QPixmap>
#include <QIcon>

/**
 * constructor sets up and organises the widget
 * @param parent the parent widget
 */
ControlBar::ControlBar(QWidget *parent) : QWidget(parent) {
    setStyleSheet("background-image: url(\"images/dark-back.png\");");


    layout = new QHBoxLayout(this);

    playing = new NowPlayingBar(this);
    fileBrowser = new SquareButton(this,FILE_PATH);//QPushButton(this); //create the buttons
    playButton = new SquareButton(this,PLAY_PATH);
    stopButton = new SquareButton(this,STOP_PATH);

    layout->addWidget(playing,30);
    layout->addWidget(fileBrowser,10); //add buttons to layout
    layout->addWidget(playButton,0,Qt::AlignRight);
    layout->addWidget(stopButton,0,Qt::AlignRight);

    setLayout(layout);
}

/**
 * destructor deletes any heap allocated fields
 */
ControlBar::~ControlBar() {
    delete layout;
    delete playing;
    delete fileBrowser;
    delete playButton;
    delete stopButton;
}

/**
 * simple get member function
 * @return the file button object
 */
QPushButton *ControlBar::getFileButton() {
    return fileBrowser;
}

/**
 * simple get member function
 * @return the play button object
 */
QPushButton *ControlBar::getPlayButton() {
    return playButton;
}

/**
 * simple get member function
 * @return the stop button object
 */
QPushButton *ControlBar::getStopButton() {
    return stopButton;
}

/**
 * simple get member function
 * @return the now playing bar object
 */
NowPlayingBar *ControlBar::getPlayBar() {
    return playing;
}