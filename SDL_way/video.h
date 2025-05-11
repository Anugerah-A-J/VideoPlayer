#ifndef VIDEO_H
#define VIDEO_H
#define INBUF_SIZE 4096

#include <libavcodec/avcodec.h>
#include <string>

class Video
{
public:
	// explicit Video(const std::string &file_path);
	Video();
	~Video();
	void set_file(const char *file_path);
protected:
    AVPacket *pkt;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    const AVCodec *codec;
    AVCodecParserContext *parser;
    AVCodecContext *c = nullptr;
    AVFrame *frame;

    // const char *filename;
    FILE *f = nullptr;

    size_t data_size;
    int eof;
    uint8_t *data;
    int ret;
};

#endif