#include <Engine.hpp>

using namespace DX11E;

class Application final : public Engine {
    
    public: 
        Application(_In_ HINSTANCE hInstance) : m_hInstance(hInstance), m_windowText(L"DirextX 11 Widnow") {};
        
        BOOL Initialize() noexcept(FALSE) override {
            Window* window = this -> GetWindow();
            
            BOOL wnd     = window -> Initialize(this -> m_hInstance, L"DX11E", this -> m_windowText, { 820, 520 }, this),
                 r_input = this -> InitializeRawInputDevices(),
                gfx     = this -> GetGFX() -> Initialize(window -> GetWindowHandle(), window -> GetWindowDimention());
            
            this -> GetGFX() -> SetBufferColor(227u, 112u, 45u, 255u);
            
            this -> GetGFX() -> AttachShader(L"shaders/bin/VertexShader.cso", DX11E_SHADER_TYPE_VERTEX, DX11E_SHADER_SYSTEM_LOAD, nullptr);
            this -> GetGFX() -> AttachShader(L"shaders/bin/PixelShader.cso", DX11E_SHADER_TYPE_PIXEL, DX11E_SHADER_SYSTEM_LOAD, nullptr);
            
            D3D11_INPUT_ELEMENT_DESC ied[] = {
                { "POSITION", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "COLOR", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, 2u * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0u}
            };
            
            this -> GetGFX() -> CreateInputLayout(ied, std::size(ied));
            
            return wnd && r_input && gfx;
        }
        
        BOOL Update() override {
            BOOL blx = Engine::Update();
            
            if(blx) {
                Window* window = this -> GetWindow();
                Keyboard* keyboard = this -> GetKeyboard();
                Mouse* mouse = this -> GetMouse();
                
                while(!keyboard -> IsKeyBufferEmpty()) {
                    KeyboardEvent ke = keyboard -> ReadKey();
                    
                    if(ke.IsPressed()) {
                        switch(ke.GetKeyCode()) {
                            case VK_ESCAPE: 
                                this -> Destroy();
                                break;
                            
                            case 'M': 
                                if(window -> IsCursorEnabled()) 
                                    window -> DisableCursor();
                                else {
                                    WindowDimention wd = window -> GetWindowDimention();
                                    RECT rect;
                                    
                                    ::GetWindowRect(window -> GetWindowHandle(), &rect);
                                    
                                    window -> CursorPosition(rect.left + wd.width / 2, rect.top + wd.height / 2);
                                    window -> EnableCursor();
                                }
                        }
                    }
                }
                
                while(!keyboard -> IsCharBufferEmpty()) {
                    const char ch = keyboard -> ReadChar();
                }
                
                while(!mouse -> IsEventBufferEmpty()) {
                    MouseEvent me = mouse -> ReadEvent();
                }
            }
            
            return blx;
        }
        
        const WCHAR* GetCaptionText() override {
            return this -> m_windowText;
        }
        
        void Draw(Graphics* const gfx) override {
            Window* const window = this -> GetWindow();
            
            /** VERTEX BUFFER **/
            
            const Vertex vertices[] = {
                {  0.0f,  0.5f,     255u, 0u, 0u, 255u },
                { -0.5f, -0.3f,     0u, 255u, 0u, 255u },
                {  0.5f, -0.3f,     0u, 0u, 255u, 255u }
            };
            
            ID3D11Buffer* vertexBuffer = nullptr;
            
            D3D11_BUFFER_DESC vertexBufferDesc = { NULL };
            
            vertexBufferDesc.ByteWidth = sizeof(vertices);
            vertexBufferDesc.StructureByteStride = sizeof(Vertex);
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0u;
            vertexBufferDesc.MiscFlags = 0u;
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            
            D3D11_SUBRESOURCE_DATA vertexSubresourceData = { NULL };
            
            vertexSubresourceData.pSysMem = vertices;
            
            HRESULT hr;
            
            if(FAILED(hr = gfx -> GetDevice() -> CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer))) {
                DX11E_GFX_EXCEPTION(L"FAILED TO BIND VERTEX BUFFER!", hr);
                exit(-1);
            }
            
            UINT vertexOffset = 0;
            
            gfx -> GetDeviceContext() -> IASetVertexBuffers(0u, 1u, &vertexBuffer, &vertexBufferDesc.StructureByteStride, &vertexOffset);
            
            /** INDEX BUFFER **/
            
            const int16_t indicies[] = { 0, 1, 2, 0 };
            
            ID3D11Buffer* indexBuffer = nullptr;
            
            D3D11_BUFFER_DESC indexBufferDesc = { NULL };
            
            indexBufferDesc.ByteWidth = sizeof(indicies);
            indexBufferDesc.StructureByteStride = sizeof(int16_t);
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0u;
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.MiscFlags = 0u;
            
            D3D11_SUBRESOURCE_DATA indexSubresourceData = { NULL };
            
            indexSubresourceData.pSysMem = indicies;
            
            if(FAILED(hr = gfx -> GetDevice() -> CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer))) {
                DX11E_GFX_EXCEPTION(L"FAILED TO BIND INDEX BUFFER!", hr);
                exit(-1);
            }
            
            gfx -> GetDeviceContext() -> IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);
            
            gfx -> SetRenderTargets();
            gfx -> UseAllShaders();
            
            if(gfx -> SetInputLayout() == FALSE) {
                DX11E_GFX_EXCEPTION(L"FAILED TO SET INPUT LAYOUT! NO INPUT LAYOUT ARRAY IN BOUND.", GetLastError());
                exit(-1);
            }
            
            gfx -> SetViewport(window -> GetWindowDimention().width, window -> GetWindowDimention().height);
            gfx -> SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
            
            gfx -> GetDeviceContext() -> DrawIndexed(std::size(indicies), 0u, 0u);
            
            // Releasing the COM objects.
            if(vertexBuffer != nullptr) 
                vertexBuffer -> Release();
            
            if(indexBuffer != nullptr) 
                indexBuffer -> Release();
        }
    
    private: 
        HINSTANCE m_hInstance;
        const WCHAR* m_windowText;
    
};

INT WINAPI WinMain(
    _In_      HINSTANCE hInstance,
    _In_opt_  HINSTANCE,
    _In_      LPSTR pCmdLine,
    _In_      INT nCmdShow
) {
    Application app(hInstance);
    
    try {
        Engine::Run(app);
    } catch(const DX11Exception& ex) {
        std::string str = ex.what();
        ::MessageBox(HWND_DESKTOP, std::wstring(str.begin(), str.end()).c_str(),  L"DX11E Error", MB_OK | MB_ICONERROR);
    } catch(...) {
        ::MessageBox(HWND_DESKTOP, L"Unknown error occured: Direct X 11 Framework is unable to identify the error.", L"Unknown Error", MB_OK | MB_ICONERROR | MB_ICONEXCLAMATION);
    }
    
    return 0;
}