#include "StdAfx.h"
#include "DuiGif.h"

namespace DuiLib
{

    IMPLEMENT_DUICONTROL(CDuiGif)

    CDuiGif::CDuiGif(void)
        : m_bIsPlaying(FALSE)
        , m_bIsAutoPlay(TRUE)
        , m_bIsAutoSize(FALSE)
        , m_nFrameCount(0)
        , m_nFramePosition(0)
        , m_pPropertyItem(NULL)
        , m_pGifImage(NULL)
    {
    }


    CDuiGif::~CDuiGif(void)
    {
        KillTimer(TIMER_ID_GIF);
        DeleteGif();
    }

    CDuiString CDuiGif::GetClass() const
    {
        return DUI_CTR_GIF;
    }

    LPVOID CDuiGif::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_GIF) == 0)
        {
            return static_cast<CDuiGif*>(this);
        }
        return CDuiControl::GetInterface(pstrName);
    }

    void CDuiGif::PlayGif()
    {
        if(m_bIsPlaying || m_pGifImage == NULL)
        {
            return;
        }

        long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
        if(lPause == 0)
        {
            lPause = 100;
        }
        SetTimer(TIMER_ID_GIF, lPause);

        m_bIsPlaying = TRUE;
    }

    void CDuiGif::PauseGif()
    {
        if(!m_bIsPlaying || m_pGifImage == NULL)
        {
            return;
        }

        KillTimer(TIMER_ID_GIF);
        Invalidate();
        m_bIsPlaying = FALSE;
    }

    void CDuiGif::StopGif()
    {
        if(!m_bIsPlaying)
        {
            return;
        }

        KillTimer(TIMER_ID_GIF);
        m_nFramePosition = 0;
        Invalidate();
        m_bIsPlaying = FALSE;
    }

    BOOL CDuiGif::IsAutoPlay() const
    {
        return m_bIsAutoPlay;
    }

    void CDuiGif::SetAutoPlay(BOOL bIsAuto /*= TRUE*/)
    {
        m_bIsAutoPlay = bIsAuto;
    }

    BOOL CDuiGif::IsAutoSize() const
    {
        return m_bIsAutoSize;
    }

    void CDuiGif::SetAutoSize(BOOL bIsAuto /*= TRUE*/)
    {
        m_bIsAutoSize = bIsAuto;
    }

    void CDuiGif::SetBkImage(LPCTSTR pStrImage)
    {
        if(GetBkImage() == pStrImage || NULL == pStrImage)
        {
            return;
        }
        DeleteGif();
        CDuiControl::SetBkImage(pStrImage);
    }

    void CDuiGif::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("autoplay")) == 0)
        {
            SetAutoPlay(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("autosize")) == 0)
        {
            SetAutoSize(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiGif::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetVisible(bVisible);
        if(bVisible)
        {
            PlayGif();
        }
        else
        {
            StopGif();
        }
    }

    void CDuiGif::SetInternVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiControl::SetInternVisible(bVisible);
        if(bVisible)
        {
            PlayGif();
        }
        else
        {
            StopGif();
        }
    }

    void CDuiGif::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }
        if(NULL == m_pGifImage)
        {
            InitGifImage();
        }
        DrawFrame(hDC);
    }

    void CDuiGif::DoInit()
    {
        if(NULL == m_pGifImage)
        {
            InitGifImage();
        }
    }

    void CDuiGif::DoEvent(CDuiEvent& event)
    {
        if(event.Type == DUIEVENT_TIMER && event.wParam == TIMER_ID_GIF)
        {
            m_nFramePosition = (++m_nFramePosition) % m_nFrameCount;
            Invalidate();
        }
        else
        {
            CDuiControl::DoEvent(event);
        }
    }

    void CDuiGif::DrawFrame(HDC hDC)
    {
        if(NULL == hDC || NULL == m_pGifImage)
        {
            return;
        }
        GUID pageGuid = Gdiplus::FrameDimensionTime;
        Gdiplus::Graphics graphics(hDC);
        graphics.DrawImage(m_pGifImage, GetPos().left, GetPos().top, GetPos().right - GetPos().left, GetPos().bottom - GetPos().top);
        m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
    }

    void CDuiGif::InitGifImage()
    {
        m_pGifImage = CRenderEngine::GdiplusLoadImage(GetBkImage());
        if(NULL == m_pGifImage)
        {
            return;
        }
        UINT nCount	= m_pGifImage->GetFrameDimensionsCount();
        GUID* pDimensionIDs	= new GUID[ nCount ];
        m_pGifImage->GetFrameDimensionsList(pDimensionIDs, nCount);
        m_nFrameCount	=	m_pGifImage->GetFrameCount(&pDimensionIDs[0]);
        int nSize		=	m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
        m_pPropertyItem	=	(Gdiplus::PropertyItem*) malloc(nSize);
        m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
        DUI_FREE_ARRAY(pDimensionIDs);

        if(m_bIsAutoSize)
        {
            SetFixedWidth(m_pGifImage->GetWidth());
            SetFixedHeight(m_pGifImage->GetHeight());
        }
        if(m_bIsAutoPlay && Activate())
        {
            PlayGif();
        }
        else
        {
            StopGif();
        }
    }

    void CDuiGif::DeleteGif()
    {
        DUI_FREE_POINT(m_pGifImage);
        if(m_pPropertyItem != NULL)
        {
            free(m_pPropertyItem);
            m_pPropertyItem = NULL;
        }
        m_nFrameCount		=	0;
        m_nFramePosition	=	0;
    }



}