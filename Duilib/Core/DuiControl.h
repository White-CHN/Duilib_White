#pragma once

namespace DuiLib
{

    typedef CDuiControl* (CALLBACK* FINDCONTROLPROC)(CDuiControl*, LPVOID);

    typedef struct tagTPercentInfo
    {
        double left;
        double top;
        double right;
        double bottom;
    } TPercentInfo;


    class DUILIB_API CDuiControl
    {
        DECLARE_DUICONTROL(CDuiControl)
    public:
        CDuiControl(void);
        virtual ~CDuiControl(void);
    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual UINT GetControlFlags() const;

        virtual CDuiControl* GetParent() const;

        RECT& GetPaintRect();
        // ˢ�����
        void Invalidate();
        BOOL IsUpdateNeeded() const;
        void NeedParentUpdate();
        void NeedUpdate();
        // ͼ�����
        void SetBkColor(DWORD dwBackColor);
        void SetBkColor2(DWORD dwBackColor);
        void SetBkColor3(DWORD dwBackColor);
        void SetForeColor(DWORD dwForeColor);
        CDuiString GetBkImage();
        void SetBkImage(LPCTSTR pStrImage);
        CDuiString GetForeImage() const;
        void SetForeImage(LPCTSTR pStrImage);

        void SetFocusBorderColor(DWORD dwBorderColor);
        void SetColorHSL(BOOL bColorHSL);
        void SetBorderRound(SIZE cxyRound);
        //�߿����
        void SetBorderSize(int nSize);
        void SetBorderSize(RECT rc);
        void SetBorderColor(DWORD dwBorderColor);
        void SetLeftBorderSize(int nSize);
        void SetTopBorderSize(int nSize);
        void SetRightBorderSize(int nSize);
        void SetBottomBorderSize(int nSize);
        void SetBorderStyle(int nStyle);
        //���ⴰ��
        CDuiString GetVirtualWnd() const;
        void SetVirtualWnd(LPCTSTR pstrValue);

        //name����
        virtual CDuiString GetName() const;
        virtual void SetName(LPCTSTR pstrName);
        //�ı����
        virtual CDuiString GetText() const;
        virtual void SetText(LPCTSTR pstrText);
        //��ק
        virtual BOOL IsDragEnabled() const;
        virtual void SetDragEnable(BOOL bDrag);

        virtual void SetDropEnable(BOOL bDrop);

        virtual void SetResourceText(BOOL bResource);

        // �����ʾ
        virtual CDuiString GetToolTip() const;
        virtual void SetToolTip(LPCTSTR pstrText);
        virtual int GetToolTipWidth(void);	// ����ToolTip��������
        // ���
        virtual WORD GetCursor();
        virtual void SetCursor(WORD wCursor);

        // ��ݼ�
        virtual TCHAR GetShortcut() const;
        virtual void SetShortcut(TCHAR ch);
        // �˵�
        virtual BOOL IsContextMenuUsed() const;
        virtual void SetContextMenuUsed(BOOL bMenuUsed);

        // �û�����
        virtual void SetUserData(LPCTSTR pstrText); // �������������û�ʹ��

        //λ�����
        virtual const RECT& GetPos() const;
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);

        virtual SIZE GetFixedXY() const;         // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
        virtual void SetFixedXY(SIZE szXY);      // ��floatΪtrueʱ��Ч

        virtual int GetFixedWidth() const;       // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
        virtual void SetFixedWidth(int cx);      // Ԥ��Ĳο�ֵ

        virtual int GetFixedHeight() const;      // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
        virtual void SetFixedHeight(int cy);     // Ԥ��Ĳο�ֵ

        virtual RECT GetPadding() const;
        virtual void SetPadding(RECT rcPadding); // ������߾࣬���ϲ㴰�ڻ���

        virtual int GetMinWidth() const;
        virtual void SetMinWidth(int cx);

        virtual int GetMaxWidth() const;
        virtual void SetMaxWidth(int cx);

        virtual int GetMinHeight() const;
        virtual void SetMinHeight(int cy);

        virtual int GetMaxHeight() const;
        virtual void SetMaxHeight(int cy);

        virtual TPercentInfo GetFloatPercent() const;

        virtual SIZE EstimateSize(SIZE szAvailable);

        // һЩ��Ҫ������
        virtual RECT GetRelativePos() const; // ���(���ؼ�)λ��
        virtual BOOL IsVisible() const;
        virtual void SetVisible(BOOL bVisible = TRUE);
        virtual void SetEnabled(BOOL bEnable = TRUE);
        virtual BOOL IsEnabled() const;
        virtual BOOL Activate();
        virtual BOOL IsMouseEnabled() const;
        virtual void SetMouseEnabled(BOOL bEnable = TRUE);
        virtual BOOL IsKeyboardEnabled() const;
        virtual void SetKeyboardEnabled(BOOL bEnable = TRUE);
        virtual void SetInternVisible(BOOL bVisible = TRUE); // �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���

        //����
        virtual BOOL IsFocused() const;
        virtual void SetFocused(BOOL bFocused);
        virtual void SetFocus();
        //float����
        virtual BOOL IsFloat() const;
        virtual void SetFloat(BOOL bFloat = TRUE);
        virtual void SetFloatPercent(TPercentInfo piFloatPercent);
        //floatalign����
        virtual UINT GetFloatAlign() const;
        virtual void SetFloatAlign(UINT uAlign);
        //��������
        CDuiControl* ApplyAttributeList(LPCTSTR pstrValue);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        //�������
        DWORD GetAdjustColor(DWORD dwColor);
        BOOL DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
        virtual void PaintBkColor(HDC hDC);
        virtual void PaintBkImage(HDC hDC);
        virtual void PaintStatusImage(HDC hDC);
        virtual void PaintForeColor(HDC hDC);
        virtual void PaintForeImage(HDC hDC);
        virtual void PaintText(HDC hDC);
        virtual void PaintBorder(HDC hDC);
        virtual void DoPaint(HDC hDC, const RECT& rcPaint);
        virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

        virtual void Init();
        virtual CDuiPaintManager* GetManager() const;
        virtual void SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit = TRUE);

        virtual void Event(TEventUI& event);
        virtual void DoEvent(TEventUI& event);

        virtual CDuiControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
    public:
        CEventSource OnSize;
        CEventSource OnEvent;
        CEventSource OnNotify;
    private:
        TCHAR m_chShortcut;			//shortcut����

        BOOL m_bUpdateNeeded;
        BOOL m_bVisible;
        BOOL m_bInternVisible;
        BOOL m_bSetPos;				// ��ֹSetPosѭ������
        BOOL m_bFloat;				//float����
        BOOL m_bColorHSL;			//colorhsl����
        BOOL m_bDragEnabled;		//drag����
        BOOL m_bDropEnabled;		//drop����
        BOOL m_bResourceText;		//resourcetext����
        BOOL m_bEnabled;			//enabled����
        BOOL m_bMouseEnabled;		//mouse����
        BOOL m_bKeyboardEnabled;	//keyboard����
        BOOL m_bFocused;			//�Ƿ񽹵�
        BOOL m_bMenuUsed;			//menu����

        WORD m_wCursor;				//cursor����

        int m_nBorderSize;			//bordersize����
        int m_nBorderStyle;			//borderstyle����
        int m_nTooltipWidth;		//����tip���������

        UINT m_uFloatAlign;			//FloatAlign����

        DWORD m_dwBackColor;		//bkcolor��bkcolor1����
        DWORD m_dwBackColor2;		//bkcolor2����
        DWORD m_dwBackColor3;		//bkcolor3����
        DWORD m_dwForeColor;		//forecolor����
        DWORD m_dwBorderColor;		//bordercolor����
        DWORD m_dwFocusBorderColor;	//focusbordercolor����

        HINSTANCE m_hInstance;

        CDuiControl* m_pParent;			//���ؼ�ָ��
        CDuiPaintManager* m_pManager;

        SIZE m_cXY;					//pos����
        SIZE m_cxyFixed;			//pos����
        SIZE m_cxyBorderRound;		//borderround����
        SIZE m_cxyMin;				//minwidth minhegiht����
        SIZE m_cxyMax;				//maxwidth maxhegiht����

        RECT m_rcItem;				//ʵ��λ��
        RECT m_rcPadding;			//padding����
        RECT m_rcBorderSize;		//bordersize����
        RECT m_rcPaint;

        TPercentInfo m_piFloatPercent;		//float ����ֵ

        CDuiString m_sBkImage;		//bkimage����
        CDuiString m_sForeImage;	//foreimage����
        CDuiString m_sName;			//name����
        CDuiString m_sText;			//text����
        CDuiString m_sToolTip;		//tooltip����
        CDuiString m_sUserData;		//userdata����
        CDuiString m_sVirtualWnd;	//virtualwnd����
    };



}
