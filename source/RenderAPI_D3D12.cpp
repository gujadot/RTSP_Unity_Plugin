#include "RenderAPI.h"
#include "PlatformBase.h"

// Direct3D 12 implementation of RenderAPI.

//TODO : FIXME
#if SUPPORT_D3D12

#include <assert.h>
#include <d3d12.h>
#include "Unity/IUnityGraphicsD3D12.h"


class RenderAPI_D3D12 : public RenderAPI
{
public:
	RenderAPI_D3D12();
	virtual ~RenderAPI_D3D12() { }

	virtual void ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces);

	virtual void* BeginModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int* outRowPitch);
	virtual void EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int rowPitch, void* dataPtr);

private:
	IUnityGraphicsD3D12v2* s_D3D12;
	ID3D12Resource* s_D3D12Upload;
	ID3D12CommandAllocator* s_D3D12CmdAlloc;
	ID3D12GraphicsCommandList* s_D3D12CmdList;
	UINT64 s_D3D12FenceValue = 0;
	HANDLE s_D3D12Event = NULL;
};


RenderAPI* CreateRenderAPI_D3D12()
{
	return new RenderAPI_D3D12();
}


const UINT kNodeMask = 0;


RenderAPI_D3D12::RenderAPI_D3D12()
	: s_D3D12(NULL)
	, s_D3D12Upload(NULL)
	, s_D3D12CmdAlloc(NULL)
	, s_D3D12CmdList(NULL)
	, s_D3D12FenceValue(0)
	, s_D3D12Event(NULL)
{
}


void RenderAPI_D3D12::ProcessDeviceEvent(UnityGfxDeviceEventType type, IUnityInterfaces* interfaces)
{
	switch (type)
	{
	case kUnityGfxDeviceEventInitialize:
		s_D3D12 = interfaces->Get<IUnityGraphicsD3D12v2>();
		break;
	case kUnityGfxDeviceEventShutdown:
		break;
	}
}



void* RenderAPI_D3D12::BeginModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int* outRowPitch)
{
	ID3D12Fence* fence = s_D3D12->GetFrameFence();

	// Wait on the previous job (example only - simplifies resource management)
	if (fence->GetCompletedValue() < s_D3D12FenceValue)
	{
		fence->SetEventOnCompletion(s_D3D12FenceValue, s_D3D12Event);
		WaitForSingleObject(s_D3D12Event, INFINITE);
	}

	// Begin a command list
	s_D3D12CmdAlloc->Reset();
	s_D3D12CmdList->Reset(s_D3D12CmdAlloc, nullptr);

	// Fill data
	const UINT64 kDataSize = textureWidth * textureHeight * 4;
	ID3D12Resource* upload = GetUploadResource(kDataSize);
	void* mapped = NULL;
	upload->Map(0, NULL, &mapped);
	*outRowPitch = textureWidth * 4;
	return mapped;
}


void RenderAPI_D3D12::EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int rowPitch, void* dataPtr)
{
	ID3D12Device* device = s_D3D12->GetDevice();

	const UINT64 kDataSize = textureWidth * textureHeight * 4;
	ID3D12Resource* upload = GetUploadResource(kDataSize);
	upload->Unmap(0, NULL);

	ID3D12Resource* resource = (ID3D12Resource*)textureHandle;
	D3D12_RESOURCE_DESC desc = resource->GetDesc();
	assert(desc.Width == textureWidth);
	assert(desc.Height == textureHeight);

	D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
	srcLoc.pResource = upload;
	srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	device->GetCopyableFootprints(&desc, 0, 1, 0, &srcLoc.PlacedFootprint, nullptr, nullptr, nullptr);

	D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
	dstLoc.pResource = resource;
	dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLoc.SubresourceIndex = 0;

	// We inform Unity that we expect this resource to be in D3D12_RESOURCE_STATE_COPY_DEST state,
	// and because we do not barrier it ourselves, we tell Unity that no changes are done on our command list.
	UnityGraphicsD3D12ResourceState resourceState = {};
	resourceState.resource = resource;
	resourceState.expected = D3D12_RESOURCE_STATE_COPY_DEST;
	resourceState.current = D3D12_RESOURCE_STATE_COPY_DEST;

	// Queue data upload
	s_D3D12CmdList->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);

	// Execute the command list
	s_D3D12CmdList->Close();
	s_D3D12FenceValue = s_D3D12->ExecuteCommandList(s_D3D12CmdList, 1, &resourceState);
}


#endif // #if SUPPORT_D3D12
