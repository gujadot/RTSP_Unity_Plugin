#pragma once

#include <iostream>
#include <vector>
#include "FFMpegClass.h"
#include "MediaSink.h"

namespace rtsp_unity_plugin {
	class FFMpegStream
	{

	protected:
		AVFormatContext* m_pFormatCtx NULL;
		AVCodecContext *m_pCodecCtx = NULL;
		AVCodec *m_pCodec = NULL;
		AVFrame *m_pFrameSrc = NULL;

		int m_dstWidth = 0;
		int m_dstHeight = 0;
		int m_frameFinished;

		char* m_pStreamUri;

		AVPacket m_packet;

		int m_VideoStreamIndex;
		int m_AudioStreamIndex;

		bool m_hasInit = false;
		bool m_isClosed = false;

		std::vector<MediaSink> m_vMediaSink;

	public:
		FFMpegStream();
		~FFMpegStream();

		virtual int InitStream(); // return 0 if ok
		virtual int CloseStream(); // return 0 if ok, 1 if not yet init, 2 if already closed 

		inline AVCodecContext *getCodecContext() { return m_pCodecCtx; }
		inline bool hasInit() { return m_hasInit; }
		inline bool isClosed() { return m_isClosed; }

		int addMediaSink(MediaSink &media_sink);

	protected:

		int ReadFrame();

	private:

	
	};
}
