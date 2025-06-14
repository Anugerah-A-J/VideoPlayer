#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QVideoSink>

class Thumbnail : public QWidget
{
friend class PositionSlider;
friend class VideoPlayer;
public:
    Thumbnail();
private:
    QLabel pictureLabel;
    QLabel timeLabel;
    QVBoxLayout layout;
};

#endif