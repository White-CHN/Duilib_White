#pragma once


#define DATATYPE_CHAR			(_T("char"))
#define DATATYPE_INT			(_T("int"))
#define DATATYPE_INT_RECT		(_T("int或RECT"))
#define DATATYPE_STRING			(_T("string"))
#define DATATYPE_BOOL			(_T("bool"))
#define DATATYPE_SIZE			(_T("SIZE"))
#define DATATYPE_RECT			(_T("RECT"))
#define DATATYPE_BYTE			(_T("BYTE"))
#define DATATYPE_DWORD			(_T("DWORD"))
#define DATATYPE_BOOL_RECT		(_T("bool或RECT"))
#define DATATYPE_FLOAT_ALIGN	(_T("FloatAlign"))
#define DATATYPE_CURSOR			(_T("cursor"))
#define DATATYPE_ALIGN			(_T("align"))
#define DATATYPE_VALIGN			(_T("valign"))


class CAttribute
{
public:
    CDuiString m_strName;		// 属性字段
    CDuiString m_strValue;		// 值
    CDuiString m_strType;		// 类型
    CDuiString m_strDefault;	// 默认值
    CDuiString m_strRemarks;	// 备注
};

class IControl
{
public:
    vector<CAttribute> m_vtAttributes;
};

class CImage
    : public IControl
{
public:
    CImage(void);
};

class CFont
    : public IControl
{
public:
    CFont(void);
};

class CDefault
    : public IControl
{
public:
    CDefault(void);
};

class CStyle
    : public IControl
{
public:
    CStyle(void);
};

class CInclude
    : public IControl
{
public:
    CInclude(void);
};

class CWindow
    : public IControl
{
public:
    CWindow(void);
};

class CControl
    : public IControl
{
public:
    CControl(void);
};

class CLabel
    : public CControl
{
public:
    CLabel(void);
};

class CControlsName
{
public:
    CControlsName(void);
    ~CControlsName(void);
public:
    map<CDuiString, IControl*> m_mapControlsName;
};



