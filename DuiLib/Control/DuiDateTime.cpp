#include "StdAfx.h"
#include "DuiDateTime.h"

namespace DuiLib
{
    CDuiDateTimeWnd::CDuiDateTimeWnd(void)
        : m_bInit(FALSE)
        , m_bDropOpen(FALSE)
        , m_pOwner(NULL)
    {
        ZeroMemory(&m_oldSysTime, sizeof(m_oldSysTime));
    }

    CDuiDateTimeWnd::~CDuiDateTimeWnd(void)
    {
    }

    RECT CDuiDateTimeWnd::CalPos()
    {
        CDuiRect rcPos = m_pOwner->GetPos();

        CDuiControl* pParent = static_cast<CDuiControl*>(m_pOwner);
        RECT rcParent;
        while(pParent = pParent->GetParent())
        {
            if(!pParent->IsVisible())
            {
                rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
                break;
            }
            rcParent = pParent->GetClientPos();
            if(!::IntersectRect(&rcPos, &rcPos, &rcParent))
            {
                rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
                break;
            }
        }

        return rcPos;
    }

    void CDuiDateTimeWnd::Init(CDuiDateTime* pOwner)
    {
        if(pOwner == NULL)
        {
            return;
        }
        m_pOwner = pOwner;
        m_pOwner->SetUpdateFlag(DT_NONE);
        if(m_bInit == FALSE)
        {
            Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD, 0, CalPos());
            SetWindowFont(GetHWND(), m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->hFont, TRUE);
        }

        if(m_pOwner->GetText().IsEmpty())
        {
            ::GetLocalTime(&m_pOwner->GetTime());
        }
        memcpy(&m_oldSysTime, &m_pOwner->GetTime(), sizeof(SYSTEMTIME));
        DateTime_SetSystemtime(GetHWND(), 0, &m_pOwner->GetTime());
        ::ShowWindow(GetHWND(), SW_SHOWNOACTIVATE);
        ::SetFocus(GetHWND());
        m_bInit = TRUE;
    }

    BOOL CDuiDateTimeWnd::GetDropOpen()
    {
        return m_bDropOpen;
    }

    LPCTSTR CDuiDateTimeWnd::GetWindowClassName() const
    {
        return GET_CLASS_NAME(CDuiDateTimeWnd);
    }

    LPCTSTR CDuiDateTimeWnd::GetSuperClassName() const
    {
        return DATETIMEPICK_CLASS;
    }

    LRESULT CDuiDateTimeWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(wParam == VK_ESCAPE)
        {
            m_pOwner->SetTime(&m_oldSysTime);
            m_pOwner->SetUpdateFlag(DT_UPDATE);
            m_pOwner->UpdateText();
            PostMessage(WM_CLOSE);
            return 0;
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiDateTimeWnd::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        NMHDR* pHeader = (NMHDR*)lParam;
        if(pHeader != NULL && pHeader->hwndFrom == GetHWND())
        {
            if(pHeader->code == DTN_DATETIMECHANGE)
            {
                LPNMDATETIMECHANGE lpChage = (LPNMDATETIMECHANGE)lParam;
                ::SendMessage(GetHWND(), DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->GetTime());
                m_pOwner->SetUpdateFlag(DT_UPDATE);
                m_pOwner->UpdateText();
            }
            else if(pHeader->code == DTN_DROPDOWN)
            {
                m_bDropOpen = TRUE;
            }
            else if(pHeader->code == DTN_CLOSEUP)
            {
                ::SendMessage(GetHWND(), DTM_GETSYSTEMTIME, 0, (LPARAM)&m_pOwner->GetTime());
                m_pOwner->SetUpdateFlag(DT_UPDATE);
                m_pOwner->UpdateText();
                PostMessage(WM_CLOSE);
                m_bDropOpen = FALSE;
            }
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiDateTimeWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(!m_bDropOpen)
        {
            PostMessage(WM_CLOSE);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiDateTimeWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pOwner->GetManager()->IsLayered())
        {
            m_pOwner->GetManager()->AddPaintChildWnd(GetHWND());
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiDateTimeWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch(uMsg)
        {
            case WM_KEYDOWN:
                lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
                break;
            case OCM_NOTIFY:
                lRes = OnNotify(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KILLFOCUS:
                lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
                break;
            case WM_PAINT:
                lRes = OnPaint(uMsg, wParam, lParam, bHandled);
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

    void CDuiDateTimeWnd::OnFinalMessage(HWND hWnd)
    {
        if(m_pOwner->GetManager()->IsLayered())
        {
            m_pOwner->GetManager()->RemovePaintChildWnd(hWnd);
        }
        m_pOwner->EmptyDateTimeWnd();
        CDuiWnd::OnFinalMessage(hWnd);
    }

    ////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiDateTime)

    CDuiDateTime::CDuiDateTime(void)
        : m_nDTUpdateFlag(DT_UPDATE)
        , m_pDateTimeWnd(NULL)
    {
        ::GetLocalTime(&m_sysTime);
        UpdateText();
        m_nDTUpdateFlag = DT_NONE;
    }


    CDuiDateTime::~CDuiDateTime(void)
    {
    }

    CDuiString CDuiDateTime::GetClass() const
    {
        return DUI_CTR_DATETIME;
    }

    LPVOID CDuiDateTime::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_DATETIME) == 0)
        {
            return static_cast<CDuiDateTime*>(this);
        }
        return CDuiLabel::GetInterface(pstrName);
    }

    void CDuiDateTime::SetUpdateFlag(int nDTUpdateFlag)
    {
        m_nDTUpdateFlag = nDTUpdateFlag;
    }

    void CDuiDateTime::EmptyDateTimeWnd()
    {
        m_pDateTimeWnd = NULL;
    }

    void CDuiDateTime::UpdateText()
    {
        if(m_nDTUpdateFlag == DT_DELETE)
        {
            SetText(_T(""));
        }
        else if(m_nDTUpdateFlag == DT_UPDATE)
        {
            CDuiString sText;
            sText.SmallFormat(_T("%4d/%2d/%2d"), m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay, m_sysTime.wHour, m_sysTime.wMinute);
            SetText(sText);
        }
    }

    SYSTEMTIME& CDuiDateTime::GetTime()
    {
        return m_sysTime;
    }

    void CDuiDateTime::SetTime(SYSTEMTIME* pst)
    {
        m_sysTime = *pst;
        Invalidate();
        m_nDTUpdateFlag = DT_UPDATE;
        UpdateText();
        m_nDTUpdateFlag = DT_NONE;
    }

    void CDuiDateTime::DoEvent(CDuiEvent& event)
    {
        if(!IsMouseEnabled() && event.Type > DUIEVENT__MOUSEBEGIN && event.Type < DUIEVENT__MOUSEEND)
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
        if(event.Type == DUIEVENT_SETCURSOR && IsEnabled())
        {
            if(m_pDateTimeWnd == NULL)
            {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
            }

            return;
        }
        if(event.Type == DUIEVENT_WINDOWSIZE)
        {
            if(m_pDateTimeWnd != NULL)
            {
                GetManager()->SetFocusNeeded(this);
            }
        }
        if(event.Type == DUIEVENT_SCROLLWHEEL)
        {
            if(m_pDateTimeWnd != NULL)
            {
                return;
            }
        }
        if(event.Type == DUIEVENT_SETFOCUS && IsEnabled())
        {
            if(m_pDateTimeWnd)
            {
                return;
            }
            m_pDateTimeWnd = new CDuiDateTimeWnd();
            ASSERT(m_pDateTimeWnd);
            m_pDateTimeWnd->Init(this);
        }
        if(event.Type == DUIEVENT_KILLFOCUS && IsEnabled())
        {
            Invalidate();
        }
        if(event.Type == DUIEVENT_BUTTONDOWN || event.Type == DUIEVENT_DBLCLICK || event.Type == DUIEVENT_RBUTTONDOWN)
        {
            if(IsEnabled())
            {
                GetManager()->ReleaseCapture();
                if(IsFocused() && m_pDateTimeWnd == NULL)
                {
                    m_pDateTimeWnd = new CDuiDateTimeWnd();
                    ASSERT(m_pDateTimeWnd);
                }
                if(m_pDateTimeWnd != NULL)
                {
                    m_pDateTimeWnd->Init(this);
                }
            }
            return;
        }
        if(event.Type == DUIEVENT_MOUSEMOVE)
        {
            return;
        }
        if(event.Type == DUIEVENT_BUTTONUP)
        {
            return;
        }
        if(event.Type == DUIEVENT_CONTEXTMENU)
        {
            return;
        }
        if(event.Type == DUIEVENT_MOUSEENTER)
        {
            return;
        }
        if(event.Type == DUIEVENT_MOUSELEAVE)
        {
            return;
        }

        CDuiLabel::DoEvent(event);
    }

}