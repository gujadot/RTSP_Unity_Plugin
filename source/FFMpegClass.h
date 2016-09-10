#pragma once

#include "ffmpegenv.h"
#include <set>
#include "FFMpegStream.h"

namespace rtsp_unity_plugin {
	class FFMpegStream;

	class FFMpegClass
	{

	private:
		FFMpegClass();
		~FFMpegClass();

	public:
		static FFMpegClass& Instance();

		void InitAv();
		void InitAvNetwork();
		void InitAvCodec();

		//TODO not used
		//void AddNewRtspStream(const char* uri);

		void SetRtspStream(const char* uri);

		//void ReadFrames();
		void ReadFrame();

		inline bool hasInitAv() { return m_hasInitAv; };
		inline bool hasInitAvCodec() { return m_hasInitAvCodec; };
		inline bool hasInitAvNetwork() { return m_hasInitAvNetwork; };

		FFMpegStream* getStream() { return m_pStream; };

	private:

		static FFMpegClass m_instance;
		//TODO use many stream?
		//std::set<FFMpegStream> m_vStream;

		FFMpegStream* m_pStream;

		bool m_hasInitAvCodec;
		bool m_hasInitAv;
		bool m_hasInitAvNetwork;

	};
}

