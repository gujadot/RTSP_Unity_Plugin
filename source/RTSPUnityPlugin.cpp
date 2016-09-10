#include "PlatformBase.h"
#include "Unity/IUnityGraphics.h"
#include <assert.h>

#include "RTSPUnityPluginSingleton.h"
#include "UnityTextureSink.h"

// Frome Example : --------------------------------------------------------------------------
// SetTimeFromUnity, an example function we export which is called by one of the scripts.

rtsp_unity_plugin::RTSPPluginSingleton& pluginSinglet = rtsp_unity_plugin::RTSPPluginSingleton::Instance();
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTimeFromUnity(float t) { pluginSinglet.setTime(t); }

// --------------------------------------------------------------------------
// UnitySetInterfaces

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;
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
		assert(pluginSinglet.getRenderApi() == NULL);
		s_DeviceType = s_Graphics->GetRenderer();
		RenderAPI* renderApi = CreateRenderAPI(s_DeviceType);
		pluginSinglet.SetRenderApi(renderApi);
	}

	// Let the implementation process the device related events
	if (pluginSinglet.getRenderApi())
	{
		pluginSinglet.getRenderApi()->ProcessDeviceEvent(eventType, s_UnityInterfaces);
	}

	// Cleanup graphics API implementation upon shutdown
	if (eventType == kUnityGfxDeviceEventShutdown)
	{
		//delete s_CurrentAPI;
		//s_CurrentAPI = NULL;
		s_DeviceType = kUnityGfxRendererNull;
	}
}

// --------------------------------------------------------------------------
// OnRenderEvent
// This will be called for GL.IssuePluginEvent script calls; eventID will
// be the integer passed to IssuePluginEvent. In this example, we just ignore
// that value.



static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
	// Unknown / unsupported graphics device type? Do nothing
	if (pluginSinglet.getRenderApi() == NULL)
		return;

	pluginSinglet.ReadFrame();
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

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTextureAsRTSPSink(const char* rtsp_uri,void* texture_handle, int h, int w)
{
	char* uri = pluginSinglet.MakeStringCopy(rtsp_uri);
	pluginSinglet.SetRtspStream(uri);//"rtsp://localhost:8554/stream"
	rtsp_unity_plugin::UnityTextureSink* sink = new rtsp_unity_plugin::UnityTextureSink(texture_handle, "texture", h, w);
	//ffmpegClassPtr.SetDummySink(sink);
	pluginSinglet.getStream()->setMediaSink(sink);
}
