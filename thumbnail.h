#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QWidget>
#include <QLabel>
// #include <QGridLayout>
#include <QVBoxLayout>

class Thumbnail : public QWidget
{
friend class PositionSlider;
friend class VideoPlayer;
public:
    Thumbnail();
private:
    QLabel pictureLabel;
    QLabel timeLabel;
    // QGridLayout layout;
    QVBoxLayout layout;
};

#endif