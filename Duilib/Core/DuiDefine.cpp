#include "StdAfx.h"
#include "DuiDefine.h"

namespace DuiLib
{
#ifndef WM_SYSTIMER
    #define WM_SYSTIMER 0x0118 //系统用来通知光标跳动的一个消息
#endif


    void DUILIB_API DuiTrace(LPCTSTR pstrFormat, ...)
    {
        TCHAR szBuffer[2048] = {0};
        va_list args;
        va_start(args, pstrFormat);
        _vsntprintf(szBuffer, 2048, pstrFormat, args);
        va_end(args);
        _tprintf(szBuffer);
    }

    LPCTSTR DUILIB_API DuiTraceMsg(UINT uMsg)
    {
#define MSGDEF(x) if(uMsg==x) return _T(#x)
        MSGDEF(WM_SETCURSOR);
        MSGDEF(WM_NCHITTEST);
        MSGDEF(WM_NCPAINT);
        MSGDEF(WM_PAINT);
        MSGDEF(WM_ERASEBKGND);
        MSGDEF(WM_NCMOUSEMOVE);
        MSGDEF(WM_MOUSEMOVE);
        MSGDEF(WM_MOUSELEAVE);
        MSGDEF(WM_MOUSEHOVER);
        MSGDEF(WM_NOTIFY);
        MSGDEF(WM_COMMAND);
        MSGDEF(WM_MEASUREITEM);
        MSGDEF(WM_DRAWITEM);
        MSGDEF(WM_LBUTTONDOWN);
        MSGDEF(WM_LBUTTONUP);
        MSGDEF(WM_LBUTTONDBLCLK);
        MSGDEF(WM_RBUTTONDOWN);
        MSGDEF(WM_RBUTTONUP);
        MSGDEF(WM_RBUTTONDBLCLK);
        MSGDEF(WM_SETFOCUS);
        MSGDEF(WM_KILLFOCUS);
        MSGDEF(WM_MOVE);
        MSGDEF(WM_SIZE);
        MSGDEF(WM_SIZING);
        MSGDEF(WM_MOVING);
        MSGDEF(WM_GETMINMAXINFO);
        MSGDEF(WM_CAPTURECHANGED);
        MSGDEF(WM_WINDOWPOSCHANGED);
        MSGDEF(WM_WINDOWPOSCHANGING);
        MSGDEF(WM_NCCALCSIZE);
        MSGDEF(WM_NCCREATE);
        MSGDEF(WM_NCDESTROY);
        MSGDEF(WM_TIMER);
        MSGDEF(WM_KEYDOWN);
        MSGDEF(WM_KEYUP);
        MSGDEF(WM_CHAR);
        MSGDEF(WM_SYSKEYDOWN);
        MSGDEF(WM_SYSKEYUP);
        MSGDEF(WM_SYSCOMMAND);
        MSGDEF(WM_SYSCHAR);
        MSGDEF(WM_VSCROLL);
        MSGDEF(WM_HSCROLL);
        MSGDEF(WM_CHAR);
        MSGDEF(WM_SHOWWINDOW);
        MSGDEF(WM_PARENTNOTIFY);
        MSGDEF(WM_CREATE);
        MSGDEF(WM_NCACTIVATE);
        MSGDEF(WM_ACTIVATE);
        MSGDEF(WM_ACTIVATEAPP);
        MSGDEF(WM_CLOSE);
        MSGDEF(WM_DESTROY);
        MSGDEF(WM_GETICON);
        MSGDEF(WM_GETTEXT);
        MSGDEF(WM_GETTEXTLENGTH);

        MSGDEF(WM_SYSTIMER);
        MSGDEF(WM_NCLBUTTONDOWN);
        MSGDEF(WM_NCLBUTTONUP);
        static TCHAR szMsg[10] = {0};
        ZeroMemory(szMsg, sizeof(szMsg));
        _stprintf(szMsg, _T("0x%04X"), uMsg);
        return szMsg;
    }

    DWORD GetLocalIpAddress()
    {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if(WSAStartup(wVersionRequested, &wsaData) != 0)
        {
            return 0;
        }
        char local[255] = {0};
        gethostname(local, sizeof(local));
        hostent* ph = gethostbyname(local);
        if(ph == NULL)
        {
            return 0;
        }
        in_addr addr;
        memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));
        DWORD dwIP = MAKEIPADDRESS(addr.S_un.S_un_b.s_b1, addr.S_un.S_un_b.s_b2, addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b4);
        return dwIP;
    }

}