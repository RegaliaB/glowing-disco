
extern "C" {
	#include "libavcodec/avcodec.h"
	#include "libavformat/avformat.h"
    #include <stdint.h>
}
//#include <stdio.h>


namespace Decode {

    struct reader_state {
        int width, height;
        AVCodecContext* codec_ctx;
        AVFormatContext* av_fmt_ctx;
        int video_stream_index;
        AVFrame* frame;
        AVPacket* pkt;
        AVCodecParserContext* parser;
    };
    //extern struct reader_state read_state;


	bool decoder_open(reader_state* state, const char* filename);
	void decode_read(reader_state* state, uint8_t* frame_buffer, int64_t* pts);
	void decode_seek_frame(reader_state* state, int64_t ts);
    void decode_reader_close(reader_state* state);

    bool decodev2_open(reader_state* state, const char* filename);
    bool decodev2_read(reader_state* state);
    void decodev2_close(reader_state* state);
}

/*

Throwing this code here to use later for grabbign information from a file, might work
will have to see

AVFrame* OpenImage(const char* imageFileName)
{
    AVFormatContext *pFormatCtx;

    if(av_open_input_file(&pFormatCtx, imageFileName, NULL, 0, NULL)!=0)
    {
        printf("Can't open image file '%s'\n", imageFileName);
        return NULL;
    }

    dump_format(pFormatCtx, 0, imageFileName, false);

    AVCodecContext *pCodecCtx;

    pCodecCtx = pFormatCtx->streams[0]->codec;
    pCodecCtx->width = W_VIDEO;
    pCodecCtx->height = H_VIDEO;
    pCodecCtx->pix_fmt = PIX_FMT_YUV420P;

    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec)
    {
        printf("Codec not found\n");
        return NULL;
    }

    // Open codec
    if(avcodec_open(pCodecCtx, pCodec)<0)
    {
        printf("Could not open codec\n");
        return NULL;
    }

    //
    AVFrame *pFrame;

    pFrame = avcodec_alloc_frame();

    if (!pFrame)
    {
        printf("Can't allocate memory for AVFrame\n");
        return NULL;
    }

    int frameFinished;
    int numBytes;

    // Determine required buffer size and allocate buffer
    numBytes = avpicture_get_size(PIX_FMT_YUVJ420P, pCodecCtx->width, pCodecCtx->height);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    avpicture_fill((AVPicture *) pFrame, buffer, PIX_FMT_YUVJ420P, pCodecCtx->width, pCodecCtx->height);

    // Read frame

    AVPacket packet;

    int framesNumber = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if(packet.stream_index != 0)
            continue;

        int ret = avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
        if (ret > 0)
        {
            printf("Frame is decoded, size %d\n", ret);
            pFrame->quality = 4;
            return pFrame;
        }
        else
            printf("Error [%d] while decoding frame: %s\n", ret, strerror(AVERROR(ret)));
    }
}
*/

/*
* 
* 
    Another Referencd to possibly use
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    //#include <libavfilter/avfilter.h">
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
    #include <libavutil/opt.h>
    #include <libavutil/pixdesc.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/timecode.h>
    #include <libavutil/bprint.h>
    #include <libavutil/time.h>
    #include <libswscale/swscale.h>
}
#include <stdio.h>
#include <string.h>


int OperationFrame_EncodeAndWrite_Inner_SaveJpg(AVFrame *pFrame, const char *out_file) {
    int width = pFrame->width;
    int height = pFrame->height;

    AVFormatContext* pFormatCtx = avformat_alloc_context();

    pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);
    if( avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
        printf("Couldn't open output file.");
        return -1;
    }

    AVStream* pAVStream = avformat_new_stream(pFormatCtx, 0);
    if( pAVStream == NULL ) {
        return -1;
    }

    AVCodecContext* pCodecCtx = pAVStream->codec;

    pCodecCtx->codec_id = pFormatCtx->oformat->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;

    // Begin Output some information
    av_dump_format(pFormatCtx, 0, out_file, 1);
    // End Output some information

    AVCodec* pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if( !pCodec ) {
        printf("Codec not found.");
        return -1;
    }
    if( avcodec_open2(pCodecCtx, pCodec, NULL) < 0 ) {
        printf("Could not open codec.");
        return -1;
    }

    //Write Header
    avformat_write_header(pFormatCtx, NULL);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    AVPacket pkt;
    av_new_packet(&pkt, y_size * 3);

    //
    int got_picture = 0;
    int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
    if( ret < 0 ) {
        printf("Encode Error.\n");
        return -1;
    }
    if( got_picture == 1 ) {
        //pkt.stream_index = pAVStream->index;
        ret = av_write_frame(pFormatCtx, &pkt);
    }

    av_free_packet(&pkt);

    //Write Trailer
    av_write_trailer(pFormatCtx);

    if( pAVStream ) {
        avcodec_close(pAVStream->codec);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    return 0;
}


AVFrame* OpenImage(const char* imageFileName)
{
    AVFormatContext *pFormatCtx= NULL;
    if(avformat_open_input(&(pFormatCtx), imageFileName, NULL, NULL)!=0)
    {
        printf("Can't open image file '%s'\n", imageFileName);
        return NULL;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL ) < 0){
        printf("Can't find stream\n");
        return NULL;
    }

    av_dump_format(pFormatCtx, 0, imageFileName, false);
    AVCodecContext *pCodecCtx;
    int index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    // pCodecCtx = pFormatCtx->streams[index]->codec;
    // pCodecCtx->width = 640;
    // pCodecCtx->height = 480;
    // pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    AVCodec *dec = avcodec_find_decoder(pFormatCtx->streams[index]->codecpar->codec_id);
    pCodecCtx = avcodec_alloc_context3(dec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[index]->codecpar);

    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec)
    {
        printf("Codec not found\n");
        return NULL;
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
    {
        printf("Could not open codec\n");
        return NULL;
    }

    //
    AVFrame *pFrame;

    pFrame = av_frame_alloc();

    if (!pFrame)
    {
        printf("Can't allocate memory for AVFrame\n");
        return NULL;
    }

    int frameFinished;

    AVPacket packet;
    packet.data = NULL;
    packet.size = 0;

    int framesNumber = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if(packet.stream_index != index){
            continue;
        }
        //pFrame = av_frame_alloc();

        int ret = avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
        if (frameFinished)
        {
            printf("Frame is decoded, size %d\n", ret);
            break;
        }
    }

    AVFrame* dst = av_frame_alloc();

    int width = 1280;
    int height = 960;
    enum AVPixelFormat dst_pixfmt = AV_PIX_FMT_YUV420P;
    int numBytes = avpicture_get_size(dst_pixfmt, width, height);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill( (AVPicture *)dst, buffer, dst_pixfmt, width, height);

    struct SwsContext *convert_ctx=NULL;
    enum AVPixelFormat src_pixfmt = (enum AVPixelFormat)pFrame->format;

    convert_ctx = sws_getContext(pFrame->width, pFrame->height, pCodecCtx->pix_fmt, width, height, dst_pixfmt,
                    SWS_POINT, NULL, NULL, NULL);
    sws_scale(convert_ctx, pFrame->data, pFrame->linesize, 0, pFrame->height,
                    dst->data, dst->linesize);
    sws_freeContext(convert_ctx);

    av_frame_free(&pFrame);
    avformat_close_input(&(pFormatCtx));
    avcodec_free_context(&pCodecCtx);


    dst->format = (int)dst_pixfmt;
    dst->width = width;
    dst->height = height;
    dst->pts = 0;
    dst->pkt_pts = 0;
    dst->pkt_dts = 0;


    return dst;
}

int main(int argc, char *argv[]){
    const char* imageFileName = "/data/test/nosignal.png";
    const char* outputFile = "/data/test/test.png";
    int64_t start_time = av_gettime();
    AVFrame* frame = OpenImage(imageFileName);
    std::cout<< av_gettime() - start_time <<std::endl;
   OperationFrame_EncodeAndWrite_Inner_SaveJpg(frame,outputFile);
}*/