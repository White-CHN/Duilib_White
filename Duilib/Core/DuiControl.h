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

        CDuiControl* GetParent() const;
        CDuiPaintManager* GetManager() const;

        virtual void Init();
        virtual void SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit = TRUE);

        RECT GetPaintRect() const;
        // ˢ�����
        void Invalidate();
        BOOL IsUpdateNeeded() const;
        void NeedParentUpdate();
        void NeedUpdate();
        // ͼ�����
        DWORD GetBkColor();
        void SetBkColor(DWORD dwBackColor);

        DWORD GetBkColor2();
        void SetBkColor2(DWORD dwBackColor);

        DWORD GetBkColor3();
        void SetBkColor3(DWORD dwBackColor);

        DWORD GetForeColor() const;
        void SetForeColor(DWORD dwForeColor);

        CDuiString GetBkImage() const;
        virtual void SetBkImage(LPCTSTR pStrImage);

        CDuiString GetForeImage() const;
        void SetForeImage(LPCTSTR pStrImage);

        DWORD GetFocusBorderColor();
        void SetFocusBorderColor(DWORD dwBorderColor);

        void SetColorHSL(BOOL bColorHSL);

        CDuiSize GetBorderRound();
        void SetBorderRound(CDuiSize cxyRound);
        //�߿����
        int GetBorderSize();
        void SetBorderSize(int nSize);

        CDuiRect GetBorderSizeV2();
        void SetBorderSize(CDuiRect rc);

        DWORD GetBorderColor();
        void SetBorderColor(DWORD dwBorderColor);

        int GetLeftBorderSize() const;
        void SetLeftBorderSize(int nSize);

        int GetTopBorderSize() const;
        void SetTopBorderSize(int nSize);

        int GetRightBorderSize() const;
        void SetRightBorderSize(int nSize);

        int GetBottomBorderSize() const;
        void SetBottomBorderSize(int nSize);

        int GetBorderStyle() const;
        void SetBorderStyle(int nStyle);
        //���ⴰ��
        CDuiString GetVirtualWnd() const;
        void SetVirtualWnd(LPCTSTR pstrValue);

        //name����
        CDuiString GetName() const;
        void SetName(LPCTSTR pstrName);
        //�ı����
        virtual CDuiString GetText() const;
        virtual void SetText(LPCTSTR pstrText);

        BOOL IsResourceText() const;
        virtual void SetResourceText(BOOL bResource);
        //��ק
        BOOL IsDragEnabled() const;
        virtual void SetDragEnable(BOOL bDrag);

        virtual void SetDropEnable(BOOL bDrop);

        // �����ʾ
        virtual CDuiString GetToolTip() const;
        virtual void SetToolTip(LPCTSTR pstrText);
        virtual int GetToolTipWidth() const;	// ����ToolTip��������
        virtual void SetToolTipWidth(int nWidth);
        // ���
        virtual WORD GetCursor() const;
        virtual void SetCursor(WORD wCursor);

        // ��ݼ�
        virtual TCHAR GetShortcut() const;
        virtual void SetShortcut(TCHAR ch);
        // �˵�
        virtual BOOL IsContextMenuUsed() const;
        virtual void SetContextMenuUsed(BOOL bMenuUsed);

        // �û�����
        virtual const CDuiString& GetUserData() const; // �������������û�ʹ��
        virtual void SetUserData(LPCTSTR pstrText); // �������������û�ʹ��

        //λ�����
        virtual CDuiRect GetRelativePos() const; // ���(���ؼ�)λ��
        virtual RECT GetClientPos() const; // �ͻ����򣨳�ȥscrollbar��inset��
        virtual const RECT& GetPos() const;
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);

        virtual SIZE GetFixedXY() const;         // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
        virtual void SetFixedXY(CDuiSize szXY);      // ��floatΪtrueʱ��Ч

        virtual int GetFixedWidth() const;       // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
        virtual void SetFixedWidth(int cx);      // Ԥ��Ĳο�ֵ

        virtual int GetFixedHeight() const;      // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
        virtual void SetFixedHeight(int cy);     // Ԥ��Ĳο�ֵ

        virtual CDuiRect GetPadding() const;
        virtual void SetPadding(CDuiRect rcPadding); // ������߾࣬���ϲ㴰�ڻ���

        virtual int GetMinWidth() const;
        virtual void SetMinWidth(int cx);

        virtual int GetMaxWidth() const;
        virtual void SetMaxWidth(int cx);

        virtual int GetMinHeight() const;
        virtual void SetMinHeight(int cy);

        virtual int GetMaxHeight() const;
        virtual void SetMaxHeight(int cy);

        virtual TPercentInfo GetFloatPercent() const;
        virtual void SetFloatPercent(TPercentInfo piFloatPercent);

        virtual UINT GetFloatAlign() const;
        virtual void SetFloatAlign(UINT uAlign);

        // һЩ��Ҫ������
        virtual BOOL IsVisible() const;
        virtual void SetVisible(BOOL bVisible = TRUE);
        virtual void SetInternVisible(BOOL bVisible = TRUE); // �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���

        virtual BOOL IsEnabled() const;
        virtual void SetEnabled(BOOL bEnable = TRUE);

        virtual BOOL Activate();

        virtual BOOL IsMouseEnabled() const;
        virtual void SetMouseEnabled(BOOL bEnable = TRUE);

        virtual BOOL IsKeyboardEnabled() const;
        virtual void SetKeyboardEnabled(BOOL bEnable = TRUE);
        //����
        virtual BOOL IsFocused() const;
        virtual void SetFocused(BOOL bFocused);
        virtual void SetFocus();
        //float����
        virtual BOOL IsFloat() const;
        virtual void SetFloat(BOOL bFloat = TRUE);

        //��������
        CDuiControl* ApplyAttributeList(LPCTSTR pstrValue);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        //�������
        DWORD GetAdjustColor(DWORD dwColor) const;
        BOOL DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
        virtual SIZE EstimateSize(SIZE szAvailable);
        virtual void PaintBkColor(HDC hDC);
        virtual void PaintBkImage(HDC hDC);
        virtual void PaintStatusImage(HDC hDC);
        virtual void PaintForeColor(HDC hDC);
        virtual void PaintForeImage(HDC hDC);
        virtual void PaintText(HDC hDC);
        virtual void PaintBorder(HDC hDC);
        virtual void DoPaint(HDC hDC, const RECT& rcPaint);
        virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

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

        CDuiSize m_cXY;					//pos���Ͻ�xy
        CDuiSize m_cxyFixed;			//pos��ȸ߶�
        CDuiSize m_cxyBorderRound;		//borderround����
        CDuiSize m_cxyMin;				//minwidth minhegiht����
        CDuiSize m_cxyMax;				//maxwidth maxhegiht����

        CDuiRect m_rcItem;				//ʵ��λ��
        CDuiRect m_rcPadding;			//padding����
        CDuiRect m_rcBorderSize;		//bordersize����
        CDuiRect m_rcPaint;				//ʵ�ʻ�������

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
