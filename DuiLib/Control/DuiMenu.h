#pragma once
namespace DuiLib
{
    enum MenuItemDefaultInfo
    {
        ITEM_DEFAULT_HEIGHT = 30,		//ÿһ��item��Ĭ�ϸ߶ȣ�ֻ����״����ʱ�Զ��壩
        ITEM_DEFAULT_WIDTH = 150,		//���ڵ�Ĭ�Ͽ��

        ITEM_DEFAULT_ICON_WIDTH = 26,	//Ĭ��ͼ����ռ���
        ITEM_DEFAULT_ICON_SIZE = 16,	//Ĭ��ͼ��Ĵ�С

        ITEM_DEFAULT_EXPLAND_ICON_WIDTH = 20,	//Ĭ���¼��˵���չͼ����ռ���
        ITEM_DEFAULT_EXPLAND_ICON_SIZE = 9,		//Ĭ���¼��˵���չͼ��Ĵ�С

        DEFAULT_LINE_LEFT_INSET = ITEM_DEFAULT_ICON_WIDTH + 3,	//Ĭ�Ϸָ��ߵ���߾�
        DEFAULT_LINE_RIGHT_INSET = 7,	//Ĭ�Ϸָ��ߵ��ұ߾�
        DEFAULT_LINE_HEIGHT = 6,		//Ĭ�Ϸָ�����ռ�߶�
        DEFAULT_LINE_COLOR = 0xFFBCBFC4	//Ĭ�Ϸָ�����ɫ

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
        BOOL m_bDrawLine;	//���ָ���
        BOOL m_bShowExplandIcon;

        DWORD		m_dwLineColor;  //�ָ�����ɫ

        CDuiMenuWnd* m_pMenuWnd;

        CDuiRect m_rcLinePadding;	//�ָ��ߵ����ұ߾�
        CDuiSize m_szIconSize; 	//��ͼ��
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
         *	@pOwner һ���˵���Ҫָ��������������ǲ˵��ڲ�ʹ�õ�
         *	@xml	�˵��Ĳ����ļ�
         *	@point	�˵������Ͻ�����
         *	@pMainPaintManager	�˵��ĸ����������ָ��
         *	@pMenuCheckInfo	����˵��ĵ�ѡ�͸�ѡ��Ϣ�ṹָ��
         *	@dwAlignment		�˵��ĳ���λ�ã�Ĭ�ϳ������������²ࡣ
         */
        void Init(CDuiMenuElement* pOwner, CIdToResource xml, POINT point,
                  CDuiPaintManager* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo = NULL,
                  DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top);

        CDuiMenu* GetMenuRoot();// ��ȡ���˵��ؼ������ڶ�̬����Ӳ˵�
        BOOL IsClosing() const;

        void ResizeMenu();// ���µ����˵��Ĵ�С
        void ResizeSubMenu();// ���µ����Ӳ˵��Ĵ�С
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
        DWORD m_dwAlignment;	//�˵����뷽ʽ
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