#ifndef FRAME_INDEXER_H
#define FRAME_INDEXER_H

#include <QUrl>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class FrameIndexer
{
public:
    FrameIndexer();
    ~FrameIndexer();
    void load(const QUrl& url);
    const QPixmap& getFrameByTime(uint second) const;
    // const cv::Mat& getFrameByTime(double second);
private:
    std::vector<QPixmap> frames;
    cv::VideoCapture cap;
    int frameWidth;
    int frameHeight;
    int frameCount;
    double fps;
    cv::Mat frame;
};

#endif