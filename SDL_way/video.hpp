#ifndef VIDEO_HPP
#define VIDEO_HPP
#define INBUF_SIZE 4096

#include <libavcodec/avcodec.h>
// #include <SDL3/SDL_video.h>
// #include <string>

class Video
{
public:
	Video();
	~Video();
	void set_file(const char *file_path, SDL_Window *window);
protected:
    void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt);

    AVPacket *pkt;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    const AVCodec *codec;
    AVDictionary** opts;
    int w;
    int h;
    AVCodecParserContext *parser;
    AVCodecContext *c = nullptr;
    AVFrame *frame;

    // const char *filename;
    FILE *f = nullptr;

    size_t data_size;
    bool reach_eof;
    uint8_t *data;
    int ret;
};

#endif