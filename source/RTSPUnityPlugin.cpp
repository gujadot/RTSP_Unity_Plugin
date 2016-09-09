#include "PlateformeBase.h"
#include "Unity/IUnityGraphics.h"

// Frome Example : --------------------------------------------------------------------------
// SetTimeFromUnity, an example function we export which is called by one of the scripts.
static float g_Time;
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetTimeFromUnity(float t) { g_Time = t; }






