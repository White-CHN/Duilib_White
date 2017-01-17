#pragma once

namespace ControlAttributes
{



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
#define DATATYPE_ANIMATION		(_T("animation"))

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
        void Add(CDuiString strName = _T(""), CDuiString strType = _T(""), CDuiString strDefault = _T(""), CDuiString strRemarks = _T(""), CDuiString strValue = _T(""));
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


    class CScrollBar
        : public CControl
    {
    public:
        CScrollBar(void);
    };

    class CButton
        : public CLabel
    {
    public:
        CButton(void);
    };

    class COption
        : public CButton
    {
    public:
        COption(void);
    };

    class CRadio
        : public COption
    {
    };

    class CEdit
        : public CLabel
    {
    public:
        CEdit(void);
    };

    class CProgress :
        public CLabel
    {
    public:
        CProgress(void);
    };

    class CSlider :
        public CProgress
    {
    public:
        CSlider(void);
    };

    class  CContainer
        : public CControl
    {
    public:
        CContainer(void);
    };

    class  CListElement
        : public CControl
    {
    public:
        CListElement(void);
    };

    class  CListLabelElement : public CListElement
    {

    };

    class CListTextElement : public CListLabelElement
    {

    };

    class CDateTime :
        public CLabel
    {
    };

    class CIPAddress
        : public CLabel
    {
    };

    class CRing
        : public CLabel
    {
    };

    class CGif
        : public CControl
    {
    public:
        CGif(void);
    };

    class CColorPalette
        : public CControl
    {
    public:
        CColorPalette(void);
    };

    class CHorizontalLayout
        : public CContainer
    {
    public:
        CHorizontalLayout(void);
    };

    class CVerticalLayout
        : public CContainer
    {
    public:
        CVerticalLayout(void);
    };

    class CTabLayout
        : public CContainer
    {
    public:
        CTabLayout(void);
    };

    class CAnimationTabLayout
        : public CTabLayout
    {
    public:
        CAnimationTabLayout(void);
    };

    class CCombo
        : public CContainer
    {
    public:
        CCombo(void);
    };



    class CGroupBox
        : public CVerticalLayout
    {
    public:
        CGroupBox(void);
    };

    class  CList
        : public CVerticalLayout
    {
    public:
        CList(void);
    };

    class CMenu
        : public CList
    {
    };

    class CListHeader
        : public CHorizontalLayout
    {
    public:
        CListHeader(void);
    };

    class  CListHeaderItem
        : public CContainer
    {
    public:
        CListHeaderItem(void);
    };

    class CListContainerElement
        : public CHorizontalLayout
    {
    public:
        CListContainerElement(void);
    };

    class CMenuElement
        : public CListContainerElement
    {
    public:
        CMenuElement(void);
    };
    class CControlsName
    {
    public:
        CControlsName(void);
        ~CControlsName(void);
    public:
        map<CDuiString, IControl*> m_mapControlsName;
    };


}
