#pragma once
#include <cassert>
#include <windows.h>

// A minimal HFILE wrapper
// It's far from being general enough for real-world use,
// but for our simple needs will do.

class CVFile {
	HANDLE m_hFile;
	CVFile(HANDLE hf): m_hFile(hf) { }

public:
	~CVFile() {
		assert( m_hFile != INVALID_HANDLE_VALUE );
		::CloseHandle(m_hFile);
	}

	enum OpenMode {
		omRead = GENERIC_READ,
		omWrite = GENERIC_WRITE,
		omReadWrite = GENERIC_READ | GENERIC_WRITE
	};

	enum SeekMode {
		smSet = FILE_BEGIN,
		smCur = FILE_CURRENT,
		smEnd = FILE_END
	};

	enum OpenPolicy {
		opExisting  = OPEN_EXISTING,
		opNew       = CREATE_NEW,
		opAlways    = OPEN_ALWAYS,
		opCreateNew = CREATE_ALWAYS,
		opTruncate  = TRUNCATE_EXISTING,
		opInvalid   = ~0
	};

	friend CVFile *FileOpen(const char *pchFilename, OpenMode om = omRead, OpenPolicy = opInvalid);

	HRESULT Read(void *pData, DWORD dwCount) {
		assert( m_hFile != INVALID_HANDLE_VALUE );

		DWORD dwRead;
		BOOL bResult;

		bResult = ::ReadFile(m_hFile, pData, dwCount, &dwRead, 0);
		return bResult ? S_OK : S_FALSE;
	}

	HRESULT Write(void *pData, DWORD dwCount) {
		assert( m_hFile != INVALID_HANDLE_VALUE );

		DWORD dwRead;
		BOOL bResult;

		bResult = ::WriteFile(m_hFile, pData, dwCount, &dwRead, 0);
		return bResult ? S_OK : S_FALSE;
	}

	DWORD GetFileSize() {
		assert( m_hFile != INVALID_HANDLE_VALUE );
		return ::GetFileSize(m_hFile, 0);
	}

	DWORD GetFilePointer() {
		assert( m_hFile != INVALID_HANDLE_VALUE );
		return ::SetFilePointer(m_hFile, 0, 0, smCur);
	}

	DWORD Seek(LONG lOffset, SeekMode sm = smCur) {
		assert( m_hFile != INVALID_HANDLE_VALUE );
		return ::SetFilePointer(m_hFile, lOffset, 0, sm);
	}

};

inline CVFile *FileOpen(const char *pchFilename, CVFile::OpenMode om, CVFile::OpenPolicy op)
{
	HANDLE hFile;

	if( op == CVFile::opInvalid ) {
		switch( om  ) {
		case CVFile::omRead      : op = CVFile::opExisting;  break;
		case CVFile::omWrite     : op = CVFile::opCreateNew; break;
		case CVFile::omReadWrite : op = CVFile::opAlways;    break;
		default                 : assert(!"Invalid open mode");
		}
	}

	hFile = ::CreateFile(
		pchFilename,   // Filename
		(DWORD)om,     // Desired Access
		0,             // Share Mode - don't share
		0,             // security - no security
		OPEN_ALWAYS,   // creation - create always
		0,             // file attributes - normal
		0              // file template - none
		);
	
	if( hFile == INVALID_HANDLE_VALUE ) {
		return 0;
	}
	return new CVFile(hFile);		
}