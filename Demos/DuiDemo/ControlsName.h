#pragma once


#define DATATYPE_INT		(_T("int"))
#define DATATYPE_STRING		(_T("string"))
#define DATATYPE_BOOL		(_T("bool"))

class CAttribute
{
public:
    CAttribute(void);
public:
    CDuiString m_strName;		// 属性字段
    CDuiString m_strValue;		// 值
    CDuiString m_strType;		// 类型
    CDuiString m_strDefault;	// 默认值
    CDuiString m_strRemarks;	// 备注
};

class CControl
{
public:
    CControl(void);
public:
    vector<CAttribute> m_vtAttributes;
};

class CControlsName
{
public:
    CControlsName(void);
    ~CControlsName(void);
public:
    map<CDuiString, CControl*> m_mapControlsName;
};



