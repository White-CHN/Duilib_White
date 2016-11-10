#pragma once
#pragma warning (disable : 4251)
namespace DuiLib
{
    typedef CDuiControl* (*CreateClass)();
    typedef std::map<CDuiString, CreateClass> MAP_DUI_CTRATECLASS;

    class DUILIB_API CDuiControlFactory
    {
    public:
        CDuiControl* CreateControl(CDuiString strClassName);
        void RegistControl(CDuiString strClassName, CreateClass pFunc);

        static CDuiControlFactory* GetInstance();
        void Release();

    private:
        CDuiControlFactory();
        virtual ~CDuiControlFactory();

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