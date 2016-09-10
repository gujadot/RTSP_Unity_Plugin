#include "FFMpegRTSPStream.h"
#include "RTSPUnityPluginSingleton.h"



rtsp_unity_plugin::FFMpegRTSPStream::FFMpegRTSPStream(const char* uri) :
	FFMpegStream(uri)
{
	InitStream();
}


rtsp_unity_plugin::FFMpegRTSPStream::~FFMpegRTSPStream()
{
	CloseStream();
}

int rtsp_unity_plugin::FFMpegRTSPStream::InitStream()
{
	// init libav, codec and network
	RTSPPluginSingleton& rtspPluginSinglet = RTSPPluginSingleton::Instance();
	rtspPluginSinglet.InitAv();
	rtspPluginSinglet.InitAvCodec();
	rtspPluginSinglet.InitAvNetwork();

	AVCodecParameters *pCodecPar = NULL;
	m_pFormatCtx = avformat_alloc_context();
	//get context and format 
	if (avformat_open_input(&m_pFormatCtx, m_pStreamUri, NULL, NULL) < 0) {
		avformat_free_context(m_pFormatCtx);
		return -1; // error, init faild, return -1
	}
	if (avformat_find_stream_info(m_pFormatCtx, NULL)) {
		avformat_free_context(m_pFormatCtx);
		return -2;// error, codec not found, return -2
	}
	//search video stream

	for (int i = 0; i<m_pFormatCtx->nb_streams; i++)
	{
		if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_VideoStreamIndex = i;
		}

		else if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			m_AudioStreamIndex = i;
		}
	}

	//init packet
	av_init_packet(&m_packet);

	//play RTSP
	av_read_play(m_pFormatCtx);

	// Get a pointer to the codec parameter for the video stream
	pCodecPar = m_pFormatCtx->streams[m_VideoStreamIndex]->codecpar;
	// Find the decoder for the video stream
	m_pCodec = avcodec_find_decoder(pCodecPar->codec_id);
	if (m_pCodec == NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}
	// set context to codecContext (create new codec_context)
	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	if (avcodec_parameters_to_context(m_pCodecCtx, pCodecPar) < 0) {
		fprintf(stderr, "Couldn't copy codec context");
		return -1; // Error copying codec context
	}
	// Open codec
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL)<0)
		return -1; // Could not open codec

				   // Allocate video frame
				   // Allocate an AVFrame structure
	m_pFrameSrc = av_frame_alloc();

	m_hasInit = true;
	m_isClosed = false;

	return 0;
}

int rtsp_unity_plugin::FFMpegRTSPStream::CloseStream()
{
	if (!isClosed() && hasInit()) {
		// free the source frame
		av_free(m_pFrameSrc);
		// Close the codecs
		avcodec_close(m_pCodecCtx);
		// free the format context
		avformat_free_context(m_pFormatCtx);
		m_isClosed = true;
		return 0;
	}
	else if (!hasInit()) {
		return 1; // 1= not yet initialized
	}
	else {
		return 2; // already closed;
	}

}
