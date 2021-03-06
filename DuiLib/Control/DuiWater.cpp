#include "StdAfx.h"
#include "DuiWater.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiWater)

    CDuiWater::CDuiWater(void)
        : m_bUpdate(FALSE)
    {
    }


    CDuiWater::~CDuiWater(void)
    {
        KillTimer(TIMER_ID_WATER);
    }

    CDuiString CDuiWater::GetClass() const
    {
        return DUI_CTR_WATER;
    }

    LPVOID CDuiWater::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_WATER)
        {
            return static_cast<CDuiWater*>(this);
        }
        return CDuiLabel::GetInterface(strName);
    }

    void CDuiWater::DoInit()
    {
        if(Activate())
        {
            SetTimer(TIMER_ID_WATER, 15);
        }
        else
        {
            KillTimer(TIMER_ID_WATER);
        }
    }

    void CDuiWater::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiLabel::SetVisible(bVisible);
        if(bVisible)
        {
            SetTimer(TIMER_ID_WATER, 15);
        }
        else
        {
            KillTimer(TIMER_ID_WATER);
        }
    }

    void CDuiWater::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiLabel::SetInternVisible(bVisible);
        if(bVisible)
        {
            SetTimer(TIMER_ID_WATER, 15);
        }
        else
        {
            KillTimer(TIMER_ID_WATER);
        }
    }

    void CDuiWater::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        CDuiLabel::DoPaint(hDC, rcPaint);
        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }
        CDuiRect rc = GetPos();

        if(m_bUpdate == TRUE)
        {
            m_renderSrc.Create32BitFromPicture(hDC, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
            m_renderDest.Create32BitFromPicture(hDC, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
            m_waterEffect.Create(rc.GetWidth(), rc.GetHeight());
            m_bUpdate = FALSE;
        }

    }

    void CDuiWater::DoEvent(CDuiEvent& event)
    {
        if(event.Type == DUIEVENT_TIMER && event.wParam == TIMER_ID_WATER)
        {
            m_waterEffect.Render((DWORD*)m_renderSrc.GetDIBits(), (DWORD*)m_renderDest.GetDIBits());
            POINT ptOrigin;
            ptOrigin.x = GetPos().left;
            ptOrigin.y = GetPos().top;
            m_renderDest.Draw(GetManager()->GetPaintDC(), ptOrigin);
        }
        else if(event.Type == DUIEVENT_BUTTONDOWN)
        {
            POINT   point;
            point.x = event.ptMouse.x;
            point.y = event.ptMouse.y;
            if(PtInRect(&GetPos(), point) == TRUE)
            {
                point.x -= GetPos().left;
                point.y = GetPos().bottom - point.y;

                m_waterEffect.Blob(point.x , point.y, 10, 1600, m_waterEffect.m_iHpage);
            }
        }
        else if(event.Type == DUIEVENT_MOUSEMOVE)
        {

            POINT   point;
            point.x = event.ptMouse.x;
            point.y = event.ptMouse.y;
            if(PtInRect(&GetPos(), point) == TRUE)
            {
                point.x -= GetPos().left;
                point.y = GetPos().bottom - point.y;
                if(event.wParam & MK_LBUTTON)
                {
                    m_waterEffect.Blob(point.x , point.y, 10, 1600, m_waterEffect.m_iHpage);
                }
                else
                {
                    m_waterEffect.Blob(point.x , point.y, 5, 50, m_waterEffect.m_iHpage);
                }
            }

        }
    }

    void CDuiWater::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiLabel::SetPos(rc, bNeedInvalidate);
        m_bUpdate = TRUE;
    }

}