#include "thumbnail.h"

Thumbnail::Thumbnail():
    QWidget{nullptr, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint} // without title bar and always on top
{
    // layout.addWidget(&pictureLabel, 0, 0, 1, 3);
    // layout.addWidget(&timeLabel, 1, 1, 1, 1);
    // layout.setColumnStretch(0, 1);
    // layout.setColumnStretch(2, 1);
    timeLabel.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&pictureLabel);
    layout.addWidget(&timeLabel);
    setLayout(&layout);
}