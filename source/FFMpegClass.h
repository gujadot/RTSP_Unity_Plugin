#pragma once

#include "ffmpegenv.h"

namespace rtsp_unity_plugin {

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

		inline bool hasInitAv() { return m_hasInitAv; };
		inline bool hasInitAvCodec() { return m_hasInitAvCodec; };
		inline bool hasInitAvNetwork() { return m_hasInitAvNetwork; };

	private:

		static FFMpegClass m_instance;
		FFMpegClass();
		~FFMpegClass();


		bool m_hasInitAvCodec;
		bool m_hasInitAv;
		bool m_hasInitAvNetwork;

	};
}

