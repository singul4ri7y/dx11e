#ifndef _DX11E_SHADERLIST_
#define _DX11E_SHADERLIST_

#pragma once

#include <Shader.hpp>
#include <vector>

DX11E_START

typedef enum {
    DX11E_SHADER_SYSTEM_LOAD = 1,
    DX11E_SHADER_SYSTEM_LOAD_AND_COMPILE
} DX11E_SHADER_SYSTEM;

typedef struct {
    D3D_SHADER_MACRO* shaderMacro;
    std::string entryPoint;
    DX11E_SHADER_TARGET shaderTarget;
} DX11E_COMPILE_MACROS;

class DX11E_API ShaderList final {
    
    public: 
        ShaderList();
    
        ShaderList(_In_ ID3D11Device*, _In_ ID3D11DeviceContext*);
        
        void Use(_In_ UINT);
        void UseAll();
        void Add(_In_ std::wstring, _In_ DX11E_SHADER_TYPE, _In_ DX11E_SHADER_SYSTEM, _In_ void*);
        std::vector<Shader> GetShaderVector() const;
        
    private: 
        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
        
        std::vector<Shader> m_shaders;
    
};

DX11E_END

#endif // !_DX11E_SHADERLIST_