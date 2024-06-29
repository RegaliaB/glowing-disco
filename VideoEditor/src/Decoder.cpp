#include "Decoder.hpp"


namespace Decode {

	
	bool decoder_open(reader_state* state, const char* filename) {
		
		//FILE* f;
		const AVCodec* codec{};
		auto& width = state->width;
		auto& height = state->height;
		auto& av_fmt_ctx = state->av_fmt_ctx;
		auto& codec_ctx = state->codec_ctx;
		auto& frame = state->frame;
		auto& pkt = state->pkt;

		state->av_fmt_ctx = avformat_alloc_context();
		avformat_open_input(&av_fmt_ctx, filename, NULL, NULL);

		AVCodecParameters* av_codec_params{};
		int video_stream_index = -1;

		for (int i = 0; i < av_fmt_ctx->nb_streams; ++i) {
			av_codec_params = av_fmt_ctx->streams[i]->codecpar;
			codec = avcodec_find_decoder(av_codec_params->codec_id);
			if (!codec) {
				continue;
			}
			if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
				video_stream_index = i;
				width = av_codec_params->width;
				height = av_codec_params->height;
				//time_base = av_format_ctx->streams[i]->time_base;
				break;
			}
		}
		if (video_stream_index == -1) {
			return false;
		}


		//needs actual variable
		codec_ctx = avcodec_alloc_context3(codec);

		avcodec_parameters_to_context(codec_ctx, av_codec_params);

		avcodec_open2(codec_ctx, codec, NULL);
		frame = av_frame_alloc();

		
		//codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
		pkt = av_packet_alloc();

		return true;
	}
	void decode_read(reader_state* state, uint8_t* frame_buffer, int64_t* pts) {
		
		
		//AVCodecParserContext* parser;
		auto& width = state->width;
		auto& height = state->height;
		auto& av_fmt_ctx = state->av_fmt_ctx;
		auto& codec_ctx = state->codec_ctx;
		auto& frame = state->frame;
		auto& pkt = state->pkt;

		int response;


		while (av_read_frame(av_fmt_ctx, pkt) >= 0) {
			response = avcodec_send_packet(codec_ctx, pkt);
			response = avcodec_receive_frame(codec_ctx, frame);

			av_packet_unref(pkt);
			break;
		}
		
		*pts = frame->pts;


		


		
	}

	void decode_seek_frame(reader_state* state, int64_t ts) {
		auto& av_format_ctx = state->av_fmt_ctx;
		auto& av_codec_ctx = state->codec_ctx;
		auto& video_stream_index = state->video_stream_index;
		auto& av_packet = state->pkt;
		auto& av_frame = state->frame;

		av_seek_frame(av_format_ctx, video_stream_index, ts, AVSEEK_FLAG_BACKWARD);

		// av_seek_frame takes effect after one frame, so I'm decoding one here
		// so that the next call to video_reader_read_frame() will give the correct
		// frame
		int response;
		while (av_read_frame(av_format_ctx, av_packet) >= 0) {
			if (av_packet->stream_index != video_stream_index) {
				av_packet_unref(av_packet);
				continue;
			}

			response = avcodec_send_packet(av_codec_ctx, av_packet);
			if (response < 0) {
				
				return;
			}

			response = avcodec_receive_frame(av_codec_ctx, av_frame);
			if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
				av_packet_unref(av_packet);
				continue;
			}
			else if (response < 0) {
				//printf("Failed to decode packet: %s\n", av_make_error(response));
				return;
			}

			av_packet_unref(av_packet);
			break;
		}
	}

	
	void decode_reader_close(reader_state* state) {
		avformat_close_input(&state->av_fmt_ctx);
		avformat_free_context(state->av_fmt_ctx);
		av_frame_free(&state->frame);
		av_packet_free(&state->pkt);
		avcodec_free_context(&state->codec_ctx);
	}



	bool decodev2_open(reader_state* state, const char* filename) 
	{
		const AVCodec* codec{};
		auto& width = state->width;
		auto& height = state->height;
		auto& av_fmt_ctx = state->av_fmt_ctx;
		auto& codec_ctx = state->codec_ctx;
		auto& frame = state->frame;
		auto& pkt = state->pkt;
		auto& parser = state->parser;

		state->av_fmt_ctx = avformat_alloc_context();
		avformat_open_input(&av_fmt_ctx, filename, NULL, NULL);

		AVCodecParameters* av_codec_params{};
		av_codec_params = av_fmt_ctx->streams[0]->codecpar;
		codec = avcodec_find_decoder(av_codec_params->codec_id);

		codec_ctx = avcodec_alloc_context3(codec);
		parser = av_parser_init(codec->id);
		avcodec_parameters_to_context(codec_ctx, av_codec_params);

		avcodec_open2(codec_ctx, codec, NULL);
		frame = av_frame_alloc();


		//codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
		pkt = av_packet_alloc();
		
		return true;
	}

	bool decodev2_read(reader_state* state) 
	{
		/*AVPacket p = state->av_fmt_ctx->streams[0]->attached_pic;
		p.buf;
		avcodec_send_frame();
		avcodec_receive_packet();

		avcodec_send_packet();
		avcodec_receive_frame();*/
		/*av_read_frame();
		av_dump_format()*/
		return true;
	}

	void decodev2_close(reader_state* state) 
	{
		av_parser_close(state->parser);
		avformat_close_input(&state->av_fmt_ctx);
		avformat_free_context(state->av_fmt_ctx);
		av_frame_free(&state->frame);
		av_packet_free(&state->pkt);
		avcodec_free_context(&state->codec_ctx);
	}
}