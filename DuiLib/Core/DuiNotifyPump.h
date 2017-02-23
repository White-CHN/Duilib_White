#pragma once

namespace DuiLib
{
    class CDuiNotifyPump;	//����

    typedef void (CDuiNotifyPump::*DUI_PMSG)(CDuiNotify& msg);  //ָ������

    // ��Ϣ����
    enum DuiSig
    {
        DuiSig_end = 0, // [marks end of message map]
        DuiSig_lwl,     // LRESULT (WPARAM, LPARAM)
        DuiSig_vn,      // void (CDuiNotify)
    };

    union DuiMessageMapFunctions
    {
        DUI_PMSG pfn;   // generic member function pointer
        LRESULT(CDuiNotifyPump::*pfn_Notify_lwl)(WPARAM, LPARAM);
        void (CDuiNotifyPump::*pfn_Notify_vn)(CDuiNotify&);
    };

    //����һ���ṹ�壬�������Ϣ��Ϣ
    struct DUI_MSGMAP_ENTRY
    {
        CDuiString sMsgType;          // DUI��Ϣ����
        CDuiString sCtrlName;         // �ؼ�����
        UINT       nSig;              // ��Ǻ���ָ������
        DUI_PMSG   pfn;               // ָ������ָ��
    };

    struct DUI_MSGMAP
    {
#ifndef DUILIB_STATIC
        const DUI_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
        const DUI_MSGMAP* pBaseMap;
#endif
        const DUI_MSGMAP_ENTRY* lpEntries;
    };

    //����
#ifndef DUILIB_STATIC
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static const DUI_MSGMAP messageMap;                                   \
	static const DUI_MSGMAP* PASCAL _GetBaseMessageMap();                 \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#else
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
	static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
	static  const DUI_MSGMAP messageMap;				                  \
	virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#endif

    //���忪ʼ
#ifndef DUILIB_STATIC
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
	{ return NULL; }                                                  \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	DUILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{  &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };\
	DUILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	DUILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{  NULL, &theClass::_messageEntries[0] };                         \
	DUILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif


    //����������ʼ
#ifndef DUILIB_STATIC
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
	{ return &baseClass::messageMap; }                                \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	DUILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
	DUILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#else
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
	const DUI_MSGMAP* theClass::GetMessageMap() const                     \
	{ return &theClass::messageMap; }                                 \
	DUILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
	{ &baseClass::messageMap, &theClass::_messageEntries[0] };        \
	DUILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
	{                                                                     \

#endif

    //��������
#define DUI_END_MESSAGE_MAP()                                             \
	{ _T(""), _T(""), DuiSig_end, (DUI_PMSG)0 }                           \
	};                                                                        \


    // @msgtype:������Ϣ���� @memberFxn:ִ�к�����
#define DUI_ON_MSGTYPE(msgtype, memberFxn)                                \
	{ msgtype, _T(""), DuiSig_vn, (DUI_PMSG)&memberFxn},                  \

    // @msgtype:������Ϣ���� @ctrname:�ؼ����� @memberFxn:ִ�к�����
#define DUI_ON_MSGTYPE_CTRNAME(msgtype,ctrname,memberFxn)                 \
	{ msgtype, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },                \

    // @ctrname:����click��Ϣ�Ŀؼ����� @memberFxn:ִ�к�����
#define DUI_ON_CLICK_CTRNAME(ctrname,memberFxn)                           \
	{ DUI_MSGTYPE_CLICK, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },      \

    // @ctrname:����selectchanged��Ϣ�Ŀؼ����� @memberFxn:ִ�к�����
#define DUI_ON_SELECTCHANGED_CTRNAME(ctrname,memberFxn)                   \
	{ DUI_MSGTYPE_SELECTCHANGED,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn }, \

    // @ctrname:����killfocus��Ϣ�Ŀؼ����� @memberFxn:ִ�к�����
#define DUI_ON_KILLFOCUS_CTRNAME(ctrname,memberFxn)                       \
	{ DUI_MSGTYPE_KILLFOCUS,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },     \

    // @ctrname:����menu��Ϣ�Ŀؼ����� @memberFxn:ִ�к�����
#define DUI_ON_MENU_CTRNAME(ctrname,memberFxn)                            \
	{ DUI_MSGTYPE_MENU,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },          \


    //������ؼ������޹ص���Ϣ��

    //����windowinit��Ϣ--ִ�к�����
#define DUI_ON_WINDOWINIT()                                                    \
	{ DUI_MSGTYPE_WINDOWINIT, _T(""), DuiSig_vn,(DUI_PMSG)&OnWindowInit }, \

    //����timer��Ϣ--ִ�к�����
#define DUI_ON_TIMER()                                                    \
	{ DUI_MSGTYPE_TIMER, _T(""), DuiSig_vn,(DUI_PMSG)&OnTimer },          \







    class DUILIB_API CDuiNotifyPump
    {
        DUI_DECLARE_MESSAGE_MAP()
    public:
        CDuiNotifyPump(void);
        virtual ~CDuiNotifyPump(void);
    private:
        static const DUI_MSGMAP_ENTRY* DuiFindMessageEntry(const DUI_MSGMAP_ENTRY* lpEntry, CDuiNotify& msg);
        BOOL LoopDispatch(CDuiNotify& msg);
    public:
        BOOL AddVirtualWnd(CDuiString strName, CDuiNotifyPump* pObject);
        BOOL RemoveVirtualWnd(CDuiString strName);
        void NotifyPump(CDuiNotify& msg);
    private:
        CStdStringPtrMap m_VirtualWndMap;
    };

}