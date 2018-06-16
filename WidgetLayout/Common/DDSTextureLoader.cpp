//--------------------------------------------------------------------------------------
// File: DDSTextureLoader.cpp
//
// Functions for loading a DDS _Texture and creating a Direct3D runtime resource for it
//
// Note these functions are useful as a light-weight runtime loader for DDS files. For
// a full-featured DDS file reader, writer, and _Texture processing pipeline see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include "pch.h"

#include "DDSTextureLoader.h"

#include "dds.h"
#include "Common\DirectXHelper.h"
//#include "PlatformHelpers.h"
#include "LoaderHelpers.h"

namespace DirectX
{
	uint32_t CountMips(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return 0;

		uint32_t count = 1;
		while (width > 1 || height > 1)
		{
			width >>= 1;
			height >>= 1;
			count++;
		}
		return count;
	}
}

using namespace DirectX;
using namespace DirectX::LoaderHelpers;

static_assert(DDS_DIMENSION_TEXTURE1D == D3D12_RESOURCE_DIMENSION_TEXTURE1D, "dds mismatch");
static_assert(DDS_DIMENSION_TEXTURE2D == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "dds mismatch");
static_assert(DDS_DIMENSION_TEXTURE3D == D3D12_RESOURCE_DIMENSION_TEXTURE3D, "dds mismatch");

namespace
{

	//--------------------------------------------------------------------------------------
	HRESULT FillInitData(_In_ size_t width,
		_In_ size_t height,
		_In_ size_t depth,
		_In_ size_t mipCount,
		_In_ size_t arraySize,
		_In_ DXGI_FORMAT format,
		_In_ size_t maxsize,
		_In_ size_t bitSize,
		_In_reads_bytes_(bitSize) const uint8_t* bitData,
		_Out_ size_t& twidth,
		_Out_ size_t& theight,
		_Out_ size_t& tdepth,
		_Out_ size_t& skipMip,
		_Out_writes_(mipCount*arraySize) D3D12_SUBRESOURCE_DATA* initData)
	{
		if (!bitData || !initData)
		{
			return E_POINTER;
		}

		skipMip = 0;
		twidth = 0;
		theight = 0;
		tdepth = 0;

		size_t NumBytes = 0;
		size_t RowBytes = 0;
		const uint8_t* pSrcBits = bitData;
		const uint8_t* pEndBits = bitData + bitSize;

		size_t index = 0;
		for (size_t j = 0; j < arraySize; j++)
		{
			size_t w = width;
			size_t h = height;
			size_t d = depth;
			for (size_t i = 0; i < mipCount; i++)
			{
				GetSurfaceInfo(w,
					h,
					format,
					&NumBytes,
					&RowBytes,
					nullptr
				);

				if ((mipCount <= 1) || !maxsize || (w <= maxsize && h <= maxsize && d <= maxsize))
				{
					if (!twidth)
					{
						twidth = w;
						theight = h;
						tdepth = d;
					}

					assert(index < mipCount * arraySize);
					_Analysis_assume_(index < mipCount * arraySize);
					initData[index].pData = reinterpret_cast<const void*>(pSrcBits);
					initData[index].RowPitch = RowBytes;
					initData[index].SlicePitch = NumBytes;
					++index;
				}
				else if (!j)
				{
					// Count number of skipped mipmaps (first item only)
					++skipMip;
				}

				if (pSrcBits + (NumBytes*d) > pEndBits)
				{
					return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
				}

				pSrcBits += NumBytes * d;

				w = w >> 1;
				h = h >> 1;
				d = d >> 1;
				if (w == 0)
				{
					w = 1;
				}
				if (h == 0)
				{
					h = 1;
				}
				if (d == 0)
				{
					d = 1;
				}
			}
		}

		return (index > 0) ? S_OK : E_FAIL;
	}

	//--------------------------------------------------------------------------------------
	HRESULT CreateTextureResource(
		_In_ ID3D12Device* d3dDevice,
		D3D12_RESOURCE_DIMENSION resDim,
		size_t width,
		size_t height,
		size_t depth,
		size_t mipCount,
		size_t arraySize,
		DXGI_FORMAT format,
		D3D12_RESOURCE_FLAGS resFlags,
		unsigned int loadFlags,
		_Outptr_ ID3D12Resource** _Texture)
	{
		if (!d3dDevice)
			return E_POINTER;

		HRESULT hr = E_FAIL;

		if (loadFlags & DDS_LOADER_FORCE_SRGB)
		{
			format = MakeSRGB(format);
		}

		D3D12_RESOURCE_DESC desc = {};
		desc.Width = static_cast<UINT>(width);
		desc.Height = static_cast<UINT>(height);
		desc.MipLevels = static_cast<UINT16>(mipCount);
		desc.DepthOrArraySize = (resDim == D3D12_RESOURCE_DIMENSION_TEXTURE3D) ? static_cast<UINT16>(depth) : static_cast<UINT16>(arraySize);
		desc.Format = format;
		desc.Flags = resFlags;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Dimension = resDim;

		CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

		DX::ThrowIfFailed( d3dDevice->CreateCommittedResource( &defaultHeapProperties , D3D12_HEAP_FLAG_NONE , &desc , D3D12_RESOURCE_STATE_COPY_DEST , nullptr , IID_PPV_ARGS(_Texture)));
		_Analysis_assume_(*_Texture != 0);

		return NOERROR;
	}

	//--------------------------------------------------------------------------------------
	HRESULT CreateTextureFromDDS(_In_ ID3D12Device* d3dDevice,
		_In_ const DDS_HEADER* header,
		_In_reads_bytes_(bitSize) const uint8_t* bitData,
		size_t bitSize,
		size_t maxsize,
		D3D12_RESOURCE_FLAGS resFlags,
		unsigned int loadFlags,
		_Outptr_ ID3D12Resource** _Texture,
		std::vector<D3D12_SUBRESOURCE_DATA>& subresources,
		_Out_opt_ bool* outIsCubeMap)
	{
		HRESULT hr = S_OK;

		UINT width = header->width;
		UINT height = header->height;
		UINT depth = header->depth;

		D3D12_RESOURCE_DIMENSION resDim = D3D12_RESOURCE_DIMENSION_UNKNOWN;
		UINT arraySize = 1;
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		bool isCubeMap = false;

		size_t mipCount = header->mipMapCount;
		if (0 == mipCount)
		{
			mipCount = 1;
		}

		if ((header->ddspf.flags & DDS_FOURCC) &&
			(MAKEFOURCC('D', 'X', '1', '0') == header->ddspf.fourCC))
		{
			auto d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>((const char*)header + sizeof(DDS_HEADER));

			arraySize = d3d10ext->arraySize;
			if (arraySize == 0)
			{
				return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
			}

			switch (d3d10ext->dxgiFormat)
			{
			case DXGI_FORMAT_AI44:
			case DXGI_FORMAT_IA44:
			case DXGI_FORMAT_P8:
			case DXGI_FORMAT_A8P8:
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

			default:
				if (BitsPerPixel(d3d10ext->dxgiFormat) == 0)
				{
					return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
				}
			}

			format = d3d10ext->dxgiFormat;

			switch (d3d10ext->resourceDimension)
			{
			case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
				// D3DX writes 1D textures with a fixed Height of 1
				if ((header->flags & DDS_HEIGHT) && height != 1)
				{
					return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
				}
				height = depth = 1;
				break;

			case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
				if (d3d10ext->miscFlag & 0x4 /* RESOURCE_MISC_TEXTURECUBE */)
				{
					arraySize *= 6;
					isCubeMap = true;
				}
				depth = 1;
				break;

			case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
				if (!(header->flags & DDS_HEADER_FLAGS_VOLUME))
				{
					return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
				}

				if (arraySize > 1)
				{
					return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
				}
				break;

			default:
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}

			resDim = static_cast<D3D12_RESOURCE_DIMENSION>(d3d10ext->resourceDimension);
		}
		else
		{
			format = GetDXGIFormat(header->ddspf);

			if (format == DXGI_FORMAT_UNKNOWN)
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}

			if (header->flags & DDS_HEADER_FLAGS_VOLUME)
			{
				resDim = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}
			else
			{
				if (header->caps2 & DDS_CUBEMAP)
				{
					// We require all six faces to be defined
					if ((header->caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
					{
						return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
					}

					arraySize = 6;
					isCubeMap = true;
				}

				depth = 1;
				resDim = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

				// Note there's no way for a legacy Direct3D 9 DDS to express a '1D' _Texture
			}

			assert(BitsPerPixel(format) != 0);
		}

		// Bound sizes (for security purposes we don't trust DDS file metadata larger than the Direct3D hardware requirements)
		if (mipCount > D3D12_REQ_MIP_LEVELS)
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		switch (resDim)
		{
		case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
			if ((arraySize > D3D12_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) ||
				(width > D3D12_REQ_TEXTURE1D_U_DIMENSION))
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
			break;

		case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
			if (isCubeMap)
			{
				// This is the right bound because we set arraySize to (NumCubes*6) above
				if ((arraySize > D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
					(width > D3D12_REQ_TEXTURECUBE_DIMENSION) ||
					(height > D3D12_REQ_TEXTURECUBE_DIMENSION))
				{
					return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
				}
			}
			else if ((arraySize > D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
				(width > D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
				(height > D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION))
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
			break;

		case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
			if ((arraySize > 1) ||
				(width > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
				(height > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
				(depth > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
			break;

		default:
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		if (outIsCubeMap != nullptr)
		{
			*outIsCubeMap = isCubeMap;
		}

		// Create the _Texture
		std::vector<D3D12_SUBRESOURCE_DATA> initData;
		initData.resize(mipCount * arraySize);

		size_t skipMip = 0;
		size_t twidth = 0;
		size_t theight = 0;
		size_t tdepth = 0;
		hr = FillInitData(width, height, depth, mipCount, arraySize, format, maxsize, bitSize, bitData,
			twidth, theight, tdepth, skipMip, &initData[0]);

		if (SUCCEEDED(hr))
		{
			size_t reservedMips = mipCount;
			if (loadFlags & (DDS_LOADER_MIP_AUTOGEN | DDS_LOADER_MIP_RESERVE))
			{
				reservedMips = std::min<size_t>(D3D12_REQ_MIP_LEVELS, CountMips(width, height));
			}

			hr = CreateTextureResource(d3dDevice, resDim, twidth, theight, tdepth, reservedMips - skipMip, arraySize,
				format, resFlags, loadFlags, _Texture);

			if (FAILED(hr) && !maxsize && (mipCount > 1))
			{
				maxsize = (resDim == D3D12_RESOURCE_DIMENSION_TEXTURE3D)
					? D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION
					: D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION;

				hr = FillInitData(width, height, depth, mipCount, arraySize, format, maxsize, bitSize, bitData,
					twidth, theight, tdepth, skipMip, &initData[0]);
				if (SUCCEEDED(hr))
				{
					hr = CreateTextureResource(d3dDevice, resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize,
						format, resFlags, loadFlags, _Texture);
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			subresources.insert(subresources.end(), initData.begin(), initData.end());
		}

		return hr;
	}
} // anonymous namespace


namespace DirectX
{

//--------------------------------------------------------------------------------------
_Use_decl_annotations_
	HRESULT __cdecl DirectX::LoadDDSTextureFromMemory(
		_In_ ID3D12Device* d3dDevice,
		_In_reads_bytes_(ddsDataSize) const uint8_t* ddsData,
		size_t ddsDataSize,
		_Outptr_ ID3D12Resource** _Texture,
		std::vector<D3D12_SUBRESOURCE_DATA>& subresources,
		size_t maxsize,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode,
		_Out_opt_ bool* isCubeMap)
{
	return LoadDDSTextureFromMemoryEx(
		d3dDevice,
		ddsData,
		ddsDataSize,
		maxsize,
		D3D12_RESOURCE_FLAG_NONE,
		DDS_LOADER_DEFAULT,
		_Texture,
		subresources,
		alphaMode,
		isCubeMap);
}


_Use_decl_annotations_
	HRESULT __cdecl DirectX::LoadDDSTextureFromMemoryEx(
		_In_ ID3D12Device* d3dDevice,
		_In_reads_bytes_(ddsDataSize) const uint8_t* ddsData,
		size_t ddsDataSize,
		size_t maxsize,
		D3D12_RESOURCE_FLAGS resFlags,
		unsigned int loadFlags,
		_Outptr_ ID3D12Resource** _Texture,
		std::vector<D3D12_SUBRESOURCE_DATA>& subresources,
		_Out_opt_ DDS_ALPHA_MODE* alphaMode,
		_Out_opt_ bool* isCubeMap)
{
	if (_Texture)
	{
		*_Texture = nullptr;
	}
	if (alphaMode)
	{
		*alphaMode = DDS_ALPHA_MODE_UNKNOWN;
	}
	if (isCubeMap)
	{
		*isCubeMap = false;
	}

	if ( !d3dDevice || !ddsData || !_Texture )
	{
		return E_INVALIDARG;
	}

	// Validate DDS file in memory
	if (ddsDataSize < (sizeof(uint32_t) + sizeof(DDS_HEADER)))
	{
		return E_FAIL;
	}

	uint32_t dwMagicNumber = *(const uint32_t*)(ddsData);
	if (dwMagicNumber != DDS_MAGIC)
	{
		return E_FAIL;
	}

	auto header = reinterpret_cast<const DDS_HEADER*>(ddsData + sizeof(uint32_t));

	// Verify header to validate DDS file
	if (header->size != sizeof(DDS_HEADER) ||
		header->ddspf.size != sizeof(DDS_PIXELFORMAT))
	{
		return E_FAIL;
	}

	// Check for DX10 extension
	bool bDXT10Header = false;
	if ((header->ddspf.flags & DDS_FOURCC) &&
		(MAKEFOURCC('D', 'X', '1', '0') == header->ddspf.fourCC))
	{
		// Must be long enough for both headers and magic value
		if (ddsDataSize < (sizeof(DDS_HEADER) + sizeof(uint32_t) + sizeof(DDS_HEADER_DXT10)))
		{
			return E_FAIL;
		}

		bDXT10Header = true;
	}

	ptrdiff_t offset = sizeof(uint32_t)	+ sizeof(DDS_HEADER) + (bDXT10Header ? sizeof(DDS_HEADER_DXT10) : 0);
	DX::ThrowIfFailed(CreateTextureFromDDS(d3dDevice, header, ddsData + offset, ddsDataSize - offset, maxsize, resFlags, loadFlags, _Texture, subresources, isCubeMap));


	if (alphaMode)
	{
		*alphaMode = GetAlphaMode(header);
	}

	return NOERROR;
}


}
