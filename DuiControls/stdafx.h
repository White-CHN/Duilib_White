// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#ifdef _MSC_VER
    #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
#endif // _MSC_VER

#include "..\DuiLib\DuiLib.h"

using namespace DuiLib;



#ifdef _WIN64
    #ifdef _DEBUG
        #ifdef _UNICODE
            #pragma comment(lib, "..\\Lib\\DuiLibx64_d.lib")
        #else
            #pragma comment(lib, "..\\Lib\\DuiLibx64A_d.lib")
        #endif
    #else
        #ifdef _UNICODE
            #pragma comment(lib, "..\\Lib\\DuiLibx64.lib")
        #else
            #pragma comment(lib, "..\\Lib\\DuiLibx64A.lib")
        #endif
    #endif
#else
    #ifdef _DEBUG
        #ifdef _UNICODE
            #pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
        #else
            #pragma comment(lib, "..\\Lib\\DuiLibA_d.lib")
        #endif
    #else
        #ifdef _UNICODE
            #pragma comment(lib, "..\\Lib\\DuiLib.lib")
        #else
            #pragma comment(lib, "..\\Lib\\DuiLibA.lib")
        #endif
    #endif
#endif


#include "DuiControls.h"
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
