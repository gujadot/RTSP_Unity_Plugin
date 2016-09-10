#include "FFMpegClass.h"
#include "FFMpegRTSPStream.h"

rtsp_unity_plugin::FFMpegClass rtsp_unity_plugin::FFMpegClass::m_instance = rtsp_unity_plugin::FFMpegClass();


void rtsp_unity_plugin::FFMpegClass::InitAv()
{
	if (!m_hasInitAv)
		av_register_all();
	m_hasInitAv = true;
}

void rtsp_unity_plugin::FFMpegClass::InitAvCodec()
{
	if (!m_hasInitAvCodec)
		avcodec_register_all();
	m_hasInitAvCodec = true;
}
void rtsp_unity_plugin::FFMpegClass::InitAvNetwork()
{
	if (!m_hasInitAvNetwork)
		avformat_network_init();
	m_hasInitAvNetwork = true;
}



void rtsp_unity_plugin::FFMpegClass::SetRtspStream(const char * uri)
{
	if (m_pStream != NULL)
		delete m_pStream;
	m_pStream = new FFMpegRTSPStream(uri);
}
void rtsp_unity_plugin::FFMpegClass::ReadFrame()
{
	if (m_pStream != NULL)
		m_pStream->ReadFrame();
}
/*/
void rtsp_unity_plugin::FFMpegClass::AddNewRtspStream(const char * uri)
{
	m_vStream.insert(FFMpegRTSPStream(uri));
}*/


rtsp_unity_plugin::FFMpegClass::FFMpegClass()
{
	m_hasInitAv = false;
	m_hasInitAvCodec = false;
	m_hasInitAvNetwork = false;
	m_pStream = NULL;
}

rtsp_unity_plugin::FFMpegClass::~FFMpegClass()
{
}

rtsp_unity_plugin::FFMpegClass& rtsp_unity_plugin::FFMpegClass::Instance()
{
	return m_instance;
}
