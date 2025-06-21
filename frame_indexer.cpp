#include "frame_indexer.h"
#include <iostream>
#include <QVideoFrame>
#include "video_player.h"

FrameIndexer::FrameIndexer()
{
	cap.set(cv::CAP_PROP_FORMAT, -1);
}

FrameIndexer::~FrameIndexer()
{
    cap.release();
}

void FrameIndexer::load(const QUrl& url, int& w, int& h)
{
    const std::string filepath = url.path().toStdString();
    const char *sourcefile = &filepath.at(1);
    std::cout << sourcefile << std::endl;

    cap = cv::VideoCapture(sourcefile);
	if (!cap.isOpened())
        std::cout << "OpenCV Couldn't open video file.\n";
	else
	{
        w = cap.get(cv::CAP_PROP_FRAME_WIDTH);
		h = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

		std::cout << "frame width : " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
		std::cout << "frame height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
        std::cout << "frame count : " << cap.get(cv::CAP_PROP_FRAME_COUNT) << std::endl;
		std::cout << "fps         : " << cap.get(cv::CAP_PROP_FPS) << std::endl;

		// delta_time = 1 / fps;
		// std::cout << "delta time: " << delta_time << " ms" << std::endl;
	}
}

QPixmap FrameIndexer::getFrameByTime(uint ms)
{
    QImage img;
    cap.set(cv::CAP_PROP_POS_MSEC, ms);

    if (cap.read(frame))
    {
        img = QImage(
            frame.data,
            cap.get(cv::CAP_PROP_FRAME_WIDTH),
            cap.get(cv::CAP_PROP_FRAME_HEIGHT),
            frame.cols * 3,
            QImage::Format_BGR888
        );
    }
    else
    {
        img = QImage(
            cap.get(cv::CAP_PROP_FRAME_WIDTH),
            cap.get(cv::CAP_PROP_FRAME_HEIGHT),
            QImage::Format_BGR888
        );
        img.fill(QColor(128, 128, 128));
    }

    // return QPixmap::fromImage(img, Qt::NoFormatConversion).scaledToHeight(VideoPlayer::thumbnailHeight);
    return QPixmap::fromImage(img).scaledToHeight(VideoPlayer::thumbnailHeight);
}