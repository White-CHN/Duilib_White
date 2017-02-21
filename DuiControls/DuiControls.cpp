// DuiControls.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


class CWkeInit
{
public:
    CWkeInit()
    {
        CDuiWkeWebkit::InitializeWebkit();
    }
    ~CWkeInit()
    {
        CDuiWkeWebkit::UninitializeWebkit();
    }
};

CWkeInit g_WkeInit;

CDuiControl* CreateControl(LPCTSTR pstrType)
{
    static BOOL bRegister = FALSE;
    if(bRegister == FALSE)
    {
        REGIST_DUICONTROL(CDuiAnimButton);
        REGIST_DUICONTROL(CDuiWkeWebkit);
        bRegister = TRUE;
    }
    CDuiString strClass =  _T("CDui");
    strClass = strClass + pstrType ;
    return CDuiControlFactory::GetInstance()->CreateControl(strClass);
}