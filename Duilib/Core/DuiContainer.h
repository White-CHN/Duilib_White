#pragma once

namespace DuiLib
{
    class CDuiScrollBar;

    class DUILIB_API IContainer
    {
    public:
        virtual CStdPtrArray* GetItems() = 0;
        virtual CDuiControl* GetItemAt(int iIndex) const = 0;
        virtual int GetItemIndex(CDuiControl* pControl) const  = 0;
        virtual BOOL SetItemIndex(CDuiControl* pControl, int iIndex)  = 0;
        virtual int GetCount() const = 0;
        virtual BOOL Add(CDuiControl* pControl) = 0;
        virtual BOOL AddAt(CDuiControl* pControl, int iIndex)  = 0;
        virtual BOOL Remove(CDuiControl* pControl) = 0;
        virtual BOOL RemoveAt(int iIndex)  = 0;
        virtual void RemoveAll() = 0;
    };

    class DUILIB_API CDuiContainer
        : public CDuiControl
        , public IContainer
    {
        DECLARE_DUICONTROL(CDuiContainer)
    public:
        CDuiContainer(void);
        virtual ~CDuiContainer(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        virtual CDuiRect GetInset() const;
        virtual void SetInset(CDuiRect rcInset); // 设置内边距，相当于设置客户区

        virtual int GetChildPadding() const;
        virtual void SetChildPadding(int iPadding);

        virtual UINT GetChildAlign() const;
        virtual void SetChildAlign(UINT iAlign);

        virtual UINT GetChildVAlign() const;
        virtual void SetChildVAlign(UINT iVAlign);

        virtual BOOL IsAutoDestroy() const;
        virtual void SetAutoDestroy(BOOL bAuto);

        virtual BOOL IsDelayedDestroy() const;
        virtual void SetDelayedDestroy(BOOL bDelayed);

        virtual BOOL IsMouseChildEnabled() const;
        virtual void SetMouseChildEnabled(BOOL bEnable = TRUE);

        virtual SIZE GetScrollPos() const;
        virtual SIZE GetScrollRange() const;
        virtual int GetScrollStepSize() const;
        virtual void SetScrollStepSize(int nSize);
        virtual void SetScrollPos(SIZE szPos, BOOL bMsg = TRUE);
        virtual void LineUp();
        virtual void LineDown();
        virtual void LineLeft();
        virtual void LineRight();
        virtual void PageUp();
        virtual void PageDown();
        virtual void PageLeft();
        virtual void PageRight();
        virtual void HomeUp();
        virtual void EndDown();
        virtual void HomeLeft();
        virtual void EndRight();

        virtual CDuiScrollBar* GetVerticalScrollBar() const;
        virtual CDuiScrollBar* GetHorizontalScrollBar() const;

        CStdPtrArray* GetItems() OVERRIDE;
        CDuiControl* GetItemAt(int iIndex) const OVERRIDE;
        int GetItemIndex(CDuiControl* pControl) const OVERRIDE;
        BOOL SetItemIndex(CDuiControl* pControl, int iIndex) OVERRIDE;
        int GetCount() const OVERRIDE;
        BOOL Add(CDuiControl* pControl) OVERRIDE;
        BOOL AddAt(CDuiControl* pControl, int iIndex) OVERRIDE;
        BOOL Remove(CDuiControl* pControl) OVERRIDE;
        BOOL RemoveAt(int iIndex) OVERRIDE;
        void RemoveAll() OVERRIDE;

        virtual int FindSelectable(int iIndex, BOOL bForward = TRUE) const ;

        RECT GetClientPos() const OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void Move(SIZE szOffset, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

        void DoEvent(TEventUI& event) OVERRIDE;
        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetEnabled(BOOL bEnabled) OVERRIDE;
        void SetMouseEnabled(BOOL bEnable = TRUE) OVERRIDE;

        virtual void EnableScrollBar(BOOL bEnableVertical = TRUE, BOOL bEnableHorizontal = FALSE) ;
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit = TRUE) OVERRIDE;

        CDuiString GetSubControlText(LPCTSTR pstrSubControlName);
        BOOL SetSubControlText(LPCTSTR pstrSubControlName, LPCTSTR pstrText);

        int GetSubControlFixedHeight(LPCTSTR pstrSubControlName) ;
        BOOL SetSubControlFixedHeight(LPCTSTR pstrSubControlName, int cy);

        int GetSubControlFixedWdith(LPCTSTR pstrSubControlName) ;
        BOOL SetSubControlFixedWdith(LPCTSTR pstrSubControlName, int cx);

        CDuiString GetSubControlUserData(LPCTSTR pstrSubControlName) ;
        BOOL SetSubControlUserData(LPCTSTR pstrSubControlName, LPCTSTR pstrText);

        CDuiControl* FindSubControl(LPCTSTR pstrSubControlName) ;
        CDuiControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags) OVERRIDE;
    protected:
        virtual void SetFloatPos(int iIndex);
        virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
    private:
        BOOL m_bAutoDestroy;
        BOOL m_bDelayedDestroy;
        BOOL m_bMouseChildEnabled;

        int m_iChildPadding;
        int	m_nScrollStepSize;

        UINT m_iChildAlign;
        UINT m_iChildVAlign;

        CDuiScrollBar* m_pVerticalScrollBar;
        CDuiScrollBar* m_pHorizontalScrollBar;

        CDuiRect m_rcInset;

        CDuiString	m_sVerticalScrollBarStyle;
        CDuiString	m_sHorizontalScrollBarStyle;

        CStdPtrArray m_items;

    };


}

