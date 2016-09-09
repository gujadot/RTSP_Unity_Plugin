#include "UnityTextureSink.h"




rtsp_unity_plugin::UnityTextureSink::UnityTextureSink(void* textureHandle, const char * id, int height, int width)
	: MediaSink(height,width,AV_PIX_FMT_RGB32)
{
	m_id = id;
	m_textureHandle = textureHandle;
}

rtsp_unity_plugin::UnityTextureSink::~UnityTextureSink()
{
}

int rtsp_unity_plugin::UnityTextureSink::WriteAudio(AVFrame * audio_frame)
{
	//not yet implemented
	return 0;
}

int rtsp_unity_plugin::UnityTextureSink::WriteVideo(AVFrame * Video_frame)
{
	//
	return 0;
}
