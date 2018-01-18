/**
  * header file for widget which represents a name tile,
  * could be AI or user based
  * Author: Charlie Street
  */

#ifndef NAMETILE_H
#define NAMETILE_H

#include <string>
#include <QWidget>
#include <QPainter>
#include <QString>

#define ACTIVE_COLOUR "#535c50"
#define INACTIVE_COLOUR "#333333"
#define USER_COLOUR "#08c207"
#define AI_COLOUR "#c60404"
#define HIGHLIGHT_COLOUR "#ffffff"

#define HUMAN "C:/Users/charl/CLionProjects/FYP/images/002-human.png"
#define ROBOT "C:/Users/charl/CLionProjects/FYP/images/001-robot.png"

#define WIDTH_DIVISION 1.0
#define FONT "Newyork"

class NameTile: public QWidget {
private:
    bool active; //used to determine if name tile should appear active
    bool isAI; //effects colour of text
    void drawTile();

protected:
    void paintEvent(QPaintEvent *event);

public:
    NameTile(QWidget *parent = 0, bool initActive = false, bool isAI = false);
    void switchActive();
};
#endif // NAMETILE_H
