#pragma once

#ifdef DUILIB_STATIC
    #define DUILIB_API
#else
    #if defined(DUILIB_EXPORTS)
        #if defined(_MSC_VER)
            #define DUILIB_API __declspec(dllexport)
        #else
            #define DUILIB_API
        #endif
    #else
        #if defined(_MSC_VER)
            #define DUILIB_API __declspec(dllimport)
        #else
            #define DUILIB_API
        #endif
    #endif
#endif

#if defined _M_IX86
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
    #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


#include <windows.h>
#include <windowsx.h>
#include <winsock.h>
#include <comdef.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <locale.h>
#include <tchar.h>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;
using namespace Gdiplus;

#include "Utils/DuiString.h"
#include "Utils/DuiUtils.h"
#include "Utils/DuiDPI.h"
#include "Utils/DuiDelegate.h"
#include "Utils/DuiShadow.h"
#include "Utils/DuiIcon.h"
#include "Utils/DragDropImpl.h"
#include "Utils/unzip.h"

#include "Core/DuiMarkupNode.h"
#include "Core/DuiMarkup.h"
#include "Core/DuiDefine.h"
#include "Core/DuiControlFactory.h"
#include "Core/DuiPaintManager.h"
#include "Core/DuiControl.h"
#include "Core/DuiContainer.h"
#include "Core/DuiResourceManager.h"
#include "Core/DuiNotifyPump.h"
#include "Core/DuiWnd.h"
#include "Core/DuiDlgBuilder.h"
#include "Core/DuiDlgImplBase.h"
#include "Core/DuiRender.h"
#include "Core/DuiAnimation.h"


#include "Layout/DuiVerticalLayout.h"
#include "Layout/DuiHorizontalLayout.h"
#include "Layout/DuiTabLayout.h"
#include "Layout/DuiAnimationTabLayout.h"

#include "Control/DuiScrollBar.h"
#include "Control/DuiLabel.h"
#include "Control/DuiButton.h"
#include "Control/DuiOption.h"
#include "Control/DuiRadio.h"
#include "Control/DuiEdit.h"
#include "Control/DuiProgress.h"
#include "Control/DuiSlider.h"
#include "Control/DuiCombo.h"
#include "Control/DuiGroupBox.h"
#include "Control/DuiDateTime.h"
#include "Control/DuiIPAddress.h"
#include "Control/DuiRing.h"
#include "Control/DuiGif.h"
#include "Control/DuiColorPalette.h"
#include "Control/DuiChart.h"
#include "Control/DuiList.h"
#include "Control/DuiMenu.h"
#include "Control/DuiFadeButton.h"
#include "Control/DuiCheckBox.h"

#pragma comment( lib, "GdiPlus.lib" )
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "ws2_32.lib" )