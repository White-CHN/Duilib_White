#include "StdAfx.h"
#include "DuiWnd.h"

namespace DuiLib {

	CDuiWnd::CDuiWnd(void)
		: m_hWnd(NULL)
		, m_bSubclassed(FALSE)
		, m_OldWndProc(::DefWindowProc)
	{
	}


	CDuiWnd::~CDuiWnd(void)
	{
	}

	HWND CDuiWnd::GetHWND() const
	{
		return m_hWnd;
	}

	CDuiWnd::operator HWND() const
	{
		return m_hWnd;
	}

	BOOL CDuiWnd::RegisterWindowClass()
	{
		WNDCLASS wc = { 0 };
		wc.style = GetClassStyle();
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hIcon = NULL;
		wc.lpfnWndProc = CDuiWnd::__WndProc;
		wc.hInstance = CDuiPaintManager::GetInstance();
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = GetWindowClassName();
		ATOM ret = ::RegisterClass(&wc);
		ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
		return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
	}

	BOOL CDuiWnd::RegisterSuperclass()
	{
		// Get the class information from an existing
		// window so we can subclass it later on...
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) 
		{
			if( !::GetClassInfoEx(CDuiPaintManager::GetInstance(), GetSuperClassName(), &wc) ) 
			{
				ASSERT(!"Unable to locate window class");
				return NULL;
			}
		}
		m_OldWndProc = wc.lpfnWndProc;
		wc.lpfnWndProc = CDuiWnd::__ControlProc;
		wc.hInstance = CDuiPaintManager::GetInstance();
		wc.lpszClassName = GetWindowClassName();
		ATOM ret = ::RegisterClassEx(&wc);
		ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
		return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
	}

	HWND CDuiWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu /*= NULL*/)
	{
		return Create(hwndParent, pstrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
	}

	HWND CDuiWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x /*= CW_USEDEFAULT*/, int y /*= CW_USEDEFAULT*/, int cx /*= CW_USEDEFAULT*/, int cy /*= CW_USEDEFAULT*/, HMENU hMenu /*= NULL*/)
	{
		if( GetSuperClassName() != NULL && !RegisterSuperclass() ) 
		{
			return NULL;
		}
		if( GetSuperClassName() == NULL && !RegisterWindowClass() ) 
		{
			return NULL;
		}
		
		m_hWnd = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, CDuiPaintManager::GetInstance(), this);
		ASSERT(m_hWnd!=NULL);
		return m_hWnd;
	}

	HWND CDuiWnd::SubClass(HWND hWnd)
	{
		ASSERT(::IsWindow(hWnd));
		ASSERT(m_hWnd==NULL);
		m_OldWndProc = SubclassWindow(hWnd, __WndProc);
		if( m_OldWndProc == NULL ) 
		{
			return NULL;
		}
		m_bSubclassed = TRUE;
		m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
		return m_hWnd;
	}

	void CDuiWnd::UnSubClass()
	{
		ASSERT(::IsWindow(m_hWnd));
		if( !::IsWindow(m_hWnd) || !m_bSubclassed)
		{
			return;
		}
		SubclassWindow(m_hWnd, m_OldWndProc);
		m_OldWndProc = ::DefWindowProc;
		m_bSubclassed = FALSE;
	}

	void CDuiWnd::CenterWindow()
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
		RECT rcDlg = { 0 };
		::GetWindowRect(m_hWnd, &rcDlg);

		HWND hWnd=*this;
		HWND hWndCenter = ::GetWindowOwner(m_hWnd);
		if (hWndCenter!=NULL)
		{
			hWnd = hWndCenter;
		}

		// 处理多显示器模式下屏幕居中
		RECT rcArea = {0};
		RECT rcCenter = {0};
		MONITORINFO oMonitor = {0};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
		rcArea = oMonitor.rcWork;

		if( hWndCenter == NULL )
		{
			rcCenter = rcArea;
		}
		else
		{
			::GetWindowRect(hWndCenter, &rcCenter);
		}

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// Find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// The dialog is outside the screen, move it inside
		if( xLeft < rcArea.left ) xLeft = rcArea.left;
		else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
		if( yTop < rcArea.top ) yTop = rcArea.top;
		else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
		::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	UINT CDuiWnd::GetClassStyle() const
	{
		return 0;
	}

	LPCTSTR CDuiWnd::GetSuperClassName() const
	{
		return NULL;
	}

	LRESULT CDuiWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
	}

	void CDuiWnd::OnFinalMessage(HWND hWnd)
	{

	}

	LRESULT CALLBACK CDuiWnd::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CDuiWnd* pThis = NULL;
		if( uMsg == WM_NCCREATE ) 
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			pThis = static_cast<CDuiWnd*>(lpcs->lpCreateParams);
			pThis->m_hWnd = hWnd;
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
		} 
		else 
		{
			pThis = reinterpret_cast<CDuiWnd*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if( uMsg == WM_NCDESTROY && pThis != NULL ) 
			{
				LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
				::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
				if( pThis->m_bSubclassed ) 
				{
					pThis->UnSubClass();
				}
				pThis->m_hWnd = NULL;
				pThis->OnFinalMessage(hWnd);
				return lRes;
			}
		}
		if( pThis != NULL ) 
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		} 
		else 
		{
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK CDuiWnd::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CDuiWnd* pThis = NULL;
		if( uMsg == WM_NCCREATE ) 
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			pThis = static_cast<CDuiWnd*>(lpcs->lpCreateParams);
			::SetProp(hWnd, _T("WndX"), (HANDLE) pThis);
			pThis->m_hWnd = hWnd;
		} 
		else 
		{
			pThis = reinterpret_cast<CDuiWnd*>(::GetProp(hWnd, _T("WndX")));
			if( uMsg == WM_NCDESTROY && pThis != NULL ) 
			{
				LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
				if( pThis->m_bSubclassed ) 
				{
					pThis->UnSubClass();
				}
				::SetProp(hWnd, _T("WndX"), NULL);
				pThis->m_hWnd = NULL;
				pThis->OnFinalMessage(hWnd);
				return lRes;
			}
		}
		if( pThis != NULL ) {
			return pThis->HandleMessage(uMsg, wParam, lParam);
		} 
		else {
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

}
