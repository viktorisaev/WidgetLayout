#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"
#include <ppltasks.h>
#include <synchapi.h>
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "Common/DDSTextureLoader.h"


using namespace Concurrency;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Storage;

// Indices into the application state map.
Platform::String^ AngleKey = "Angle";
Platform::String^ TrackingKey = "Tracking";

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_radiansPerSecond(XM_PI),	// rotate 45 degrees per second
	m_angle(0),
	m_tracking(false),
	m_mappedConstantBuffer(nullptr),
	m_deviceResources(deviceResources)
{
	LoadState();
	ZeroMemory(&m_constantBufferData, sizeof(m_constantBufferData));

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

Sample3DSceneRenderer::~Sample3DSceneRenderer()
{
	m_constantBuffer->Unmap(0, nullptr);
	m_mappedConstantBuffer = nullptr;
}

//const UINT NUMBER_OF_INDICES = 6;




const UINT MAX_WIDGET_COUNT = 1000;



//const float X = 0.5f * SCREEN_WIDTH - 1.0f;
//const float Y = 1.0f - 0.5f * SCREEN_HEIGHT;
//const float SX = 0.2f * SCREEN_WIDTH;
//const float SY = 0.2f * SCREEN_HEIGHT;

// Cube vertices. Each vertex has a position and a color.
VertexPositionColorTexture cubeVertices[MAX_WIDGET_COUNT];// =
//{
//	{ XMFLOAT3(X, Y,  0.0f),				XMFLOAT3(SX, SY, 0.0f),			XMFLOAT3(0.0f, 0.0f, 1.0f) },	// bottom left
//	{ XMFLOAT3(X,  Y,  0.0f),				XMFLOAT3(0.3f, 0.1f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 1.0f) },	// top left
//	{ XMFLOAT3(-1, 1,  0.0f),				XMFLOAT3(1.0f, 1.0f, 0.0f),		XMFLOAT3(1.0f, 0.0f, 1.0f) },	// bottom right
//	{ XMFLOAT3(X + SX,  Y,  0.0f),			XMFLOAT3(0.8f, 0.6f, 0.0f),		XMFLOAT3(1.0f, 1.0f, 0.0f) },	// top right
//};





// lifetime - till WaitForGpu finished
// Create an upload command list.
Microsoft::WRL::ComPtr<ID3D12CommandAllocator> uploadCommandAllocator;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> uploadCommandList;
ComPtr<ID3D12Resource> uploadHeap;


void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	// Create a root signature with a single constant buffer slot.
	{
		CD3DX12_DESCRIPTOR_RANGE rangeWidget;
		CD3DX12_DESCRIPTOR_RANGE rangeTexture;	// texture
		CD3DX12_ROOT_PARAMETER parameter[2];

		// shader registers
		const UINT SHADER_REGISTER_B0 = 0;
		const UINT SHADER_REGISTER_T0 = 0;
		const UINT SHADER_REGISTER_S0 = 0;

		rangeWidget.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, SHADER_REGISTER_B0);
		rangeTexture.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, N_TEXTURES, SHADER_REGISTER_T0);	// Texture2D texDiffuse[2] : register(t0);
		parameter[0].InitAsDescriptorTable(1, &rangeWidget, D3D12_SHADER_VISIBILITY_VERTEX);
		parameter[1].InitAsDescriptorTable(1, &rangeTexture, D3D12_SHADER_VISIBILITY_PIXEL);

		// sampler for the texture
		D3D12_STATIC_SAMPLER_DESC samplerRing = {};
		samplerRing.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerRing.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerRing.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerRing.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerRing.MipLODBias = 0;
		samplerRing.MaxAnisotropy = 0;
		samplerRing.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerRing.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerRing.MinLOD = 0.0f;
		samplerRing.MaxLOD = D3D12_FLOAT32_MAX;
		samplerRing.ShaderRegister = SHADER_REGISTER_S0;															// SamplerState textureSampler : register(s0);
		samplerRing.RegisterSpace = 0;
		samplerRing.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // Only the input assembler stage needs access to the constant buffer.
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
//			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
		descRootSignature.Init(2, parameter, 1, &samplerRing, rootSignatureFlags);

		ComPtr<ID3DBlob> pSignature;
		ComPtr<ID3DBlob> pError;
		DX::ThrowIfFailed(D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pError.GetAddressOf()));
		DX::ThrowIfFailed(d3dDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
        NAME_D3D12_OBJECT(m_rootSignature);
	}


	// Create a global single descriptor heap
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = CBV_TOTAL_NUMBER_OF_DESCRIPTORS;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		// This flag indicates that this descriptor heap can be bound to the pipeline and that descriptors contained in it can be referenced by a root table.
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DX::ThrowIfFailed(d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvDescriptorHeap)));
		NAME_D3D12_OBJECT(m_cbvDescriptorHeap);

		// should be initialized before heap operations!!!
		m_cbvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}



	// then - asynchroneous tasks ===========================================


	// Load shaders asynchronously.
	auto createVSTask = DX::ReadDataAsync(L"VertexShader.cso").then([this](std::vector<byte>& fileData) {
		m_vertexShader = fileData;
	});

	auto createPSTask = DX::ReadDataAsync(L"PixelShader.cso").then([this](std::vector<byte>& fileData) {
		m_pixelShader = fileData;
	});

	auto createGSTask = DX::ReadDataAsync(L"GeometryShader.cso").then([this](std::vector<byte>& fileData) {
		m_geometryShader = fileData;
	});


	// textures
	auto loadAllTexturesTask = create_task([this](){

		// textures file names ===============================================
		std::wstring texturesFileNames[N_TEXTURES] = {
		  L"Assets\\bkg_fill.DDS"
		, L"Assets\\bkg_box.DDS"
		, L"Assets\\bkg_exact.DDS"
		, L"Assets\\bkg_button.DDS"
		};

		for (int i = 0, ei = N_TEXTURES; i < ei; ++i)
		{
			LoadTexture(texturesFileNames[i], m_Texture[i], CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), CBV_TEXTURE + i, m_cbvDescriptorSize));
		}
	});





	// Create the pipeline state once the shaders are loaded.
	auto createPipelineStateTask = (createPSTask && createVSTask && createGSTask).then([this]() {

		static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 2, DXGI_FORMAT_R32_UINT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
		state.InputLayout = { inputLayout, _countof(inputLayout) };
		state.pRootSignature = m_rootSignature.Get();
        state.VS = CD3DX12_SHADER_BYTECODE(&m_vertexShader[0], m_vertexShader.size());
		state.PS = CD3DX12_SHADER_BYTECODE(&m_pixelShader[0], m_pixelShader.size());
		state.GS = CD3DX12_SHADER_BYTECODE(&m_geometryShader[0], m_geometryShader.size());
		state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		state.DepthStencilState.DepthEnable = false;											// DISABLE DEPTH TEST!!!
		state.SampleMask = UINT_MAX;
		state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		state.NumRenderTargets = 1;
		state.RTVFormats[0] = m_deviceResources->GetBackBufferFormat();
		state.DSVFormat = m_deviceResources->GetDepthBufferFormat();
		state.SampleDesc.Count = 1;
		state.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		state.BlendState.AlphaToCoverageEnable = false;
		state.BlendState.IndependentBlendEnable = false;
		state.BlendState.RenderTarget[0].BlendEnable = true;
		state.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		state.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		state.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		state.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		state.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		state.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		state.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&m_pipelineState)));

		// Shader data can be deleted once the pipeline state is created.
		m_vertexShader.clear();
		m_pixelShader.clear();
		m_geometryShader.clear();
	});

	// Create and upload cube geometry resources to the GPU.
	auto createAssetsTask = createPipelineStateTask.then([this]() {
		auto d3dDevice = m_deviceResources->GetD3DDevice();

		// Create a command list.
		DX::ThrowIfFailed(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
        NAME_D3D12_OBJECT(m_commandList);


		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);




		const UINT vertexBufferSize = sizeof(cubeVertices);

		// Create the vertex buffer resource in the GPU's default heap and copy vertex data into it using the upload heap.
		// The upload resource must not be released until after the GPU has finished using it.
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUpload;

		CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
		//DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		//	&defaultHeapProperties,
		//	D3D12_HEAP_FLAG_NONE,
		//	&vertexBufferDesc,
		//	D3D12_RESOURCE_STATE_COPY_DEST,
		//	nullptr,
		//	IID_PPV_ARGS(&m_vertexBuffer)));

		DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&vertexBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)
//			IID_PPV_ARGS(&vertexBufferUpload)
		));
        NAME_D3D12_OBJECT(m_vertexBuffer);

		// Upload the vertex buffer to the GPU.
		{
			// map
			CD3DX12_RANGE readRange(0, 0);
			m_vertexBuffer->Map(0, &readRange, &((void*)m_Vertices));

			// copy
			memcpy(m_Vertices, cubeVertices, sizeof(cubeVertices));

			// Create vertex buffer view
			m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
			m_vertexBufferView.StrideInBytes = sizeof(VertexPositionColorTexture);
			m_vertexBufferView.SizeInBytes = sizeof(cubeVertices);

			//D3D12_SUBRESOURCE_DATA vertexData = {};
			//vertexData.pData = reinterpret_cast<BYTE*>(cubeVertices);
			//vertexData.RowPitch = vertexBufferSize;
			//vertexData.SlicePitch = vertexData.RowPitch;
			//UpdateSubresources(m_commandList.Get(), m_vertexBuffer.Get(), vertexBufferUpload.Get(), 0, 0, 1, &vertexData);
			//m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
		}

		// Load mesh indices. Each trio of indices represents a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes 0, 2 and 1 from the vertex buffer compose the
		// first triangle of this mesh.
		//unsigned short cubeIndices[NUMBER_OF_INDICES] =
		//{
		//	0, 1, 3, // +z
		//	0, 3, 2,
		//};

		//const UINT indexBufferSize = sizeof(cubeIndices);

		//// Create the index buffer resource in the GPU's default heap and copy index data into it using the upload heap.
		//// The upload resource must not be released until after the GPU has finished using it.
		//Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUpload;

		//CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
		//DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		//	&defaultHeapProperties,
		//	D3D12_HEAP_FLAG_NONE,
		//	&indexBufferDesc,
		//	D3D12_RESOURCE_STATE_COPY_DEST,
		//	nullptr,
		//	IID_PPV_ARGS(&m_indexBuffer)));

		//DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		//	&uploadHeapProperties,
		//	D3D12_HEAP_FLAG_NONE,
		//	&indexBufferDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&indexBufferUpload)));

		//NAME_D3D12_OBJECT(m_indexBuffer);

		//// Upload the index buffer to the GPU.
		//{
		//	D3D12_SUBRESOURCE_DATA indexData = {};
		//	indexData.pData = reinterpret_cast<BYTE*>(cubeIndices);
		//	indexData.RowPitch = indexBufferSize;
		//	indexData.SlicePitch = indexData.RowPitch;

		//	UpdateSubresources(m_commandList.Get(), m_indexBuffer.Get(), indexBufferUpload.Get(), 0, 0, 1, &indexData);

		//	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));
		//}

		CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(DX::c_frameCount * c_alignedConstantBufferSize);
		DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&constantBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantBuffer)));

        NAME_D3D12_OBJECT(m_constantBuffer);

		// Create constant buffer views to access the upload buffer.
		D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(m_cbvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (int n = 0; n < DX::c_frameCount; n++)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			desc.BufferLocation = cbvGpuAddress;
			desc.SizeInBytes = c_alignedConstantBufferSize;
			d3dDevice->CreateConstantBufferView(&desc, cbvCpuHandle);

			cbvGpuAddress += desc.SizeInBytes;
			cbvCpuHandle.Offset(m_cbvDescriptorSize);
		}

		// Map the constant buffers.
		CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
		DX::ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer)));
		ZeroMemory(m_mappedConstantBuffer, DX::c_frameCount * c_alignedConstantBufferSize);
		// We don't unmap this until the app closes. Keeping things mapped for the lifetime of the resource is okay.

		// Close the command list and execute it to begin the vertex/index buffer copy into the GPU's default heap.
		DX::ThrowIfFailed(m_commandList->Close());
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_deviceResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);


		// Create index buffer view.
		//m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		//m_indexBufferView.SizeInBytes = sizeof(cubeIndices);
		//m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

		// imgui

		// do not create special DescriptorHeap, use global single one
		//D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//desc.NumDescriptors = 1;
		//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//m_deviceResources->GetD3DDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap));

		ImGui_ImplDX12_Init(NULL, DX::c_frameCount, m_deviceResources->GetD3DDevice(),
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cbvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), CBV_IMGUI, m_cbvDescriptorSize),
			CD3DX12_GPU_DESCRIPTOR_HANDLE( m_cbvDescriptorHeap->GetGPUDescriptorHandleForHeapStart() , CBV_IMGUI, m_cbvDescriptorSize)
		);

	});

	// finally wait for all tasks
	(loadAllTexturesTask/*createTextureTask*/ && createAssetsTask).then([this]() {

		// Wait for the command list to finish executing; the vertex/index buffers need to be uploaded to the GPU before the upload resources go out of scope.
		m_deviceResources->WaitForGpu();

		m_loadingComplete = true;
	});
}







void Sample3DSceneRenderer::LoadTexture(const std::wstring & _TextureName, Microsoft::WRL::ComPtr<ID3D12Resource>& _Texture, const CD3DX12_CPU_DESCRIPTOR_HANDLE& _CpuTextureRingHandle)
{

	// textures
	auto loadTextureTask = DX::ReadDataAsync(_TextureName);

	auto createTextureTask = loadTextureTask.then([this, &_Texture, &_CpuTextureRingHandle](const std::vector<byte>& ddsTextureData)
	{
		auto d3dDevice = m_deviceResources->GetD3DDevice();

		// from https://github.com/Microsoft/DirectXTK12/wiki/DDSTextureLoader

		d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&uploadCommandAllocator));
		NAME_D3D12_OBJECT(uploadCommandAllocator);

		DX::ThrowIfFailed(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, uploadCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&uploadCommandList)));
		NAME_D3D12_OBJECT(uploadCommandList);

		// create texture
		std::vector<D3D12_SUBRESOURCE_DATA> subresources;
		DX::ThrowIfFailed(DirectX::LoadDDSTextureFromMemory(d3dDevice, ddsTextureData.data(), ddsTextureData.size(), _Texture.ReleaseAndGetAddressOf(), subresources));
		NAME_D3D12_OBJECT(_Texture);

		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(_Texture.Get(), 0, static_cast<UINT>(subresources.size()));

		// Create the GPU upload buffer.
		DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadHeap.GetAddressOf())));

		UpdateSubresources(uploadCommandList.Get(), _Texture.Get(), uploadHeap.Get(), 0, 0, static_cast<UINT>(subresources.size()), subresources.data());
		uploadCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_Texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		D3D12_SHADER_RESOURCE_VIEW_DESC textureRingDesc = {};
		D3D12_RESOURCE_DESC desc = _Texture->GetDesc();

		textureRingDesc.Format = desc.Format;
		textureRingDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		if (desc.DepthOrArraySize > 1)
		{
			textureRingDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			textureRingDesc.Texture2DArray.MipLevels = (!desc.MipLevels) ? -1 : desc.MipLevels;
			textureRingDesc.Texture2DArray.ArraySize = static_cast<UINT>(desc.DepthOrArraySize);
		}
		else
		{
			textureRingDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			textureRingDesc.Texture2D.MipLevels = (!desc.MipLevels) ? -1 : desc.MipLevels;
		}

		assert(m_cbvDescriptorSize > 0);
		d3dDevice->CreateShaderResourceView(_Texture.Get(), &textureRingDesc, _CpuTextureRingHandle);

		// Close the command list and execute it to begin the vertex/index buffer copy into the GPU's default heap.
		DX::ThrowIfFailed(uploadCommandList->Close());
		ID3D12CommandList* ppCommandLists[] = { uploadCommandList.Get() };
		m_deviceResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		////// Wait for the command list to finish executing; the vertex/index buffers need to be uploaded to the GPU before the upload resources go out of scope.
		////m_deviceResources->WaitForGpu();
	});


	createTextureTask.wait();

}






// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	D3D12_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height)};

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	//XMStoreFloat4x4(
	//	&m_constantBufferData.projection,
	//	XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	//	);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	//static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	//static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	//static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	//XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (m_loadingComplete)
	{
		if (!m_tracking)
		{
			// Rotate the cube a small amount.
			m_angle += static_cast<float>(timer.GetElapsedSeconds()) * m_radiansPerSecond;

			Rotate(m_angle);
		}

		// Update the constant buffer resource.
		UINT8* destination = m_mappedConstantBuffer + (m_deviceResources->GetCurrentFrameIndex() * c_alignedConstantBufferSize);
		memcpy(destination, &m_constantBufferData, sizeof(m_constantBufferData));

		// copy widgets
		memcpy(m_Vertices, cubeVertices, sizeof(cubeVertices));


	}
}

// Saves the current state of the renderer.
void Sample3DSceneRenderer::SaveState()
{
	auto state = ApplicationData::Current->LocalSettings->Values;

	if (state->HasKey(AngleKey))
	{
		state->Remove(AngleKey);
	}
	if (state->HasKey(TrackingKey))
	{
		state->Remove(TrackingKey);
	}

	state->Insert(AngleKey, PropertyValue::CreateSingle(m_angle));
	state->Insert(TrackingKey, PropertyValue::CreateBoolean(m_tracking));
}

// Restores the previous state of the renderer.
void Sample3DSceneRenderer::LoadState()
{
	auto state = ApplicationData::Current->LocalSettings->Values;
	if (state->HasKey(AngleKey))
	{
		m_angle = safe_cast<IPropertyValue^>(state->Lookup(AngleKey))->GetSingle();
		state->Remove(AngleKey);
	}
	if (state->HasKey(TrackingKey))
	{
		m_tracking = safe_cast<IPropertyValue^>(state->Lookup(TrackingKey))->GetBoolean();
		state->Remove(TrackingKey);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader.
//	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}








// Renders one frame using the vertex and pixel shaders.
bool Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return false;
	}

	DX::ThrowIfFailed(m_deviceResources->GetCommandAllocator()->Reset());

	// The command list can be reset anytime after ExecuteCommandList() is called.
	DX::ThrowIfFailed(m_commandList->Reset(m_deviceResources->GetCommandAllocator(), m_pipelineState.Get()));

	PIXBeginEvent(m_commandList.Get(), 0, L"Draw the cube");
	{
		// Set the graphics root signature and descriptor heaps to be used by this frame.
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvDescriptorHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// set root parameters
		// Bind the current frame's constant buffer to the pipeline.
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), CBV_CONSTANTS + m_deviceResources->GetCurrentFrameIndex(), m_cbvDescriptorSize);
		m_commandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

		// bind texture to the pipeline
		CD3DX12_GPU_DESCRIPTOR_HANDLE textureGpuHandle(m_cbvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), CBV_TEXTURE, m_cbvDescriptorSize);
		m_commandList->SetGraphicsRootDescriptorTable(1, textureGpuHandle);



		// Set the viewport and scissor rectangle.
		D3D12_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
		m_commandList->RSSetViewports(1, &viewport);
		m_commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate this resource will be in use as a render target.
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		// Record drawing commands.
		D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = m_deviceResources->GetRenderTargetView();
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_deviceResources->GetDepthStencilView();
		m_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue, 0, nullptr);
		m_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		m_commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
//		m_commandList->IASetIndexBuffer(&m_indexBufferView);
//		m_commandList->DrawIndexedInstanced(NUMBER_OF_INDICES, 1, 0, 0, 0);
		m_commandList->DrawInstanced(_countof(cubeVertices),1,0,0);


		// ImGui
		Size outputSize = m_deviceResources->GetOutputSize();
		ImGui_ImplDX12_NewFrame(m_commandList.Get(), int(outputSize.Width), int(outputSize.Height));

		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{
			ImGui::SetNextWindowPos(ImVec2(900, 50));
			ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("London control");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("%i x %i", int(outputSize.Width), int(outputSize.Height));

			ImGui::Text("Number: %i", m_NumberToDisplay);

			ImGui::End();

			//ImVec4 clear_col = ImColor(114, 144, 154);
			//ImGui::ColorEdit3("clear color", (float*)&clear_col);
		}

		// set by m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//		m_commandList.Get()->SetDescriptorHeaps(1, g_pd3dSrvDescHeap.GetAddressOf());

		ImGui::Render();

		// Indicate that the render target will now be used to present when the command list is done executing.
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	}
	PIXEndEvent(m_commandList.Get());

	DX::ThrowIfFailed(m_commandList->Close());

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_deviceResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	return true;
}












// widget interface ///////////////////////////////////////////////////////////


const UINT REF_WIDTH = 1366;
const UINT REF_HEIGHT = 696;

const float SCREEN_WIDTH = 2.0f;
const float SCREEN_HEIGHT = 2.0f;



// called at the begining of the widget list setup
void Sample3DSceneRenderer::ResetWidgetList()
{
	m_WidgetCount = 0;
}




namespace {
	XMFLOAT2 PixelToScreen(const XMINT2 &p)
	{
		return XMFLOAT2(((float)p.x * SCREEN_WIDTH) / REF_WIDTH, ((float)p.y * SCREEN_HEIGHT) / REF_HEIGHT);
	}
}





// add widget to render
void Sample3DSceneRenderer::AddColoredRectToList(const WindowRect& _WindowRect, XMFLOAT4 _Color, UINT _TextureIndex)
{
	if (m_WidgetCount < MAX_WIDGET_COUNT)
	{
		XMFLOAT2 pos = PixelToScreen( XMINT2(_WindowRect.GetPosition().GetX(), _WindowRect.GetPosition().GetY()) );
		const WindowSize& winSize = _WindowRect.GetSize();
		XMFLOAT2 size = PixelToScreen(DirectX::XMINT2(winSize.GetWidth(), winSize.GetHeight()));
		XMFLOAT4 color = _Color;

		VertexPositionColorTexture v{ 
			XMFLOAT3(-(SCREEN_HEIGHT / 2) + pos.x, (SCREEN_HEIGHT / 2) - pos.y, 0.0f), 
			XMFLOAT3(size.x, size.y, 0.0f),
			XMFLOAT4(color.x, color.y, color.z, color.w), 
			_TextureIndex
		};

		cubeVertices[m_WidgetCount] = v;
		m_WidgetCount += 1;
	}
}





void Sample3DSceneRenderer::DisplayNumber(const int _NumberToDisplay)
{
	m_NumberToDisplay = _NumberToDisplay;
}





