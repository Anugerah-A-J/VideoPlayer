#ifndef FRAME_INDEXER_H
#define FRAME_INDEXER_H

#include <ffms.h>
#include <QUrl>

class FrameIndexer
{
public:
    FrameIndexer();
    ~FrameIndexer();
    void load(const QUrl&);
    const FFMS_Frame* getFrameByNumber(int framenumber);
    const FFMS_Frame* getFrameByTime(double second);
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
};

#endif