#pragma once
#include "FFMpegStream.h"



namespace rtsp_unity_plugin {

	class FFMpegRTSPStream : public FFMpegStream
	{
	public:
		FFMpegRTSPStream(const char* uri);
		~FFMpegRTSPStream();
		
		int InitStream();
		int CloseStream();
		
	};

}