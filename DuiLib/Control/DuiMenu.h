#pragma once
namespace DuiLib
{
    enum MenuItemDefaultInfo
    {
        ITEM_DEFAULT_HEIGHT = 30,		//每一个item的默认高度（只在竖状排列时自定义）
        ITEM_DEFAULT_WIDTH = 150,		//窗口的默认宽度

        ITEM_DEFAULT_ICON_WIDTH = 26,	//默认图标所占宽度
        ITEM_DEFAULT_ICON_SIZE = 16,	//默认图标的大小

        ITEM_DEFAULT_EXPLAND_ICON_WIDTH = 20,	//默认下级菜单扩展图标所占宽度
        ITEM_DEFAULT_EXPLAND_ICON_SIZE = 9,		//默认下级菜单扩展图标的大小

        DEFAULT_LINE_LEFT_INSET = ITEM_DEFAULT_ICON_WIDTH + 3,	//默认分隔线的左边距
        DEFAULT_LINE_RIGHT_INSET = 7,	//默认分隔线的右边距
        DEFAULT_LINE_HEIGHT = 6,		//默认分隔线所占高度
        DEFAULT_LINE_COLOR = 0xFFBCBFC4	//默认分隔线颜色

    };

    enum MenuAlignment
    {
        eMenuAlignment_Left = 1 << 1,
        eMenuAlignment_Top = 1 << 2,
        eMenuAlignment_Right = 1 << 3,
        eMenuAlignment_Bottom = 1 << 4,
    };

    struct ContextMenuParam
    {
        // 1: remove all
        // 2: remove the sub menu
        WPARAM wParam;
        HWND hWnd;
    };

    struct MenuItemInfo
    {
        CDuiString strName;
        BOOL bChecked;
    };

    struct MenuCmd
    {
        CDuiString strName;
        CDuiString strUserData;
        CDuiString strText;
        BOOL bChecked;
    };

    class MenuMenuReceiverImplBase;
    class MenuMenuObserverImplBase;

    class MenuMenuObserverImplBase
    {
    public:
        virtual void AddReceiver(MenuMenuReceiverImplBase* receiver) = 0;
        virtual void RemoveReceiver(MenuMenuReceiverImplBase* receiver) = 0;
        virtual BOOL RBroadcast(ContextMenuParam param) = 0;
    };

    class DUILIB_API MenuObserverImpl : public MenuMenuObserverImplBase
    {
    public:
        MenuObserverImpl();
        virtual ~MenuObserverImpl();
    public:
        void AddReceiver(MenuMenuReceiverImplBase* receiver) OVERRIDE;
        void RemoveReceiver(MenuMenuReceiverImplBase* receiver) OVERRIDE;

        virtual BOOL RBroadcast(ContextMenuParam param);

        virtual CDuiPaintManager* GetManager() const;
        virtual void SetManger(CDuiPaintManager* pManager);

        virtual CStdStringPtrMap* GetMenuCheckInfo() const;
        virtual void SetMenuCheckInfo(CStdStringPtrMap* pInfo);
    public:
        class Iterator
        {
        public:
            Iterator(MenuObserverImpl& table);
            Iterator(const Iterator& v);
        public:
            MenuMenuReceiverImplBase* next();
        private:
            DWORD index;
            MenuObserverImpl& _tbl;
            MenuMenuReceiverImplBase* ptr;
        };
    private:
        vector<MenuMenuReceiverImplBase*> m_vtReceivers;
        CDuiPaintManager* m_pPaintManager;
        CStdStringPtrMap* m_pMenuCheckInfo;
    };

    class MenuMenuReceiverImplBase
    {
    public:
        virtual void AddObserver(MenuMenuObserverImplBase* observer) = 0;
        virtual void RemoveObserver() = 0;
        virtual BOOL Receive(ContextMenuParam param) = 0;
    };

    class DUILIB_API MenuReceiverImpl : public MenuMenuReceiverImplBase
    {
    public:
        MenuReceiverImpl();
        virtual ~MenuReceiverImpl();
    public:
        void AddObserver(MenuMenuObserverImplBase* observer) OVERRIDE;
        void RemoveObserver() OVERRIDE;
        BOOL Receive(ContextMenuParam param) OVERRIDE;
    private:
        std::vector<MenuMenuObserverImplBase*> m_vtObservers;
    };


    class CDuiMenuWnd;
    class DUILIB_API CDuiMenuElement
        : public CDuiListContainerElement
    {
        DECLARE_DUICONTROL(CDuiMenuElement)
    public:
        CDuiMenuElement();
        virtual ~CDuiMenuElement();
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        void EmptyMenuWnd();

        void SetLineType();

        DWORD GetLineColor() const;
        void SetLineColor(DWORD color);

        CDuiRect GetLinePadding() const;
        void SetLinePadding(CDuiRect rcInset);

        CDuiString GetIcon() const;
        void SetIcon(LPCTSTR strIcon);

        CDuiSize GetIconSize();
        void SetIconSize(CDuiSize szIcon);

        MenuItemInfo* GetItemInfo(LPCTSTR pstrName) const;
        MenuItemInfo* SetItemInfo(LPCTSTR pstrName, BOOL bChecked);

        BOOL GetChecked() const;
        void SetChecked(BOOL bCheck = TRUE);

        BOOL GetCheckItem() const;
        void SetCheckItem(BOOL bCheckItem = FALSE);

        BOOL GetShowExplandIcon() const;
        void SetShowExplandIcon(BOOL bShow);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void DrawItemText(HDC hDC, const RECT& rcItem) OVERRIDE;
        virtual void DrawItemIcon(HDC hDC, const RECT& rcItem);
        virtual void DrawItemExpland(HDC hDC, const RECT& rcItem);
        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

        SIZE EstimateSize(SIZE szAvailable) OVERRIDE;

        void CreateMenuWnd();
        void DoEvent(CDuiEvent& event) OVERRIDE;
    private:
        BOOL m_bCheckItem;
        BOOL m_bDrawLine;	//画分隔线
        BOOL m_bShowExplandIcon;

        DWORD		m_dwLineColor;  //分隔线颜色

        CDuiMenuWnd* m_pMenuWnd;

        CDuiRect m_rcLinePadding;	//分割线的左右边距
        CDuiSize m_szIconSize; 	//画图标
        CDuiString	m_strIcon;
    };

    class CDuiMenu;
    class DUILIB_API CDuiMenuWnd
        : public CDuiWnd
        , public MenuReceiverImpl
        , public INotify
        , public IDialogBuilderCallback
    {
    public:
        CDuiMenuWnd();
        virtual ~CDuiMenuWnd();
    public:
        LPCTSTR GetWindowClassName() const OVERRIDE;
        void OnFinalMessage(HWND hWnd) OVERRIDE;
        void Notify(CDuiNotify& msg) OVERRIDE;
        CDuiControl* CreateControl(LPCTSTR pstrClassName) OVERRIDE;
        void Close(UINT nRet = IDOK) OVERRIDE;
        BOOL Receive(ContextMenuParam param) OVERRIDE;
        /*
         *	@pOwner 一级菜单不要指定这个参数，这是菜单内部使用的
         *	@xml	菜单的布局文件
         *	@point	菜单的左上角坐标
         *	@pMainPaintManager	菜单的父窗体管理器指针
         *	@pMenuCheckInfo	保存菜单的单选和复选信息结构指针
         *	@dwAlignment		菜单的出现位置，默认出现在鼠标的右下侧。
         */
        void Init(CDuiMenuElement* pOwner, CIdToResource xml, POINT point,
                  CDuiPaintManager* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo = NULL,
                  DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top);

        CDuiMenu* GetMenuRoot();// 获取根菜单控件，用于动态添加子菜单
        BOOL IsClosing() const;

        void ResizeMenu();// 重新调整菜单的大小
        void ResizeSubMenu();// 重新调整子菜单的大小
        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;
    public:
        static MenuObserverImpl& GetGlobalContextMenuObserver();
        static void DestroyMenu();
        static MenuItemInfo* SetMenuItemInfo(LPCTSTR pstrName, BOOL bChecked);
    private:
        BOOL bIsClosing;
        DWORD m_dwAlignment;	//菜单对齐方式
        CDuiMenu* m_pLayout;
        CDuiMenuElement* m_pOwner;
        CIdToResource m_xml;
        CDuiPoint m_BasedPoint;
        CDuiPaintManager m_PaintManager;
    };


    class DUILIB_API CDuiMenu
        : public CDuiList
    {
        DECLARE_DUICONTROL(CDuiMenu)
    public:
        CDuiMenu(void);
        virtual ~CDuiMenu(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        BOOL Add(CDuiControl* pControl) OVERRIDE;
        BOOL AddAt(CDuiControl* pControl, int iIndex) OVERRIDE;

        int GetItemIndex(CDuiControl* pControl) const OVERRIDE;
        BOOL SetItemIndex(CDuiControl* pControl, int iIndex) OVERRIDE;
        BOOL Remove(CDuiControl* pControl) OVERRIDE;

        CDuiMenuWnd* GetMenuWnd();
        void SetMenuWnd(CDuiMenuWnd* pMenuWnd);

        SIZE EstimateSize(SIZE szAvailable) OVERRIDE;
    private:
        CDuiMenuWnd* m_pMenuWnd;
    };

}