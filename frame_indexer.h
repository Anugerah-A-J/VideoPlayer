#ifndef FRAME_INDEXER_H
#define FRAME_INDEXER_H

#include <QUrl>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QMediaPlayer>
#include <QVideoSink>

class FrameIndexer
{
public:
    FrameIndexer();
    ~FrameIndexer();
    void load(const QUrl& url, int& w, int& h);
    QPixmap getFrameByTime(uint ms);
private:
    cv::VideoCapture cap;
    cv::Mat frame;
};

#endif