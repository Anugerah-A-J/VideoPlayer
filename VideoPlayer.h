#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QWidget>
#include <QLabel>

class VideoPlayer : public QWidget
{
Q_OBJECT
public:
    explicit VideoPlayer(QWidget *parent = 0);
private:
    QLabel screen;
signals:
};

#endif