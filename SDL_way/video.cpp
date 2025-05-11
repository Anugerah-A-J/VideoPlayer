#include "video.h"

Video::Video():
	pkt{av_packet_alloc()},
	codec{avcodec_find_decoder(AV_CODEC_ID_MPEG4)},
    parser{av_parser_init(codec->id)},
    c{avcodec_alloc_context3(codec)},
    frame{av_frame_alloc()}
{
    if (!pkt)
        exit(1);

    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
 
    /* find the MPEG-4 video decoder */
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
 
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }
 
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    /* For some codecs, such as msmpeg4 and mpeg4, width and height
    MUST be initialized there because this information is not
    available in the bitstream. */
 
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
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
        eof = !data_size;
 
        /* use the parser to split the data into frames */
        data = inbuf;
        while (data_size > 0 || eof) {
            ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                fprintf(stderr, "Error while parsing\n");
                exit(1);
            }
            data      += ret;
            data_size -= ret;
 
            if (pkt->size)
                decode(c, frame, pkt, outfilename);
            else if (eof)
                break;
        }
    } while (!eof);
 
    /* flush the decoder */
    decode(c, frame, NULL, outfilename);
}

Video::~Video()
{
	fclose(f);
 
    av_parser_close(parser);
    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

void Video::set_file(const char *file_path)
{
	FILE *f = fopen(file_path, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", file_path);
        exit(1);
    }
}