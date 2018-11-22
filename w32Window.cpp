#ifndef _MT
#define _MT
#endif
#include <process.h>    /* _beginthread, _endthread */
#include "w32Window.h"

T_WIN32FRAME gArFrames[16];
int			 gCptFrames=0;

T_WIN32FRAME* 
FindFrameWithHWND(HWND hWnd)
{
	for(int i= 0; i < gCptFrames; ++i){
		if(gArFrames[i].m_hW32Wnd == hWnd){
			return &gArFrames[i];
		}
	}
	while(1);
}

long FAR PASCAL 
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
T_WIN32FRAME* p_frame;

    switch(message){
	case WM_DESTROY:	PostQuitMessage(0);			return 0;
	case WM_KEYUP:	
		p_frame= FindFrameWithHWND(hWnd);
		switch(wParam){
		case 'a':
		case 'A':		p_frame->m_W32Keys&= ~0x0001;	break;
		case 'b':
		case 'B':		p_frame->m_W32Keys&= ~0x0002;	break;
		case VK_SPACE:	p_frame->m_W32Keys&= ~0x0004;	break;
		case VK_RETURN:	p_frame->m_W32Keys&= ~0x0008;	break;
		case VK_RIGHT:	p_frame->m_W32Keys&= ~0x0010;	break;
		case VK_LEFT:	p_frame->m_W32Keys&= ~0x0020;	break;
		case VK_UP:		p_frame->m_W32Keys&= ~0x0040;	break;
		case VK_DOWN:	p_frame->m_W32Keys&= ~0x0080;	break;
		case 'r':
		case 'R':		p_frame->m_W32Keys&= ~0x0100;	break;
		case 'l':		
		case 'L':		p_frame->m_W32Keys&= ~0x0200;	break;
		}
		return 0;
	case WM_KEYDOWN:	
		p_frame= FindFrameWithHWND(hWnd);
		switch(wParam){
		case 'a':
		case 'A':		p_frame->m_W32Keys|= 0x0001;	break;
		case 'b':
		case 'B':		p_frame->m_W32Keys|= 0x0002;	break;
		case VK_SPACE:	p_frame->m_W32Keys|= 0x0004;	break;
		case VK_RETURN:	p_frame->m_W32Keys|= 0x0008;	break;
		case VK_RIGHT:	p_frame->m_W32Keys|= 0x0010;	break;
		case VK_LEFT:	p_frame->m_W32Keys|= 0x0020;	break;
		case VK_UP:		p_frame->m_W32Keys|= 0x0040;	break;
		case VK_DOWN:	p_frame->m_W32Keys|= 0x0080;	break;
		case 'r':
		case 'R':		p_frame->m_W32Keys|= 0x0100;	break;
		case 'l':		
		case 'L':		p_frame->m_W32Keys|= 0x0200;	break;
		}
				
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 1;
}

int WINAPI 
WinMainThread(HINSTANCE hInstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) 
{
WNDCLASS		wc;
MSG				msg;
T_WIN32FRAME*	p_frame;

	p_frame= &gArFrames[gCptFrames];
	p_frame->m_hW32Instance	= hInstance?hInstance:hprevinstance;
	p_frame->m_W32procHandler= (WNDPROC)WindowProc;

	if(p_frame->m_hW32Wnd= FindWindow(p_frame->m_szW32AppName, p_frame->m_szW32WindowTitle)){
		SetForegroundWindow(p_frame->m_hW32Wnd);
	}

	wc.style		= CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc	= (WNDPROC)p_frame->m_W32procHandler;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hIcon		= 0;
	wc.hInstance	= p_frame->m_hW32Instance;
	wc.hCursor		= 0;
	wc.hbrBackground= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName= p_frame->m_szW32AppName;
	
	RegisterClass(&wc);

	p_frame->m_hW32Wnd= CreateWindow(p_frame->m_szW32AppName, p_frame->m_szW32WindowTitle, WS_VISIBLE, CW_USEDEFAULT,	
									CW_USEDEFAULT,	p_frame->m_nW32Width, p_frame->m_nW32Height, 0, 0, p_frame->m_hW32Instance, 0);

	ShowWindow((HWND)p_frame->m_hW32Wnd, SW_SHOWNORMAL);
	UpdateWindow((HWND)p_frame->m_hW32Wnd);

	p_frame->m_bW32Ok	= TRUE;
	while(1){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if(msg.message == WM_QUIT){
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(!p_frame->m_fctFrame(p_frame, p_frame->lData))
				break;
		}
	}
	p_frame->m_bW32Ok	= TRUE;
	return 0;
}


T_WIN32FRAME* 
W32_Init(int nW, int nH, int nD, int nFS, char* appName, char* appTitle, unsigned char (*p_fctFrame)(tagT_WIN32FRAME*, unsigned long))
{
T_WIN32FRAME* p_frame;

	if(gCptFrames > 16)
		_asm{int 3};

	p_frame= &gArFrames[gCptFrames];

	p_frame->m_bW32Fs		= nFS;
	p_frame->m_nW32Depth	= nD;
	p_frame->m_nW32Width	= nW;
	p_frame->m_nW32Height	= nH;
	p_frame->m_bW32Ok		= FALSE;
	p_frame->m_fctFrame		= p_fctFrame;

	strncpy(&p_frame->m_szW32AppName[0], appName, 128);
	strncpy(&p_frame->m_szW32WindowTitle[0], appTitle, 128);
	_beginthread((void (__cdecl *)(void *))WinMainThread,0,NULL);
	while(!p_frame->m_bW32Ok)
		MessageBox(0, "Waiting", "Waiting", 0);
	gCptFrames++;
	return p_frame;
}
