#pragma once
#include "BoxDlg.h"
#include "ToolsDlg.h"
class CBaoFengFrame
    : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CBaoFengFrame(void);
    virtual ~CBaoFengFrame(void);
public:
    void InitWindow() OVERRIDE;
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    void OnClick(CDuiNotify& msg);
    void OnSelectChanged(CDuiNotify& msg);
private:
    void OnMoveBoxDlg();
    void OnMoveToolsDlg();
private:
    CDuiOption* m_pOptionPlayList;
    CDuiVerticalLayout* m_pViewRight;
    CDuiAnimationTabLayout* m_pTabSwitch;
    CBoxDlg* m_pBoxDlg;
    CDuiOption* m_pOptionBox;
    CDuiMenuWnd* m_pMainMenu;
    CToolsDlg* m_pToolsDlg;
};

