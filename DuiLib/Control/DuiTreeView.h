#pragma once
namespace DuiLib
{
    class CDuiTreeView;
    class DUILIB_API CDuiTreeNode
        : public CDuiListContainerElement
    {
        DECLARE_DUICONTROL(CDuiTreeNode)
    public:
        CDuiTreeNode(CDuiTreeNode* pParentNode = NULL);
        virtual ~CDuiTreeNode(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString pstrName) OVERRIDE;

        void Invalidate();

        BOOL Select(BOOL bSelect = TRUE) OVERRIDE;
        BOOL SelectMulti(BOOL bSelect = TRUE) OVERRIDE;

        BOOL Add(CDuiControl* pControl) OVERRIDE;
        BOOL AddAt(CDuiControl* pControl, int iIndex) OVERRIDE;
        BOOL Remove(CDuiControl* pControl) OVERRIDE;

        BOOL GetVisibleTag() const;
        void SetVisibleTag(BOOL bIsVisible);

        CDuiString GetItemText();
        void SetItemText(LPCTSTR pstrValue);

        BOOL IsCheckBoxSelected() const;
        void CheckBoxSelected(BOOL bSelected);

        BOOL IsHasChild() const;
        BOOL AddChildNode(CDuiTreeNode* pTreeNodeUI);
        BOOL RemoveAt(CDuiTreeNode* pTreeNodeUI);

        CDuiTreeNode* GetParentNode() const;
        void SetParentNode(CDuiTreeNode* pParentTreeNode);

        long GetCountChild() const;
        void SetTreeView(CDuiTreeView* pCTreeViewUI);
        CDuiTreeView* GetTreeView() const;
        CDuiTreeNode* GetChildNode(int nIndex) const;

        CDuiHorizontalLayout*	GetTreeNodeHoriznotal() const;
        CDuiCheckBox*			GetFolderButton() const;
        CDuiLabel*				GetDottedLine() const;
        CDuiCheckBox*			GetCheckBox() const;
        CDuiOption*				GetItemButton() const;

        CStdPtrArray GetTreeNodes();
        int			 GetTreeIndex();
        int			 GetNodeIndex();


        BOOL GetVisibleFolderBtn() const;
        void SetVisibleFolderBtn(BOOL bIsVisibled);

        BOOL GetVisibleCheckBtn() const;
        void SetVisibleCheckBtn(BOOL bIsVisibled);

        DWORD GetItemTextColor() const;
        void SetItemTextColor(DWORD dwItemTextColor);

        DWORD GetItemHotTextColor() const;
        void SetItemHotTextColor(DWORD dwItemHotTextColor);

        DWORD GetSelItemTextColor() const;
        void SetSelItemTextColor(DWORD dwSelItemTextColor);

        DWORD GetSelItemHotTextColor() const;
        void SetSelItemHotTextColor(DWORD dwSelHotItemTextColor);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void DoEvent(CDuiEvent& event) OVERRIDE;
    private:
        CDuiTreeNode* GetLastNode();
        CDuiTreeNode* CalLocation(CDuiTreeNode* pTreeNodeUI);
    private:
        BOOL	m_bIsVisable;
        BOOL	m_bIsCheckBox;
        long	m_iTreeLavel;
        DWORD	m_dwItemTextColor;
        DWORD	m_dwItemHotTextColor;
        DWORD	m_dwSelItemTextColor;
        DWORD	m_dwSelItemHotTextColor;

        CDuiTreeView*			m_pTreeView;
        CDuiHorizontalLayout*	m_pHoriz;
        CDuiCheckBox*			m_pFolderButton;
        CDuiLabel*				m_pDottedLine;
        CDuiCheckBox*			m_pCheckBox;
        CDuiOption*				m_pItemButton;
        CDuiTreeNode*			m_pParentTreeNode;
        CStdPtrArray			m_TreeNodes;
    };

    class DUILIB_API CDuiTreeView
        : public CDuiList
    {
        DECLARE_DUICONTROL(CDuiTreeView)
    public:
        CDuiTreeView(void);
        virtual ~CDuiTreeView(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString pstrName) OVERRIDE;

        UINT GetListType() OVERRIDE;
        virtual BOOL Add(CDuiTreeNode* pControl);
        virtual long AddAt(CDuiTreeNode* pControl, int iIndex);
        virtual BOOL AddAt(CDuiTreeNode* pControl, CDuiTreeNode* _IndexNode);
        virtual BOOL Remove(CDuiTreeNode* pControl);

        BOOL RemoveAt(int iIndex) OVERRIDE;
        void RemoveAll() OVERRIDE;

        bool OnCheckBoxChanged(void* param);
        bool OnFolderChanged(void* param);
        bool OnDBClickItem(void* param);

        BOOL SetItemCheckBox(BOOL bSelected, CDuiTreeNode* pTreeNode = NULL);
        void SetItemExpand(BOOL bExpanded, CDuiTreeNode* pTreeNode = NULL);

        BOOL GetVisibleFolderBtn() const;
        void SetVisibleFolderBtn(BOOL bIsVisibled);

        BOOL GetVisibleCheckBtn() const;
        void SetVisibleCheckBtn(BOOL bIsVisibled);

        UINT GetItemMinWidth() const;
        void SetItemMinWidth(UINT bItemMinWidth);

        void SetItemTextColor(DWORD dwItemTextColor);
        void SetItemHotTextColor(DWORD dwItemHotTextColor);
        void SetSelItemTextColor(DWORD dwSelItemTextColor);
        void SetSelItemHotTextColor(DWORD dwSelHotItemTextColor);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
    private:
        BOOL m_bVisibleFolderBtn;
        BOOL m_bVisibleCheckBtn;
        UINT m_uItemMinWidth;
    };

}