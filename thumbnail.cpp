#include "thumbnail.h"
#include "video_player.h"
#include <QPainter>
#include <QVideoFrame>

Thumbnail::Thumbnail():
    QWidget{nullptr, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint} // without title bar and always on top
{
    // mediaPlayer.setVideoSink(&videoSink);
    // connect(&videoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&Thumbnail::update));

    timeLabel.setAlignment(Qt::AlignHCenter);
    layout.addWidget(&pictureLabel);
    layout.addWidget(&timeLabel);
    setLayout(&layout);
    setFixedHeight(VideoPlayer::thumbnailHeight);
}

// void Thumbnail::paintEvent(QPaintEvent*)
// {
//     QPainter painter{this};
//     QVideoFrame::PaintOptions paintOptions;
//     videoSink.videoFrame().paint(&painter, rect(), paintOptions);
// }