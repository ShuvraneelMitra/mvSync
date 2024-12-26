#include <string>
#include <iostream>
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/error.h>
    #include <libswscale/swscale.h>
    #include <inttypes.h>
}

bool load_frame(std::string filepath, int& w, int& h, unsigned char** data_out) {

    AVFormatContext* av_fmt_ctx = avformat_alloc_context();
    if(!av_fmt_ctx) {
        std::cerr << "Could not allocate context\n";
        return false;
    }

    if(avformat_open_input(&av_fmt_ctx, filepath.c_str(), NULL, NULL)) {
        std::cerr << "Couldn't open video file\n";
        return false;
    }

    int vid_idx = -1;
    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;

    for (int i=0; i<av_fmt_ctx->nb_streams; i++){
        auto stream = av_fmt_ctx->streams[i];
        av_codec_params = av_fmt_ctx->streams[i]->codecpar;
        av_codec = avcodec_find_decoder(av_codec_params->codec_id);

        if(!av_codec) continue;
        else if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
            vid_idx = i;
            break;
        }
    }

    if (vid_idx == -1){
        std::cerr << "Could not find valid video stream\n";
        return false;
    }

    AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx) {
        std::cerr << "Couldn't create codec context\n";
        return false;
    }

    if(avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0) {
        std::cerr << "Couldn't initialize AVCodec_Context\n";
        return false;
    }

    if(avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) {
        std::cerr << "Couldn't open codec\n";
        return false;
    }

    AVFrame* av_frame = av_frame_alloc();
    if(!av_frame) {
        std::cerr << "Couldn't allocate AVFrame\n";
        return false;
    }

    AVPacket* av_packet = av_packet_alloc();
    if(!av_packet) {
        std::cerr << "Couldn't allocate AVPacket\n";
        return false;
    }

    int response;

    while (av_read_frame(av_fmt_ctx, av_packet) >= 0) {
        if (av_packet->stream_index != vid_idx) {
            continue;
        }
        response = avcodec_send_packet(av_codec_ctx, av_packet);
        char err_buf[AV_ERROR_MAX_STRING_SIZE];
        if(response < 0) {
            av_make_error_string(err_buf, AV_ERROR_MAX_STRING_SIZE, response);
            std::cerr << "Failed to decode packet: " << err_buf << "\n";
            return false;
        }
        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if(response == AVERROR(EAGAIN) || response == AVERROR_EOF){
            continue;
        } 
        else if (response < 0) {
            av_make_error_string(err_buf, AV_ERROR_MAX_STRING_SIZE, response);
            std::cerr << "Failed to receive frame: " << err_buf << "\n";
            return false;
        }
        av_packet_unref(av_packet);
        break;
    }

    uint8_t* data = new uint8_t[av_frame->width * av_frame->height * 4];

    SwsContext* sws_scaler_ctx = sws_getContext(av_frame->width, av_frame->height, av_codec_ctx->pix_fmt,
                                                av_frame->width, av_frame->height, AV_PIX_FMT_RGB0,
                                                SWS_BILINEAR,
                                                NULL,
                                                NULL,
                                                NULL);

    if (!sws_scaler_ctx) {
        std::cerr << "Couldn't initialize scaler context\n";
        return false;
    }

    uint8_t* dest[4] = {data, NULL, NULL, NULL};
    int dest_linesize[4] = {av_frame->width * 4, 0, 0, 0};
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);
    sws_freeContext(sws_scaler_ctx);

    w = av_frame->width;
    h = av_frame->height;
    *data_out = data;

    avformat_close_input(&av_fmt_ctx);
    avformat_free_context(av_fmt_ctx);
    av_frame_free(&av_frame);
    av_packet_free(&av_packet);
    avcodec_free_context(&av_codec_ctx);
    return true; 
}
