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
	void AddColoredRectToList(const WindowRect& _WindowRect, DirectX::XMFLOAT4 _Color);	// add widget to render
	void DisplayNumber(const int _NumberToDisplay);	// display this number in the debug window





private:
	void LoadState();
	void Rotate(float radians);

private:
	// Constant buffers must be 256-byte aligned.
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	// Direct3D resources for cube geometry.
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_commandList;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>			m_pipelineState;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_cbvHeap;
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

	// imgui
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        g_pd3dSrvDescHeap;

	// vertex data
	VertexPositionColor	*m_Vertices;

	UINT m_WidgetCount;

	int m_NumberToDisplay;

};

