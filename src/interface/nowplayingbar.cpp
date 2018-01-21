/**
 * implements member functions from
 * nowplayingbar.h
 * Author: Charlie Street
 */

#include <utility>

#include "../../include/interface/nowplayingbar.h"

/**
 * constructor calls super constructor
 * and initialises field
 * @param parent the parent widget
 */
NowPlayingBar::NowPlayingBar(QWidget *parent): QWidget(parent), currentTrack(""), currentIndex(-1) {

    //set up timer nicely
    scrollTimer = new QTimer(this);
    connect(scrollTimer,SIGNAL(timeout()),this,SLOT(timerUpdateSlot()));
    scrollTimer->start(200);
}

/**
 * function changes the name of the track being displayed
 * @param newTrack the new track to be displayed
 */
void NowPlayingBar::switchTrack(QString newTrack) {
    currentTrack = newTrack;
    if(newTrack.count() == 0) {
        currentIndex = -1;
    } else {
        currentIndex = 0;
    }
}

/**
 * overwrites function from super class
 * @param event the (unused) event
 */
void NowPlayingBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    paintBar();
}

/**
 * function paints the now playing bar on the screen
 */
void NowPlayingBar::paintBar() {

    //non-moving string
    QString now = "Now Playing: ";

    //dimensions of rectangles
    double outerHeight = height()/1.5;
    double innerHeight = height()/2.0;
    double innerWidth = width() * 0.9875;


    QPainter painter(this); //set the painter object

    //start with blank pen for outer/inner rectangles
    QPen blankPen;
    blankPen.setStyle(Qt::NoPen);
    painter.setPen(blankPen);

    painter.setBrush(QColor(OUTSIDE_COLOUR)); //colour for outer rectangle
    painter.drawRect(0,(int)((height() - outerHeight)/2),width(),(int)outerHeight);

    painter.setBrush(QColor(INSIDE_COLOUR));
    painter.drawRect((int)((width()-innerWidth)/2),
                     (int)((height() - outerHeight)/2 + (outerHeight-innerHeight)/2),
                     (int)innerWidth,(int)innerHeight);


    //once rectangles drawn, add text on top
    painter.setPen(QColor(Qt::black));

    //dynamically adjust the font size

    QFont font("Courier");
    font.setItalic(true);
    font.setPointSize(12);
    font.setStyleHint(QFont::SansSerif);

    QString total = now + currentTrack;

    //find width of text
    QFontMetrics fm(font);
    int textWidth = fm.width(total);
    int textHeight = fm.height();

    if(textWidth > 0.9 * innerWidth) {
       int maxChars = (int)((0.9 * innerWidth-fm.width(now))/fm.width(" ")); //how many characters can I write?

       //generate string to display
       int start = currentIndex;
       int endPoint = currentIndex + maxChars;
       if(endPoint >= currentTrack.count()) {
            QString sub1 = currentTrack.mid(start,currentTrack.count()-1);

            if(endPoint - currentTrack.count()-1 <= 2){
                total = now + sub1 + "  ";
            } else {
                QString sub2 = currentTrack.mid(0,endPoint-currentTrack.count()-3);
                total = now + sub1 + "  " + sub2;
            }
       } else {
           QString sub = currentTrack.mid(start,endPoint-1);
           total = now + sub;
       }

       textWidth = fm.width(total);
    }

    painter.setFont(font); //set the font

    painter.translate((width()-innerWidth)/2+5,height()/2+textHeight/4);
    painter.drawText(0,0,total);
}

/**
 * slot for when a new track is selected from a file browser
 * @param newTrack the new track to be displayed
 */
void NowPlayingBar::newTrackSlot(QString newTrack) {
    switchTrack(std::move(newTrack));
    update();
}

/**
 * slot for when timer expires for rolling text
 */
void NowPlayingBar::timerUpdateSlot() {

    if(currentIndex == -1) return; //account for empty case

    if(currentIndex == currentTrack.count()) {
        currentIndex = 0;
    } else {
        currentIndex++;
    }

    update();
}

/**
 * simple get member function
 * @return the current track
 */
QString NowPlayingBar::getCurrentTrack() {
    return currentTrack;
}

/**
 * destructor deletes any heap allocated fields
 */
NowPlayingBar::~NowPlayingBar() {
    delete scrollTimer;
}