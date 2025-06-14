#include "thumbnail.h"
#include <QPainter>
#include <QVideoFrame>

Thumbnail::Thumbnail():
    QWidget{nullptr, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint} // without title bar and always on top
{
    timeLabel.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&pictureLabel);
    layout.addWidget(&timeLabel);
    setLayout(&layout);
}