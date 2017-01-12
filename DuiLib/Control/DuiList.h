#pragma once
namespace DuiLib
{

    class IListCallback
    {
    public:
        virtual LPCTSTR GetItemText(CDuiControl* pList, int iItem, int iSubItem) = 0;
    };

    class DUILIB_API CDuiListHeaderItem
        : public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiListHeaderItem)
    public:
        CDuiListHeaderItem();
        virtual ~CDuiListHeaderItem();
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void SetEnabled(BOOL bEnable = TRUE) OVERRIDE;

        BOOL IsDragable() const;
        void SetDragable(BOOL bDragable);

        DWORD GetSepWidth() const;
        void SetSepWidth(int iWidth);

        DWORD GetTextStyle() const;
        void SetTextStyle(UINT uStyle);

        DWORD GetTextColor() const;
        void SetTextColor(DWORD dwTextColor);

        void SetTextPadding(RECT rc);
        RECT GetTextPadding() const;

        int GetFont() const;
        void SetFont(int index);

        BOOL IsShowHtml();
        void SetShowHtml(BOOL bShowHtml = TRUE);

        LPCTSTR GetNormalImage() const;
        void SetNormalImage(LPCTSTR pStrImage);

        LPCTSTR GetHotImage() const;
        void SetHotImage(LPCTSTR pStrImage);

        LPCTSTR GetPushedImage() const;
        void SetPushedImage(LPCTSTR pStrImage);

        LPCTSTR GetFocusedImage() const;
        void SetFocusedImage(LPCTSTR pStrImage);

        LPCTSTR GetSepImage() const;
        void SetSepImage(LPCTSTR pStrImage);

        int GetScale() const;
        void SetScale(int nScale);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        SIZE EstimateSize(SIZE szAvailable) OVERRIDE;
        void PaintText(HDC hDC) OVERRIDE;
        RECT GetThumbRect() const;
        void PaintStatusImage(HDC hDC) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
    private:
        BOOL m_bShowHtml;
        BOOL m_bDragable;

        int m_iFont;
        int m_nScale;
        int m_iSepWidth;

        DWORD m_dwTextColor;

        UINT m_uButtonState;
        UINT m_uTextStyle;

        CDuiPoint ptLastMouse;

        CDuiRect m_rcTextPadding;

        CDuiString m_sNormalImage;
        CDuiString m_sHotImage;
        CDuiString m_sPushedImage;
        CDuiString m_sFocusedImage;
        CDuiString m_sSepImage;
        CDuiString m_sSepImageModify;
    };

    class DUILIB_API CDuiListHeader
        : public CDuiHorizontalLayout
    {
        DECLARE_DUICONTROL(CDuiListHeader)
    public:
        CDuiListHeader();
        virtual ~CDuiListHeader();
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        BOOL IsScaleHeader() const;
        void SetScaleHeader(BOOL bIsScale);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        SIZE EstimateSize(SIZE szAvailable) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
    private:
        BOOL m_bIsScaleHeader;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    //

    class DUILIB_API CDuiListContainerElement
        : public CDuiHorizontalLayout
        , public IListItem
    {
        DECLARE_DUICONTROL(CDuiListContainerElement)
    public:
        CDuiListContainerElement();
        virtual ~CDuiListContainerElement();
    public:
        CDuiString GetClass() const OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        int GetIndex() const OVERRIDE;
        void SetIndex(int iIndex) OVERRIDE;

        IListOwner* GetOwner() OVERRIDE;
        void SetOwner(CDuiControl* pOwner) OVERRIDE;
        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetEnabled(BOOL bEnable = TRUE) OVERRIDE;

        BOOL IsSelected() const OVERRIDE;
        BOOL Select(BOOL bSelect = TRUE) OVERRIDE;
        BOOL SelectMulti(BOOL bSelect = TRUE) OVERRIDE;
        BOOL IsExpanded() const OVERRIDE;
        BOOL Expand(BOOL bExpand = TRUE) OVERRIDE;

        void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
        BOOL Activate() OVERRIDE;

        UINT GetButtonState();
        void SetButtonState(UINT uButtonState);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        virtual void DrawItemText(HDC hDC, const RECT& rcItem);
        virtual void DrawItemBk(HDC hDC, const RECT& rcItem);
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;

        void DoEvent(TEventUI& event) OVERRIDE;
    private:
        int m_iIndex;
        BOOL m_bSelected;
        UINT m_uButtonState;
        IListOwner* m_pOwner;
    };

    /////////////////////////////////////////////////////////////////////////////////////////

    class IList;
    class DUILIB_API CDuiListTextElement : public CDuiListLabelElement
    {
        DECLARE_DUICONTROL(CDuiListTextElement)
    public:
        CDuiListTextElement();
        virtual ~CDuiListTextElement();
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;
        UINT GetControlFlags() const OVERRIDE;

        LPCTSTR GetText(int iIndex) const;
        void SetText(int iIndex, LPCTSTR pstrText);

        void SetOwner(CDuiControl* pOwner) OVERRIDE;
        CDuiString* GetLinkContent(int iIndex);

        void DoEvent(TEventUI& event) OVERRIDE;
        SIZE EstimateSize(SIZE szAvailable) OVERRIDE;

        void DrawItemText(HDC hDC, const RECT& rcItem) OVERRIDE;

    private:
        enum { MAX_LINK = 8 };
    private:
        int m_nLinks;
        int m_nHoverLink;
        IList* m_pOwner;
        CDuiRect m_rcLinks[MAX_LINK];
        CDuiString m_sLinks[MAX_LINK];
        CStdPtrArray m_aTexts;
    };

    /////////////////////////////////////////////////////////////////////////////////////
    //
    typedef int (CALLBACK* PULVCompareFunc)(UINT_PTR, UINT_PTR, UINT_PTR);
    class CDuiList;
    class DUILIB_API CDuiListBody : public CDuiVerticalLayout
    {
    public:
        CDuiListBody(CDuiList* pOwner);
        virtual ~CDuiListBody();
    public:
        BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);

        int GetScrollStepSize() const OVERRIDE;
        void SetScrollPos(SIZE szPos, BOOL bMsg = TRUE) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
    protected:
        static int __cdecl ItemComareFunc(void* pvlocale, const void* item1, const void* item2);
        int __cdecl ItemComareFunc(const void* item1, const void* item2);
    private:
        CDuiList* m_pOwner;
        PULVCompareFunc m_pCompareFunc;
        UINT_PTR m_compareData;
    };

    class IList : public IListOwner
    {
    public:
        virtual CDuiListHeader* GetHeader() const = 0;
        virtual CDuiContainer* GetList() const = 0;

        virtual IListCallback* GetTextCallback() const = 0;
        virtual void SetTextCallback(IListCallback* pCallback) = 0;

        virtual int GetExpandedItem() const = 0;
        virtual BOOL ExpandItem(int iIndex, BOOL bExpand = TRUE) = 0;

        virtual BOOL IsMultiSelect() const = 0;
        virtual void SetMultiSelect(BOOL bMultiSel) = 0;

        virtual void UnSelectAllItems() = 0;
        virtual void SelectAllItems() = 0;

        virtual int GetSelectItemCount() const = 0;
        virtual int GetNextSelItem(int nItem) const = 0;
    };

    class DUILIB_API CDuiList
        : public CDuiVerticalLayout
        , public IList
    {
        DECLARE_DUICONTROL(CDuiList)
    public:
        CDuiList(void);
        virtual ~CDuiList(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);

        //IContainer
        CStdPtrArray* GetItems() OVERRIDE;
        CDuiControl* GetItemAt(int iIndex) const OVERRIDE;
        int GetItemIndex(CDuiControl* pControl) const  OVERRIDE;
        BOOL SetItemIndex(CDuiControl* pControl, int iIndex) OVERRIDE;
        int GetCount() const OVERRIDE;
        BOOL Add(CDuiControl* pControl) OVERRIDE;
        BOOL AddAt(CDuiControl* pControl, int iIndex) OVERRIDE;
        BOOL Remove(CDuiControl* pControl) OVERRIDE ;
        BOOL RemoveAt(int iIndex) OVERRIDE;
        void RemoveAll() OVERRIDE;
        //IListOwner
        UINT GetListType() OVERRIDE;
        ListInfo* GetListInfo() OVERRIDE;
        int GetCurSel() const OVERRIDE;
        void Scroll(int dx, int dy);
        void EnsureVisible(int iIndex);
        BOOL SelectItem(int iIndex, BOOL bTakeFocus = FALSE) OVERRIDE;
        BOOL SelectMultiItem(int iIndex, BOOL bTakeFocus = FALSE) OVERRIDE;
        BOOL UnSelectItem(int iIndex, BOOL bOthers = FALSE) OVERRIDE;

        //IList
        CDuiListHeader* GetHeader() const OVERRIDE;
        CDuiContainer* GetList() const OVERRIDE;

        IListCallback* GetTextCallback() const OVERRIDE;
        void SetTextCallback(IListCallback* pCallback) OVERRIDE;

        int GetExpandedItem() const OVERRIDE;
        BOOL ExpandItem(int iIndex, BOOL bExpand = TRUE) OVERRIDE;

        BOOL IsMultiSelect() const OVERRIDE;
        void SetMultiSelect(BOOL bMultiSel) OVERRIDE;

        void UnSelectAllItems() OVERRIDE;
        void SelectAllItems() OVERRIDE;

        int GetSelectItemCount() const OVERRIDE;
        int GetNextSelItem(int nItem) const OVERRIDE;
        //CDuiContainer
        SIZE GetScrollPos() const OVERRIDE;
        SIZE GetScrollRange() const OVERRIDE;
        void SetScrollPos(SIZE szPos, BOOL bMsg = TRUE) OVERRIDE;
        void LineUp() OVERRIDE;
        void LineDown() OVERRIDE;
        void LineLeft() OVERRIDE;
        void LineRight() OVERRIDE;
        void PageUp() OVERRIDE;
        void PageDown() OVERRIDE;
        void PageLeft() OVERRIDE;
        void PageRight() OVERRIDE;
        void HomeUp() OVERRIDE;
        void EndDown() OVERRIDE;
        void HomeLeft() OVERRIDE;
        void EndRight() OVERRIDE;
        CDuiScrollBar* GetVerticalScrollBar() const OVERRIDE;
        CDuiScrollBar* GetHorizontalScrollBar() const OVERRIDE;

        BOOL GetScrollSelect();
        void SetScrollSelect(BOOL bScrollSelect);

        BOOL IsMultiExpanding();
        void SetMultiExpanding(BOOL bMultiExpandable);

        int GetItemFont() const;
        void SetItemFont(int index);

        UINT GetItemTextStyle();
        void SetItemTextStyle(UINT uStyle);

        RECT GetItemTextPadding();
        void SetItemTextPadding(RECT rc);

        DWORD GetItemTextColor();
        void SetItemTextColor(DWORD dwTextColor);

        DWORD GetItemBkColor();
        void SetItemBkColor(DWORD dwBkColor);

        LPCTSTR GetItemBkImage();
        void SetItemBkImage(LPCTSTR pStrImage);

        BOOL IsAlternateBk();
        void SetAlternateBk(BOOL bAlternateBk);

        DWORD GetSelectedItemTextColor();
        void SetSelectedItemTextColor(DWORD dwTextColor);

        DWORD GetSelectedItemBkColor();
        void SetSelectedItemBkColor(DWORD dwBkColor);

        LPCTSTR GetSelectedItemImage();
        void SetSelectedItemImage(LPCTSTR pStrImage);

        DWORD GetHotItemTextColor();
        void SetHotItemTextColor(DWORD dwTextColor);

        DWORD GetHotItemBkColor();
        void SetHotItemBkColor(DWORD dwBkColor);

        LPCTSTR GetHotItemImage();
        void SetHotItemImage(LPCTSTR pStrImage);

        DWORD GetDisabledItemTextColor();
        void SetDisabledItemTextColor(DWORD dwTextColor);

        DWORD GetDisabledItemBkColor();
        void SetDisabledItemBkColor(DWORD dwBkColor);

        LPCTSTR GetDisabledItemImage();
        void SetDisabledItemImage(LPCTSTR pStrImage);

        DWORD GetItemLineColor();
        void SetItemLineColor(DWORD dwLineColor);

        BOOL IsItemShowRowLine();
        void SetItemShowRowLine(BOOL bShowLine = FALSE);

        BOOL IsItemShowColumnLine();
        void SetItemShowColumnLine(BOOL bShowLine = FALSE);

        BOOL IsItemShowHtml();
        void SetItemShowHtml(BOOL bShowHtml = TRUE);

        void EnableScrollBar(BOOL bEnableVertical = TRUE, BOOL bEnableHorizontal = FALSE) OVERRIDE;
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void Move(SIZE szOffset, BOOL bNeedInvalidate = TRUE) OVERRIDE;
        void DoEvent(TEventUI& event) OVERRIDE;
    protected:
        int GetMinSelItemIndex();
        int GetMaxSelItemIndex();
    private:
        BOOL m_bMultiSel;
        BOOL m_bScrollSelect;
        int m_iCurSel;
        int m_iExpandedItem;

        IListCallback* m_pCallback;
        CDuiListBody* m_pList;
        CDuiListHeader* m_pHeader;

        CStdPtrArray m_aSelItems;

        ListInfo m_ListInfo;
    };

}