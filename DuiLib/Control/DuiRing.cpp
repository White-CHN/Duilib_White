#include "StdAfx.h"
#include "DuiRing.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiRing)

    CDuiRing::CDuiRing(void)
        : m_fCurAngle(0.0f)
        , m_pBkimage(NULL)
    {
    }


    CDuiRing::~CDuiRing(void)
    {
        StopImage();
        DeleteImage();
    }

    CDuiString CDuiRing::GetClass() const
    {
        return DUI_CTR_RING;
    }

    LPVOID CDuiRing::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_RING)
        {
            return static_cast<CDuiRing*>(this);
        }
        return CDuiLabel::GetInterface(strName);
    }

    void CDuiRing::SetBkImage(LPCTSTR pStrImage)
    {
        if(GetBkImage() == pStrImage)
        {
            return;
        }
        DeleteImage();
        CDuiLabel::SetBkImage(pStrImage);
    }

    void CDuiRing::PaintBkImage(HDC hDC)
    {
        if(m_pBkimage == NULL)
        {
            InitImage();
        }

        if(m_pBkimage != NULL)
        {
            RECT rcItem = GetPos();
            int iWidth = rcItem.right - rcItem.left;
            int iHeight = rcItem.bottom - rcItem.top;
            float x = (float)(rcItem.left) + iWidth / 2;
            float y = (float)(rcItem.top) + iHeight / 2;
            Gdiplus::PointF centerPos(x, y);

            Gdiplus::Graphics graphics(hDC);
            graphics.TranslateTransform(centerPos.X, centerPos.Y);
            graphics.RotateTransform(m_fCurAngle);
            graphics.TranslateTransform(-centerPos.X, -centerPos.Y);//��ԭԴ��
            graphics.DrawImage(m_pBkimage, rcItem.left, rcItem.top, iWidth, iHeight);
        }

    }

    void CDuiRing::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetVisible(bVisible);
        if(bVisible)
        {
            PlayImage();

        }
        else
        {
            StopImage();
        }
    }

    void CDuiRing::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetInternVisible(bVisible);
        if(bVisible)
        {
            PlayImage();
        }
        else
        {
            StopImage();
        }
    }

    void CDuiRing::DoEvent(CDuiEvent& event)
    {
        if(event.Type == DUIEVENT_TIMER && event.wParam == TIMER_ID_RING)
        {
            if(m_fCurAngle > 359)
            {
                m_fCurAngle = 0;
            }
            m_fCurAngle += 36.0;
            Invalidate();
        }
        else
        {
            CDuiLabel::DoEvent(event);
        }
    }

    void CDuiRing::PlayImage()
    {
        if(GetManager())
        {
            SetTimer(TIMER_ID_RING, 100);
        }
    }

    void CDuiRing::StopImage()
    {
        if(GetManager())
        {
            KillTimer(TIMER_ID_RING);
        }
    }

    void CDuiRing::InitImage()
    {
        m_pBkimage = CRenderEngine::GdiplusLoadImage(GetBkImage());
        if(Activate())
        {
            PlayImage();
        }
        else
        {
            StopImage();
        }

    }

    void CDuiRing::DeleteImage()
    {
        DUI_FREE_POINT(m_pBkimage);
    }

    void CDuiRing::DoInit()
    {
        InitImage();
    }

}