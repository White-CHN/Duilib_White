#include "StdAfx.h"
#include "DuiWater.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiWater)

    CDuiWater::CDuiWater(void)
    {
    }


    CDuiWater::~CDuiWater(void)
    {
    }

    CDuiString CDuiWater::GetClass() const
    {
        return DUI_CTR_WATER;
    }

    LPVOID CDuiWater::GetInterface(LPCTSTR pstrName)
    {
        if(_tcscmp(pstrName, DUI_CTR_WATER) == 0)
        {
            return static_cast<CDuiWater*>(this);
        }
        return CDuiLabel::GetInterface(pstrName);
    }

    void CDuiWater::DoInit()
    {
        GetManager()->SetTimer(this, TIMER_ID_WATER, 15);
    }

    void CDuiWater::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetInternVisible(bVisible);
        if(bVisible)
        {
            GetManager()->SetTimer(this, TIMER_ID_WATER, 15);
        }
        else
        {
            GetManager()->KillTimer(this, TIMER_ID_WATER);
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
        m_renderSrc.Create32BitFromPicture(hDC, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
        m_renderDest.Create32BitFromPicture(hDC, rc.left, rc.top, rc.GetWidth(), rc.GetHeight());
        m_waterEffect.Create(rc.GetWidth(), rc.GetHeight());
    }

    void CDuiWater::DoEvent(TEventUI& event)
    {
        if(event.Type == UIEVENT_TIMER && event.wParam == TIMER_ID_WATER)
        {
            m_waterEffect.Render((DWORD*)m_renderSrc.GetDIBits(), (DWORD*)m_renderDest.GetDIBits());
            POINT ptOrigin;
            ptOrigin.x = GetPos().left;
            ptOrigin.y = GetPos().top;
            m_renderDest.Draw(GetManager()->GetPaintDC(), ptOrigin);
        }
        else if(event.Type == UIEVENT_BUTTONDOWN)
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
        else if(event.Type == UIEVENT_MOUSEMOVE)
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

}