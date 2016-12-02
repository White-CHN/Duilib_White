#pragma once
namespace DuiLib
{
    //CDuiIPAddress::m_nDTUpdateFlag
#define IP_NONE   0
#define IP_UPDATE 1
#define IP_DELETE 2
#define IP_KEEP   3

    class CDuiIPAddress;

    class CDuiIPAddressWnd
        : public CDuiWnd
    {
    public:
        CDuiIPAddressWnd(void);
        virtual ~CDuiIPAddressWnd(void);

        void Init(CDuiIPAddress* pOwner);

        virtual LPCTSTR GetWindowClassName() const;
        virtual LPCTSTR GetSuperClassName() const;

        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void OnFinalMessage(HWND hWnd);

    private:
        BOOL m_bInit;
        CDuiIPAddress* m_pOwner;
    };

    class DUILIB_API CDuiIPAddress
        : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiIPAddress)
    public:
        CDuiIPAddress(void);
        virtual ~CDuiIPAddress(void);
    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

        void EmptyIPAddressWnd();

        int GetUpdateFlag();
        void SetUpdateFlag(int nIPUpdateFlag);

        void UpdateText();

        DWORD GetIP();
        void SetIP(DWORD dwIP);

        virtual void DoEvent(TEventUI& event);
    private:
        DWORD m_dwIP;
        int	m_nIPUpdateFlag;
        CDuiIPAddressWnd* m_pIPAddressWnd;
    };

}