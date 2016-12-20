#pragma once
namespace DuiLib
{
    //CDuiDateTime::m_nDTUpdateFlag
#define DT_NONE   0
#define DT_UPDATE 1
#define DT_DELETE 2
#define DT_KEEP   3


    class CDuiDateTime;

    class CDuiDateTimeWnd
        : public CDuiWnd
    {
    public:
        CDuiDateTimeWnd(void);
        virtual ~CDuiDateTimeWnd(void);
    public:
        RECT CalPos();
        void Init(CDuiDateTime* pOwner);

        BOOL GetDropOpen();

        LPCTSTR GetWindowClassName() const override;
        LPCTSTR GetSuperClassName() const override;

        virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        void OnFinalMessage(HWND hWnd) override;
    private:
        BOOL m_bInit;
        BOOL m_bDropOpen;
        CDuiDateTime* m_pOwner;
        SYSTEMTIME m_oldSysTime;
    };

    class DUILIB_API CDuiDateTime :
        public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiDateTime)
    public:
        CDuiDateTime(void);
        virtual ~CDuiDateTime(void);
    public:
        LPCTSTR GetClass() const override;
        LPVOID GetInterface(LPCTSTR pstrName) override;

        void SetUpdateFlag(int nDTUpdateFlag);
        void EmptyDateTimeWnd();
        void UpdateText();

        SYSTEMTIME& GetTime();
        void SetTime(SYSTEMTIME* pst);

        void DoEvent(TEventUI& event) override;
    private:
        int m_nDTUpdateFlag;
        CDuiDateTimeWnd* m_pDateTimeWnd;
        SYSTEMTIME m_sysTime;
    };

}