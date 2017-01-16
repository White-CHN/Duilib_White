#include "stdafx.h"
#include "ControlsName.h"

namespace ControlAttributes
{

    void IControl::Add(CDuiString strName /*= _T("")*/, CDuiString strType /*= _T("")*/, CDuiString strDefault /*= _T("")*/, CDuiString strRemarks /*= _T("")*/, CDuiString strValue /*= _T("")*/)
    {
        CAttribute Attribute;

        Attribute.m_strName = strName;
        Attribute.m_strType = strType;
        Attribute.m_strDefault = strDefault;
        Attribute.m_strRemarks = strRemarks;
        Attribute.m_strValue = strValue;
        m_vtAttributes.push_back(Attribute);
    }


////////////////////////////////////////////////////////////////////


    CImage::CImage(void)
    {
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("ͼƬ����"));
        Add(_T("restype"), DATATYPE_STRING, _T(""), _T("����Դ����,����res��restype����,��Ҫ����file����"));
        Add(_T("mask"), DATATYPE_INT, _T("0"), _T("����֧��alphaͨ����ͼƬ��ʽ����bmp��ָ��͸��ɫ,��#FF0000"));
        Add(_T("shared"), DATATYPE_BOOL, _T("false"), _T("���пؼ�����"));
    }

///////////////////////////////////////////////

    CFont::CFont(void)
    {
        Add(_T("id"), DATATYPE_INT, _T("-1"), _T("����id,ͬһ�����ڱ���Ψһ"));
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("��������,��΢���ź�"));
        Add(_T("size"), DATATYPE_INT, _T("12"), _T("�����С"));
        Add(_T("bold"), DATATYPE_BOOL, _T("false"), _T("�������"));
        Add(_T("underline"), DATATYPE_BOOL, _T("false"), _T("�����»���"));
        Add(_T("italic"), DATATYPE_BOOL, _T("false"), _T("б��"));
        Add(_T("default"), DATATYPE_BOOL, _T("false"), _T("Ĭ������"));
        Add(_T("shared"), DATATYPE_BOOL, _T("false"), _T("���пؼ�����"));
    }

//////////////////////////////////////////////////////////

    CDefault::CDefault(void)
    {
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("�ؼ���������,��Menu,Button"));
        Add(_T("value"), DATATYPE_STRING, _T(""), _T("�ؼ������б�,����֮��Ҫ�пո���'size=0,0' bkcolor=&quot;0xFFFF0000&quot;"));
        Add(_T("shared"), DATATYPE_BOOL , _T("false"), _T("���пؼ�����"));
    }

////////////////////////////////////////
    CStyle::CStyle(void)
    {
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("��ʽ����,��btn_style"));
        Add(_T("value"), DATATYPE_STRING, _T(""), _T("�ؼ������б�,����֮��Ҫ�пո�,��'size=0,0' bkcolor=&quot;0xFFFF0000&quot;"));
        Add(_T("shared"), DATATYPE_BOOL, _T("false"), _T("���пؼ�����"));
    }

/////////////////////////////////////////////////
    CInclude::CInclude(void)
    {
        Add(_T("source"), DATATYPE_STRING, _T(""), _T("XML�����ļ�"));
        Add(_T("count"), DATATYPE_INT, _T("1"), _T("Ƕ��Ĵ���"));
    }
//////////////////////////////////////////////

    CWindow::CWindow(void)
    {
        Add(_T("size"), DATATYPE_SIZE, _T("0,0"), _T("���ڵĳ�ʼ����Ⱥ͸߶ȣ���(800,600)"));
        Add(_T("sizebox"), DATATYPE_RECT, _T("0,0,0,0"), _T("���ڿ��϶��ı䴰�ڴ�С�ı߾���(���ϣ��ң���),��(4,4,6,6)"));
        Add(_T("caption"), DATATYPE_RECT, _T("0,0,0,0"), _T("���ڿ��϶��ı�������С���ڱ߾࣬��(0,0,0,28)"));
        Add(_T("roundcorner"), DATATYPE_SIZE , _T("0,0"), _T("����Բ�Ǵ�С,��(4,4)"));
        Add(_T("mininfo"), DATATYPE_SIZE, _T("0,0"), _T("������С��С,��(320,240)"));
        Add(_T("maxinfo"), DATATYPE_SIZE, _T("0,0"), _T("��������С,��(1600,1200)"));
        Add(_T("showdirty"), DATATYPE_BOOL, _T("false"), _T("���ڻ��ƺ�ɫ�߿�"));
        Add(_T("alpha"), DATATYPE_BYTE, _T("255"), _T("���ڵ�alphaֵ(0-255)"));
        Add(_T("layeredopacity"), DATATYPE_BYTE, _T("255"), _T("�ֲ㴰�ڵ�alphaֵ(0-255)"));
        Add(_T("bktrans"), DATATYPE_BOOL, _T("false"), _T("�����Ƿ�ʹ�þ�̬͸������"));
        Add(_T("disabledfontcolor"), DATATYPE_DWORD, _T("0xFFA7A6AA"), _T("Ĭ�ϵ�disabled������ɫ"));
        Add(_T("defaultfontcolor"), DATATYPE_DWORD, _T("0xFF000000"), _T("Ĭ�ϵ�������ɫ"));
        Add(_T("linkfontcolor"), DATATYPE_DWORD, _T("0xFF0000FF"), _T("Ĭ�ϵ�link������ɫ"));
        Add(_T("linkhoverfontcolor"), DATATYPE_DWORD, _T("0xFFD3215F"), _T("Ĭ�ϵ�linkhoverfont������ɫ"));
        Add(_T("selectedcolor"), DATATYPE_DWORD, _T("0xFFBAE4FF"), _T("Ĭ�ϵ�selected������ɫ"));
        Add(_T("showshadow"), DATATYPE_BOOL, _T("false"), _T("�Ƿ����ô�����Ӱ"));
        Add(_T("shadowimage"), DATATYPE_STRING , _T(""), _T("��ӰͼƬ,ʹ�ô����Ժ��Զ������㷨��Ӱ(��֧��source����������)"));
        Add(_T("shadowcorner"), DATATYPE_RECT, _T("0,0,0,0"), _T("ͼƬ��Ӱ�ľŹ�������"));
        Add(_T("shadowsize"), DATATYPE_BYTE, _T("0"), _T("�㷨��Ӱ�Ŀ��(-20��20)"));
        Add(_T("shadowsharpness"), DATATYPE_BYTE, _T("255"), _T("�㷨��Ӱ�����"));
        Add(_T("shadowdarkness"), DATATYPE_BYTE, _T("255"), _T("�㷨��Ӱ�����(�൱��͸����)"));
        Add(_T("shadowposition"), DATATYPE_SIZE , _T("0,0"), _T("�㷨��Ӱ��ƫ����"));
        Add(_T("shadowcolor"), DATATYPE_DWORD, _T("0x000000"), _T("�㷨��Ӱ����ɫ,RGB��ʽ,��֧��͸����,ʹ��shadowdarkness����͸����"));
        Add(_T("gdiplustext"), DATATYPE_BOOL, _T("false"), _T("�Ƿ���gdi+��Ⱦ����(�����Ч�����ã�Ĭ�Ͽ���������Ч�ʵ�)"));
        Add(_T("textrenderinghint"), DATATYPE_INT, _T("0"), _T("gdi+��Ⱦ������ʾ(0-5),������ʱ���������Ϊ4"));
    }
//////////////////////////////////////////

    CControl::CControl(void)
    {
        Add(_T("pos"), DATATYPE_RECT, _T("0,0,0,0"), _T("λ��,���Ϊfloat�ؼ���ָ��λ�úʹ�С,����ָֻ����С,��(0,0,100,100)"));
        Add(_T("float"), DATATYPE_BOOL_RECT, _T("false��0,0,0,0"), _T("�Ƿ�ʹ�þ��Զ�λ��ָ��λ��,���Ϊfloat�ؼ���ָ��λ�úʹ�С,����ָֻ����С,��(0,0,100,100)"));
        Add(_T("floatalign"), DATATYPE_FLOAT_ALIGN, _T("null"), _T("���Զ�λ���뷽ʽ"));
        Add(_T("padding"), DATATYPE_RECT, _T("0,0,0,0"), _T("��߾�,��(2,2,2,2)"));
        Add(_T("bkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("������ɫ"));
        Add(_T("bkcolor1"), DATATYPE_DWORD, _T("0x00000000"), _T("��������ɫ1"));
        Add(_T("bkcolor2"), DATATYPE_DWORD, _T("0x00000000"), _T("��������ɫ2"));
        Add(_T("bkcolor3"), DATATYPE_DWORD, _T("0x00000000"), _T("��������ɫ3"));
        Add(_T("forecolor"), DATATYPE_DWORD, _T("0x00000000"), _T("�����ı�ʱ��ǰ��ɫ"));
        Add(_T("bordercolor"), DATATYPE_DWORD, _T("0x00000000"), _T("�߿���ɫ"));
        Add(_T("focusbordercolor"), DATATYPE_DWORD, _T("0x00000000"), _T("��ý���ʱ�߿����ɫ"));
        Add(_T("colorhsl"), DATATYPE_BOOL, _T("false"), _T("���ؼ�����ɫ�Ƿ��洰�ڵ�hsl�仯���仯"));
        Add(_T("bordersize"), DATATYPE_INT_RECT, _T("0��0,0,0,0"), _T("�߿��С"));
        Add(_T("leftbordersize"), DATATYPE_INT, _T("0"), _T("��߱߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����"));
        Add(_T("rightbordersize"), DATATYPE_INT, _T("0"), _T("�ұ߱߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����"));
        Add(_T("topbordersize"), DATATYPE_INT, _T("0"), _T("�����߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����"));
        Add(_T("bottombordersize"), DATATYPE_INT, _T("0"), _T("�ײ��߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����"));
        Add(_T("borderstyle"), DATATYPE_INT, _T("0"), _T("�߿���ʽ������,��ֵ��Χ0-5"));
        Add(_T("borderround"), DATATYPE_SIZE, _T("0,0"), _T("�߿�Բ�ǰ뾶,��(2,2)"));
        Add(_T("bkimage"), DATATYPE_STRING, _T(""), _T("����ͼƬ,��(bk.bmp��file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"));
        Add(_T("foreimage"), DATATYPE_STRING, _T(""), _T("ǰ��ͼƬ"));
        Add(_T("width"), DATATYPE_INT, _T("0"), _T("�ؼ�Ԥ��Ŀ��"));
        Add(_T("height"), DATATYPE_INT, _T("0"), _T("�ؼ�Ԥ��ĸ߶�"));
        Add(_T("minwidth"), DATATYPE_INT, _T("0"), _T("�ؼ�Ԥ�����С���"));
        Add(_T("minheight"), DATATYPE_INT, _T("0"), _T("�ؼ�Ԥ�����С�߶�"));
        Add(_T("maxwidth"), DATATYPE_INT, _T("0"), _T("�ؼ�Ԥ��������"));
        Add(_T("maxheight"), DATATYPE_INT, _T("0"), _T("�ؼ�Ԥ������߶�"));
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("�ؼ�����,ͬһ�����ڱ���Ψһ"));
        Add(_T("drag"), DATATYPE_BOOL , _T("false"), _T("��ק"));
        Add(_T("resourcetext"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�֧�ֶ�����"));
        Add(_T("text"), DATATYPE_STRING, _T(""), _T("�ı�����"));
        Add(_T("tooltip"), DATATYPE_STRING, _T(""), _T("���������ʾ"));
        Add(_T("userdata"), DATATYPE_STRING, _T(""), _T("�û��Զ�������"));
        Add(_T("enabled"), DATATYPE_BOOL, _T("true"), _T("�Ƿ������Ӧ�û�����"));
        Add(_T("mouse"), DATATYPE_BOOL, _T("true"), _T("���ؼ��Ƿ������Ӧ������"));
        Add(_T("keyboard"), DATATYPE_BOOL, _T("true"), _T("��Button����Ը�ֵ��Ϊfalseʱ��֧��TAB_STOP,�Ҹö��󲻴��������Ϣ"));
        Add(_T("visible"), DATATYPE_BOOL, _T("true"), _T("�Ƿ�ɼ�"));
        Add(_T("shortcut"), DATATYPE_CHAR, _T(""), _T("��Ӧ�Ŀ�ݼ�,��(P)"));
        Add(_T("menu"), DATATYPE_BOOL, _T("false"), _T("�Ƿ���Ҫ�Ҽ��˵�"));
        Add(_T("cursor"), DATATYPE_CURSOR, _T(""), _T("���������"));
        Add(_T("virtualwnd"), DATATYPE_STRING, _T(""), _T("���ÿؼ������ⴰ�������"));
        Add(_T("innerstyle"), DATATYPE_STRING, _T(""), _T("ͨ����ʽ�����ÿؼ�����ʽ"));
    }
/////////////////////////////////////////

    CLabel::CLabel(void)
    {
        Add(_T("align"), DATATYPE_ALIGN, _T("left"), _T("���ֺ�����뷽ʽ"));
        Add(_T("valign"), DATATYPE_VALIGN, _T("vcenter"), _T("����������뷽ʽ"));
        Add(_T("endellipsis"), DATATYPE_BOOL, _T("false"), _T("��ĩ��ʾ��ȫ�Ƿ�ʹ��...����"));
        Add(_T("wordbreak"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�֧�ֶ�������"));
        Add(_T("noprefix"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�֧��'&'�ַ�ת��Ϊ���»���"));
        Add(_T("font"), DATATYPE_INT, _T("-1"), _T("����id"));
        Add(_T("textcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("������ɫ,0��ʾʹ��Ĭ��������ɫ"));
        Add(_T("disabledtextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("disable������ɫ,0��ʾʹ��Ĭ��������ɫ"));
        Add(_T("textpadding"), DATATYPE_RECT, _T("0,0,0,0"), _T("������ʾ�ı߾�"));
        Add(_T("showhtml"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ʹ����html���ı�����"));
        Add(_T("�Ƿ�ʹ����html���ı�����"), DATATYPE_BOOL, _T("false"), _T("�Ƿ��Զ�������"));
    }

////////////////////////////////////////////////
    CScrollBar::CScrollBar(void)
    {
        Add(_T("button1normalimage"), DATATYPE_STRING, _T(""), _T("����ϰ�ť��ͨ״̬ͼƬ"));
        Add(_T("button1hotimage"),  DATATYPE_STRING, _T(""), _T("����ϰ�ť�������״̬ͼƬ"));
        Add(_T("button1pushedimage"), DATATYPE_STRING, _T(""), _T("����ϰ�ť��갴��״̬ͼƬ"));
        Add(_T("button1disabledimage"), DATATYPE_STRING, _T(""), _T("����ϰ�ť����״̬ͼƬ"));
        Add(_T("button2normalimage"),  DATATYPE_STRING, _T(""), _T("�һ��°�ť��ͨ״̬ͼƬ"));
        Add(_T("button2hotimage"), DATATYPE_STRING, _T(""), _T("�һ��°�ť�������״̬ͼƬ"));
        Add(_T("button2pushedimage"),  DATATYPE_STRING, _T(""), _T("�һ��°�ť��갴��״̬ͼƬ"));
        Add(_T("button2disabledimage"), DATATYPE_STRING, _T(""), _T("�һ��°�ť����״̬ͼƬ"));
        Add(_T("thumbnormalimage"), DATATYPE_STRING, _T(""), _T("������ͨ״̬ͼƬ"));
        Add(_T("thumbhotimage"), DATATYPE_STRING, _T(""), _T("�����������״̬ͼƬ"));
        Add(_T("thumbpushedimage"), DATATYPE_STRING, _T(""), _T("������갴��״̬ͼƬ"));
        Add(_T("thumbdisabledimage"),  DATATYPE_STRING, _T(""), _T("�������״̬ͼƬ"));
        Add(_T("railnormalimage"), DATATYPE_STRING, _T(""), _T("�����м��ʶ��ͨ״̬ͼƬ"));
        Add(_T("railhotimage"),  DATATYPE_STRING, _T(""), _T("�����м��ʶ�������״̬ͼƬ"));
        Add(_T("railpushedimage"),  DATATYPE_STRING, _T(""), _T("�����м��ʶ��갴��״̬ͼƬ"));
        Add(_T("raildisabledimage"),  DATATYPE_STRING, _T(""), _T("�����м��ʶ����״̬ͼƬ"));
        Add(_T("bknormalimage"), DATATYPE_STRING, _T(""), _T("������ͨ״̬ͼƬ"));
        Add(_T("bkhotimage"),  DATATYPE_STRING, _T(""), _T("�����������״̬ͼƬ"));
        Add(_T("bkpushedimage"), DATATYPE_STRING, _T(""), _T("������갴��״̬ͼƬ"));
        Add(_T("bkdisabledimage"), DATATYPE_STRING, _T(""), _T("��������״̬ͼƬ"));
        Add(_T("hor"), DATATYPE_BOOL, _T("false"), _T("ˮƽ��ֱ"));
        Add(_T("linesize"), DATATYPE_INT, _T("8"), _T("����һ�еĴ�С"));
        Add(_T("range"), DATATYPE_INT, _T("100"), _T("������Χ"));
        Add(_T("value"), DATATYPE_INT, _T("0"), _T("����λ��"));
        Add(_T("showbutton1"), DATATYPE_BOOL, _T("true"), _T("�Ƿ���ʾ����ϰ�ť"));
        Add(_T("showbutton2"), DATATYPE_BOOL, _T("true"), _T("�Ƿ���ʾ�һ��°�ť"));
    }

////////////////////////////////////////////////
    CButton::CButton(void)
    {
        Add(_T("normalimage"), DATATYPE_STRING, _T(""), _T("��ͨ״̬ͼƬ"));
        Add(_T("hotimage"), DATATYPE_STRING, _T(""), _T("���������״̬ͼƬ"));
        Add(_T("pushedimage"), DATATYPE_STRING, _T(""), _T("��갴�µ�״̬ͼƬ"));
        Add(_T("focusedimage"), DATATYPE_STRING, _T(""), _T("��ý���ʱ��״̬ͼƬ"));
        Add(_T("disabledimage"), DATATYPE_STRING, _T(""), _T("���õ�״̬ͼƬ"));
        Add(_T("hotforeimage"), DATATYPE_STRING, _T(""), _T("��ý���ʱ��״̬ͼƬ"));
        Add(_T("bindtabindex"), DATATYPE_INT, _T(""), _T("����󽫴�����TabLayout�ؼ��Ķ�Ӧ������"));
        Add(_T("bindtablayoutname"), DATATYPE_STRING, _T(""), _T("��TabLayout�ؼ�������"));
        Add(_T("hotbkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("�������������ɫ"));
        Add(_T("pushedbkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("��갴��������ɫ"));
        Add(_T("hottextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("�������������ɫ"));
        Add(_T("pushedtextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("��갴�µ�������ɫ"));
        Add(_T("focusedtextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("��ý���ʱ��������ɫ"));
    }

//////////////////////////////////////////
    COption::COption(void)
    {
        Add(_T("group"), DATATYPE_STRING, _T(""), _T("�����������,�ɲ���"));
        Add(_T("selected"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ѡ��"));
        Add(_T("selectedimage"), DATATYPE_STRING, _T(""), _T("ѡ�е�״̬ͼƬ"));
        Add(_T("selectedhotimage"), DATATYPE_STRING, _T(""), _T("ѡ����������ĵ�״̬ͼƬ"));
        Add(_T("selectedpushedimage"), DATATYPE_STRING, _T(""), _T("ѡ����갴�µĵ�״̬ͼƬ"));
        Add(_T("selectedforeimage"), DATATYPE_STRING, _T(""), _T("ѡ�е�ǰ��ͼƬ"));
        Add(_T("selectedbkcolor"), DATATYPE_DWORD, _T(""), _T("ѡ��״̬������ɫ"));
        Add(_T("selectedtextcolor"), DATATYPE_DWORD, _T(""), _T("ѡ��״̬������ɫ"));
    }

/////////////////////////////////////////////

    CEdit::CEdit(void)
    {
        Add(_T("readonly"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ֻ��"));
        Add(_T("numberonly"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ֻ������������"));
        Add(_T("password"), DATATYPE_BOOL, _T("false"), _T("�Ƿ���ʾ�����ַ�"));
        Add(_T("passwordchar"), DATATYPE_BYTE, _T("*"), _T("���������ַ�"));
        Add(_T("maxchar"), DATATYPE_INT, _T("255"), _T("�����ַ���󳤶�"));
        Add(_T("normalimage"), DATATYPE_STRING, _T(""), _T("��ͨ״̬ͼƬ"));
        Add(_T("hotimage"), DATATYPE_STRING, _T(""), _T("�������״̬ͼƬ"));
        Add(_T("focusedimage"), DATATYPE_STRING, _T(""), _T("��ý���״̬ͼƬ"));
        Add(_T("disabledimage"), DATATYPE_STRING, _T(""), _T("����״̬ͼƬ"));
        Add(_T("tipvalue"), DATATYPE_STRING, _T(""), _T("�ı�������ʾ���֣����ı���textΪ��ʱ��ʾ��ʧȥ����ʱ��ʾ"));
        Add(_T("tipvaluecolor"), DATATYPE_DWORD, _T("0xFFBAC0C5"), _T("�ı�������ʾ���ֵ���ɫ"));
        Add(_T("nativetextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("windowsԭ��edit�ؼ���������ɫ"));
        Add(_T("nativebkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("windowsԭ��edit�ؼ��ı�����ɫ"));
    }
/////////////////////////////////////////////



    CProgress::CProgress(void)
    {
        Add(_T("hor"), DATATYPE_BOOL, _T("true"), _T("ˮƽ��ֱ"));
        Add(_T("min"), DATATYPE_INT, _T("0"), _T("������Сֵ"));
        Add(_T("max"), DATATYPE_INT, _T("100"), _T("�������ֵ"));
        Add(_T("value"), DATATYPE_INT, _T("0"), _T("����ֵ"));
        Add(_T("isstretchfore"), DATATYPE_BOOL, _T("true"), _T("ָ��ǰ��ͼƬ�Ƿ�������ʾ"));
        Add(_T("showtext"), DATATYPE_BOOL, _T("false"), _T("�Ƿ���ʾ�ı�����"));
    }
    //////////////////////////////////////////////
    CSlider::CSlider(void)
    {
        Add(_T("thumbimage"), DATATYPE_STRING, _T(""), _T("�϶�������ͨ״̬ͼƬ"));
        Add(_T("thumbhotimage"), DATATYPE_STRING, _T(""), _T("�϶������������״̬ͼƬ"));
        Add(_T("thumbpushedimage"), DATATYPE_STRING, _T(""), _T("�϶�������갴��״̬ͼƬ"));
        Add(_T("thumbsize"), DATATYPE_SIZE, _T("10,10"), _T("�϶������С"));
        Add(_T("step"), DATATYPE_INT, _T("1"), _T("���Ȳ���"));
        Add(_T("sendmove"), DATATYPE_BOOL, _T("false"), _T("�Ƿ���movevaluechanged��Ϣ"));
    }
    /////////////////////////////////////////////////
    CContainer::CContainer(void)
    {
        Add(_T("inset"), DATATYPE_RECT, _T("0,0,0,0"), _T("�������ڱ߾�"));
        Add(_T("mousechild"), DATATYPE_BOOL, _T("true"), _T("���ؼ����ӿؼ��Ƿ������Ӧ�û�����"));
        Add(_T("vscrollbar"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ʹ�����������"));
        Add(_T("vscrollbarstyle"), DATATYPE_STRING, _T(""), _T("���ñ��������������������ʽ"));
        Add(_T("hscrollbar"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ʹ�ú��������"));
        Add(_T("hscrollbarstyle"), DATATYPE_STRING, _T(""), _T("���ñ������ĺ������������ʽ"));
        Add(_T("childpadding"), DATATYPE_INT, _T("0"), _T("�ӿؼ�֮��Ķ������"));
        Add(_T("childalign"), DATATYPE_ALIGN, _T("left"), _T("�ӿؼ�������뷽ʽ"));
        Add(_T("childvalign"), DATATYPE_VALIGN, _T("top"), _T("�ӿؼ�������뷽ʽ"));
        Add(_T("scrollstepsize"), DATATYPE_INT, _T("8"), _T("�����Ĺ�������������"));
    }
    ////////////////////////////////////////////////////////////////
    CListElement::CListElement(void)
    {
        Add(_T("selected"), DATATYPE_BOOL, _T("false"), _T("�Ƿ�ѡ��"));
    }
    //////////////////////////////////////////////////////////

    CGif::CGif(void)
    {
        Add(_T("autoplay"), DATATYPE_BOOL, _T("true"), _T("�Ƿ��Զ�����GIF����"));
        Add(_T("autosize"), DATATYPE_BOOL, _T("false"), _T("�Ƿ����ͼƬ�Զ����ÿؼ���С(������width��height����ʧЧ)"));
    }



    ////////////////////////////////////////////

    CControlsName::CControlsName(void)
    {
        m_mapControlsName[DUI_WINDOW]				= new CWindow;
        m_mapControlsName[DUI_IMAGE]				= new CImage;
        m_mapControlsName[DUI_FONT]					= new CFont;
        m_mapControlsName[DUI_DEFAULT]				= new CDefault;
        m_mapControlsName[DUI_STYLE]				= new CStyle;
        m_mapControlsName[DUI_INCLUDE]				= new CInclude;
        m_mapControlsName[DUI_CTR_CONTROL]			= new CControl;
        m_mapControlsName[DUI_CTR_LABEL]			= new CLabel;
        m_mapControlsName[DUI_CTR_SCROLLBAR]		= new CScrollBar;
        m_mapControlsName[DUI_CTR_BUTTON]			= new CButton;
        m_mapControlsName[DUI_CTR_OPTION]			= new COption;
        m_mapControlsName[DUI_CTR_RADIO]			= new CRadio;
        m_mapControlsName[DUI_CTR_EDIT]				= new CEdit;
        m_mapControlsName[DUI_CTR_PROGRESS]			= new CProgress;
        m_mapControlsName[DUI_CTR_SLIDER]			= new CSlider;
        m_mapControlsName[DUI_CTR_CONTAINER]		= new CContainer;
        m_mapControlsName[DUI_CTR_LISTELEMENT]		= new CListElement;
        m_mapControlsName[DUI_CTR_LISTLABELELEMENT]	= new CListLabelElement;
        m_mapControlsName[DUI_CTR_LISTTEXTELEMENT]	= new CListTextElement;
        m_mapControlsName[DUI_CTR_DATETIME]			= new CDateTime;
        m_mapControlsName[DUI_CTR_IPADDRESS]		= new CIPAddress;
        m_mapControlsName[DUI_CTR_RING]				= new CRing;
        m_mapControlsName[DUI_CTR_GIF]				= new CGif;

        //m_vtControlsName.push_back(DUI_CTR_COLORPALETTE);

        //m_vtControlsName.push_back(DUI_CTR_VERTICALLAYOUT);
        //m_vtControlsName.push_back(DUI_CTR_HORIZONTALLAYOUT);
        //m_vtControlsName.push_back(DUI_CTR_TABLAYOUT);
        //m_vtControlsName.push_back(DUI_CTR_ANIMATION_TABLAYOUT);
        //m_vtControlsName.push_back(DUI_CTR_COMBO);
        //m_vtControlsName.push_back(DUI_CTR_GROUPBOX);
        //m_vtControlsName.push_back(DUI_CTR_LISTHEADERITEM);
        //m_vtControlsName.push_back(DUI_CTR_LISTHEADER);
        //m_vtControlsName.push_back(DUI_CTR_LIST);
        //m_vtControlsName.push_back(DUI_CTR_LISTCONTAINERELEMENT);
        //m_vtControlsName.push_back(DUI_CTR_MENU);
        //m_vtControlsName.push_back(DUI_CTR_MENUELEMENT);
    }



    CControlsName::~CControlsName(void)
    {
        for(map<CDuiString, IControl*>::iterator it = m_mapControlsName.begin(); it != m_mapControlsName.end();)
        {
            IControl* pControl = it->second;
            it = m_mapControlsName.erase(it);
            DUI_FREE_POINT(pControl);
        }
    }



}