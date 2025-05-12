#include "video.hpp"
#include <iostream>

Video::Video():
	pkt{av_packet_alloc()},
    codec{nullptr},
    opts{NULL},
    parser{nullptr},
    c{NULL},
    frame{av_frame_alloc()}
{
    if (!pkt)
        exit(1);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
}

Video::~Video()
{
	fclose(f);

    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

void Video::set_file(const char *file_path, SDL_Window *window)
{
	f = fopen(file_path, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", file_path);
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
    MUST be initialized there because this information is not
    available in the bitstream. */
    if()
    {
        SDL_GetWindowSize(window, &w, &h);
        av_dict_set(opts, "width", (w), 0);
        av_dict_set(opts, "height", (h), 0);
        codec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
    }

    /* find the decoder */
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* open it */
    if (avcodec_open2(c, codec, opts) < 0)
    {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }

    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    do {
        /* read raw data from the input file */
        data_size = fread(inbuf, 1, INBUF_SIZE, f);
        if (ferror(f))
            break;
        // eof = !data_size;
        reach_eof = data_size > 0 ? false : true;

        /* use the parser to split the data into frames */
        data = inbuf;
        while (data_size > 0 || reach_eof) {
            ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            data      += ret;
            data_size -= ret;

            if (pkt->size)
                decode(c, frame, pkt);
            else if (reach_eof)
                break;
        }
    } while (!reach_eof);

    /* flush the decoder */
    decode(c, frame, NULL);
}

void Video::decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt)
{
    // char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        printf("saving frame %3" PRId64"\n", dec_ctx->frame_num);
        fflush(stdout);

        /* the picture is allocated by the decoder. no need to
           free it */

        // This line is used to create a unique filename for each frame being saved.
        // For example, if the program is saving frames as image files,
        // it might use this string to name each file (e.g., output-1.pgm, output-2.pgm, etc.).
        // snprintf(buf, sizeof(buf), "%s-%" PRId64, filename, dec_ctx->frame_num);

        // pgm_save(frame->data[0], frame->linesize[0],
        //          frame->width, frame->height, buf);

        for (int i = 0; i < frame->height; ++i)
        {
            std::cout << ;
        }
    }
}

// void Video::pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
// {
//     FILE *f = fopen(filename,"wb");;
//     int i;

//     // This line writes the PGM file header to the file f
//     // For example, if:
//     //     xsize = 640
//     //     ysize = 480
//     // The line will write the following to the file:
//     // P5
//     // 640 480
//     // 255
//     fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);

//     for (i = 0; i < ysize; i++)
//         fwrite(buf + i * wrap, 1, xsize, f);
//     fclose(f);
// }