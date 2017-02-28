// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "..\..\DuiLib\DuiLib.h"

using namespace DuiLib;

#ifdef _WIN64
#ifdef _DEBUG
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64_d.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64A_d.lib")
    #endif
#else
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64A.lib")
    #endif
#endif
#else
#ifdef _DEBUG
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLib_d.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibA_d.lib")
    #endif
#else
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLib.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibA.lib")
    #endif
#endif
#endif