#include <Shader.hpp>

DX11E_START

Shader::Shader() : 
    m_shader(nullptr),
    m_pShaderBlob(nullptr),
    m_shaderType(DX11E_SHADER_TYPE_VERTEX),
    m_pDevice(nullptr),
    m_pDeviceContext(nullptr)
{};

Shader::Shader(_In_ DX11E_SHADER_TYPE shaderType, _In_ std::wstring source, _In_ ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pDeviceContext): 
    m_shader(nullptr),
    m_pShaderBlob(nullptr),
    m_shaderType(shaderType),
    m_source(source),
    m_pDevice(pDevice),
    m_pDeviceContext(pDeviceContext)
{}

DX11E_SHADER_STAT Shader::Load() {
    HRESULT hr;
    
    if(FAILED(hr = D3DReadFileToBlob(this -> m_source.c_str(), &this -> m_pShaderBlob))) 
        return { nullptr, hr, L"Failed to load shader from file!", FALSE };
    
    if(FAILED(hr = this -> LoadShader())) 
        if(FAILED(hr = this -> LoadShader())) 
        return { nullptr, hr, L"Shader::Load(): Failed to create shader!",  FALSE};
    
    return { nullptr, hr, L"", TRUE };
}

DX11E_SHADER_STAT Shader::LoadAndCompile(_In_ const D3D_SHADER_MACRO* defines, _In_ std::string entryPoint, _In_ DX11E_SHADER_TARGET shaderTarget) {
    std::string target;
    
    switch(this -> m_shaderType) {
        case DX11E_SHADER_TYPE_VERTEX: 
            target = "vs_";
            break;
        case DX11E_SHADER_TYPE_PIXEL: 
            target = "ps_";
            break;
    }
    
    target += shaderTarget.Model.MAJOR + '_' + shaderTarget.Model.MINOR + "_level_" + shaderTarget.Level.MAJOR + '_' + shaderTarget.Level.MINOR;
    
    HRESULT hr;
    ID3DBlob* error = nullptr;
    
    if(FAILED(hr = D3DCompileFromFile(this -> m_source.c_str(), defines, (ID3DInclude*)(UINT_PTR) 1, entryPoint.c_str(), target.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, &this -> m_pShaderBlob, &error))) 
        return { error, hr, L"Failed to load and compile shader from file!", FALSE };
    
    if(error != nullptr) 
        error -> Release();
    
    if(FAILED(hr = this -> LoadShader())) 
        return { nullptr, hr, L"Shader::LoadAndCompile(): Failed to create shader!",  FALSE};
    
    return { nullptr, hr, L"", TRUE };
}

void Shader::Use() {
    switch(this -> m_shaderType) {
        case DX11E_SHADER_TYPE_VERTEX: 
            this -> m_pDeviceContext -> VSSetShader(reinterpret_cast<ID3D11VertexShader*>(this -> m_shader), nullptr, 0u);
            break;
        case DX11E_SHADER_TYPE_PIXEL: 
            this -> m_pDeviceContext -> PSSetShader(reinterpret_cast<ID3D11PixelShader*>(this -> m_shader), nullptr, 0u);
            break;
    }
}

HRESULT Shader::LoadShader() {
    HRESULT hr;
    
    switch(this -> m_shaderType) {
        case DX11E_SHADER_TYPE_VERTEX: 
            hr = this -> m_pDevice -> CreateVertexShader(this -> m_pShaderBlob -> GetBufferPointer(), this -> m_pShaderBlob -> GetBufferSize(), nullptr, reinterpret_cast<ID3D11VertexShader**>(&this -> m_shader));
            break;
        
        case DX11E_SHADER_TYPE_PIXEL: 
            hr = this -> m_pDevice -> CreatePixelShader(this -> m_pShaderBlob -> GetBufferPointer(), this -> m_pShaderBlob -> GetBufferSize(), nullptr, reinterpret_cast<ID3D11PixelShader**>(&this -> m_shader));
            break;
    }
    
    return hr;
}

DX11E_SHADER_TYPE Shader::GetShaderType() const {
    return this -> m_shaderType;
}

void* Shader::GetBufferPointer() const {
    return this -> m_pShaderBlob -> GetBufferPointer();
}

SIZE_T Shader::GetBufferSize() const {
    return this -> m_pShaderBlob -> GetBufferSize();
}

DX11E_END