#include <Graphics.hpp>

static std::vector<IDXGIAdapter*> EnumerateAdapters(void) {
    IDXGIFactory* pFactory = nullptr;
    
    std::vector<IDXGIAdapter*> adapters = { nullptr };
    
    if(FAILED(::CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory)))) 
        return adapters;
    
    IDXGIAdapter* pAdapter = nullptr;
    
    for(register UINT i = 0; pFactory -> EnumAdapters(i, &pAdapter); i++) 
        adapters.push_back(pAdapter);
    
    if(pFactory != nullptr) 
        pFactory -> Release();
    
    return adapters;
}

DX11E_START

Graphics::Graphics() : 
    m_bufferColor( { 255u, 255u, 255u, 255u } ),
    m_pDevice(nullptr),
    m_pDeviceContext(nullptr),
    m_pSwapChain(nullptr),
    m_pRenderTargetView(nullptr),
    m_pInputLayout(nullptr),
    m_inputLayoutInitialized(FALSE)
{}

Graphics::~Graphics() {
    if(m_pDevice != nullptr) 
        m_pDevice -> Release();
        
    if(m_pDeviceContext != nullptr) 
        m_pDeviceContext -> Release();
        
    if(m_pSwapChain != nullptr) 
        m_pSwapChain -> Release();
        
    if(m_pRenderTargetView != nullptr) 
        m_pRenderTargetView -> Release();
}

BOOL Graphics::Initialize(_In_ HWND hWnd, _In_ WindowDimention windowDimention) noexcept(FALSE) {
    this -> m_windowDimention = windowDimention;
    
    std::vector<IDXGIAdapter*> adapters = EnumerateAdapters();
    
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_9_1,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1
    };
    
    DXGI_SWAP_CHAIN_DESC swapChainDesc = { NULL };
    
    swapChainDesc.BufferDesc.Width = windowDimention.width;
    swapChainDesc.BufferDesc.Height = windowDimention.height;
    swapChainDesc.BufferDesc.RefreshRate = { 0, 1 };
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1u;
    swapChainDesc.SampleDesc.Quality = 0u;
    swapChainDesc.BufferCount = 2u;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.Flags = 0u;
    
    HRESULT hr;
    UINT flags = 0u;
    
#if defined(DEBUG) || defined(_DEBUG) 
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    if(FAILED(hr = ::D3D11CreateDeviceAndSwapChain(
        adapters.at(0),
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &this -> m_pSwapChain,
        &this -> m_pDevice,
        nullptr,
        &this -> m_pDeviceContext
    ))) {
        DX11E_GFX_EXCEPTION(L"Failed to create device and swap chain!", hr);
        return FALSE;
    }
    
    // Getting the render target view.
    ID3D11Resource* pResource = nullptr;
    
    if(FAILED(hr = this -> m_pSwapChain -> GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pResource)))) {
        DX11E_GFX_EXCEPTION(L"Failed to retrieve back buffer for RENDER TARGET VIEW!", hr);
        return FALSE;
    }
    
    this -> m_pDevice -> CreateRenderTargetView(pResource, nullptr, &this -> m_pRenderTargetView);
    
    if(pResource != nullptr) 
        pResource -> Release();
    
    // Setting the ShaderList.
    this -> m_shaders = ShaderList(this -> m_pDevice, this -> m_pDeviceContext);
    
    return TRUE;
}

void Graphics::LoadDefaultShaders() {
    this -> AttachShader(L"shaders/bin/Default_VS.cso", DX11E_SHADER_TYPE_VERTEX, DX11E_SHADER_SYSTEM_LOAD, nullptr);
    this -> AttachShader(L"shaders/bin/Default_PS.cso", DX11E_SHADER_TYPE_PIXEL, DX11E_SHADER_SYSTEM_LOAD, nullptr);
}

void Graphics::AttachShader(_In_ std::wstring source, _In_ DX11E_SHADER_TYPE shaderType, _In_ DX11E_SHADER_SYSTEM shaderSystem, _In_ void* compileMacros) {
    this -> m_shaders.Add(source, shaderType, shaderSystem, compileMacros);
}

ID3D11Device* const Graphics::GetDevice() const {
    return this -> m_pDevice;
}

ID3D11DeviceContext* const Graphics::GetDeviceContext() const {
    return this -> m_pDeviceContext;
}

void Graphics::CreateInputLayout(_In_ D3D11_INPUT_ELEMENT_DESC* layoutDescs, _In_ UINT numElements) {
    Shader shader;
    
    for(Shader s : m_shaders.GetShaderVector()) {
        if(s.GetShaderType() == DX11E_SHADER_TYPE_VERTEX) {
            shader = s;
            break;
        }
    }
    
    HRESULT hr;
    
    if(FAILED(hr = this -> m_pDevice -> CreateInputLayout(layoutDescs, numElements, shader.GetBufferPointer(), shader.GetBufferSize(), &this -> m_pInputLayout))) 
        DX11E_GFX_EXCEPTION(L"Failed to initialize input layout!", hr);
    
    this -> m_inputLayoutInitialized = TRUE;
}

BOOL Graphics::SetInputLayout() const {
    this -> m_pDeviceContext -> IASetInputLayout(this -> m_pInputLayout);
    
    return this -> m_inputLayoutInitialized;
}

void Graphics::SetPrimitiveTopology(_In_ D3D_PRIMITIVE_TOPOLOGY topology) const {
    this -> m_pDeviceContext -> IASetPrimitiveTopology(topology);
}

void Graphics::SetViewport(_In_ UINT width, _In_ UINT height) const {
    D3D11_VIEWPORT viewport;
    
    viewport.Width = width;
    viewport.Height = height;
    viewport.MaxDepth = 1;
    viewport.MinDepth = 0;
    viewport.TopLeftX  = 0;
    viewport.TopLeftY  = 0;

    this -> m_pDeviceContext -> RSSetViewports(1u, &viewport);
}

void Graphics::SetRenderTargets() const {
    this -> m_pDeviceContext -> OMSetRenderTargets(1u, &this -> m_pRenderTargetView, nullptr);
}

void Graphics::UseShaders(_In_ UINT x) {
    this -> m_shaders.Use(x);
}

void Graphics::UseAllShaders() {
    this -> m_shaders.UseAll();
}

void Graphics::Present() const {
    HRESULT hr = this -> m_pSwapChain -> Present(1u, 0u);
    
    // If the device has been removed, throw an gfx error.
    if(hr == DXGI_ERROR_DEVICE_REMOVED) {
        DX11E_GFX_EXCEPTION(L"Exception [DXGI_ERROR_DEVICE_REMOVED]", this -> m_pDevice -> GetDeviceRemovedReason());
        exit(-1);
    }
}

void Graphics::SetBufferColor(_In_ USHORT red, _In_ USHORT green, _In_ USHORT blue, _In_ USHORT alpha) noexcept {
    this -> m_bufferColor[0] = red;
    this -> m_bufferColor[1] = green;
    this -> m_bufferColor[2] = blue;
    this -> m_bufferColor[3] = alpha;
}

void Graphics::ClearBuffer() noexcept {
    FLOAT color[4];
    
    for(register USHORT i = 0u; i < 4u; i++) 
        color[i] = this -> m_bufferColor[i] / 255.0f;
    
    this -> m_pDeviceContext -> ClearRenderTargetView(this -> m_pRenderTargetView, color);
}

GraphicsException::GraphicsException(int line, const WCHAR* file, const WCHAR* note, HRESULT hr): 
    DX11Exception(line, file, note),
    hr(hr)
{};

const char* GraphicsException::what() const noexcept {
    std::wostringstream oss;

    oss << this -> GetType() << std::endl
        << L"[ERROR CODE]: " << this -> GetErrorCode() << std::endl
        << L"[DESCRIPTION]: " << this -> GetErrorString()
        << this -> GetSpecificString();

    this -> m_wstringBuffer = oss.str();
    this -> m_stringBuffer = std::string(m_wstringBuffer.begin(), m_wstringBuffer.end());

    return this -> m_stringBuffer.c_str();
};

const WCHAR* GraphicsException::GetType() const {
    return L"Direct X 11 Graphics Exception";
};

HRESULT GraphicsException::GetErrorCode() const noexcept {
    return this -> hr;
};

const std::wstring GraphicsException::GetErrorString() const {
    std::array<WCHAR, 512> arr;
    
    DXGetErrorDescription(this -> hr, arr.data(), arr.size());
    
    return std::wstring(arr.data());
};

DX11E_END