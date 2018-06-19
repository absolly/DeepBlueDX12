#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include <string>
#include <GL/glew.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <wincodec.h>
#include "mge\core\Renderer.hpp"
class Texture
{
	public:
		static Texture* load(const std::string pTexturePath);

		virtual ~Texture();

		GLuint getId();


#ifdef API_DIRECTX
		static ID3D12DescriptorHeap* GetDescriptorHeap();
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ID3D12Resource* textureBuffer; //the resource heap containing our texture

#endif
	protected:

		//all previously loaded meshes;
		static std::map<std::string, Texture*> _textures;
        static Texture* _loadFromFile(const std::string pTexturePath);

	    //OpenGL id for texture buffer
		GLuint _id;

		Texture();

#ifdef API_DIRECTX
		static ID3D12DescriptorHeap* mainDescriptorHeap;
		static UINT mCbvSrvDescriptorSize;
		static UINT texCount;
		ID3D12Resource* textureBufferUploadHeap;

		BYTE* imageData;


		//load and decode image from file
		static int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int &bytesPerRow);

		//get DXGI format from the WIC format GUID
		static DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);

		//converted format for dxgi unknown format
		static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);

		//get the bit depth
		static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);

		static ID3D12Resource* CreateTextureDefaultBuffer(
			const void* initData,
			int bytesPerRow,
			D3D12_RESOURCE_DESC& textureDesc,
			ID3D12Resource*& uploadBuffer
		);

		static std::wstring s2ws(const std::string& s)
		{
			int len;
			int slength = (int)s.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
		}

#endif 

};

#endif // TEXTURE_H
