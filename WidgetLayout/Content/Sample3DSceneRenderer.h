#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include ".\WindowRect.h"

// This sample renderer instantiates a basic rendering pipeline.
class Sample3DSceneRenderer
{
public:
	Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	~Sample3DSceneRenderer();
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void Update(DX::StepTimer const& timer);
	bool Render();
	void SaveState();

	void StartTracking();
	void TrackingUpdate(float positionX);
	void StopTracking();
	bool IsTracking() { return m_tracking; }




// widget interface
	void ResetWidgetList();	// called at the begining of the widget list setup
	void AddColoredRectToList(const WindowRect& _WindowRect, DirectX::XMFLOAT4 _Color, UINT _TextureIndex);	// add widget to render
	void DisplayNumber(const int _NumberToDisplay);	// display this number in the debug window

private:	// func
	void LoadState();
	void Rotate(float radians);

	void LoadTexture(const std::wstring& _TextureName, Microsoft::WRL::ComPtr<ID3D12Resource>& _Texture, const CD3DX12_CPU_DESCRIPTOR_HANDLE& _CpuTextureRingHandle);

private:	// data
	// Constant buffers must be 256-byte aligned.
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	// Direct3D resources for cube geometry.
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_commandList;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>			m_pipelineState;

	
	// single global CBV descriptors heap: constants + imgui + texture*N

	static const UINT N_TEXTURES = 4;

	enum CBV_HEAP_STRUCTURE
	{
		CBV_CONSTANTS = 0,
		CBV_IMGUI = CBV_CONSTANTS + DX::c_frameCount,
		CBV_TEXTURE,
		CBV_TOTAL_NUMBER_OF_DESCRIPTORS = CBV_TEXTURE + N_TEXTURES,
	};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_cbvDescriptorHeap;


	Microsoft::WRL::ComPtr<ID3D12Resource>				m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>				m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource>				m_constantBuffer;
	ModelViewProjectionConstantBuffer					m_constantBufferData;
	UINT8*												m_mappedConstantBuffer;
	UINT												m_cbvDescriptorSize;
	D3D12_RECT											m_scissorRect;

	std::vector<byte>									m_vertexShader;
	std::vector<byte>									m_pixelShader;
	std::vector<byte>									m_geometryShader;

	D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
//		D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;
	float	m_radiansPerSecond;
	float	m_angle;
	bool	m_tracking;

	// textures
	Microsoft::WRL::ComPtr<ID3D12Resource>				m_Texture[N_TEXTURES];


	// vertex data
	VertexPositionColorTexture	*m_Vertices;

	UINT m_WidgetCount;

	int m_NumberToDisplay;

};

