#pragma once
#include "MediaSink.h"
#include "RenderAPI.h"


namespace rtsp_unity_plugin {



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
		int WriteVideo(RenderAPI* render_api,AVFrame* Video_frame);
	private:
		void draw_plasma(RenderAPI * render_api);
	};







}

