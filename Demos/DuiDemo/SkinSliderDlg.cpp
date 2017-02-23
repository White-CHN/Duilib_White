#include "StdAfx.h"
#include "SkinSliderDlg.h"


DUI_BEGIN_MESSAGE_MAP(CSkinSliderDlg, CDuiDlgImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED_MOVE, OnValueChangedMove)
DUI_END_MESSAGE_MAP()


CSkinSliderDlg::CSkinSliderDlg(CDuiPaintManager* pPaintManager, CDuiButton* pBtn)
    : m_pPaintManager(pPaintManager)
    , m_pBtn(pBtn)
    , m_pSlider(NULL)
{
}


CSkinSliderDlg::~CSkinSliderDlg(void)
{
}


void CSkinSliderDlg::InitWindow()
{
    m_pSlider =  static_cast<CDuiSlider*>(GetPaintManager()->FindControl(_T("slider")));
    if(m_pSlider)
    {
        m_pSlider->SetValue(255 - m_pPaintManager->GetOpacity());
    }
    CDuiString str;
    str.Format(_T("%d %%"), (255 - m_pPaintManager->GetOpacity()) * 100 / 255);
    m_pBtn->SetText(str);
}

CDuiString CSkinSliderDlg::GetSkinFile()
{
    return _T("SkinSliderDlg.xml");
}

LPCTSTR CSkinSliderDlg::GetWindowClassName() const
{
    return GET_CLASS_NAME(CSkinSliderDlg);
}

LRESULT CSkinSliderDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    Close();
    return 0;
}

LRESULT CSkinSliderDlg::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    Close();
    return 0;
}

void CSkinSliderDlg::OnValueChanged(CDuiNotify& msg)
{
    if(msg.pSender == m_pSlider)
    {
        m_pPaintManager->SetOpacity(255 - m_pSlider->GetValue());
        CDuiString str;
        str.Format(_T("%d %%"), (255 - m_pPaintManager->GetOpacity()) * 100 / 255);
        m_pBtn->SetText(str);
    }
}

void CSkinSliderDlg::OnValueChangedMove(CDuiNotify& msg)
{
    if(msg.pSender == m_pSlider)
    {
        m_pPaintManager->SetOpacity(255 - m_pSlider->GetValue());
        CDuiString str;
        str.Format(_T("%d %%"), (255 - m_pPaintManager->GetOpacity()) * 100 / 255);
        m_pBtn->SetText(str);
    }
}
