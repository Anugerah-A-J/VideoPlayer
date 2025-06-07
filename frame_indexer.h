#ifndef FRAME_INDEXER_H
#define FRAME_INDEXER_H

#include <ffms.h>
#include <QUrl>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class FrameIndexer
{
public:
    FrameIndexer();
    ~FrameIndexer();
    void load(const QUrl&);
    // const FFMS_Frame* getFrameByNumber(int framenumber);
    // const FFMS_Frame* getFrameByTime(double second);
    const QPixmap& getFrameByTime(uint second) const;
    // const cv::Mat& getFrameByTime(double second);
private:
    char errmsg[1024];
    FFMS_ErrorInfo errinfo;
    FFMS_Indexer *indexer;
    FFMS_Index *index;
    int trackno;
    FFMS_VideoSource *videosource;
    // const FFMS_VideoProperties *videoprops;
    int num_frames;
    int pixfmts[2];
    std::vector<QPixmap> frames;

    // cv::VideoCapture cap;
    // int frameWidth;
    // int frameHeight;
    // int frameCount;
    // double fps;
    // cv::Mat frame;
};

#endif