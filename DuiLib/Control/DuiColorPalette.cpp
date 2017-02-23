#include "StdAfx.h"
#include "DuiColorPalette.h"

namespace DuiLib
{
#define HSLMAX   255	/* H,L, and S vary over 0-HSLMAX */
#define RGBMAX   255    /* R,G, and B vary over 0-RGBMAX */
#define HSLUNDEFINED (HSLMAX*2/3)

    /*
    * Convert hue value to RGB
    */
    static float HueToRGB(float v1, float v2, float vH)
    {
        if(vH < 0.0f)
        {
            vH += 1.0f;
        }
        if(vH > 1.0f)
        {
            vH -= 1.0f;
        }
        if((6.0f * vH) < 1.0f)
        {
            return (v1 + (v2 - v1) * 6.0f * vH);
        }
        if((2.0f * vH) < 1.0f)
        {
            return (v2);
        }
        if((3.0f * vH) < 2.0f)
        {
            return (v1 + (v2 - v1) * ((2.0f / 3.0f) - vH) * 6.0f);
        }
        return (v1);
    }

    /*
    * Convert color RGB to HSL
    * pHue HSL hue value			[0 - 1]
    * pSat HSL saturation value		[0 - 1]
    * pLue HSL luminance value		[0 - 1]
    */

    static void RGBToHSL(DWORD clr, float* pHue, float* pSat, float* pLue)
    {
        float R = (float)(GetRValue(clr) / 255.0f);  //RGB from 0 to 255
        float G = (float)(GetGValue(clr) / 255.0f);
        float B = (float)(GetBValue(clr) / 255.0f);

        float H, S, L;

        float fMin = min(R, min(G, B));		//Min. value of RGB
        float fMax = max(R, max(G, B));		//Max. value of RGB
        float fDelta = fMax - fMin;				//Delta RGB value

        L = (fMax + fMin) / 2.0f;

        if(fDelta == 0)                      //This is a gray, no chroma...
        {
            H = 0.0f;                          //HSL results from 0 to 1
            S = 0.0f;
        }
        else                                   //Chromatic data...
        {
            float del_R, del_G, del_B;

            if(L < 0.5)
            {
                S = fDelta / (fMax + fMin);
            }
            else
            {
                S = fDelta / (2.0f - fMax - fMin);
            }

            del_R = (((fMax - R) / 6.0f) + (fDelta / 2.0f)) / fDelta;
            del_G = (((fMax - G) / 6.0f) + (fDelta / 2.0f)) / fDelta;
            del_B = (((fMax - B) / 6.0f) + (fDelta / 2.0f)) / fDelta;

            if(R == fMax)
            {
                H = del_B - del_G;
            }
            else if(G == fMax)
            {
                H = (1.0f / 3.0f) + del_R - del_B;
            }
            else if(B == fMax)
            {
                H = (2.0f / 3.0f) + del_G - del_R;
            }

            if(H < 0.0f)
            {
                H += 1.0f;
            }
            if(H > 1.0f)
            {
                H -= 1.0f;
            }
        }

        *pHue = H;
        *pSat = S;
        *pLue = L;
    }

    /*
    * Convert color HSL to RGB
    * H HSL hue value				[0 - 1]
    * S HSL saturation value		[0 - 1]
    * L HSL luminance value			[0 - 1]
    */
    static DWORD HSLToRGB(float H, float S, float L)
    {
        BYTE R, G, B;
        float var_1, var_2;

        if(S == 0)                        //HSL from 0 to 1
        {
            R = G = B = (BYTE)(L * 255.0f);   //RGB results from 0 to 255
        }
        else
        {
            if(L < 0.5)
            {
                var_2 = L * (1.0f + S);
            }
            else
            {
                var_2 = (L + S) - (S * L);
            }

            var_1 = 2.0f * L - var_2;

            R = (BYTE)(255.0f * HueToRGB(var_1, var_2, H + (1.0f / 3.0f)));
            G = (BYTE)(255.0f * HueToRGB(var_1, var_2, H));
            B = (BYTE)(255.0f * HueToRGB(var_1, var_2, H - (1.0f / 3.0f)));
        }

        return RGB(R, G, B);
    }

    /*
    * _HSLToRGB color HSL value to RGB
    * clr  RGB color value
    * nHue HSL hue value			[0 - 360]
    * nSat HSL saturation value		[0 - 200]
    * nLue HSL luminance value		[0 - 200]
    */
#define _HSLToRGB(h,s,l) (0xFF << 24 | HSLToRGB((float)h / 360.0f,(float)s / 200.0f,l / 200.0f))

    ///////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiColorPalette)

    CDuiColorPalette::CDuiColorPalette(void)
        : m_MemDc(NULL)
        , m_hMemBitmap(NULL)
        , m_bIsInBar(FALSE)
        , m_bIsInPallet(FALSE)
        , m_nPalletHeight(200)
        , m_nBarHeight(10)
        , m_nCurH(180)
        , m_nCurS(200)
        , m_nCurB(100)
        , m_uButtonState(0)
        , m_pBits(NULL)
    {
        ZeroMemory(&m_bmInfo, sizeof(m_bmInfo));
    }


    CDuiColorPalette::~CDuiColorPalette(void)
    {
        if(m_pBits)
        {
            free(m_pBits);
            m_pBits = NULL;
        }

        if(m_hMemBitmap)
        {
            ::DeleteObject(m_hMemBitmap);
            m_hMemBitmap = NULL;
        }
        if(m_MemDc)
        {
            DeleteDC(m_MemDc);
            m_MemDc = NULL;
        }
    }

    CDuiString CDuiColorPalette::GetClass() const
    {
        return DUI_CTR_COLORPALETTE;
    }

    LPVOID CDuiColorPalette::GetInterface(CDuiString strName)
    {
        if(strName == DUI_CTR_COLORPALETTE)
        {
            return static_cast<CDuiColorPalette*>(this);
        }
        return CDuiControl::GetInterface(strName);
    }

    DWORD CDuiColorPalette::GetSelectColor()
    {
        DWORD dwColor = _HSLToRGB(m_nCurH, m_nCurS, m_nCurB);
        return 0xFF << 24 | GetRValue(dwColor) << 16 | GetGValue(dwColor) << 8 | GetBValue(dwColor);
    }

    void CDuiColorPalette::SetSelectColor(DWORD dwColor)
    {
        float H = 0, S = 0, B = 0;
        COLORREF dwBkClr = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
        RGBToHSL(dwBkClr, &H, &S, &B);
        m_nCurH = (int)(H * 360);
        m_nCurS = (int)(S * 200);
        m_nCurB = (int)(B * 200);
        UpdatePalletData();
        NeedUpdate();
    }

    void CDuiColorPalette::SetPalletHeight(int nHeight)
    {
        m_nPalletHeight = nHeight;
    }
    int	 CDuiColorPalette::GetPalletHeight() const
    {
        return m_nPalletHeight;
    }
    void CDuiColorPalette::SetBarHeight(int nHeight)
    {
        if(nHeight > 150)
        {
            nHeight = 150; //�������߶ȣ����ڵ�ǰ��ƣ�nheight����190���������Խ����ʱ���
        }
        m_nBarHeight = nHeight;
    }
    int  CDuiColorPalette::GetBarHeight() const
    {
        return m_nBarHeight;
    }

    void CDuiColorPalette::SetThumbImage(LPCTSTR pszImage)
    {
        if(m_strThumbImage != pszImage)
        {
            m_strThumbImage = pszImage;
            NeedUpdate();
        }
    }

    CDuiString CDuiColorPalette::GetThumbImage() const
    {
        return m_strThumbImage;
    }

    void CDuiColorPalette::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcscmp(pstrName, _T("palletheight")) == 0)
        {
            SetPalletHeight(_ttoi(pstrValue));
        }
        else if(_tcscmp(pstrName, _T("barheight")) == 0)
        {
            SetBarHeight(_ttoi(pstrValue));
        }
        else if(_tcscmp(pstrName, _T("thumbimage")) == 0)
        {
            SetThumbImage(pstrValue);
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiColorPalette::DoInit()
    {
        m_MemDc = CreateCompatibleDC(GetManager()->GetPaintDC());
        m_hMemBitmap = CreateCompatibleBitmap(GetManager()->GetPaintDC(), 400, 360);
        HBITMAP pOldBit = (HBITMAP)SelectObject(m_MemDc, m_hMemBitmap);

        ::GetObject(m_hMemBitmap, sizeof(m_bmInfo), &m_bmInfo);
        DWORD dwSize = m_bmInfo.bmHeight * m_bmInfo.bmWidthBytes;
        m_pBits = (BYTE*)malloc(dwSize);
        ::GetBitmapBits(m_hMemBitmap, dwSize, m_pBits);
    }

    void CDuiColorPalette::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);

        m_ptLastPalletMouse.x = m_nCurH * (GetPos().right - GetPos().left) / 360 + GetPos().left;
        m_ptLastPalletMouse.y = (200 - m_nCurB) * m_nPalletHeight / 200 + GetPos().top;

        UpdatePalletData();
        UpdateBarData();
    }

    void CDuiColorPalette::PaintPallet(HDC hDC)
    {
        int nSaveDC = ::SaveDC(hDC);

        ::SetStretchBltMode(hDC, HALFTONE);
        //����ģʽ���ڴ�ͼ�����ؼ���
        CDuiRect rc = GetPos();
        StretchBlt(hDC, rc.left, rc.top, rc.right - rc.left, m_nPalletHeight, m_MemDc, 0, 1, 360, 200, SRCCOPY);
        StretchBlt(hDC, rc.left, rc.bottom - m_nBarHeight, rc.right - rc.left, m_nBarHeight, m_MemDc, 0, 210, 200, m_nBarHeight, SRCCOPY);

        RECT rcCurSorPaint = { m_ptLastPalletMouse.x - 4, m_ptLastPalletMouse.y - 4, m_ptLastPalletMouse.x + 4, m_ptLastPalletMouse.y + 4 };
        CRenderEngine::DrawImageString(hDC, GetManager(), rcCurSorPaint, GetPaintRect(), m_strThumbImage);

        rcCurSorPaint.left = rc.left + m_nCurS * (rc.right - rc.left) / 200 - 4;
        rcCurSorPaint.right = rc.left + m_nCurS * (rc.right - rc.left) / 200 + 4;
        rcCurSorPaint.top = rc.bottom - m_nBarHeight / 2 - 4;
        rcCurSorPaint.bottom = rc.bottom - m_nBarHeight / 2 + 4;
        CRenderEngine::DrawImageString(hDC, GetManager(), rcCurSorPaint, GetPaintRect(), m_strThumbImage);
        ::RestoreDC(hDC, nSaveDC);
    }

    void CDuiColorPalette::PaintBkColor(HDC hDC)
    {
        PaintPallet(hDC);
    }

    void CDuiColorPalette::DoEvent(CDuiEvent& event)
    {
        if(event.Type == DUIEVENT_BUTTONDOWN)
        {
            CDuiRect rc = GetPos();
            if(event.ptMouse.x >= rc.left && event.ptMouse.y >= rc.top &&
                    event.ptMouse.x < rc.right && event.ptMouse.y < rc.top + m_nPalletHeight)
            {
                int x = (event.ptMouse.x - rc.left) * 360 / (rc.right - rc.left);
                int y = (event.ptMouse.y - rc.top) * 200 / m_nPalletHeight;
                x = min(max(x, 0), 360);
                y = min(max(y, 0), 200);

                m_ptLastPalletMouse = event.ptMouse;
                if(m_ptLastPalletMouse.x < rc.left)
                {
                    m_ptLastPalletMouse.x = rc.left;
                }
                if(m_ptLastPalletMouse.x > rc.right)
                {
                    m_ptLastPalletMouse.x = rc.right;
                }
                if(m_ptLastPalletMouse.y < rc.top)
                {
                    m_ptLastPalletMouse.y = rc.top;
                }
                if(m_ptLastPalletMouse.y > rc.top + m_nPalletHeight)
                {
                    m_ptLastPalletMouse.y = rc.top + m_nPalletHeight;
                }

                m_nCurH = x;
                m_nCurB = 200 - y;

                m_uButtonState |= UISTATE_PUSHED;
                m_bIsInPallet = TRUE;
                m_bIsInBar = FALSE;

                UpdateBarData();
            }
            //::PtInRect(&m_rcItem, event.ptMouse)
            if(event.ptMouse.x >= rc.left && event.ptMouse.y >= rc.bottom - m_nBarHeight &&
                    event.ptMouse.x < rc.right && event.ptMouse.y < rc.bottom)
            {
                m_nCurS = (event.ptMouse.x - rc.left) * 200 / (rc.right - rc.left);
                m_uButtonState |= UISTATE_PUSHED;
                m_bIsInBar = TRUE;
                m_bIsInPallet = FALSE;
                UpdatePalletData();
            }
            NeedParentUpdate();
            return;
        }
        if(event.Type == DUIEVENT_BUTTONUP)
        {
            if((m_uButtonState & UISTATE_PUSHED) && (IsEnabled()))
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_COLORCHANGED, GetSelectColor(), 0);
            }

            m_uButtonState &= ~UISTATE_PUSHED;
            m_bIsInPallet = FALSE;
            m_bIsInBar = FALSE;

            NeedParentUpdate();
            return;
        }
        if(event.Type == DUIEVENT_MOUSEMOVE)
        {
            CDuiRect rc = GetPos();
            if(!(m_uButtonState & UISTATE_PUSHED))
            {
                m_bIsInBar = FALSE;
                m_bIsInPallet = FALSE;
            }
            if(m_bIsInPallet == TRUE)
            {

                POINT pt = event.ptMouse;
                pt.x -= rc.left;
                pt.y -= rc.top;

                if(pt.x >= 0 && pt.y >= 0 && pt.x <= rc.right && pt.y <= rc.top + m_nPalletHeight)
                {
                    int x = pt.x * 360 / (rc.right - rc.left);
                    int y = pt.y * 200 / m_nPalletHeight;
                    x = min(max(x, 0), 360);
                    y = min(max(y, 0), 200);

                    m_ptLastPalletMouse = event.ptMouse;
                    if(m_ptLastPalletMouse.x < rc.left)
                    {
                        m_ptLastPalletMouse.x = rc.left;
                    }
                    if(m_ptLastPalletMouse.x > rc.right)
                    {
                        m_ptLastPalletMouse.x = rc.right;
                    }
                    if(m_ptLastPalletMouse.y < rc.top)
                    {
                        m_ptLastPalletMouse.y = rc.top;
                    }
                    if(m_ptLastPalletMouse.y > rc.top + m_nPalletHeight)
                    {
                        m_ptLastPalletMouse.y = rc.top + m_nPalletHeight;
                    }
                    m_nCurH = x;
                    m_nCurB = 200 - y;
                    UpdateBarData();
                }
            }
            else if(m_bIsInBar == TRUE)
            {
                m_nCurS = (event.ptMouse.x - rc.left) * 200 / (rc.right - rc.left);
                m_nCurS = min(max(m_nCurS, 0), 200);
                UpdatePalletData();
            }
            if(m_bIsInBar || m_bIsInPallet)
            {
                if((m_uButtonState & UISTATE_PUSHED) && (IsEnabled()))
                {
                    GetManager()->SendNotify(this, DUI_MSGTYPE_COLORCHANGED, GetSelectColor(), 0);
                }
            }
            NeedParentUpdate();
            return;
        }
        CDuiControl::DoEvent(event);
    }

    void CDuiColorPalette::UpdatePalletData()
    {
        int x, y;
        BYTE* pPiexl;
        DWORD dwColor;
        for(y = 0; y < 200; ++y)
        {
            for(x = 0; x < 360; ++x)
            {
                pPiexl = LPBYTE(m_pBits) + ((200 - y) * m_bmInfo.bmWidthBytes) + ((x * m_bmInfo.bmBitsPixel) / 8);
                dwColor = _HSLToRGB(x, m_nCurS, y);
                if(dwColor == 0xFF000000)
                {
                    dwColor = 0xFF000001;
                }
                pPiexl[0] = GetBValue(dwColor);
                pPiexl[1] = GetGValue(dwColor);
                pPiexl[2] = GetRValue(dwColor);
            }
        }

        SetBitmapBits(m_hMemBitmap, m_bmInfo.bmWidthBytes * m_bmInfo.bmHeight, m_pBits);
    }

    void CDuiColorPalette::UpdateBarData()
    {
        int x, y;
        BYTE* pPiexl;
        DWORD dwColor;
        //���ﻭ��Bar
        for(y = 0; y < m_nBarHeight; ++y)
        {
            for(x = 0; x < 200; ++x)
            {
                pPiexl = LPBYTE(m_pBits) + ((210 + y) * m_bmInfo.bmWidthBytes) + ((x * m_bmInfo.bmBitsPixel) / 8);
                dwColor = _HSLToRGB(m_nCurH, x, m_nCurB);
                if(dwColor == 0xFF000000)
                {
                    dwColor = 0xFF000001;
                }
                pPiexl[0] = GetBValue(dwColor);
                pPiexl[1] = GetGValue(dwColor);
                pPiexl[2] = GetRValue(dwColor);
            }
        }

        SetBitmapBits(m_hMemBitmap, m_bmInfo.bmWidthBytes * m_bmInfo.bmHeight, m_pBits);
    }

}