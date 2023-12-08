#ifndef _DX11E_EXCEPTION_
#define _DX11E_EXCEPTION_

#pragma once

#include <dx11ewin.hpp>
#include <exception>
#include <string>
#include <sstream>
#include <comdef.h>

DX11E_START

class DX11E_API DX11Exception: public std::exception {

	public: 
		DX11Exception(_In_ int, _In_ const WCHAR*, _In_ const WCHAR*);
		const char* what() const noexcept override;
		virtual const WCHAR* GetType() const;
		int GetLine() const;
		const std::wstring& GetFile() const;
		const std::wstring GetSpecificString() const;

		static std::wstring TranslateErrorCode(_In_ HRESULT);

	private: 
		int m_line;
		std::wstring m_file;
		std::wstring m_note;

	protected: 
		mutable std::string m_stringBuffer;
		mutable std::wstring m_wstringBuffer;
	
};

DX11E_END

#endif // !_DX11E_EXCEPTION_