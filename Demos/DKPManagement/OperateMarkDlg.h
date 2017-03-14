#pragma once
class COperateMarkDlg
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    COperateMarkDlg(CDuiString* pMark, CDuiString* pRemarks);
    virtual ~COperateMarkDlg(void);
public:
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    void InitWindow() OVERRIDE;

    void OnClick(CDuiNotify& msg);
private:
    CDuiString* m_pMark;
    CDuiString* m_pRemarks;
    CDuiEdit* m_pMarkEdit;
    CDuiEdit* m_pRemarksEdit;
public:
    int m_nType;
};

