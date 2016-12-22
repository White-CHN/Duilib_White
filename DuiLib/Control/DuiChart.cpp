#include "StdAfx.h"
#include "DuiChart.h"


namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiChart)

    CDuiChart::CDuiChart(void)
        : m_bShowHtml(FALSE)
        , m_bShowText(TRUE)
        , m_iFont(-1)
        , m_ViewStyle(CHARTVIEW_PIE)
        , m_PillarImageWidth(0)
        , m_ShadowImageHeight(0)
        , m_dwTextColor(0)
        , m_dwDisabledTextColor(0)
    {
    }


    CDuiChart::~CDuiChart(void)
    {
    }

    BOOL CDuiChart::Add(LPCTSTR name, double value)
    {
        CHARTITEM item;
        item.name = name;
        item.value = value;

        m_items.push_back(item);
        return TRUE;
    }

    BOOL CDuiChart::AddAt(LPCTSTR name, double value, int iIndex)
    {
        if(iIndex < 0 || iIndex > (int)m_items.size())
        {
            return FALSE;
        }

        CHARTITEM item;
        item.name = name;
        item.value = value;

        m_items.insert(m_items.begin() + iIndex, item);

        return TRUE;
    }

    void CDuiChart::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {

        if(_tcscmp(pstrName, _T("view")) == 0)
        {
            if(_tcscmp(pstrValue, _T("pie")) == 0)
            {
                m_ViewStyle = CHARTVIEW_PIE;
            }
            else if(_tcscmp(pstrValue, _T("histpgram")) == 0)
            {
                m_ViewStyle = CHARTVIEW_HISTOGRAM;
            }
        }
        else if(_tcscmp(pstrName, _T("shadowimage")) == 0)
        {
            m_sShadowImage = pstrValue;
        }
        else if(_tcscmp(pstrName, _T("shadowimageheight")) == 0)
        {
            LPTSTR pstr = NULL;
            m_ShadowImageHeight = _tcstol(pstrValue, &pstr, 10);
        }
        else if(_tcscmp(pstrName, _T("pillarimage")) == 0)
        {
            m_sPillarImage = pstrValue;
        }
        else if(_tcscmp(pstrName, _T("pillarimagewidth")) == 0)
        {
            LPTSTR pstr = NULL;
            m_PillarImageWidth = _tcstol(pstrValue, &pstr, 10);
        }
        else if(_tcscmp(pstrName, _T("showhtml")) == 0)
        {
            m_bShowHtml = (_tcscmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcscmp(pstrName, _T("showtext")) == 0)
        {
            m_bShowText = (_tcscmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcscmp(pstrName, _T("font")) == 0)
        {
            LPTSTR pstr = NULL;
            m_iFont = _tcstol(pstrValue, &pstr, 10);
        }
        else
        {
            CDuiControl::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiChart::DoPaintPie(HDC hDC, const RECT& rcPaint)
    {
        // ����ͼ�� ������ʹ����һЩӲ���룬 ���ݱ������� �Ϳ���֪�������˼��
        // ��ɫ�����Լ���ɫ
        const int NumOfColor = 18;
        static COLORREF bkColor[] =
        {
            RGB(63, 150, 245),
            RGB(255, 90, 1),
            RGB(106, 244, 192),
            RGB(245, 234, 106),
            RGB(252, 123, 151),
            RGB(162, 245, 63),
            RGB(157, 144, 252),
            RGB(210, 144, 252),
            RGB(127, 176, 203),
            RGB(244, 196, 113),
            RGB(162, 194, 133),
            RGB(189, 23, 23),
            RGB(189, 23, 179),
            RGB(97, 23, 189),
            RGB(23, 43, 189),
            RGB(18, 136, 76),
            RGB(136, 121, 18),
            RGB(136, 38, 18)
        };

        int numOfItem = (int)m_items.size();
        if(0 == numOfItem)
        {
            return;
        }

        // �滭����������ֵ�Ŀ��
        int drawTextWidth = (m_bShowText ? 100 : 0);
        int drawTextHeight = 18;

        int i = 0;
        double amountOfItem = 0.f;
        for(i = 0; i < numOfItem; i++)
        {
            amountOfItem += m_items[i].value;
        }

        Gdiplus::Graphics graphics(hDC);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

        int drawPadding = 5; // ����һ��λ�û���Ӱ
        // �����ͼ�ĺ���λ��(���л���ͼ)
        CDuiRect rc = GetPos();
        int itemWidth = rc.right - rc.left;
        int itemHeight = rc.bottom - rc.top;
        int drawWidth = itemWidth - drawTextWidth - drawPadding * 2;
        int drawHeight = itemHeight - drawPadding * 2;
        int drawMaxSize = min(drawWidth, drawHeight);
        int drawLeft = rc.left + drawPadding + (drawWidth - drawMaxSize) / 2;
        int drawTop = rc.top + drawPadding + (drawHeight - drawMaxSize) / 2;
        int drawTextTop = rc.top + (drawHeight - numOfItem * drawTextHeight - numOfItem * 4) / 2;
        if(drawTextTop <= rc.top + drawPadding)
        {
            drawTextTop = rc.top + drawPadding;
        }

        // ���ⲿ��ǳɫ��Ӱ
        Gdiplus::Color ellipseColor1(30, 30, 30, 30);
        Gdiplus::SolidBrush ellipseBrush1(ellipseColor1);
        graphics.FillEllipse(&ellipseBrush1, drawLeft - 4, drawTop - 4, drawMaxSize + 8, drawMaxSize + 8);
        // ���ⲿ����ɫ����
        Gdiplus::Color ellipseColor2(100, 100, 100, 100);
        Gdiplus::SolidBrush ellipseBrush2(ellipseColor2);
        graphics.FillEllipse(&ellipseBrush2, drawLeft - 1, drawTop - 1, drawMaxSize + 2, drawMaxSize + 2);

        Gdiplus::REAL startAngle = 0;
        for(int i = 0; i < numOfItem; i++)
        {
            // ����ͼ
            Gdiplus::Color color;
            color.SetFromCOLORREF(bkColor[i % NumOfColor]);
            Gdiplus::SolidBrush brush(color);

            Gdiplus::REAL endAngle = (Gdiplus::REAL)(m_items[i].value / amountOfItem * 360);
            graphics.FillPie(&brush, drawLeft, drawTop, drawMaxSize, drawMaxSize, startAngle, endAngle);
            startAngle += endAngle;

            // ������
            if(m_bShowText)
            {
                // �滭����
                RECT rcText = {0};
                rcText.left = drawLeft + drawMaxSize + 10; // 10Ϊ��ͼ������֮��ļ��
                rcText.top = drawTextTop;
                rcText.right = rcText.left + drawTextWidth + drawTextHeight;
                rcText.bottom = rcText.top + drawTextHeight;

                if(::IntersectRect(&GetPaintRect(), &rcText, &GetPos()))
                {
                    graphics.FillRectangle(&brush, rcText.left, rcText.top, drawTextHeight, drawTextHeight);

                    rcText.left += drawTextHeight + 5; // ��Ϊ������ǰ�滭��һ��ɫ�飬 ��������Ҫ��ɫ��������, 5Ϊ���ֺ�ɫ��ļ��
                    int nLinks = 0;
                    DWORD clrColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
                    if(m_bShowHtml)
                    {
                        CRenderEngine::DrawHtmlText(hDC, GetManager(), rcText, m_items[i].name, m_dwTextColor, \
                                                    NULL, NULL, nLinks, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    }
                    else
                    {
                        CRenderEngine::DrawText(hDC, GetManager(), rcText, m_items[i].name, m_dwTextColor, \
                                                m_iFont, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                    }
                }

                // ������һ�����������ֱλ�õ�ƫ������ 4Ϊ���ִ�ֱ���
                drawTextTop += drawTextHeight + 4;
            }
        }
    }

    void CDuiChart::DoPaintHistogram(HDC hDC, const RECT& rcPaint)
    {
        // ����״ͼ
        int drawTextHeight = (m_bShowText ? 25 : 0);	// ��״ͼ�ײ�������ֵ�ĸ߶��Ի�������

        // ����״ͼ��Ӱ
        if(!m_sShadowImage.IsEmpty())
        {
            RECT rcShadow = GetPos();
            rcShadow.top = GetPos().bottom - m_ShadowImageHeight - drawTextHeight;
            rcShadow.right = GetPos().right;
            rcShadow.bottom = GetPos().bottom - drawTextHeight;
            if(!CRenderEngine::DrawImageString(hDC, GetManager(), rcShadow, GetPaintRect(), m_sShadowImage, NULL))
            {
                m_sShadowImage.SetEmpty();
            }
        }

        int numOfItem = (int)m_items.size();
        if(0 == numOfItem)
        {
            return;
        }

        int i = 0;
        double maxOfItem = 0.f;
        double amountOfItem = 0.f;
        for(i = 0; i < numOfItem; i++)
        {
            if(m_items[i].value > maxOfItem)
            {
                maxOfItem = m_items[i].value;
            }

            amountOfItem += m_items[i].value;
        }
        CDuiRect rc = GetPos();
        int drawWidth = rc.right - rc.left;	// ��ͼ���
        int drawHeight = rc.bottom - rc.top - drawTextHeight; // ��ͼ�߶�
        int pillarSpacing = (drawWidth / m_PillarImageWidth) * 2;	// ���Ӽ���

        // ���ӵĻ���ˮƽƫ��λ��
        int drawLeft = pillarSpacing;
        for(i = 0; i < numOfItem; i++)
        {
            // �滭����
            if(!m_sPillarImage.IsEmpty())
            {
                double rateOfItem = m_items[i].value / maxOfItem;
                RECT rcPillar = rc;
                rcPillar.left = rc.left + drawLeft;
                rcPillar.top = rc.bottom - drawTextHeight - (int)(drawHeight * rateOfItem);
                rcPillar.right = rc.left + drawLeft + m_PillarImageWidth;
                rcPillar.bottom = rc.bottom - drawTextHeight;
                if(!CRenderEngine::DrawImageString(hDC, GetManager(), rcPillar, GetPaintRect(), m_sPillarImage, NULL))
                {
                    m_sPillarImage.SetEmpty();
                }
            }

            if(m_bShowText)
            {
                // �滭����
                RECT rcText = rc;
                rcText.left = rc.left + drawLeft - pillarSpacing;
                rcText.top = rc.bottom - drawTextHeight;
                rcText.right = rc.left + drawLeft + m_PillarImageWidth + pillarSpacing;
                rcText.bottom = rc.bottom;

                int nLinks = 0;
                DWORD clrColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
                if(m_bShowHtml)
                {
                    CRenderEngine::DrawHtmlText(hDC, GetManager(), rcText, m_items[i].name, m_dwTextColor, \
                                                NULL, NULL, nLinks, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                else
                {
                    CRenderEngine::DrawText(hDC, GetManager(), rcText, m_items[i].name, m_dwTextColor, \
                                            m_iFont, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }


            // ������һ�����ӵ�ƫ��λ��
            drawLeft += pillarSpacing;
            drawLeft += m_PillarImageWidth;
        }
    }

    void CDuiChart::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }
        CDuiControl::DoPaint(hDC, rcPaint);

        if(CHARTVIEW_PIE == m_ViewStyle)
        {
            DoPaintPie(hDC, rcPaint);
        }
        else if(CHARTVIEW_HISTOGRAM == m_ViewStyle)
        {
            DoPaintHistogram(hDC, rcPaint);
        }
    }



}