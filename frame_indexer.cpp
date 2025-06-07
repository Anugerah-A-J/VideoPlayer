#include "frame_indexer.h"
#include <iostream>
#include "video_player.h"

FrameIndexer::FrameIndexer()
// :
//     frameWidth{0},
//     frameHeight{0},
//     frameCount{0},
//     fps{0}
{
    FFMS_Init(0, 0);
    errinfo.Buffer     = errmsg;
    errinfo.BufferSize = sizeof(errmsg);
    errinfo.ErrorType  = FFMS_ERROR_SUCCESS;
    errinfo.SubType    = FFMS_ERROR_SUCCESS;

	// cap.set(cv::CAP_PROP_FORMAT, -1);
}

FrameIndexer::~FrameIndexer()
{
    FFMS_DestroyVideoSource(videosource);
    // cap.release();
}

void FrameIndexer::load(const QUrl &url)
{
    // const std::string sourcefilebuffer = url.path().toStdString(); // don;t use this
    const std::string sourcefilebuffer = url.fileName().toStdString();
    const char *sourcefile = &sourcefilebuffer.front();
    // /*
    indexer = FFMS_CreateIndexer(sourcefile, &errinfo);
    if (indexer == NULL)
    {
        // handle error (print errinfo.Buffer somewhere)
        std::cout << errinfo.Buffer << '\n';
    }

    index = FFMS_DoIndexing2(indexer, FFMS_IEH_ABORT, &errinfo);
    if (index == NULL)
    {
        // handle error (you should know what to do by now)
        std::cout << errinfo.Buffer << '\n';
    }

    // Retrieve the track number of the first video track
    trackno = FFMS_GetFirstTrackOfType(index, FFMS_TYPE_VIDEO, &errinfo);
    if (trackno < 0)
    {
        // no video tracks found in the file, this is bad and you should handle it
        // (print the errmsg somewhere)
        std::cout << errmsg << '\n';
    }

    // We now have enough information to create the video source object
    videosource = FFMS_CreateVideoSource(sourcefile, trackno, index, 1, FFMS_SEEK_NORMAL, &errinfo);
    if (videosource == NULL)
    {
        // handle error
        std::cout << errmsg << '\n';
    }

    // Since the index is copied into the video source object upon its creation,
    // we can and should now destroy the index object.
    FFMS_DestroyIndex(index);

    // Retrieve video properties so we know what we're getting.
    // As the lack of the errmsg parameter indicates, this function cannot fail.
    const FFMS_VideoProperties *videoprops = FFMS_GetVideoProperties(videosource);

    // Now you may want to do something with the info, like check how many frames the video has
    num_frames = videoprops->NumFrames;

    // Get the first frame for examination so we know what we're getting. This is required
    // because resolution and colorspace is a per frame property and NOT global for the video.
    const FFMS_Frame *propframe = FFMS_GetFrame(videosource, 0, &errinfo);

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

    if (FFMS_SetOutputFormatV2(
        videosource,
        pixfmts,
        propframe->EncodedWidth,
        propframe->EncodedHeight,
        FFMS_RESIZER_BICUBIC,
        &errinfo))
    {
        // handle error
        std::cout << errmsg << '\n';
    }

    uint n = videoprops->LastTime - videoprops->FirstTime;
    frames.clear();
    frames.reserve(n);
    for (uint i = 0; i < n; i++)
    {
        const FFMS_Frame *frame = FFMS_GetFrameByTime(videosource, videoprops->FirstTime + i, &errinfo);

        if (frame == NULL)
            std::cout << errmsg << '\n';

        QImage img = QImage(
            frame->Data[0],
            frame->ScaledWidth,
            frame->ScaledHeight,
            frame->Linesize[0],
            QImage::Format_RGBA8888
        );

        frames.emplace_back(
            QPixmap::fromImage(img).scaledToHeight(VideoPlayer::thumbnailHeight)
        );
        std::cout << "frame " << i + 1 << " of " << n << std::endl;
    }
    // */
    // cap = cv::VideoCapture(sourcefile);
	// if (!cap.isOpened())
    //     std::cout << "OpenCV Couldn't open video file.\n";
	// else
	// {
	// 	frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	// 	std::cout << "frame width: " << frameWidth << std::endl;

	// 	frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	// 	std::cout << "frame height: " << frameHeight << std::endl;

	// 	frameCount = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
	// 	std::cout << "frame count: " << frameCount << std::endl;

	// 	fps = cap.get(cv::CAP_PROP_FPS);
	// 	std::cout << "fps: " << fps << std::endl;

	// 	// delta_time = 1 / fps;
	// 	// std::cout << "delta time: " << delta_time << " ms" << std::endl;
	// }
}

// const FFMS_Frame* FrameIndexer::getFrameByNumber(int framenumber)
// {
//     const FFMS_Frame *curframe = FFMS_GetFrame(videosource, framenumber - 1, &errinfo);
//     if (curframe == NULL)
//     {
//         /* handle error */
//         std::cout << errmsg << '\n';
//     }
//     /* do something with curframe */
//     /* continue doing this until you're bored, or something */
//     return curframe;
// }

// const FFMS_Frame* FrameIndexer::getFrameByTime(double second)
// {
//     const FFMS_Frame *curframe = FFMS_GetFrameByTime(videosource, second, &errinfo);
//     if (curframe == NULL)
//     {
//         /* handle error */
//         std::cout << errmsg << '\n';
//     }
//     return curframe;
// }

const QPixmap& FrameIndexer::getFrameByTime(uint second) const
{
    return frames.at(second);
}