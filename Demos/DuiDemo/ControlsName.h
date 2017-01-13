#pragma once


#define DATATYPE_INT		(_T("int"))
#define DATATYPE_STRING		(_T("string"))
#define DATATYPE_BOOL		(_T("bool"))

class CAttribute
{
public:
    CAttribute(void);
public:
    CDuiString m_strName;		// �����ֶ�
    CDuiString m_strValue;		// ֵ
    CDuiString m_strType;		// ����
    CDuiString m_strDefault;	// Ĭ��ֵ
    CDuiString m_strRemarks;	// ��ע
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



