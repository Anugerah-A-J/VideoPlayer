#include "frame_indexer.h"
#include <iostream>
#include "video_player.h"

FrameIndexer::FrameIndexer()
:
    frameWidth{0},
    frameHeight{0},
    frameCount{0},
    fps{0}
{
	cap.set(cv::CAP_PROP_FORMAT, -1);
}

FrameIndexer::~FrameIndexer()
{
    cap.release();
}

void FrameIndexer::load(const QUrl& url)
{
    // const std::string sourcefilebuffer = url.path().toStdString(); // don;t use this
    const std::string sourcefilebuffer = url.fileName().toStdString();
    const char *sourcefile = &sourcefilebuffer.front();

    cap = cv::VideoCapture(sourcefile);
	if (!cap.isOpened())
        std::cout << "OpenCV Couldn't open video file.\n";
	else
	{
		frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
		std::cout << "frame width: " << frameWidth << std::endl;

		frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
		std::cout << "frame height: " << frameHeight << std::endl;

		frameCount = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
		std::cout << "frame count: " << frameCount << std::endl;

		fps = cap.get(cv::CAP_PROP_FPS);
		std::cout << "fps: " << fps << std::endl;

		// delta_time = 1 / fps;
		// std::cout << "delta time: " << delta_time << " ms" << std::endl;
	}

    // cap.set(cv::CAP_PROP_POS_AVI_RATIO, 1);
    // uint n = cap.get(cv::CAP_PROP_POS_MSEC) / 1000;
    uint n = frameCount / fps;
    std::cout << "n: " << n << std::endl;
    // cap.set(cv::CAP_PROP_POS_AVI_RATIO, 0);
    frames.clear();
    frames.reserve(n);
    for (uint i = 0; i < n; i++)
    {
        QImage* imgp;

        cap.set(cv::CAP_PROP_POS_MSEC, i * 1000);

        if (cap.read(frame))
        {
            QImage img = QImage(
                frame.data,
                frameWidth,
                frameHeight,
                frame.cols,
                QImage::Format_BGR888
            );
            imgp = &img;
        }
        else
        {
            QImage img = QImage(
                frameWidth,
                frameHeight,
                QImage::Format_BGR888
            );
            img.fill(QColor(128, 128, 128));
            imgp = &img;
        }

        frames.emplace_back(
            QPixmap::fromImage(*imgp, Qt::NoFormatConversion).scaledToHeight(VideoPlayer::thumbnailHeight)
        );
        std::cout << "frame " << i + 1 << " of " << n << std::endl;
    }
}

const QPixmap& FrameIndexer::getFrameByTime(uint second) const
{
    return frames.at(second);
}