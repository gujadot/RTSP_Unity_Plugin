#pragma once
#include "MediaSink.h"


#include "Unity/IUnityGraphics.h"


namespace rtsp_unity_plugin {

	class UnityTextureSink
		: public MediaSink
	{
	private:
		const char *m_id;
		void* m_textureHandle;

	public:
		UnityTextureSink(void* textureHandle, const char* id, int height, int width);
		~UnityTextureSink();

		int WriteAudio(AVFrame* audio_frame);
		int WriteVideo(AVFrame* Video_frame);
	};

}
