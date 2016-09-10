#include "UnityTextureSink.h"
#include "RTSPUnityPluginSingleton.h"

rtsp_unity_plugin::UnityTextureSink::UnityTextureSink(void* textureHandle, const char * id, int height, int width)
	: MediaSink(height,width, AV_PIX_FMT_RGBA)
{
	//AV_PIX_FMT_ARGB != textureformat ARGB32
	//AV_PIX_FMT_RGBA ?= textureformat RGBA32
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
//to test texture manipulation
void rtsp_unity_plugin::UnityTextureSink::draw_plasma(RenderAPI* render_api) {

	int width = m_Width;
	int height = m_Height;
	if (!m_pTextureHandle)
		return;

	int textureRowPitch;
	void* textureDataPtr = (unsigned char*)render_api->BeginModifyTexture(m_pTextureHandle, width, height, &textureRowPitch);
	if (!textureDataPtr)
		return;

	rtsp_unity_plugin::RTSPPluginSingleton& ffmpegClassPtr = rtsp_unity_plugin::RTSPPluginSingleton::Instance();
	const float t = ffmpegClassPtr.getTime() *4.0f;

	unsigned char* dst = (unsigned char*)textureDataPtr;
	for (int y = 0; y < height; ++y)
	{
		unsigned char* ptr = dst;
		for (int x = 0; x < width; ++x)
		{
			// Simple "plasma effect": several combined sine waves
			int vv = int(
				(127.0f + (127.0f * sinf(x / 7.0f + t))) +
				(127.0f + (127.0f * sinf(y / 5.0f - t))) +
				(127.0f + (127.0f * sinf((x + y) / 6.0f - t))) +
				(127.0f + (127.0f * sinf(sqrtf(float(x*x + y*y)) / 4.0f - t)))
				) / 4;

			// Write the texture pixel
			ptr[0] = vv;
			ptr[1] = 0;
			ptr[2] = 0;
			ptr[3] = 127;

			// To next pixel (our pixels are 4 bpp)
			ptr += 4;
		}

		// To next image row
		dst += textureRowPitch;
	}
	render_api->EndModifyTexture(m_pTextureHandle, m_Width, m_Height, textureRowPitch, textureDataPtr);

}

int rtsp_unity_plugin::UnityTextureSink::WriteVideo(RenderAPI* render_api, AVFrame * Video_frame)
{

	int test = 0;

	//draw_plasma(render_api);
	//return 0;

	if (!m_pTextureHandle)
		return -2;
	//TODO adjust time here
	// Convert the image from its native format to RGB
	sws_scale(
		m_pSwsContext,
		(const uint8_t* const*)Video_frame->data,
		Video_frame->linesize,
		0, 
		Video_frame->height,
		m_pFrameDst->data, 
		m_pFrameDst->linesize);


	int textureRowPitch;
	void* textureDataPtr = (uint8_t*)render_api->BeginModifyTexture(m_pTextureHandle, m_Width, m_Height, &textureRowPitch);
	if (!textureDataPtr)
		return -1;

	unsigned char* dst = (unsigned char*)textureDataPtr;
	unsigned char* src = (unsigned char*)m_pFrameDst->data[0];

	for (int y = 0; y < m_Height; ++y)
	{
		unsigned char* ptr_dst = dst;
		unsigned char* ptr_src = src;
		for (int x = 0; x < m_Width; ++x)
		{
			// Simple "plasma effect": several combined sine waves

			// Write the texture pixel
			ptr_dst[0] = ptr_src[0];
			ptr_dst[1] = ptr_src[1];
			ptr_dst[2] = ptr_src[2];
			ptr_dst[3] = 127;

			// To next pixel (our pixels are 4 bpp)
			ptr_dst += 4;
			ptr_src += 4;
		}
		// To next image row
		dst += textureRowPitch;
		src += m_pFrameDst->linesize[0];
	}

	render_api->EndModifyTexture(m_pTextureHandle, m_Width, m_Height, textureRowPitch, textureDataPtr );
	
	return test;

}

