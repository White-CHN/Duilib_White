#pragma once

namespace DuiLib
{
    class CDuiNotifyPump;	//声明

    typedef void (CDuiNotifyPump::*DUI_PMSG)(CDuiNotify& msg);  //指针类型

    // 消息类型
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

    //定义一个结构体，来存放消息信息
    struct DUI_MSGMAP_ENTRY
    {
        CDuiString sMsgType;          // DUI消息类型
        CDuiString sCtrlName;         // 控件名称
        UINT       nSig;              // 标记函数指针类型
        DUI_PMSG   pfn;               // 指向函数的指针
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

    //声明
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

    //定义开始
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


    //子类声明开始
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

    //声明结束
#define DUI_END_MESSAGE_MAP()                                             \
	{ _T(""), _T(""), DuiSig_end, (DUI_PMSG)0 }                           \
	};                                                                        \


    // @msgtype:定义消息类型 @memberFxn:执行函数宏
#define DUI_ON_MSGTYPE(msgtype, memberFxn)                                \
	{ msgtype, _T(""), DuiSig_vn, (DUI_PMSG)&memberFxn},                  \

    // @msgtype:定义消息类型 @ctrname:控件名称 @memberFxn:执行函数宏
#define DUI_ON_MSGTYPE_CTRNAME(msgtype,ctrname,memberFxn)                 \
	{ msgtype, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },                \

    // @ctrname:定义click消息的控件名称 @memberFxn:执行函数宏
#define DUI_ON_CLICK_CTRNAME(ctrname,memberFxn)                           \
	{ DUI_MSGTYPE_CLICK, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },      \

    // @ctrname:定义selectchanged消息的控件名称 @memberFxn:执行函数宏
#define DUI_ON_SELECTCHANGED_CTRNAME(ctrname,memberFxn)                   \
	{ DUI_MSGTYPE_SELECTCHANGED,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn }, \

    // @ctrname:定义killfocus消息的控件名称 @memberFxn:执行函数宏
#define DUI_ON_KILLFOCUS_CTRNAME(ctrname,memberFxn)                       \
	{ DUI_MSGTYPE_KILLFOCUS,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },     \

    // @ctrname:定义menu消息的控件名称 @memberFxn:执行函数宏
#define DUI_ON_MENU_CTRNAME(ctrname,memberFxn)                            \
	{ DUI_MSGTYPE_MENU,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },          \


    //定义与控件名称无关的消息宏

    //定义windowinit消息--执行函数宏
#define DUI_ON_WINDOWINIT()                                                    \
	{ DUI_MSGTYPE_WINDOWINIT, _T(""), DuiSig_vn,(DUI_PMSG)&OnWindowInit }, \

    //定义timer消息--执行函数宏
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