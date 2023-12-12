#include <DX11Exception.hpp>

DX11E_START

DX11Exception::DX11Exception(_In_ int line, _In_ const WCHAR* file, _In_ const WCHAR* note) :
    m_line(line),
    m_file(file),
    m_note(note)
{}

const char* DX11Exception::what() const noexcept {
    std::wostringstream oss;

    oss << this -> GetType() << L":\ns" << std::endl << this -> GetSpecificString();

    this -> m_wstringBuffer = oss.str();
    this -> m_stringBuffer = std::string(m_wstringBuffer.begin(), m_wstringBuffer.end());

    return this -> m_stringBuffer.c_str();
}

const WCHAR* DX11Exception::GetType() const {
    return L"Direct X 11 Exception";
}

int DX11Exception::GetLine() const {
    return this -> m_line;
}

const std::wstring& DX11Exception::GetFile() const {
    return this -> m_file;
}

const std::wstring DX11Exception::GetSpecificString() const {
    std::wostringstream oss;

    oss << std::endl 
        << L"[NOTE]: " << this -> m_note.c_str() << std::endl
        << L"[FILE]: " << this -> m_file.c_str() << std::endl
        << L"[LINE]: " << this -> m_line << std::endl;

    return oss.str();
}

std::wstring DX11Exception::TranslateErrorCode(_In_ HRESULT hr) {
    _com_error err(hr);

    return err.ErrorMessage();
}

DX11E_END