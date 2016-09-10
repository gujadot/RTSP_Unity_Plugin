#include "RTSPUnityPluginSingleton.h"
#include "FFMpegRTSPStream.h"

rtsp_unity_plugin::RTSPPluginSingleton rtsp_unity_plugin::RTSPPluginSingleton::m_instance = rtsp_unity_plugin::RTSPPluginSingleton();


void rtsp_unity_plugin::RTSPPluginSingleton::InitAv()
{
	if (!m_hasInitAv)
		av_register_all();
	m_hasInitAv = true;
}

void rtsp_unity_plugin::RTSPPluginSingleton::InitAvCodec()
{
	if (!m_hasInitAvCodec)
		avcodec_register_all();
	m_hasInitAvCodec = true;
}
void rtsp_unity_plugin::RTSPPluginSingleton::InitAvNetwork()
{
	if (!m_hasInitAvNetwork)
		avformat_network_init();
	m_hasInitAvNetwork = true;
}



void rtsp_unity_plugin::RTSPPluginSingleton::SetRtspStream(const char * uri)
{
	if (m_pStream != NULL)
		delete m_pStream;
	m_pStream = new FFMpegRTSPStream(uri);
}
// todo delete that : 


void rtsp_unity_plugin::RTSPPluginSingleton::ReadFrame()
{
	if (m_pStream != NULL)
		m_pStream->ReadFrame();
}
/*/
void rtsp_unity_plugin::FFMpegClass::AddNewRtspStream(const char * uri)
{
	m_vStream.insert(FFMpegRTSPStream(uri));
}*/


rtsp_unity_plugin::RTSPPluginSingleton::RTSPPluginSingleton()
{
	m_hasInitAv = false;
	m_hasInitAvCodec = false;
	m_hasInitAvNetwork = false;
	m_pStream = NULL;
}

rtsp_unity_plugin::RTSPPluginSingleton::~RTSPPluginSingleton()
{
	if (m_pRenderApi!= NULL) {
		delete m_pRenderApi;
	}
}

rtsp_unity_plugin::RTSPPluginSingleton& rtsp_unity_plugin::RTSPPluginSingleton::Instance()
{
	return m_instance;
}
