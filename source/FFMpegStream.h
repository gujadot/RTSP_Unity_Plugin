#pragma once

#include <iostream>
#include <string>;
#include "MediaSink.h"


namespace rtsp_unity_plugin {
	class MediaSink;
	class FFMpegStream
	{

	protected:
		AVFormatContext* m_pFormatCtx= NULL;
		AVCodecContext *m_pCodecCtx = NULL;
		AVCodec *m_pCodec = NULL;
		AVFrame *m_pFrameSrc = NULL;

		int m_dstWidth = 0;
		int m_dstHeight = 0;
		int m_frameFinished;

		const char* m_pStreamUri;

		AVPacket m_packet;

		int m_VideoStreamIndex;
		int m_AudioStreamIndex;

		bool m_hasInit = false;
		bool m_isClosed = false;

		MediaSink* m_pMediaSink;
		//std::vector<MediaSink> m_vMediaSink;

	public:
		FFMpegStream(const char* uri);
		~FFMpegStream();

		virtual int InitStream() = 0; // return 0 if ok
		virtual int CloseStream() = 0; // return 0 if ok, 1 if not yet init, 2 if already closed 
		
		inline AVCodecContext *getCodecContext() { return m_pCodecCtx; }
		inline bool hasInit() { return m_hasInit; }
		inline bool isClosed() { return m_isClosed; }
		//int addMediaSink(MediaSink &media_sink);
		int setMediaSink(MediaSink* media_sink);

		const char* getUri() const;

		int ReadFrame();

	protected:

	private:

	
	};
	/*
	struct FFMpegStreamCompare {
		bool operator() (const FFMpegStream& lhs, const FFMpegStream& rhs) const {
			return strcmp(lhs.getUri(), rhs.getUri()) < 0;
		}
	};*/
}
