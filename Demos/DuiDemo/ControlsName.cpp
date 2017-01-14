#include "stdafx.h"
#include "ControlsName.h"





CImage::CImage(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("图片名称");
    m_vtAttributes.push_back(Attribute);


    Attribute.m_strName = _T("restype");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("从资源加载,设置res和restype属性,不要设置file属性");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("mask");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("给不支持alpha通道的图片格式（如bmp）指定透明色,如#FF0000");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("所有控件共享");
    m_vtAttributes.push_back(Attribute);
}

///////////////////////////////////////////////

CFont::CFont(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("id");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("-1");
    Attribute.m_strRemarks = _T("字体id,同一窗口内必须唯一");
    m_vtAttributes.push_back(Attribute);


    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("字体名称,如微软雅黑");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("size");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("12");
    Attribute.m_strRemarks = _T("字体大小");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bold");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("字体粗体");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("underline");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("字体下划线");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("italic");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("斜体");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("default");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("默认字体");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("所有控件共享");
    m_vtAttributes.push_back(Attribute);

}

//////////////////////////////////////////////////////////

CDefault::CDefault(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("控件类型名称,如Menu,Button");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("value");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("控件属性列表,属性之间要有空格，如'size=0,0' bkcolor=&quot;0xFFFF0000&quot;");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("所有控件共享");
    m_vtAttributes.push_back(Attribute);
}

////////////////////////////////////////
CStyle::CStyle(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("样式名称,如btn_style");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("value");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("控件属性列表,属性之间要有空格,如'size=0,0' bkcolor=&quot;0xFFFF0000&quot;");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shared");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("所有控件共享");
    m_vtAttributes.push_back(Attribute);
}

/////////////////////////////////////////////////
CInclude::CInclude(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("source");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("XML布局文件");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("count");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("1");
    Attribute.m_strRemarks = _T("嵌入的次数");
    m_vtAttributes.push_back(Attribute);
}
//////////////////////////////////////////////

CWindow::CWindow(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("size");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("窗口的初始化宽度和高度，如(800,600)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("sizebox");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("窗口可拖动改变窗口大小的边距宽度(左，上，右，下),如(4,4,6,6)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("caption");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("窗口可拖动的标题栏大小的内边距，如(0,0,0,28)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("roundcorner");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("窗口圆角大小,如(4,4)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("mininfo");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("窗口最小大小,如(320,240)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("maxinfo");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("窗口最大大小,如(1600,1200)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("showdirty");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("窗口绘制红色边框");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("alpha");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("窗口的alpha值(0-255)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("layeredopacity");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("分层窗口的alpha值(0-255)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bktrans");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("窗口是否使用静态透明背景");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("disabledfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFFA7A6AA");
    Attribute.m_strRemarks = _T("默认的disabled字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("defaultfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFF000000");
    Attribute.m_strRemarks = _T("默认的字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("linkfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFF0000FF");
    Attribute.m_strRemarks = _T("默认的link字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("linkhoverfontcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFFD3215F");
    Attribute.m_strRemarks = _T("默认的linkhoverfont字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("selectedcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0xFFBAE4FF");
    Attribute.m_strRemarks = _T("默认的selected字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("showshadow");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否启用窗体阴影");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowimage");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("阴影图片,使用此属性后自动屏蔽算法阴影(不支持source等属性设置)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowcorner");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,");
    Attribute.m_strRemarks = _T("图片阴影的九宫格描述");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowsize");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("算法阴影的宽度(-20到20)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowsharpness");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("算法阴影的锐度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowdarkness");
    Attribute.m_strType = DATATYPE_BYTE;
    Attribute.m_strDefault = _T("255");
    Attribute.m_strRemarks = _T("算法阴影的深度(相当于透明度)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowposition");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("算法阴影的偏移量");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shadowcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x000000");
    Attribute.m_strRemarks = _T("算法阴影的颜色,RGB格式,不支持透明度,使用shadowdarkness设置透明度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("gdiplustext");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否用gdi+渲染文字(抗锯齿效果更好，默认开启，但是效率低)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("textrenderinghint");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("gdi+渲染文字提示(0-5),字体大的时候可以设置为4");
    m_vtAttributes.push_back(Attribute);

}
//////////////////////////////////////////

CControl::CControl(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("pos");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("位置,如果为float控件则指定位置和大小,否则只指定大小,如(0,0,100,100)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("float");
    Attribute.m_strType = DATATYPE_BOOL_RECT;
    Attribute.m_strDefault = _T("false或0,0,0,0");
    Attribute.m_strRemarks = _T("是否使用绝对定位或指定位置,如果为float控件则指定位置和大小,否则只指定大小,如(0,0,100,100)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("floatalign");
    Attribute.m_strType = DATATYPE_FLOAT_ALIGN;
    Attribute.m_strDefault = _T("null");
    Attribute.m_strRemarks = _T("绝对定位对齐方式");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("padding");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("外边距,如(2,2,2,2)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("背景颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor1");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("背景渐变色1");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor2");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("背景渐变色2");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkcolor3");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("背景渐变色3");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("forecolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("设置文本时的前景色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bordercolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("边框颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("focusbordercolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("获得焦点时边框的颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("colorhsl");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("本控件的颜色是否随窗口的hsl变化而变化");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bordersize");
    Attribute.m_strType = DATATYPE_INT_RECT;
    Attribute.m_strDefault = _T("0或0,0,0,0");
    Attribute.m_strRemarks = _T("边框大小");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("leftbordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("左边边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("rightbordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("右边边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("topbordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("顶部边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bottombordersize");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("底部边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("borderstyle");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("边框样式的设置,数值范围0-5");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("borderround");
    Attribute.m_strType = DATATYPE_SIZE;
    Attribute.m_strDefault = _T("0,0");
    Attribute.m_strRemarks = _T("边框圆角半径,如(2,2)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("bkimage");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("foreimage");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("前景图片");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("width");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("控件预设的宽度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("height");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("控件预设的高度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("minwidth");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("控件预设的最小宽度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("minheight");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("控件预设的最小高度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("maxwidth");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("控件预设的最大宽度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("maxheight");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("0");
    Attribute.m_strRemarks = _T("控件预设的最大高度");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("name");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("控件名称,同一窗口内必须唯一");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("drag");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("拖拽");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("resourcetext");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否支持多语言");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("text");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("文本内容");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("tooltip");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("鼠标悬浮提示");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("userdata");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("用户自定义数据");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("enabled");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("是否可以响应用户操作");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("mouse");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("本控件是否可以响应鼠标操作");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("keyboard");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("非Button类忽略该值，为false时不支持TAB_STOP,且该对象不处理键盘信息");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("visible");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("true");
    Attribute.m_strRemarks = _T("是否可见");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("shortcut");
    Attribute.m_strType = DATATYPE_CHAR;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("对应的快捷键,如(P)");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("menu");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否需要右键菜单");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("cursor");
    Attribute.m_strType = DATATYPE_CURSOR;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("鼠标光标类型");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("virtualwnd");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("设置控件的虚拟窗体的名字");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("innerstyle");
    Attribute.m_strType = DATATYPE_STRING;
    Attribute.m_strDefault = _T("");
    Attribute.m_strRemarks = _T("通过样式表设置控件的样式");
    m_vtAttributes.push_back(Attribute);
}
/////////////////////////////////////////

CLabel::CLabel(void)
{
    CAttribute Attribute;

    Attribute.m_strName = _T("align");
    Attribute.m_strType = DATATYPE_ALIGN;
    Attribute.m_strDefault = _T("left");
    Attribute.m_strRemarks = _T("文字横向对齐方式");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("valign");
    Attribute.m_strType = DATATYPE_VALIGN;
    Attribute.m_strDefault = _T("vcenter");
    Attribute.m_strRemarks = _T("文字纵向对齐方式");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("endellipsis");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("句末显示不全是否使用...代替");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("wordbreak");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否支持多行文字");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("noprefix");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否支持'&'字符转义为带下划线");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("font");
    Attribute.m_strType = DATATYPE_INT;
    Attribute.m_strDefault = _T("-1");
    Attribute.m_strRemarks = _T("字体id");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("textcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("字体颜色，0表示使用默认字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("disabledtextcolor");
    Attribute.m_strType = DATATYPE_DWORD;
    Attribute.m_strDefault = _T("0x00000000");
    Attribute.m_strRemarks = _T("disable字体颜色，0表示使用默认字体颜色");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("textpadding");
    Attribute.m_strType = DATATYPE_RECT;
    Attribute.m_strDefault = _T("0,0,0,0");
    Attribute.m_strRemarks = _T("文字显示的边距");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("showhtml");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否使用类html富文本绘制");
    m_vtAttributes.push_back(Attribute);

    Attribute.m_strName = _T("autocalcwidth");
    Attribute.m_strType = DATATYPE_BOOL;
    Attribute.m_strDefault = _T("false");
    Attribute.m_strRemarks = _T("是否自动计算宽度");
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




