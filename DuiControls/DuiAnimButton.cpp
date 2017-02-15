#include "StdAfx.h"
#include "DuiAnimButton.h"

IMPLEMENT_DUICONTROL(CDuiAnimButton)


CDuiAnimButton::CDuiAnimButton(void)
{
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='0,0,95,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='95,0,190,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='190,0,285,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='285,0,380,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='380,0,475,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='475,0,570,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='570,0,665,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='665,0,760,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='760,0,855,95'"));
    m_vtImages.push_back(_T("file='bkimage/animation.png' source='855,0,950,95'"));
}


CDuiAnimButton::~CDuiAnimButton(void)
{
}

CDuiString CDuiAnimButton::GetClass() const
{

    return DUI_CTR_ANIMBUTTON;
}

LPVOID CDuiAnimButton::GetInterface(LPCTSTR pstrName)
{
    if(_tcsicmp(pstrName, DUI_CTR_ANIMBUTTON) == 0)
    {
        return static_cast<CDuiAnimButton*>(this);
    }
    return CDuiButton::GetInterface(pstrName);
}

void CDuiAnimButton::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
    {
        return;
    }
    CDuiButton::DoPaint(hDC, rcPaint);
    DrawImage(hDC, (LPCTSTR)m_vtImages[0]);
}
