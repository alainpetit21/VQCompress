#ifndef _W32WINDOW_H_
#define _W32WINDOW_H_

/*Include*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>	
#include <windowsx.h>
	
typedef struct tagT_WIN32FRAME
{
	unsigned char			(*m_fctFrame)(tagT_WIN32FRAME*, unsigned long);
	unsigned long			lData;
	unsigned int			m_nW32Width;
	unsigned int			m_nW32Height;
	unsigned int			m_nW32Depth;
	int						m_bW32Fs;
	int						m_bW32Ok;
	char					m_szW32WindowTitle[128];
	char					m_szW32AppName[128];
	WNDPROC					m_W32procHandler;
	HINSTANCE				m_hW32Instance;
	HWND					m_hW32Wnd;
	unsigned int			m_W32Keys;
}T_WIN32FRAME;

extern T_WIN32FRAME* W32_Init(int nW, int nH, int nD, int nFS, char* appName, char* appTitle, unsigned char (*p_fctFrame)(tagT_WIN32FRAME*, unsigned long));
	

#endif // _W32WINDOW_H_
