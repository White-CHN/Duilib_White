#pragma once

namespace DuiLib
{

//!检查并释放指针
#define DUI_FREE_POINT(x)				\
	{									\
		if (NULL != x)					\
		{								\
			delete x;					\
			x = NULL;					\
		}								\
	}

//!检查并释放数组
#define DUI_FREE_ARRAY(x)				\
	{									\
		if (NULL != x)					\
		{								\
			delete[] x;					\
			x = NULL;					\
		}								\
	}


#define MAX_FONT_ID		30000


#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))
#define PRINTBOOL(x) (TRUE == x ? _T("TRUE"):_T("FALSE"))

#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(x) ((sizeof(x))/(sizeof(x[0])))
#endif // SIZEOF_ARRAY

#ifndef ASSERT
    #define ASSERT(expr)  _ASSERTE(expr)
#endif

#define GET_CLASS_NAME(x) _T(#x)
    // 鼠标光标定义
#define DUI_ARROW           32512
#define DUI_IBEAM           32513
#define DUI_WAIT            32514
#define DUI_CROSS           32515
#define DUI_UPARROW         32516
#define DUI_SIZE            32640
#define DUI_ICON            32641
#define DUI_SIZENWSE        32642
#define DUI_SIZENESW        32643
#define DUI_SIZEWE          32644
#define DUI_SIZENS          32645
#define DUI_SIZEALL         32646
#define DUI_NO              32648
#define DUI_HAND            32649

    // Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_UPDATETEST    0x00000008
#define UIFIND_TOP_FIRST     0x00000010
#define UIFIND_ME_FIRST      0x80000000
    // Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

    // Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

    //定义所有消息类型
    //////////////////////////////////////////////////////////////////////////
#define DUI_MSGTYPE_MENU                   (_T("menu"))
#define DUI_MSGTYPE_LINK                   (_T("link"))

#define DUI_MSGTYPE_TIMER                  (_T("timer"))
#define DUI_MSGTYPE_CLICK                  (_T("click"))
#define DUI_MSGTYPE_DBCLICK                (_T("dbclick"))

#define DUI_MSGTYPE_RETURN                 (_T("return"))
#define DUI_MSGTYPE_SCROLL                 (_T("scroll"))

#define DUI_MSGTYPE_DROPDOWN               (_T("dropdown"))
#define DUI_MSGTYPE_SETFOCUS               (_T("setfocus"))

#define DUI_MSGTYPE_KILLFOCUS              (_T("killfocus"))
#define DUI_MSGTYPE_ITEMCLICK 		   	   (_T("itemclick"))
#define DUI_MSGTYPE_ITEMRCLICK 			   (_T("itemrclick"))
#define DUI_MSGTYPE_TABSELECT              (_T("tabselect"))

#define DUI_MSGTYPE_ITEMSELECT 		   	   (_T("itemselect"))
#define DUI_MSGTYPE_ITEMEXPAND             (_T("itemexpand"))
#define DUI_MSGTYPE_WINDOWINIT             (_T("windowinit"))
#define DUI_MSGTYPE_WINDOWSIZE             (_T("windowsize"))
#define DUI_MSGTYPE_BUTTONDOWN 		   	   (_T("buttondown"))
#define DUI_MSGTYPE_MOUSEENTER			   (_T("mouseenter"))
#define DUI_MSGTYPE_MOUSELEAVE			   (_T("mouseleave"))

#define DUI_MSGTYPE_TEXTCHANGED            (_T("textchanged"))
#define DUI_MSGTYPE_HEADERCLICK            (_T("headerclick"))
#define DUI_MSGTYPE_ITEMDBCLICK            (_T("itemdbclick"))
#define DUI_MSGTYPE_SHOWACTIVEX            (_T("showactivex"))

#define DUI_MSGTYPE_ITEMCOLLAPSE           (_T("itemcollapse"))
#define DUI_MSGTYPE_ITEMACTIVATE           (_T("itemactivate"))
#define DUI_MSGTYPE_VALUECHANGED           (_T("valuechanged"))
#define DUI_MSGTYPE_VALUECHANGED_MOVE      (_T("movevaluechanged"))

#define DUI_MSGTYPE_SELECTCHANGED 		   (_T("selectchanged"))
#define DUI_MSGTYPE_UNSELECTED	 		   (_T("unselected"))

#define DUI_MSGTYPE_TREEITEMSELECT 			(_T("treeitemselect"))
#define DUI_MSGTYPE_TREEITEMEXPAND 			(_T("treeitemexpand"))
#define DUI_MSGTYPE_TREEITEMACTIVATE 		(_T("treeitemactivate"))
#define DUI_MSGTYPE_TREEITEMDBCLICK 		(_T("treeitemdbclick"))
#define DUI_MSGTYPE_CHECKCLICK				(_T("checkclick"))
#define DUI_MSGTYPE_TEXTROLLEND 			(_T("textrollend"))
#define DUI_MSGTYPE_COLORCHANGED		    (_T("colorchanged"))

#define DUI_MSGTYPE_LISTITEMSELECT 		   	(_T("listitemselect"))
#define DUI_MSGTYPE_LISTITEMCHECKED 		(_T("listitemchecked"))
#define DUI_MSGTYPE_COMBOITEMSELECT 		(_T("comboitemselect"))
#define DUI_MSGTYPE_LISTHEADERCLICK			(_T("listheaderclick"))
#define DUI_MSGTYPE_LISTHEADITEMCHECKED		(_T("listheaditemchecked"))
#define DUI_MSGTYPE_LISTPAGECHANGED			(_T("listpagechanged"))

    //////////////////////////////////////////////////////////////////////////

//////////////BEGIN控件名称宏定义//////////////////////////////////////////////////
///

#define  DUI_CTR_CONTROL                         (_T("Control"))
#define  DUI_CTR_LABEL                           (_T("Label"))
#define  DUI_CTR_SCROLLBAR                       (_T("ScrollBar"))
#define  DUI_CTR_BUTTON                          (_T("Button"))
#define  DUI_CTR_OPTION                          (_T("Option"))
#define  DUI_CTR_RADIO                           (_T("Radio"))
#define  DUI_CTR_EDIT                            (_T("Edit"))
#define  DUI_CTR_PROGRESS                        (_T("Progress"))
#define  DUI_CTR_SLIDER                          (_T("Slider"))
#define  DUI_CTR_LISTELEMENT                     (_T("ListElement"))
#define  DUI_CTR_LISTLABELELEMENT                (_T("ListLabelElement"))

#define  DUI_CTR_CONTAINER                       (_T("Container"))
#define  DUI_CTR_VERTICALLAYOUT                  (_T("VerticalLayout"))
#define  DUI_CTR_HORIZONTALLAYOUT                (_T("HorizontalLayout"))
#define  DUI_CTR_TABLAYOUT                       (_T("TabLayout"))
#define  DUI_CTR_COMBO                           (_T("Combo"))
///
//////////////END控件名称宏定义//////////////////////////////////////////////////

//自定义消息
#define WM_USER_SET_DPI WM_USER + 200

//Duilib所用到的定时器ID
    enum
    {
        TIMERID_SCROLLBAR		 =	 10,
        TIMERID_CARET			=	0x1999
    };

    enum
    {
        // 内部保留消息
        DUIMSG_ICON = WM_USER + 1,
        // 程序自定义消息
        DUIMSG_USER = WM_USER + 100,
    };

    // 列表类型
    enum ListType
    {
        LT_LIST = 0,
        LT_COMBO,
        LT_TREE,
    };




#ifdef _DEBUG
#define DUI_TRACE(...)																	\
	{																					\
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);		\
		_tprintf(_T("\r\n[TRACE][%s][Line:%d]:<"), __FUNCTIONW__, __LINE__);			\
		DuiTrace(_T(__VA_ARGS__));														\
		_tprintf(_T(">"));																\
	}
#define DUI_ERROR(...)																	\
	{																					\
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);		\
		_tprintf(_T("\r\n[ERROR][%s][Line:%d]:<"), __FUNCTIONW__, __LINE__);			\
		DuiTrace(_T(__VA_ARGS__));														\
		_tprintf(_T(">"));																\
	}

#else
#define DUI_TRACE
#define DUI_ERROR
#endif	// _DEBUG

    void DUILIB_API DuiTrace(LPCTSTR pstrFormat, ...);
    LPCTSTR DUILIB_API DuiTraceMsg(UINT uMsg);


// 核心控件
    class CDuiControl;



}