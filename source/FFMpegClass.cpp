#include "FFMpegClass.h"


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

rtsp_unity_plugin::FFMpegClass::FFMpegClass()
{
	m_hasInitAv = false;
	m_hasInitAvCodec = false;
	m_hasInitAvNetwork = false;
}

rtsp_unity_plugin::FFMpegClass::~FFMpegClass()
{
}

rtsp_unity_plugin::FFMpegClass& rtsp_unity_plugin::FFMpegClass::Instance()
{
	return m_instance;
}
