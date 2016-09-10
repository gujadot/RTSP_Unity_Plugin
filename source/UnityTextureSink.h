#pragma once
#include "MediaSink.h"
#include "RenderAPI.h"

#include "Unity/IUnityGraphics.h"


namespace rtsp_unity_plugin {

	static RenderAPI* s_CurrentAPI = NULL;
	static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;









	class UnityTextureSink
		: public MediaSink
	{
	private:
		const char *m_id;
		void* m_pTextureHandle;

		//todo : initialize current api and unityGfxRenderer

	public:
		UnityTextureSink(void* textureHandle, const char* id, int height, int width);
		~UnityTextureSink();

		int WriteAudio(AVFrame* audio_frame);
		int WriteVideo(AVFrame* Video_frame);
	};






}

