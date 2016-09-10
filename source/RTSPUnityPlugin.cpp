#include "PlatformBase.h"
#include "Unity/IUnityGraphics.h"


#include "FFMpegClass.h"
#include "UnityTextureSink.h"
#include <assert.h>

// Frome Example : --------------------------------------------------------------------------
// SetTimeFromUnity, an example function we export which is called by one of the scripts.
static float g_Time;
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTimeFromUnity(float t) { g_Time = t; }

// --------------------------------------------------------------------------
// UnitySetInterfaces

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

static IUnityInterfaces* s_UnityInterfaces = NULL;
static IUnityGraphics* s_Graphics = NULL;

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

	// Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
	s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}


// --------------------------------------------------------------------------
// GraphicsDeviceEvent

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	// Create graphics API implementation upon initialization
	if (eventType == kUnityGfxDeviceEventInitialize)
	{
		assert(rtsp_unity_plugin::s_CurrentAPI == NULL);
		rtsp_unity_plugin::s_DeviceType = s_Graphics->GetRenderer();
		rtsp_unity_plugin::s_CurrentAPI = CreateRenderAPI(rtsp_unity_plugin::s_DeviceType);
	}

	// Let the implementation process the device related events
	if (rtsp_unity_plugin::s_CurrentAPI)
	{
		rtsp_unity_plugin::s_CurrentAPI->ProcessDeviceEvent(eventType, s_UnityInterfaces);
	}

	// Cleanup graphics API implementation upon shutdown
	if (eventType == kUnityGfxDeviceEventShutdown)
	{
		delete rtsp_unity_plugin::s_CurrentAPI;
		rtsp_unity_plugin::s_CurrentAPI = NULL;
		rtsp_unity_plugin::s_DeviceType = kUnityGfxRendererNull;
	}
}

// --------------------------------------------------------------------------
// OnRenderEvent
// This will be called for GL.IssuePluginEvent script calls; eventID will
// be the integer passed to IssuePluginEvent. In this example, we just ignore
// that value.

rtsp_unity_plugin::FFMpegClass& ffmpegClassPtr = rtsp_unity_plugin::FFMpegClass::Instance();



static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
	// Unknown / unsupported graphics device type? Do nothing
	if (rtsp_unity_plugin::s_CurrentAPI == NULL)
		return;

	ffmpegClassPtr.ReadFrame();
}
// --------------------------------------------------------------------------
// GetRenderEventFunc, an example function we export which is used to get a rendering event callback function.

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc()
{
	return OnRenderEvent;
}

/*
// get the textureHandle, the size and width of the unity texture
// Return a pointer to the new mediaSink Object Created
extern "C" void* UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTextureAsMediaSink(void* texture_handle, int h, int w)
{
	return (void*) new rtsp_unity_plugin::UnityTextureSink(texture_handle, "textureName", h, w);
}

// get the rtsp uri
// open a ffmpegRtspStream with that uri
// Return a pointer to the new stream created
extern "C" void* UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API OpenRTSPStream(void* rtsp_uri)
{
	return (void*) new rtsp_unity_plugin::FFMpegRTSPStream((char*)rtsp_uri);
}


// get the rtsp uri
// open a ffmpegRtspStream with that uri
// Return a pointer to the new stream created

extern "C" void* UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API AddMediaSinkToStream(void* rtsp_stream, void* )
{
	return (void*) new rtsp_unity_plugin::FFMpegRTSPStream((char*)rtsp_uri);
}
*/

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTextureAsRTSPSink(void* texture_handle, int h, int w)
{
	ffmpegClassPtr.SetRtspStream("rtsp://localhost:8554/stream");//
	rtsp_unity_plugin::UnityTextureSink* sink = new rtsp_unity_plugin::UnityTextureSink(texture_handle, "texture", h, w);
	ffmpegClassPtr.getStream()->setMediaSink(sink);
}
