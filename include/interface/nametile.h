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
#define HIGHLIGHT_COLOUR "#ffffff"

#define HUMAN "images/human.png"
#define ROBOT "images/robot.png"

/**
 * class represents a soloist tile on the interface
 * can be used for either a human user or an ai
 */
class NameTile: public QWidget {

    Q_OBJECT

private:
    bool active; //used to determine if name tile should appear active
    bool isAI; //effects colour of text
    void drawTile();

signals:
    void activeChanged();

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit NameTile(QWidget *parent = nullptr, bool initActive = false, bool isAI = false);
    void switchActive();//switches the active state of the tile
    bool getActive(); //get the current state of the tile
};
#endif // NAMETILE_H
