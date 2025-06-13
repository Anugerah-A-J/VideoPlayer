#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QWidget>
#include <QLabel>
// #include <QGridLayout>
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
    // void paintEvent(QPaintEvent*) override;
    // QMediaPlayer mediaPlayer;
    // QVideoSink videoSink;
    QLabel pictureLabel;
    QLabel timeLabel;
    QVBoxLayout layout;
};

#endif