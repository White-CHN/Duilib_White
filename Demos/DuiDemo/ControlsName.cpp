#include "stdafx.h"
#include "ControlsName.h"





CImage::CImage(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("ͼƬ����");
    m_vtAttributes.push_back(Attribute);


    Attribute.m_strName = _T("restype");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("����Դ����,����res��restype����,��Ҫ����file����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("mask");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("����֧��alphaͨ����ͼƬ��ʽ����bmp��ָ��͸��ɫ,��#FF0000");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("���пؼ�����");
    m_vtAttributes.push_back(Attribute);
}

///////////////////////////////////////////////

CFont::CFont(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("id");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("-1");
    Attribute.m_strRemarks = _T("����id,ͬһ�����ڱ���Ψһ");
    m_vtAttributes.push_back(Attribute);


    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("��������,��΢���ź�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("size");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("12");
    Attribute.m_strRemarks = _T("�����С");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bold");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("underline");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�����»���");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("italic");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("б��");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("default");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("Ĭ������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("���пؼ�����");
    m_vtAttributes.push_back(Attribute);

}

//////////////////////////////////////////////////////////

CDefault::CDefault(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("�ؼ���������,��Menu,Button");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("value");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("�ؼ������б�,����֮��Ҫ�пո���'size=0,0' bkcolor=&quot;0xFFFF0000&quot;");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("���пؼ�����");
    m_vtAttributes.push_back(Attribute);
}

////////////////////////////////////////
CStyle::CStyle(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("��ʽ����,��btn_style");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("value");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("�ؼ������б�,����֮��Ҫ�пո�,��'size=0,0' bkcolor=&quot;0xFFFF0000&quot;");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("���пؼ�����");
    m_vtAttributes.push_back(Attribute);
}

/////////////////////////////////////////////////
CInclude::CInclude(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("source");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("XML�����ļ�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("count");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("1");
    Attribute.m_strRemarks = _T("Ƕ��Ĵ���");
    m_vtAttributes.push_back(Attribute);
}
//////////////////////////////////////////////

CWindow::CWindow(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("size");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("���ڵĳ�ʼ����Ⱥ͸߶ȣ���(800,600)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("sizebox");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("���ڿ��϶��ı䴰�ڴ�С�ı߾���(���ϣ��ң���),��(4,4,6,6)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("caption");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("���ڿ��϶��ı�������С���ڱ߾࣬��(0,0,0,28)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("roundcorner");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("����Բ�Ǵ�С,��(4,4)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("mininfo");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("������С��С,��(320,240)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("maxinfo");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("��������С,��(1600,1200)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("showdirty");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("���ڻ��ƺ�ɫ�߿�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("alpha");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("���ڵ�alphaֵ(0-255)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("layeredopacity");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("�ֲ㴰�ڵ�alphaֵ(0-255)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bktrans");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�����Ƿ�ʹ�þ�̬͸������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("disabledfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFFA7A6AA");
    Attribute.m_strRemarks = _T("Ĭ�ϵ�disabled������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("defaultfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFF000000");
    Attribute.m_strRemarks = _T("Ĭ�ϵ�������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("linkfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFF0000FF");
    Attribute.m_strRemarks = _T("Ĭ�ϵ�link������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("linkhoverfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFFD3215F");
    Attribute.m_strRemarks = _T("Ĭ�ϵ�linkhoverfont������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("selectedcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFFBAE4FF");
    Attribute.m_strRemarks = _T("Ĭ�ϵ�selected������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("showshadow");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ����ô�����Ӱ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowimage");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("��ӰͼƬ,ʹ�ô����Ժ��Զ������㷨��Ӱ(��֧��source����������)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowcorner");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,");
    Attribute.m_strRemarks = _T("ͼƬ��Ӱ�ľŹ�������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowsize");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�㷨��Ӱ�Ŀ��(-20��20)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowsharpness");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("�㷨��Ӱ�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowdarkness");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("�㷨��Ӱ�����(�൱��͸����)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowposition");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("�㷨��Ӱ��ƫ����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x000000");
    Attribute.m_strRemarks = _T("�㷨��Ӱ����ɫ,RGB��ʽ,��֧��͸����,ʹ��shadowdarkness����͸����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("gdiplustext");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ���gdi+��Ⱦ����(�����Ч�����ã�Ĭ�Ͽ���������Ч�ʵ�)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("textrenderinghint");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("gdi+��Ⱦ������ʾ(0-5),������ʱ���������Ϊ4");
    m_vtAttributes.push_back(Attribute);

}
//////////////////////////////////////////

CControl::CControl(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("pos");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("λ��,���Ϊfloat�ؼ���ָ��λ�úʹ�С,����ָֻ����С,��(0,0,100,100)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("float");
    Attribute.m_strType = DATATYPE_BOOL_RECT;
    Attribute.m_strDefault = _T("false��0,0,0,0");
    Attribute.m_strRemarks = _T("�Ƿ�ʹ�þ��Զ�λ��ָ��λ��,���Ϊfloat�ؼ���ָ��λ�úʹ�С,����ָֻ����С,��(0,0,100,100)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("floatalign");
    Attribute.m_strType = DATATYPE_FLOAT_ALIGN;
    Attribute.m_strDefault = _T("null");
    Attribute.m_strRemarks = _T("���Զ�λ���뷽ʽ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("padding");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("��߾�,��(2,2,2,2)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor1");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("��������ɫ1");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor2");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("��������ɫ2");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor3");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("��������ɫ3");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("forecolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("�����ı�ʱ��ǰ��ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bordercolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("�߿���ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("focusbordercolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("��ý���ʱ�߿����ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("colorhsl");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("���ؼ�����ɫ�Ƿ��洰�ڵ�hsl�仯���仯");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bordersize");
    Attribute.m_strType = DATATYPE_INT_RECT;
    Attribute.m_strDefault = _T("0��0,0,0,0");
    Attribute.m_strRemarks = _T("�߿��С");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("leftbordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("��߱߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("rightbordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ұ߱߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("topbordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�����߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bottombordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ײ��߿��С,��(1),���ø�ֵ����0,�򽫺���bordersize���Ե�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("borderstyle");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�߿���ʽ������,��ֵ��Χ0-5");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("borderround");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("�߿�Բ�ǰ뾶,��(2,2)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkimage");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("����ͼƬ,��(bk.bmp��file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("foreimage");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("ǰ��ͼƬ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("width");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ��Ŀ��");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("height");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ��ĸ߶�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("minwidth");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ�����С���");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("minheight");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ�����С�߶�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("maxwidth");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ��������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("maxheight");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("�ؼ�Ԥ������߶�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("�ؼ�����,ͬһ�����ڱ���Ψһ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("drag");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("��ק");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("resourcetext");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ�֧�ֶ�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("text");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("�ı�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("tooltip");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("���������ʾ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("userdata");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("�û��Զ�������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("enabled");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("�Ƿ������Ӧ�û�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("mouse");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("���ؼ��Ƿ������Ӧ������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("keyboard");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("��Button����Ը�ֵ��Ϊfalseʱ��֧��TAB_STOP,�Ҹö��󲻴��������Ϣ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("visible");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("�Ƿ�ɼ�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shortcut");
    Attribute.m_strType = DATATYPE_CHAR;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("��Ӧ�Ŀ�ݼ�,��(P)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("menu");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ���Ҫ�Ҽ��˵�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("cursor");
    Attribute.m_strType = DATATYPE_CURSOR;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("���������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("virtualwnd");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("���ÿؼ������ⴰ�������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("innerstyle");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("ͨ����ʽ�����ÿؼ�����ʽ");
    m_vtAttributes.push_back(Attribute);
}
/////////////////////////////////////////

CLabel::CLabel(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("align");
    Attribute.m_strType = DATATYPE_ALIGN;
    Attribute.m_strDefault = _T("left");
    Attribute.m_strRemarks = _T("���ֺ�����뷽ʽ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("valign");
    Attribute.m_strType = DATATYPE_VALIGN;
    Attribute.m_strDefault = _T("vcenter");
    Attribute.m_strRemarks = _T("����������뷽ʽ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("endellipsis");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("��ĩ��ʾ��ȫ�Ƿ�ʹ��...����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("wordbreak");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ�֧�ֶ�������");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("noprefix");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ�֧��'&'�ַ�ת��Ϊ���»���");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("font");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("-1");
    Attribute.m_strRemarks = _T("����id");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("textcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("������ɫ��0��ʾʹ��Ĭ��������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("disabledtextcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("disable������ɫ��0��ʾʹ��Ĭ��������ɫ");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("textpadding");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("������ʾ�ı߾�");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("showhtml");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ�ʹ����html���ı�����");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("autocalcwidth");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("�Ƿ��Զ�������");
    m_vtAttributes.push_back(Attribute);
}

/////////////////////////////////////////////
CControlsName::CControlsName(void)
{
    m_mapControlsName[DUI_WINDOW]		= new CWindow;
    m_mapControlsName[DUI_IMAGE]		= new CImage;
    m_mapControlsName[DUI_FONT]			= new CFont;
    m_mapControlsName[DUI_DEFAULT]		= new CDefault;
    m_mapControlsName[DUI_STYLE]		= new CStyle;
    m_mapControlsName[DUI_INCLUDE]		= new CInclude;
    m_mapControlsName[DUI_CTR_CONTROL]	= new CControl;
    m_mapControlsName[DUI_CTR_LABEL]	= new CLabel;
    //m_vtControlsName.push_back();
    //m_vtControlsName.push_back(DUI_CTR_SCROLLBAR);
    //m_vtControlsName.push_back(DUI_CTR_BUTTON);
    //m_vtControlsName.push_back(DUI_CTR_OPTION);
    //m_vtControlsName.push_back(DUI_CTR_RADIO);
    //m_vtControlsName.push_back(DUI_CTR_EDIT);
    //m_vtControlsName.push_back(DUI_CTR_PROGRESS);
    //m_vtControlsName.push_back(DUI_CTR_SLIDER);
    //m_vtControlsName.push_back(DUI_CTR_LISTELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_LISTLABELELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_LISTTEXTELEMENT);
    //m_vtControlsName.push_back(DUI_CTR_DATETIME);
    //m_vtControlsName.push_back(DUI_CTR_IPADDRESS);
    //m_vtControlsName.push_back(DUI_CTR_RING);
    //m_vtControlsName.push_back(DUI_CTR_GIF);
    //m_vtControlsName.push_back(DUI_CTR_COLORPALETTE);

    //m_vtControlsName.push_back(DUI_CTR_CONTAINER);
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




