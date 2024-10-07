#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

int height, width;

float get_bright(int R, int G, int B){
  return (0.299 * R + 0.587 * G + 0.114 * B);
};

int main(int argc, char* argv[]) {
    const char* input_file = "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4";


    AVFormatContext* format_ctx = nullptr;
    if (avformat_open_input(&format_ctx, input_file, nullptr, nullptr) < 0) {
        std::cerr << "Could not open input file: " << input_file << std::endl;
        return 2;
    }

    if (avformat_find_stream_info(format_ctx, nullptr) < 0) {
        std::cerr << "Could not find stream information" << std::endl;
        return 3;
    }

    int video_stream_idx = -1;
    for (unsigned int i = 0; i < format_ctx->nb_streams; ++i) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_idx = i;
            break;
        }
    }

    if (video_stream_idx == -1) {
        std::cerr << "Could not find video stream" << std::endl;
        return 4;
    }

    AVCodecParameters* codecpar = format_ctx->streams[video_stream_idx]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codecpar);

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        return 5;
    }

    // Allocate frame and packet
    AVFrame* frame = av_frame_alloc();
    AVPacket packet;
    av_init_packet(&packet);

    // Read frames from the video stream
    while (av_read_frame(format_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream_idx) {
            // Decode video frame
            int ret = avcodec_send_packet(codec_ctx, &packet);
            if (ret < 0) {
                std::cerr << "Error sending packet for decoding" << std::endl;
                break;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(codec_ctx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    std::cerr << "Error during decoding" << std::endl;
                    break;
                }

                // Print basic frame properties
                std::cout << "Frame " << codec_ctx->frame_num
                          << " (type=" << av_get_picture_type_char(frame->pict_type)
                          << ", size=" << frame->pkt_size
                          << " bytes) pts " << frame->pts
                          << " key_frame " << frame->key_frame
                          << std::endl;
            }
        }

        av_packet_unref(&packet);
    }

    // Free allocated resources
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);


    return 0;
}
