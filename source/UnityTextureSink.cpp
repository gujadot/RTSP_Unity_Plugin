#include "UnityTextureSink.h"




rtsp_unity_plugin::UnityTextureSink::UnityTextureSink(void* textureHandle, const char * id, int height, int width)
	: MediaSink(height,width, AV_PIX_FMT_YUYV422)
{
	//AV_PIX_FMT_ARGB != textureformat ARGB32
	m_id = id;
	m_pTextureHandle = textureHandle;

}

rtsp_unity_plugin::UnityTextureSink::~UnityTextureSink()
{
}

int rtsp_unity_plugin::UnityTextureSink::WriteAudio(AVFrame * audio_frame)
{
	//TODO : not yet implemented
	return 0;
}

int rtsp_unity_plugin::UnityTextureSink::WriteVideo(AVFrame * Video_frame)
{
	
	// get all information need 
	if (!m_pTextureHandle)
		return -1;

	//TODO adjust time here
	// Convert the image from its native format to RGB
	sws_scale(m_pSwsContext,
		(const uint8_t* const*)Video_frame->data,
		Video_frame->linesize, 0, m_Height,
		m_pFrameDst->data, m_pFrameDst->linesize);


	int textureRowPitch;
	void* textureDataPtr = (unsigned char*)s_CurrentAPI->BeginModifyTexture(m_pTextureHandle, m_Width, m_Height, &textureRowPitch);
	if (!textureDataPtr)
		return -1;

	textureDataPtr = (void*)m_pFrameDst->data[0];

	s_CurrentAPI->EndModifyTexture(m_pTextureHandle, m_Width, m_Height, m_pFrameDst->linesize[0], textureDataPtr );
	
	return 0;
}
