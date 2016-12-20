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
        LPCTSTR GetClass() const override;
        LPVOID GetInterface(LPCTSTR pstrName) override;

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

        CStdPtrArray* GetItems() override;
        CDuiControl* GetItemAt(int iIndex) const override;
        int GetItemIndex(CDuiControl* pControl) const override;
        BOOL SetItemIndex(CDuiControl* pControl, int iIndex) override;
        int GetCount() const override;
        BOOL Add(CDuiControl* pControl) override;
        BOOL AddAt(CDuiControl* pControl, int iIndex) override;
        BOOL Remove(CDuiControl* pControl) override;
        BOOL RemoveAt(int iIndex) override;
        void RemoveAll();

        virtual int FindSelectable(int iIndex, BOOL bForward = TRUE) const;

        RECT GetClientPos() const override;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) override;

        void DoPaint(HDC hDC, const RECT& rcPaint) override;

        void DoEvent(TEventUI& event) override;

        void EnableScrollBar(BOOL bEnableVertical = TRUE, bool bEnableHorizontal = FALSE) ;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

        void SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit = TRUE) override;

        CDuiControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags) override;
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

