#include "StdAfx.h"
#include "DuiIPAddress.h"

namespace DuiLib
{
    CDuiIPAddressWnd::CDuiIPAddressWnd(void)
        : m_bInit(FALSE)
        , m_pOwner(NULL)
    {

    }

    CDuiIPAddressWnd::~CDuiIPAddressWnd(void)
    {

    }

    void CDuiIPAddressWnd::Init(CDuiIPAddress* pOwner)
    {
        if(pOwner == NULL)
        {
            return;
        }
        m_pOwner = pOwner;
        m_pOwner->SetUpdateFlag(IP_NONE);
        if(m_bInit == FALSE)
        {
            INITCOMMONCONTROLSEX   CommCtrl;
            CommCtrl.dwSize = sizeof(CommCtrl);
            CommCtrl.dwICC = ICC_INTERNET_CLASSES; //Ö¸¶¨Class
            if(InitCommonControlsEx(&CommCtrl))
            {
                Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD | WS_TABSTOP | WS_GROUP, 0, m_pOwner->GetPos());
                SetWindowFont(GetHWND(), m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
            }
        }
        if(m_pOwner->GetText().IsEmpty())
        {
            m_pOwner->SetIP(GetLocalIpAddress());
        }
        ::SendMessage(GetHWND(), IPM_SETADDRESS, 0, m_pOwner->GetIP());
        ::ShowWindow(GetHWND(), SW_SHOW);
        ::SetFocus(GetHWND());
        m_bInit = TRUE;
    }

    LPCTSTR CDuiIPAddressWnd::GetWindowClassName() const
    {
        return GET_CLASS_NAME(CDuiIPAddressWnd);
    }

    LPCTSTR CDuiIPAddressWnd::GetSuperClassName() const
    {
        return WC_IPADDRESS;
    }

    LRESULT CDuiIPAddressWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT CDuiIPAddressWnd::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LRESULT lRes = 0;
        if(wParam == VK_DELETE || wParam == VK_BACK)
        {
            lRes = ::DefWindowProc(GetHWND(), uMsg, wParam, lParam);
            m_pOwner->SetUpdateFlag(IP_DELETE);
            m_pOwner->UpdateText();
            PostMessage(WM_CLOSE);
            return lRes;
        }
        if(wParam == VK_ESCAPE)
        {
            LRESULT lRes = ::DefWindowProc(GetHWND(), uMsg, wParam, lParam);
            m_pOwner->SetUpdateFlag(IP_KEEP);
            PostMessage(WM_CLOSE);
            return lRes;
        }
        bHandled = FALSE;
        return lRes;
    }

    LRESULT CDuiIPAddressWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LRESULT lRes = 0;
        if(GET_WM_COMMAND_CMD(wParam, lParam) == EN_KILLFOCUS)
        {
            HWND hWndFocus = GetFocus();
            while(hWndFocus)
            {
                if(GetFocus() == GetHWND())
                {
                    return lRes;
                }
                hWndFocus = GetParent(hWndFocus);
            }

            lRes = ::DefWindowProc(GetHWND(), uMsg, wParam, lParam);
            if(m_pOwner->GetUpdateFlag() == IP_NONE)
            {
                DWORD dwIP = 0;
                ::SendMessage(GetHWND(), IPM_GETADDRESS, 0, (LPARAM)&dwIP);
                m_pOwner->SetIP(dwIP);
                m_pOwner->SetUpdateFlag(IP_UPDATE);
                m_pOwner->UpdateText();
            }
            ::ShowWindow(GetHWND(), SW_HIDE);
        }
        return lRes;
    }

    LRESULT CDuiIPAddressWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch(uMsg)
        {
            case WM_KILLFOCUS:
                lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KEYUP:
                lRes = OnKeyUp(uMsg, wParam, lParam, bHandled);
                break;
            case OCM_COMMAND:
                lRes = OnCommand(uMsg, wParam, lParam, bHandled);
                break;
            case WM_MOUSEWHEEL:
            {
                bHandled = FALSE;
            }
            break;
            default:
                bHandled = FALSE;
                break;
        }
        if(bHandled)
        {
            return lRes;
        }
        return CDuiWnd::HandleMessage(uMsg, wParam, lParam);
    }

    void CDuiIPAddressWnd::OnFinalMessage(HWND hWnd)
    {
        m_pOwner->EmptyIPAddressWnd();
        CDuiWnd::OnFinalMessage(hWnd);
    }

    ////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiIPAddress)

    CDuiIPAddress::CDuiIPAddress(void)
        : m_dwIP(GetLocalIpAddress())
        , m_nIPUpdateFlag(IP_UPDATE)
        , m_pIPAddressWnd(NULL)
    {
        UpdateText();
        m_nIPUpdateFlag = IP_NONE;
    }


    CDuiIPAddress::~CDuiIPAddress(void)
    {
    }

    CDuiString CDuiIPAddress::GetClass() const
    {
        return DUI_CTR_IPADDRESS;
    }

    LPVOID CDuiIPAddress::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_IPADDRESS) == 0)
        {
            return static_cast<CDuiIPAddress*>(this);
        }
        return CDuiLabel::GetInterface(pstrName);
    }

    void CDuiIPAddress::EmptyIPAddressWnd()
    {
        m_pIPAddressWnd = NULL;
    }

    int CDuiIPAddress::GetUpdateFlag()
    {
        return m_nIPUpdateFlag;
    }

    void CDuiIPAddress::SetUpdateFlag(int nIPUpdateFlag)
    {
        m_nIPUpdateFlag = nIPUpdateFlag;
    }

    void CDuiIPAddress::UpdateText()
    {
        if(m_nIPUpdateFlag == IP_DELETE)
        {
            SetText(_T(""));
        }
        else if(m_nIPUpdateFlag == IP_UPDATE)
        {
            TCHAR szIP[MAX_PATH] = {0};
            in_addr addr;
            addr.S_un.S_addr = m_dwIP;
            _stprintf(szIP, _T("%3d.%3d.%3d.%3d"), addr.S_un.S_un_b.s_b4, addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b2, addr.S_un.S_un_b.s_b1);
            SetText(szIP);
        }
    }

    DWORD CDuiIPAddress::GetIP()
    {
        return m_dwIP;
    }

    void CDuiIPAddress::SetIP(DWORD dwIP)
    {
        if(m_dwIP == dwIP)
        {
            return;
        }
        m_dwIP = dwIP;
        UpdateText();
    }

    void CDuiIPAddress::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiLabel::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_SETCURSOR && IsEnabled())
        {
            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
            return;
        }
        if(event.Type == UIEVENT_WINDOWSIZE)
        {
            if(m_pIPAddressWnd != NULL)
            {
                GetManager()->SetFocusNeeded(this);
            }
        }
        if(event.Type == UIEVENT_SCROLLWHEEL)
        {
            if(m_pIPAddressWnd != NULL)
            {
                return;
            }
        }
        if(event.Type == UIEVENT_SETFOCUS && IsEnabled())
        {
            if(m_pIPAddressWnd)
            {
                return;
            }
            m_pIPAddressWnd = new CDuiIPAddressWnd();
            ASSERT(m_pIPAddressWnd);
            m_pIPAddressWnd->Init(this);
        }
        if(event.Type == UIEVENT_KILLFOCUS && IsEnabled())
        {
            Invalidate();
        }
        if(event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN)
        {
            if(IsEnabled())
            {
                GetManager()->ReleaseCapture();
                if(IsFocused() && m_pIPAddressWnd == NULL)
                {
                    m_pIPAddressWnd = new CDuiIPAddressWnd();
                    ASSERT(m_pIPAddressWnd);
                }
                if(m_pIPAddressWnd != NULL)
                {
                    m_pIPAddressWnd->Init(this);
                }
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP)
        {
            return;
        }
        if(event.Type == UIEVENT_CONTEXTMENU)
        {
            return;
        }
        if(event.Type == UIEVENT_MOUSEENTER)
        {
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            return;
        }

        CDuiLabel::DoEvent(event);
    }



}