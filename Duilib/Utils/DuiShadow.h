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

	purpose:	DuiLib��Ӱ�࣬��ԭWndShadow��Ļ����ϣ�������ͨ��PNGͼƬ������Ӱ�Ĺ��ܣ����ҰѴ�����DuiLib�ں�
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
        // bShowΪ��ʱ�Żᴴ����Ӱ
        void ShowShadow(BOOL bShow);
        BOOL IsShowShadow() const;

        void DisableShadow(BOOL bDisable);
        BOOL IsDisableShadow() const;

        // �㷨��Ӱ�ĺ���
        BOOL SetSize(int NewSize = 0);
        BOOL SetSharpness(unsigned int NewSharpness = 5);
        BOOL SetDarkness(unsigned int NewDarkness = 200);
        BOOL SetPosition(int NewXOffset = 5, int NewYOffset = 5);
        BOOL SetColor(COLORREF NewColor = 0);

        // ͼƬ��Ӱ�ĺ���
        BOOL SetImage(LPCTSTR szImage);
        BOOL SetShadowCorner(RECT rcCorner);	// �Ź���ʽ������Ӱ

        // ���Լ�����Ӱ��ʽ���Ƶ��������
        BOOL CopyShadow(CDuiShadow* pShadow);

        //	������Ӱ���壬��CDuiPaintManager�Զ�����,�����Լ�Ҫ����������Ӱ
        void Create(CDuiPaintManager* pPaintManager);
        // ������ı��С���ƶ������������ػ���Ӱʱ����
        void Update(HWND hParent);

        //	��ʼ����ע����Ӱ��
        static BOOL Initialize(HINSTANCE hInstance);
    protected:


        // �����Ѿ����ӵĴ������������������Ӱ��,������ParentProc()������ͨ������õ���Ӱ��
        static std::map<HWND, CDuiShadow*>& GetShadowMap();

        //	���໯������
        static LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



        // ͨ���㷨������Ӱ
        void MakeShadow(UINT32* pShadBits, HWND hParent, RECT* rcParent);

        // ����alphaԤ��ֵ
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

        CDuiPaintManager*	m_pManager;		// �������CDuiPaintManager��������ȡ�ز���Դ�͸�������
        HWND			 m_hWnd;			// ��Ӱ����ľ��
        LONG_PTR		 m_OriParentProc;	// ���໯������
        BYTE			 m_Status;
        BOOL			 m_bIsImageMode;	// �Ƿ�ΪͼƬ��Ӱģʽ
        BOOL			 m_bIsShowShadow;	// �Ƿ�Ҫ��ʾ��Ӱ
        BOOL			m_bIsDisableShadow;
        // �㷨��Ӱ��Ա����
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

        // ͼƬ��Ӱ��Ա����
        CDuiString	m_sShadowImage;
        RECT		m_rcShadowCorner;
    };

}