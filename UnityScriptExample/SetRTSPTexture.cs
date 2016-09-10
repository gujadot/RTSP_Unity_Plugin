using UnityEngine;
using System;
using System.Collections;
using System.Runtime.InteropServices;


public class SetRTSPTexture : MonoBehaviour {

    [DllImport("RTSPUnityPlugin")]
    private static extern void SetTimeFromUnity(float t);
    [DllImport("RTSPUnityPlugin")]
    private static extern IntPtr GetRenderEventFunc();
    [DllImport("RTSPUnityPlugin")]
    private static extern void SetTextureAsRTSPSink(string rtsp_uri,System.IntPtr texture, int h, int w);


    string g_rtspUri = "rtsp://localhost:8554/stream";

    IEnumerator Start()
    {
        CreateTextureAndPassToPlugin();
        yield return StartCoroutine("CallPluginAtEndOfFrames");
    }

    private void CreateTextureAndPassToPlugin()
    {
        // Create a texture
        Texture2D tex = new Texture2D(256, 256, TextureFormat.RGBA32, false);
        // Set point filtering just so we can see the pixels clearly
        tex.filterMode = FilterMode.Point;
        // Call Apply() so it's actually uploaded to the GPU
        tex.Apply();

        // Set texture onto our material
        GetComponent<Renderer>().material.mainTexture = tex;

        // Pass texture pointer to the plugin
        SetTextureAsRTSPSink(g_rtspUri, tex.GetNativeTexturePtr(), tex.width, tex.height);
    }

    private IEnumerator CallPluginAtEndOfFrames()
    {
        while (true)
        {
            // Wait until all frame rendering is done
            yield return new WaitForEndOfFrame();

            // Set time for the plugin
            SetTimeFromUnity(Time.timeSinceLevelLoad);

            // Issue a plugin event with arbitrary integer identifier.
            // The plugin can distinguish between different
            // things it needs to do based on this ID.
            // For our simple plugin, it does not matter which ID we pass here.
            GL.IssuePluginEvent(GetRenderEventFunc(), 1);
        }
    }

    // Update is called once per frame
    void Update () {
	
	}
}
