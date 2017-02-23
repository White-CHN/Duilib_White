#include "StdAfx.h"
#include "DuiRollText.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiRollText)

    CDuiRollText::CDuiRollText(void)
        : m_nStep(5)
        , m_nScrollPos(0)
        , m_nRollDirection(ROLLTEXT_LEFT)
        , m_nText_W_H(0)
        , m_nSpeed(100)
    {
    }


    CDuiRollText::~CDuiRollText(void)
    {
        KillTimer(TIMER_ID_ROLLTEXT);
    }

    CDuiString CDuiRollText::GetClass() const
    {
        return DUI_CTR_ROLLTEXT;
    }

    LPVOID CDuiRollText::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_ROLLTEXT)
        {
            return static_cast<CDuiRollText*>(this);
        }
        return CDuiLabel::GetInterface(strName);
    }

    void CDuiRollText::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiLabel::SetVisible(bVisible);
        if(bVisible)
        {
            SetTimer(TIMER_ID_ROLLTEXT, m_nSpeed);
        }
        else
        {
            KillTimer(TIMER_ID_ROLLTEXT);
        }
    }

    void CDuiRollText::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiLabel::SetInternVisible(bVisible);
        if(bVisible)
        {
            SetTimer(TIMER_ID_ROLLTEXT, m_nSpeed);
        }
        else
        {
            KillTimer(TIMER_ID_ROLLTEXT);
        }
    }

    int CDuiRollText::GetSpeed() const
    {
        return m_nSpeed;
    }

    void CDuiRollText::SetSpeed(int nSpeed)
    {
        if(m_nSpeed == nSpeed)
        {
            return;
        }
        m_nSpeed = nSpeed;
        DoInit();
    }

    int CDuiRollText::GetRollDirection() const
    {
        return m_nRollDirection;
    }

    void CDuiRollText::SetRollDirection(int nDirection)
    {
        if(m_nRollDirection == nDirection)
        {
            return;
        }
        m_nRollDirection = nDirection;
        m_nText_W_H = 0;
    }

    void CDuiRollText::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("speed")) == 0)
        {
            SetSpeed(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("direction")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                SetRollDirection(ROLLTEXT_LEFT);
            }
            else if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                SetRollDirection(ROLLTEXT_RIGHT);
            }
            else if(_tcsstr(pstrValue, _T("up")) != NULL)
            {
                SetRollDirection(ROLLTEXT_UP);
            }
            else if(_tcsstr(pstrValue, _T("down")) != NULL)
            {
                SetRollDirection(ROLLTEXT_DOWN);
            }
        }
        else
        {
            CDuiLabel::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiRollText::DoInit()
    {
        if(Activate())
        {
            m_nScrollPos = 0;
            m_nText_W_H = 0;
            KillTimer(TIMER_ID_ROLLTEXT);
            SetTimer(TIMER_ID_ROLLTEXT, m_nSpeed);
        }
        else
        {
            KillTimer(TIMER_ID_ROLLTEXT);
        }
    }

    void CDuiRollText::PaintText(HDC hDC)
    {
        if(GetTextColor() == 0)
        {
            SetTextColor(GetManager()->GetDefaultFontColor());
        }
        if(GetDisabledTextColor() == 0)
        {
            SetDisabledTextColor(GetManager()->GetDefaultDisabledColor());
        }
        DWORD dwTextColor = IsEnabled() ? GetTextColor() : GetDisabledTextColor();
        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }
        RECT rc = rcClient;

        UINT uTextStyle = DT_WORDBREAK | DT_EDITCONTROL;

        if(m_nText_W_H == 0)
        {
            uTextStyle |= DT_CALCRECT;				//第一次计算文本宽度或高度
            if(m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT)  	//左面移动
            {
                rc.right += 10000;
            }
            else  																		//上下移动
            {
                rc.bottom += 10000;
            }
        }

        if(IsShowHtml())
        {
            int nLinks = 0;
            CRenderEngine::DrawHtmlText(hDC, GetManager(), rc, sText, dwTextColor, NULL, NULL, nLinks, uTextStyle);
        }
        else
        {
            CRenderEngine::DrawText(hDC, GetManager(), rc, sText, dwTextColor, GetFont(), uTextStyle);
        }

        if(m_nText_W_H == 0)
        {
            m_nText_W_H = (m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT) ? (rc.right - rc.left) : (rc.bottom - rc.top);		//计算文本宽度或高度
        }
    }

    void CDuiRollText::DoEvent(CDuiEvent& event)
    {
        if(event.Type == DUIEVENT_TIMER && event.wParam == TIMER_ID_ROLLTEXT)
        {
            rcClient = GetPos();
            rcClient.left += GetTextPadding().left;
            rcClient.right -= GetTextPadding().right;
            rcClient.top += GetTextPadding().top;
            rcClient.bottom -= GetTextPadding().bottom;

            if(m_nText_W_H > 0)
            {
                int nScrollRange = 0;

                if(m_nRollDirection == ROLLTEXT_LEFT || m_nRollDirection == ROLLTEXT_RIGHT)  	//左面移动
                {
                    nScrollRange = m_nText_W_H + rcClient.GetWidth();

                    rcClient.Offset((m_nRollDirection == ROLLTEXT_LEFT ? rcClient.GetWidth() : -rcClient.GetWidth()), 0);
                    rcClient.Offset((m_nRollDirection == ROLLTEXT_LEFT ? -m_nScrollPos : m_nScrollPos), 0);
                    rcClient.right += (m_nText_W_H - rcClient.GetWidth());
                }
                else  																		//上下移动
                {
                    nScrollRange = m_nText_W_H + rcClient.GetHeight();

                    rcClient.Offset(0, (m_nRollDirection == ROLLTEXT_UP ? rcClient.GetHeight() : -rcClient.GetHeight()));
                    rcClient.Offset(0, (m_nRollDirection == ROLLTEXT_UP ? -m_nScrollPos : m_nScrollPos));
                    rcClient.bottom += (m_nText_W_H - rcClient.GetHeight());
                }

                m_nScrollPos += m_nStep;
                if(m_nScrollPos > nScrollRange)
                {
                    m_nScrollPos = 0;
                }
            }
            Invalidate();
            return;
        }
        CDuiLabel::DoEvent(event);
    }

    void CDuiRollText::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiLabel::SetPos(rc);
        m_nText_W_H = 0;			//布局变化重新计算
    }

    void CDuiRollText::SetText(LPCTSTR pstrText)
    {
        CDuiLabel::SetText(pstrText);
        m_nText_W_H = 0;			//文本变化重新计算
    }

}