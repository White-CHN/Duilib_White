#pragma once

namespace DuiLib
{
#pragma warning (disable : 4251)
    typedef CDuiControl* (*CreateClass)();
    typedef std::map<CDuiString, CreateClass> MAP_DUI_CTRATECLASS;

    class DUILIB_API CDuiControlFactory
    {
    private:
        CDuiControlFactory(void);
        virtual ~CDuiControlFactory(void);
    public:
        CDuiControl* CreateControl(CDuiString strClassName);
        void RegistControl(CDuiString strClassName, CreateClass pFunc);

        static CDuiControlFactory* GetInstance();
        void Release();

    private:
        MAP_DUI_CTRATECLASS m_mapControl;
    };

#define DECLARE_DUICONTROL(class_name)\
public:\
	static CDuiControl* CreateControl();

#define IMPLEMENT_DUICONTROL(class_name)\
	CDuiControl* class_name::CreateControl()\
	{ return new class_name; }

#define REGIST_DUICONTROL(class_name)\
	CDuiControlFactory::GetInstance()->RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);

#define INNER_REGISTER_DUICONTROL(class_name)\
	RegistControl(_T(#class_name), (CreateClass)class_name::CreateControl);
}