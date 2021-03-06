// WndShadow.h : header file
//
// Version 0.1
//
// Copyright (c) 2006 Perry Zhu, All Rights Reserved.
//
// mailto:perry@live.com
//
//
// This source file may be redistributed unmodified by any means PROVIDING
// it is NOT sold for profit without the authors expressed written
// consent, and providing that this notice and the author's name and all
// copyright notices remain intact. This software is by no means to be
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit.
//
// If the source code is used in any commercial applications then a statement
// along the lines of:
//
// "Portions Copyright (c) 2006 Perry Zhu" must be included in the "Startup
// Banner", "About Box" or "Printed Documentation". This software is provided
// "as is" without express or implied warranty. Use it at your own risk! The
// author accepts no liability for any damage/loss of business that this
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
//****************************************************************************

/********************************************************************
	created:	2015/01/09
	filename: 	DuiShadow.h
	author:		Redrain

	purpose:	DuiLib阴影类，在原WndShadow类的基础上，增加了通过PNG图片设置阴影的功能，并且把代码与DuiLib融合
*********************************************************************/

#pragma once

namespace DuiLib
{

    class CDuiPaintManager;

    class DUILIB_API CDuiShadow
    {
    public:

        CDuiShadow(void);
        virtual ~CDuiShadow(void);

    public:
        // bShow为真时才会创建阴影
        void ShowShadow(BOOL bShow);
        BOOL IsShowShadow() const;

        void DisableShadow(BOOL bDisable);
        BOOL IsDisableShadow() const;

        // 算法阴影的函数
        BOOL SetSize(int NewSize = 0);
        BOOL SetSharpness(unsigned int NewSharpness = 5);
        BOOL SetDarkness(unsigned int NewDarkness = 200);
        BOOL SetPosition(int NewXOffset = 5, int NewYOffset = 5);
        BOOL SetColor(COLORREF NewColor = 0);

        // 图片阴影的函数
        BOOL SetImage(LPCTSTR szImage);
        BOOL SetShadowCorner(RECT rcCorner);	// 九宫格方式描述阴影

        // 把自己的阴影样式复制到传入参数
        BOOL CopyShadow(CDuiShadow* pShadow);

        //	创建阴影窗体，由CDuiPaintManager自动调用,除非自己要单独创建阴影
        void Create(CDuiPaintManager* pPaintManager);
        // 父窗体改变大小，移动，或者主动重绘阴影时调用
        void Update(HWND hParent);

        //	初始化并注册阴影类
        static BOOL Initialize(HINSTANCE hInstance);
    protected:


        // 保存已经附加的窗体句柄和与其关联的阴影类,方便在ParentProc()函数中通过句柄得到阴影类
        static std::map<HWND, CDuiShadow*>& GetShadowMap();

        //	子类化父窗体
        static LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



        // 通过算法计算阴影
        void MakeShadow(UINT32* pShadBits, HWND hParent, RECT* rcParent);

        // 计算alpha预乘值
        inline DWORD PreMultiply(COLORREF cl, unsigned char nAlpha)
        {
            return (GetRValue(cl) * (DWORD)nAlpha / 255) |
                   (GetGValue(cl) * (DWORD)nAlpha / 255) << 8 |
                   (GetBValue(cl) * (DWORD)nAlpha / 255) << 16 ;
        }

    protected:
        enum ShadowStatus
        {
            SS_ENABLED = 1,				// Shadow is enabled, if not, the following one is always FALSE
            SS_VISABLE = 1 << 1,		// Shadow window is visible
            SS_PARENTVISIBLE = 1 << 2	// Parent window is visible, if not, the above one is always FALSE
        };


        static BOOL s_bHasInit;

        CDuiPaintManager*	m_pManager;		// 父窗体的CDuiPaintManager，用来获取素材资源和父窗体句柄
        HWND			 m_hWnd;			// 阴影窗体的句柄
        LONG_PTR		 m_OriParentProc;	// 子类化父窗体
        BYTE			 m_Status;
        BOOL			 m_bIsImageMode;	// 是否为图片阴影模式
        BOOL			 m_bIsShowShadow;	// 是否要显示阴影
        BOOL			m_bIsDisableShadow;
        // 算法阴影成员变量
        unsigned char m_nDarkness;	// Darkness, transparency of blurred area
        unsigned char m_nSharpness;	// Sharpness, width of blurred border of shadow window
        signed char m_nSize;	// Shadow window size, relative to parent window size

        // The X and Y offsets of shadow window,
        // relative to the parent window, at center of both windows (not top-left corner), signed
        signed char m_nxOffset;
        signed char m_nyOffset;

        // Restore last parent window size, used to determine the update strategy when parent window is resized
        LPARAM m_WndSize;

        // Set this to TRUE if the shadow should not be update until next WM_PAINT is received
        BOOL m_bUpdate;

        COLORREF m_Color;	// Color of shadow

        // 图片阴影成员变量
        CDuiString	m_sShadowImage;
        RECT		m_rcShadowCorner;
    };

}