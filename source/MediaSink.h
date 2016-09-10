#pragma once
#include "ffmpegenv.h"
#include "Unity/IUnityGraphics.h"
#include "RenderApi.h"


namespace rtsp_unity_plugin {


	class MediaSink
	{
	protected:
		int m_Height;
		int m_Width;

		AVFrame *m_pFrameDst = NULL;
		AVPixelFormat m_dstFormat;
		AVCodecContext *m_pSrcCodecContext = NULL;
		struct SwsContext *m_pSwsContext = NULL;
		uint8_t *m_pBuffer = NULL;

	public:
		/*
		@param height: height of the target frame
		@param width: width of the target frame
		*/
		MediaSink(int height, int width, AVPixelFormat format= AV_PIX_FMT_YUV420P);
		~MediaSink();

		int setSrcCodecContext(AVCodecContext * p_src_codec_context);

		virtual int WriteAudio(AVFrame* audio_frame) = 0;
		virtual int WriteVideo(RenderAPI* render_api,AVFrame* Video_frame) = 0;

		inline bool hasInit() { return m_hasInit; }
	private: 
		bool m_hasInit = false;
		
	};
}
