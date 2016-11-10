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
        virtual CDuiString GetName() const;
        virtual void SetName(LPCTSTR pstrName);
        virtual LPCTSTR GetClass() const;

        void Invalidate();
        BOOL IsUpdateNeeded() const;
        void NeedParentUpdate();
        void NeedUpdate();
        CDuiControl* ApplyAttributeList(LPCTSTR pstrValue);


        DWORD GetAdjustColor(DWORD dwColor);


        virtual CDuiPaintManager* GetManager() const;
        virtual void SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit = TRUE);
        virtual CDuiControl* GetParent() const;

        // 图形相关
        void SetBkColor(DWORD dwBackColor);
        void SetBkColor2(DWORD dwBackColor);
        void SetBkColor3(DWORD dwBackColor);
        void SetForeColor(DWORD dwForeColor);
        void SetBkImage(LPCTSTR pStrImage);
        void SetForeImage(LPCTSTR pStrImage);

        void SetFocusBorderColor(DWORD dwBorderColor);
        void SetColorHSL(BOOL bColorHSL);
        void SetBorderRound(SIZE cxyRound);
        //边框相关
        void SetBorderSize(int nSize);
        void SetBorderSize(RECT rc);
        void SetBorderColor(DWORD dwBorderColor);
        void SetLeftBorderSize(int nSize);
        void SetTopBorderSize(int nSize);
        void SetRightBorderSize(int nSize);
        void SetBottomBorderSize(int nSize);
        void SetBorderStyle(int nStyle);

        //文本相关
        virtual CDuiString GetText() const;
        virtual void SetText(LPCTSTR pstrText);

        virtual void SetDragEnable(BOOL bDrag);

        virtual void SetDropEnable(BOOL bDrop);

        virtual void SetResourceText(BOOL bResource);

        // 鼠标提示
        virtual void SetToolTip(LPCTSTR pstrText);

        // 光标
        virtual void SetCursor(WORD wCursor);

        // 快捷键
        virtual void SetShortcut(TCHAR ch);
        // 菜单
        virtual BOOL IsContextMenuUsed() const;
        virtual void SetContextMenuUsed(BOOL bMenuUsed);

        // 用户属性
        virtual void SetUserData(LPCTSTR pstrText); // 辅助函数，供用户使用

        //虚拟窗口参数
        CDuiString GetVirtualWnd() const;
        void SetVirtualWnd(LPCTSTR pstrValue);

        //位置相关
        virtual const RECT& GetPos() const;
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);

        virtual SIZE GetFixedXY() const;         // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
        virtual void SetFixedXY(SIZE szXY);      // 仅float为true时有效

        virtual int GetFixedWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
        virtual void SetFixedWidth(int cx);      // 预设的参考值

        virtual int GetFixedHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
        virtual void SetFixedHeight(int cy);     // 预设的参考值

        virtual RECT GetPadding() const;
        virtual void SetPadding(RECT rcPadding); // 设置外边距，由上层窗口绘制

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

        // 一些重要的属性
        virtual RECT GetRelativePos() const; // 相对(父控件)位置
        virtual BOOL IsVisible() const;
        virtual void SetVisible(BOOL bVisible = TRUE);
        virtual void SetEnabled(BOOL bEnable = TRUE);
        virtual BOOL IsEnabled() const;
        virtual BOOL IsMouseEnabled() const;
        virtual void SetMouseEnabled(BOOL bEnable = TRUE);
        virtual void SetKeyboardEnabled(BOOL bEnable = TRUE);
        virtual void SetInternVisible(BOOL bVisible = TRUE); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数

        virtual BOOL IsFocused() const;
        virtual void SetFocus();

        virtual BOOL IsFloat() const;
        virtual void SetFloat(BOOL bFloat = TRUE);
        virtual void SetFloatPercent(TPercentInfo piFloatPercent);



        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

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
        virtual void DoInit();

        virtual void Event(TEventUI& event);
        virtual void DoEvent(TEventUI& event);

        virtual UINT GetControlFlags() const;
        virtual CDuiControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

        virtual LPVOID GetInterface(LPCTSTR pstrName);
    public:
        CEventSource OnSize;
        CEventSource OnEvent;
        CEventSource OnNotify;
    protected:
        TCHAR m_chShortcut;			//shortcut属性
        BOOL m_bUpdateNeeded;
        BOOL m_bVisible;
        BOOL m_bInternVisible;
        BOOL m_bSetPos;				// 防止SetPos循环调用
        BOOL m_bFloat;				//float属性
        BOOL m_bColorHSL;			//colorhsl属性
        BOOL m_bDragEnabled;		//drag属性
        BOOL m_bDropEnabled;		//drop属性
        BOOL m_bResourceText;		//resourcetext属性
        BOOL m_bEnabled;			//enabled属性
        BOOL m_bMouseEnabled;		//mouse属性
        BOOL m_bKeyboardEnabled;	//keyboard属性
        BOOL m_bFocused;			//是否焦点
        BOOL m_bMenuUsed;			//menu属性
        WORD m_wCursor;				//cursor属性
        int m_nBorderSize;			//bordersize属性
        int m_nBorderStyle;			//borderstyle属性
        DWORD m_dwBackColor;		//bkcolor与bkcolor1属性
        DWORD m_dwBackColor2;		//bkcolor2属性
        DWORD m_dwBackColor3;		//bkcolor3属性
        DWORD m_dwForeColor;		//forecolor属性
        DWORD m_dwBorderColor;		//bordercolor属性
        DWORD m_dwFocusBorderColor;	//focusbordercolor属性

        CDuiString m_sBkImage;		//bkimage属性
        CDuiString m_sForeImage;	//foreimage属性
        CDuiString m_sName;			//name属性
        CDuiString m_sText;			//text属性
        CDuiString m_sToolTip;		//tooltip属性
        CDuiString m_sUserData;		//userdata属性
        CDuiString m_sVirtualWnd;	//virtualwnd属性

        RECT m_rcItem;
        RECT m_rcPadding;			// padding属性
        RECT m_rcBorderSize;		// bordersize属性
        RECT m_rcPaint;

        SIZE m_cXY;					// pos属性
        SIZE m_cxyFixed;			// pos属性
        SIZE m_cxyBorderRound;		// borderround属性
        SIZE m_cxyMin;				// minwidth minhegiht属性
        SIZE m_cxyMax;				// maxwidth maxhegiht属性

        TPercentInfo m_piFloatPercent;		//float 的数值

        CDuiControl* m_pParent;			//父控件指针
        CDuiPaintManager* m_pManager;

        HINSTANCE m_hInstance;
    };



}
