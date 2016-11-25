#include "StdAfx.h"
#include "DuiProgress.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiProgress)

    CDuiProgress::CDuiProgress(void)
        : m_bShowText(FALSE)
        , m_bHorizontal(TRUE)
        , m_bStretchForeImage(TRUE)
        , m_nMin(0)
        , m_nMax(100)
        , m_nValue(0)
    {

    }


    CDuiProgress::~CDuiProgress(void)
    {
    }

    LPCTSTR CDuiProgress::GetClass() const
    {
        return DUI_CTR_PROGRESS;
    }

    LPVOID CDuiProgress::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_PROGRESS) == 0)
        {
            return static_cast<CDuiProgress*>(this);
        }
        return __super::GetInterface(pstrName);
    }

    BOOL CDuiProgress::IsShowText()
    {
        return m_bShowText;
    }

    void CDuiProgress::SetShowText(BOOL bShowText /*= TRUE*/)
    {
        if(m_bShowText == bShowText)
        {
            return;
        }
        m_bShowText = bShowText;
        if(!m_bShowText)
        {
            SetText(GetText());
        }
        else
        {
            UpdateText();
        }
    }

    BOOL CDuiProgress::IsHorizontal() const
    {
        return m_bHorizontal;
    }

    void CDuiProgress::SetHorizontal(BOOL bHorizontal /*= TRUE*/)
    {
        if(m_bHorizontal == bHorizontal)
        {
            return;
        }

        m_bHorizontal = bHorizontal;
        Invalidate();
    }

    int CDuiProgress::GetMinValue() const
    {
        return m_nMin;
    }

    void CDuiProgress::SetMinValue(int nMin)
    {
        if(m_nMin == nMin)
        {
            return;
        }
        m_nMin = nMin;
        Invalidate();
    }

    int CDuiProgress::GetMaxValue() const
    {
        return m_nMax;
    }

    void CDuiProgress::SetMaxValue(int nMax)
    {
        if(m_nMax == nMax)
        {
            return;
        }
        m_nMax = nMax;
        Invalidate();
    }

    int CDuiProgress::GetValue() const
    {
        return m_nValue;
    }

    void CDuiProgress::UpdateText()
    {
        if(m_bShowText)
        {
            CDuiString sText;
            sText.Format(_T("%.0f%%"), (m_nValue - m_nMin) * 100.0f / (m_nMax - m_nMin));
            SetText(sText);
        }
    }

    void CDuiProgress::SetValue(int nValue)
    {
        if(nValue == m_nValue || nValue < m_nMin || nValue > m_nMax)
        {
            return;
        }
        m_nValue = nValue;
        Invalidate();
        UpdateText();
    }

    void CDuiProgress::SetStretchForeImage(BOOL bStretchForeImage /*= TRUE*/)
    {
        if(m_bStretchForeImage == bStretchForeImage)
        {
            return;
        }
        m_bStretchForeImage = bStretchForeImage;
        Invalidate();
    }

    void CDuiProgress::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("hor")) == 0)
        {
            SetHorizontal(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("min")) == 0)
        {
            SetMinValue(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("max")) == 0)
        {
            SetMaxValue(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("value")) == 0)
        {
            SetValue(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("isstretchfore")) == 0)
        {
            SetStretchForeImage(_tcsicmp(pstrValue, _T("true")) == 0 ? TRUE : FALSE);
        }
        else
        {
            __super::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiProgress::PaintForeColor(HDC hDC)
    {
        if(GetForeColor() == 0)
        {
            return;
        }

        if(m_nMax <= m_nMin)
        {
            m_nMax = m_nMin + 1;
        }
        if(m_nValue > m_nMax)
        {
            m_nValue = m_nMax;
        }
        if(m_nValue < m_nMin)
        {
            m_nValue = m_nMin;
        }

        RECT rc = GetPos();
        if(m_bHorizontal)
        {
            rc.right = GetPos().left + (m_nValue - m_nMin) * (GetPos().right - GetPos().left) / (m_nMax - m_nMin);
        }
        else
        {
            rc.bottom = GetPos().top + (GetPos().bottom - GetPos().top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);

        }

        CRenderEngine::DrawColor(hDC, rc, GetAdjustColor(GetForeColor()));
    }

    void CDuiProgress::PaintForeImage(HDC hDC)
    {
        if(m_nMax <= m_nMin)
        {
            m_nMax = m_nMin + 1;
        }
        if(m_nValue > m_nMax)
        {
            m_nValue = m_nMax;
        }
        if(m_nValue < m_nMin)
        {
            m_nValue = m_nMin;
        }

        RECT rc = {0};
        if(m_bHorizontal)
        {
            rc.right = (m_nValue - m_nMin) * (GetPos().right - GetPos().left) / (m_nMax - m_nMin);
            rc.bottom = GetPos().bottom - GetPos().top;
        }
        else
        {
            rc.top = (GetPos().bottom - GetPos().top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
            rc.right = GetPos().right - GetPos().left;
            rc.bottom = GetPos().bottom - GetPos().top;
        }

        if(!GetForeImage().IsEmpty())
        {
            m_sForeImageModify.SetEmpty();
            if(m_bStretchForeImage)
            {
                m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom);
            }
            else
            {
                m_sForeImageModify.SmallFormat(_T("dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), rc.left, rc.top, rc.right, rc.bottom, rc.left, rc.top, rc.right, rc.bottom);
            }

            if(!DrawImage(hDC, GetForeImage().GetData(), (LPCTSTR)m_sForeImageModify)) {}
            else
            {
                return;
            }
        }
    }

}