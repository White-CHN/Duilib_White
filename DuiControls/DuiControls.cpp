// DuiControls.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "DuiAnimButton.h"

CDuiControl* CreateControl(LPCTSTR pstrType)
{
    static BOOL bRegister = FALSE;
    if(bRegister == FALSE)
    {
        REGIST_DUICONTROL(CDuiAnimButton);
        bRegister = TRUE;
    }
    CDuiString strClass =  _T("CDui");
    strClass = strClass + pstrType ;
    return CDuiControlFactory::GetInstance()->CreateControl(strClass);
}