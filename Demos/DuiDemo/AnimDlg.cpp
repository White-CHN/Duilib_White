#include "stdafx.h"
#include "AnimDlg.h"
#include "DuiAnimLayout.h"

DUI_BEGIN_MESSAGE_MAP(CAnimDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_WINDOWINIT()
DUI_END_MESSAGE_MAP()

CAnimDlg::CAnimDlg(void)
    : m_pAnimLayout(NULL)
{
}


CAnimDlg::~CAnimDlg(void)
{
}

CDuiString CAnimDlg::GetSkinFile()
{
    return _T("AnimDlg.xml");
}

LPCTSTR CAnimDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CAnimDlg);
}

CDuiControl* CAnimDlg::CreateControl(LPCTSTR pstrClassName)
{
    if(_tcsicmp(pstrClassName, DUI_CTR_ANIMLAYOUT) == 0)
    {
        return new CDuiAnimLayout;
    }
    return NULL;
}

void CAnimDlg::InitWindow()
{
    m_pAnimLayout = static_cast<CDuiAnimLayout*>(GetPaintManager()->FindControl(_T("vlayout_background")));
    ASSERT(m_pAnimLayout != NULL);

}

LRESULT CAnimDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    if(!::IsZoomed(*this))
    {
        RECT rcSizeBox = GetPaintManager()->GetSizeBox();
        if(pt.y < rcClient.top + rcSizeBox.top)
        {
            if(pt.x < rcClient.left + rcSizeBox.left)
            {
                return HTTOPLEFT;
            }
            if(pt.x > rcClient.right - rcSizeBox.right)
            {
                return HTTOPRIGHT;
            }
            return HTTOP;
        }
        else if(pt.y > rcClient.bottom - rcSizeBox.bottom)
        {
            if(pt.x < rcClient.left + rcSizeBox.left)
            {
                return HTBOTTOMLEFT;
            }
            if(pt.x > rcClient.right - rcSizeBox.right)
            {
                return HTBOTTOMRIGHT;
            }
            return HTBOTTOM;
        }

        if(pt.x < rcClient.left + rcSizeBox.left)
        {
            return HTLEFT;
        }
        if(pt.x > rcClient.right - rcSizeBox.right)
        {
            return HTRIGHT;
        }
    }

    RECT rcCaption = GetPaintManager()->GetCaptionRect();
    if(pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
    {
        CDuiControl* pControl = static_cast<CDuiControl*>(GetPaintManager()->FindControl(pt));
        if(pControl && pControl->GetClass() != DUI_CTR_BUTTON  &&
                pControl->GetClass() != DUI_CTR_FADEBUTTON &&
                pControl->GetClass() != DUI_CTR_OPTION &&
                pControl->GetClass() != DUI_CTR_SLIDER &&
                pControl->GetClass() != DUI_CTR_EDIT)

        {
            return HTCAPTION;
        }
    }

    return HTCLIENT;
}

void CAnimDlg::OnWindowInit(TNotifyUI& msg)
{
    for(int i = 0; i < 79; i++)
    {
        m_pAnimLayout->StartEffect();
    }
}

void CAnimDlg::OnClick(TNotifyUI& msg)
{
    if(msg.pSender->GetName() == _T("closebtn"))
    {
        Close(IDCANCEL);
        return;
    }
}
