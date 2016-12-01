#pragma once

namespace DuiLib
{
    class CDuiScrollBar;

    class IContainer
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

        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

        virtual RECT GetInset() const;
        virtual void SetInset(RECT rcInset); // 设置内边距，相当于设置客户区

        virtual int GetChildPadding() const;
        virtual void SetChildPadding(int iPadding);

        virtual UINT GetChildAlign() const;

        virtual UINT GetChildVAlign() const;

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

        virtual CStdPtrArray* GetItems();
        virtual CDuiControl* GetItemAt(int iIndex) const;
        virtual int GetItemIndex(CDuiControl* pControl) const ;
        virtual BOOL SetItemIndex(CDuiControl* pControl, int iIndex);
        virtual int GetCount() const;
        virtual BOOL Add(CDuiControl* pControl);
        virtual BOOL AddAt(CDuiControl* pControl, int iIndex);
        virtual BOOL Remove(CDuiControl* pControl);
        virtual BOOL RemoveAt(int iIndex);
        virtual void RemoveAll();

        virtual int FindSelectable(int iIndex, BOOL bForward = TRUE) const;

        virtual RECT GetClientPos() const;
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);

        virtual void DoPaint(HDC hDC, const RECT& rcPaint);

        virtual void DoEvent(TEventUI& event);

        virtual void EnableScrollBar(BOOL bEnableVertical = TRUE, bool bEnableHorizontal = FALSE);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

        virtual void SetManager(CDuiPaintManager* pManager, CDuiControl* pParent, BOOL bInit = TRUE);

        virtual CDuiControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
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

        RECT m_rcInset;

        CDuiString	m_sVerticalScrollBarStyle;
        CDuiString	m_sHorizontalScrollBarStyle;

        CStdPtrArray m_items;

    };


}

