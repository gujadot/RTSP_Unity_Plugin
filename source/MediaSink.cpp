#include "MediaSink.h"


/**
// TODO INIT THE SINK HERE! 
// set the dst size frame to default if necessary : 
if (m_dstHeight == 0 && m_dstWidth == 0) {
	m_dstHeight = m_pCodecCtx->height;
	m_dstWidth = m_pCodecCtx->width;
}


// Determine required buffer size and allocate buffer
uint8_t *buffer = NULL;
int numBytes;
numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, m_pCodecCtx->width, m_pCodecCtx->height, 1);
buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));





*/

rtsp_unity_plugin::MediaSink::MediaSink(int height, int width, AVPixelFormat format)
{
	m_Height = height;
	m_Width = width;
	m_dstFormat = format;

	int numBytes;
	// Determine required buffer size and allocate buffer
	numBytes = av_image_get_buffer_size(m_dstFormat,m_Width,m_Height, 1);
	m_pBuffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in dstFrame
	// Note that dstFrame is an AVFrame, but AVFrame is a superset
	// of AVPicture
	int dstSizeByte = av_image_fill_arrays(m_pFrameDst->data, m_pFrameDst->linesize, m_pBuffer, m_dstFormat, m_Width, m_Height, 1);

}
//m_pSrcCodecContext = p_src_codec_context;
rtsp_unity_plugin::MediaSink::~MediaSink()
{
	m_pSrcCodecContext = NULL;
	av_free(m_pBuffer);
	av_free(m_pFrameDst);
	delete m_pSwsContext;
}

int rtsp_unity_plugin::MediaSink::setSrcCodecContext(AVCodecContext * p_src_codec_context)
{
	m_pSrcCodecContext = p_src_codec_context;
	// initialize SWS context for software scaling
	//NOTE: this function is to be removed after a saner alternative is written
	//about to get DEPRECATED
	m_pSwsContext = sws_getContext(m_pSrcCodecContext->width,
		m_pSrcCodecContext->height,
		m_pSrcCodecContext->pix_fmt,
		m_Width,
		m_Height,
		m_dstFormat,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);

	return 0;
}
