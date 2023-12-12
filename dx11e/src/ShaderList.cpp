#include <Graphics.hpp>

DX11E_START

ShaderList::ShaderList() : 
    m_pDevice(nullptr),
    m_pDeviceContext(nullptr)
{}

ShaderList::ShaderList(_In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pDeviceContext): 
    m_pDevice(pDevice),
    m_pDeviceContext(pDeviceContext)
{}
        
void ShaderList::Use(_In_ UINT index) {
    this -> m_shaders[index].Use();
}

void ShaderList::UseAll() {
    for(Shader shader : this -> m_shaders) 
        shader.Use();
}

void ShaderList::Add(_In_ std::wstring source, _In_ DX11E_SHADER_TYPE shaderType, _In_ DX11E_SHADER_SYSTEM shaderSystem, void* compileMacros) {
    Shader shader(shaderType, source, this -> m_pDevice, this -> m_pDeviceContext);
    
    if(shaderSystem == DX11E_SHADER_SYSTEM_LOAD) {
        DX11E_SHADER_STAT stat = shader.Load();
        
        if(stat.success == FALSE) {
            DX11E_GFX_EXCEPTION(stat.message, stat.hResult);
            exit(-1);
        }
    }
    else if(shaderSystem == DX11E_SHADER_SYSTEM_LOAD_AND_COMPILE) {
        DX11E_COMPILE_MACROS* cMacros;
        
        if(compileMacros != nullptr) 
            cMacros = reinterpret_cast<DX11E_COMPILE_MACROS*>(compileMacros);
        else {
            DX11E_GFX_EXCEPTION(L"Compile macros are not provided though load and compile system used!", GetLastError());
            exit(-1);
        }
        
        DX11E_SHADER_STAT stat = shader.LoadAndCompile(cMacros -> shaderMacro, cMacros -> entryPoint, cMacros -> shaderTarget);
        
        if(stat.success == FALSE) {
            DX11E_GFX_EXCEPTION(stat.message, stat.hResult);
            exit(-1);
        }
    }
    
    this -> m_shaders.push_back(shader);    
}

std::vector<Shader> ShaderList::GetShaderVector() const {
    return this -> m_shaders;
}

DX11E_END