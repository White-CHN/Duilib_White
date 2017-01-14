#pragma once

#include "ControlsName.h"
class CDemoFrame : public CDuiDlgImplBase
{
    DUI_DECLARE_MESSAGE_MAP()
public:
    CDemoFrame(void);
    virtual ~CDemoFrame(void);
public:
    void InitWindow() OVERRIDE;
    CDuiString GetSkinFile() OVERRIDE;
    LPCTSTR GetWindowClassName() const OVERRIDE;

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;

    CDuiControl* CreateControl(LPCTSTR pstrClass) OVERRIDE;

    LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) OVERRIDE;

    void OnClick(TNotifyUI& msg);
    void OnWindowInit(TNotifyUI& msg);
    void OnValueChanged(TNotifyUI& msg);
    void OnValueChangedMove(TNotifyUI& msg);
    void OnIteamSelect(TNotifyUI& msg);
    void OnSelectChanged(TNotifyUI& msg);
    void OnColorChanged(TNotifyUI& msg);

    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
private:
    BOOL bEnglish;
    BOOL m_bIsSplit;
    CDuiAnimationTabLayout* m_pTabSwitch;
    CDuiSlider* m_pSlider;
    CDuiProgress* m_pProgress;
    CDuiMenuWnd* m_pMenu;
    CDuiCombo* m_pComboControlNames;
    CDuiList* m_pListControl;
    CDuiEdit* m_pEditXML;
    CDuiIcon m_Icon;
    CControlsName m_ControlsName;
};

