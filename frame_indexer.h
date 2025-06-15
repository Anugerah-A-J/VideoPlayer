#ifndef FRAME_INDEXER_H
#define FRAME_INDEXER_H

#include <QUrl>
#include <QPixmap>
#include <ffms.h>
#include <opencv2/opencv.hpp>
#include <QMediaPlayer>
#include <QVideoSink>

// #define use_FFMS
#define use_OpenCV
// #define use_Qt

class FrameIndexer
{
public:
    FrameIndexer();
    ~FrameIndexer();
    void load(const QUrl& url, int& w, int& h);
    QPixmap getFrameByTime(uint ms);
private:
#ifdef use_FFMS
    char errmsg[1024];
    FFMS_ErrorInfo errinfo;
    FFMS_Indexer *indexer;
    FFMS_Index *index;
    int trackno;
    FFMS_VideoSource *videosource;
    int num_frames;
    int pixfmts[2];
#endif
#ifdef use_OpenCV
    cv::VideoCapture cap;
    cv::Mat frame;
#endif
#ifdef use_Qt
    QMediaPlayer mediaPlayer;
    QVideoSink videoSink;
#endif
};

#endif