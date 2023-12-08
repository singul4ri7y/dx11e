#ifndef _DX11E_GRAPHICS_
#define _DX11E_GRAPHICS_

#pragma once

#include <dx11ewin.hpp>
#include <DX11Exception.hpp>
#include <Window.hpp>
#include <d3d11.h>
#include <vector>
#include <ShaderList.hpp>
#include <array>
#include <dxerr.h>
#include <dxgi.h>

// For __uuidof
#include <_mingw.h>

DX11E_START

struct Vertex final {
	struct {
		FLOAT x, y;
	} Position;
	
	struct {
		unsigned char red, green, blue, alpha;
	} Color;
};

class DX11E_API GraphicsException final : public DX11Exception {
	
	public: 
		GraphicsException(int, const WCHAR*, const WCHAR*, HRESULT);
		const char* what() const noexcept override;
		const WCHAR* GetType() const;
		HRESULT GetErrorCode() const noexcept;
		void LoadDefaultShaders() const;
		const std::wstring GetErrorString() const;

	private:
		HRESULT hr;
		
};

class DX11E_API Graphics final {
	
	public: 
		Graphics();
		Graphics(const Graphics&) = delete;

		Graphics operator = (const Graphics&) = delete;

		~Graphics();

		BOOL Initialize(_In_ HWND, _In_ WindowDimention) noexcept(FALSE);
		void Present() const;
		void AttachShader(_In_ std::wstring, _In_ DX11E_SHADER_TYPE, _In_ DX11E_SHADER_SYSTEM, _In_ void*);
		void LoadDefaultShaders();
		void CreateInputLayout(_In_ D3D11_INPUT_ELEMENT_DESC*, _In_ UINT);
		BOOL SetInputLayout() const;
		void SetPrimitiveTopology(_In_ D3D_PRIMITIVE_TOPOLOGY) const;
		void SetViewport(_In_ UINT, _In_ UINT) const;
		void SetRenderTargets() const;
		void SetBufferColor(_In_ USHORT, _In_ USHORT, _In_ USHORT, _In_ USHORT) noexcept;
		void ClearBuffer() noexcept;
		
		void UseShaders(_In_ UINT);
		void UseAllShaders();
		
		ID3D11Device* const GetDevice() const;
		ID3D11DeviceContext* const GetDeviceContext() const;
	
	private: 
		USHORT m_bufferColor[4];
	
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11InputLayout* m_pInputLayout;
		
		BOOL m_inputLayoutInitialized;
		
		ShaderList m_shaders;
		
		WindowDimention m_windowDimention;
};

DX11E_END

#endif // !_DX11E_GRAPHICS_