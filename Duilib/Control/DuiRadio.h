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
        LPCTSTR GetClass() const  override;
        LPVOID GetInterface(LPCTSTR pstrName)  override;
        void DoEvent(TEventUI& event)  override;
    };
}
