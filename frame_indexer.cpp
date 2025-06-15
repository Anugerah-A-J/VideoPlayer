#include "frame_indexer.h"
#include <iostream>
#include <QVideoFrame>
#include "video_player.h"

FrameIndexer::FrameIndexer()
{
#ifdef use_FFMS
    FFMS_Init(0, 0);
    errinfo.Buffer     = errmsg;
    errinfo.BufferSize = sizeof(errmsg);
    errinfo.ErrorType  = FFMS_ERROR_SUCCESS;
    errinfo.SubType    = FFMS_ERROR_SUCCESS;
#endif
#ifdef use_OpenCV
	cap.set(cv::CAP_PROP_FORMAT, -1);
#endif
#ifdef use_Qt
    mediaPlayer.setVideoSink(&videoSink);
    // connect(&thumbnailVideoSink, &QVideoSink::videoFrameChanged, this, QOverload<>::of(&VideoPlayer::updateThumbnail));
#endif
}

FrameIndexer::~FrameIndexer()
{
#ifdef use_FFMS
    FFMS_DestroyVideoSource(videosource);
#endif
#ifdef use_OpenCV
    cap.release();
#endif
}

void FrameIndexer::load(const QUrl& url, int& w, int& h)
{
    const std::string filepath = url.path().toStdString();
#ifdef use_FFMS
    const char *sourcefile = &filepath.at(1);
    std::cout << sourcefile << std::endl;

    indexer = FFMS_CreateIndexer(sourcefile, &errinfo);
    if (indexer == NULL)
        std::cout << errinfo.Buffer << '\n';

    index = FFMS_DoIndexing2(indexer, FFMS_IEH_ABORT, &errinfo);
    if (index == NULL)
        std::cout << errinfo.Buffer << '\n';

    trackno = FFMS_GetFirstTrackOfType(index, FFMS_TYPE_VIDEO, &errinfo);
    if (trackno < 0)
        std::cout << errmsg << '\n';

    videosource = FFMS_CreateVideoSource(sourcefile, trackno, index, 1, FFMS_SEEK_NORMAL, &errinfo);
    if (videosource == NULL)
        std::cout << errmsg << '\n';

    /* Since the index is copied into the video source object upon its creation,
    we can and should now destroy the index object. */
    // Since the index is copied into the video source object upon its creation,
    // we can and should now destroy the index object.
    FFMS_DestroyIndex(index);

    /* Retrieve video properties so we know what we're getting.
    As the lack of the errmsg parameter indicates, this function cannot fail. */
    // Retrieve video properties so we know what we're getting.
    // As the lack of the errmsg parameter indicates, this function cannot fail.
    const FFMS_VideoProperties *videoprops = FFMS_GetVideoProperties(videosource);

    /* Now you may want to do something with the info, like check how many frames the video has */
    // Now you may want to do something with the info, like check how many frames the video has
    num_frames = videoprops->NumFrames;

    /* Get the first frame for examination so we know what we're getting. This is required
    because resolution and colorspace is a per frame property and NOT global for the video. */
    // Get the first frame for examination so we know what we're getting. This is required
    // because resolution and colorspace is a per frame property and NOT global for the video.
    const FFMS_Frame *propframe = FFMS_GetFrame(videosource, 0, &errinfo);

    /* Now you may want to do something with the info; particularly interesting values are:
    propframe->EncodedWidth; (frame width in pixels)
    propframe->EncodedHeight; (frame height in pixels)
    propframe->EncodedPixelFormat; (actual frame colorspace)
    */
    std::cout << "propframe->EncodedWidth      : " << propframe->EncodedWidth << std::endl;
    std::cout << "propframe->EncodedHeight     : " << propframe->EncodedHeight << std::endl;
    std::cout << "propframe->EncodedPixelFormat: " << propframe->EncodedPixelFormat << std::endl;

    /* If you want to change the output colorspace or resize the output frame size,
    now is the time to do it. IMPORTANT: This step is also required to prevent
    resolution and colorspace changes midstream. You can you can always tell a frame's
    original properties by examining the Encoded* properties in FFMS_Frame. */
    /* See libavutil/pixfmt.h for the list of pixel formats/colorspaces.
    To get the name of a given pixel format, strip the leading PIX_FMT_
    and convert to lowercase. For example, PIX_FMT_YUV420P becomes "yuv420p". */

    /* A -1 terminated list of the acceptable output formats. */
    // Now you may want to do something with the info; particularly interesting values are:
    // propframe->EncodedWidth; (frame width in pixels)
    // propframe->EncodedHeight; (frame height in pixels)
    // propframe->EncodedPixelFormat; (actual frame colorspace)


    // If you want to change the output colorspace or resize the output frame size,
    // now is the time to do it. IMPORTANT: This step is also required to prevent
    // resolution and colorspace changes midstream. You can you can always tell a frame's
    // original properties by examining the Encoded* properties in FFMS_Frame.
    // See libavutil/pixfmt.h for the list of pixel formats/colorspaces.
    // To get the name of a given pixel format, strip the leading PIX_FMT_
    // and convert to lowercase. For example, PIX_FMT_YUV420P becomes "yuv420p".

    // A -1 terminated list of the acceptable output formats.
    // pixfmts[0] = FFMS_GetPixFmt("bgra");
    pixfmts[0] = FFMS_GetPixFmt("rgba");
    pixfmts[1] = -1;

    if (FFMS_SetOutputFormatV2(videosource,
                            pixfmts,
                            propframe->EncodedWidth,
                            propframe->EncodedHeight,
                            FFMS_RESIZER_BICUBIC,
                            &errinfo))
        std::cout << errmsg << '\n';
#endif
#ifdef use_OpenCV
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
#endif
#ifdef use_Qt
    mediaPlayer.setSource(url);
#endif
}

QPixmap FrameIndexer::getFrameByTime(uint ms)
{
#ifdef use_FFMS
    QImage img;
    const FFMS_Frame *curframe = FFMS_GetFrameByTime(videosource, ms * 0.001, &errinfo);

    if (curframe == NULL)
    {
        std::cout << errmsg << '\n';
        img = QImage(
            curframe->ScaledWidth,
            curframe->ScaledHeight,
            QImage::Format_RGBA8888
        );
        img.fill(QColor(128, 128, 128));
    }
    else
    {
        int i = 0;
        QImage img = QImage(
            curframe->Data[i],
            curframe->ScaledWidth,
            curframe->ScaledHeight,
            curframe->Linesize[i],
            QImage::Format_RGBA8888
        );
    }

    return QPixmap::fromImage(img).scaledToHeight(VideoPlayer::thumbnailHeight);
#endif
#ifdef use_OpenCV
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
#endif
#ifdef use_Qt
    QImage img;
    mediaPlayer.setPosition(ms);
    img = videoSink.videoFrame().toImage();
    return QPixmap::fromImage(img).scaledToHeight(VideoPlayer::thumbnailHeight);
#endif
}