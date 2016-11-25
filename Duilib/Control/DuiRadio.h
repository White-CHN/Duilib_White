#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiRadio :
        public CDuiOption
    {
        DECLARE_DUICONTROL(CDuiRadio)
    public:
        CDuiRadio(void);
        virtual ~CDuiRadio(void);
    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual void DoEvent(TEventUI& event);
    };
}
