#ifndef _DX11E_SHADER_
#define _DX11E_SHADER_

#pragma once

#include <dx11ewin.hpp>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

typedef enum DX11E_SHADER_TYPE {
	DX11E_SHADER_TYPE_VERTEX = 1,
	DX11E_SHADER_TYPE_PIXEL
} DX11E_SHADER_TYPE;

typedef struct DX11E_SHADER_STAT {
	ID3DBlob* pError;
	HRESULT hResult;
	const WCHAR* message;
	BOOL success;
} DX11E_SHADER_STAT;

typedef struct DX11E_SHADER_TARGET {
	struct {
		INT MAJOR = 4,
			MINOR = 0;
	} Model;
	
	struct {
		INT MAJOR = 9,
			MINOR = 0;
	} Level;
} DX11E_SHADER_TARGET;

DX11E_START

class DX11E_API Shader final {
	
	public: 
		Shader();
		
		Shader(_In_ DX11E_SHADER_TYPE, _In_ std::wstring, _In_ ID3D11Device*, _In_ ID3D11DeviceContext*);
		
		DX11E_SHADER_STAT Load();
		DX11E_SHADER_STAT LoadAndCompile(_In_ const D3D_SHADER_MACRO*, _In_ std::string, _In_ DX11E_SHADER_TARGET);
		void Use();
		DX11E_SHADER_TYPE GetShaderType() const;
		
		void* GetBufferPointer() const;
		SIZE_T GetBufferSize() const;
	
	private: 
		HRESULT LoadShader();
	
	private: 
		ID3DBlob* m_pShaderBlob;
		ID3D11DeviceChild* m_shader;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		DX11E_SHADER_TYPE m_shaderType;
		std::wstring m_source;
};

DX11E_END

#endif // !_DX11E_SHADER_