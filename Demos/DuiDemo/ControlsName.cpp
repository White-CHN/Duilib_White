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
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("图片名称"));
        Add(_T("restype"), DATATYPE_STRING, _T(""), _T("从资源加载,设置res和restype属性,不要设置file属性"));
        Add(_T("mask"), DATATYPE_INT, _T("0"), _T("给不支持alpha通道的图片格式（如bmp）指定透明色,如#FF0000"));
        Add(_T("shared"), DATATYPE_BOOL, _T("false"), _T("所有控件共享"));
    }

///////////////////////////////////////////////

    CFont::CFont(void)
    {
        Add(_T("id"), DATATYPE_INT, _T("-1"), _T("字体id,同一窗口内必须唯一"));
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("字体名称,如微软雅黑"));
        Add(_T("size"), DATATYPE_INT, _T("12"), _T("字体大小"));
        Add(_T("bold"), DATATYPE_BOOL, _T("false"), _T("字体粗体"));
        Add(_T("underline"), DATATYPE_BOOL, _T("false"), _T("字体下划线"));
        Add(_T("italic"), DATATYPE_BOOL, _T("false"), _T("斜体"));
        Add(_T("default"), DATATYPE_BOOL, _T("false"), _T("默认字体"));
        Add(_T("shared"), DATATYPE_BOOL, _T("false"), _T("所有控件共享"));
    }

//////////////////////////////////////////////////////////

    CDefault::CDefault(void)
    {
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("控件类型名称,如Menu,Button"));
        Add(_T("value"), DATATYPE_STRING, _T(""), _T("控件属性列表,属性之间要有空格，如'size=0,0' bkcolor=&quot;0xFFFF0000&quot;"));
        Add(_T("shared"), DATATYPE_BOOL , _T("false"), _T("所有控件共享"));
    }

////////////////////////////////////////
    CStyle::CStyle(void)
    {
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("样式名称,如btn_style"));
        Add(_T("value"), DATATYPE_STRING, _T(""), _T("控件属性列表,属性之间要有空格,如'size=0,0' bkcolor=&quot;0xFFFF0000&quot;"));
        Add(_T("shared"), DATATYPE_BOOL, _T("false"), _T("所有控件共享"));
    }

/////////////////////////////////////////////////
    CInclude::CInclude(void)
    {
        Add(_T("source"), DATATYPE_STRING, _T(""), _T("XML布局文件"));
        Add(_T("count"), DATATYPE_INT, _T("1"), _T("嵌入的次数"));
    }
//////////////////////////////////////////////

    CWindow::CWindow(void)
    {
        Add(_T("size"), DATATYPE_SIZE, _T("0,0"), _T("窗口的初始化宽度和高度，如(800,600)"));
        Add(_T("sizebox"), DATATYPE_RECT, _T("0,0,0,0"), _T("窗口可拖动改变窗口大小的边距宽度(左，上，右，下),如(4,4,6,6)"));
        Add(_T("caption"), DATATYPE_RECT, _T("0,0,0,0"), _T("窗口可拖动的标题栏大小的内边距，如(0,0,0,28)"));
        Add(_T("roundcorner"), DATATYPE_SIZE , _T("0,0"), _T("窗口圆角大小,如(4,4)"));
        Add(_T("mininfo"), DATATYPE_SIZE, _T("0,0"), _T("窗口最小大小,如(320,240)"));
        Add(_T("maxinfo"), DATATYPE_SIZE, _T("0,0"), _T("窗口最大大小,如(1600,1200)"));
        Add(_T("showdirty"), DATATYPE_BOOL, _T("false"), _T("窗口绘制红色边框"));
        Add(_T("alpha"), DATATYPE_BYTE, _T("255"), _T("窗口的alpha值(0-255)"));
        Add(_T("layeredopacity"), DATATYPE_BYTE, _T("255"), _T("分层窗口的alpha值(0-255)"));
        Add(_T("bktrans"), DATATYPE_BOOL, _T("false"), _T("窗口是否使用静态透明背景"));
        Add(_T("disabledfontcolor"), DATATYPE_DWORD, _T("0xFFA7A6AA"), _T("默认的disabled字体颜色"));
        Add(_T("defaultfontcolor"), DATATYPE_DWORD, _T("0xFF000000"), _T("默认的字体颜色"));
        Add(_T("linkfontcolor"), DATATYPE_DWORD, _T("0xFF0000FF"), _T("默认的link字体颜色"));
        Add(_T("linkhoverfontcolor"), DATATYPE_DWORD, _T("0xFFD3215F"), _T("默认的linkhoverfont字体颜色"));
        Add(_T("selectedcolor"), DATATYPE_DWORD, _T("0xFFBAE4FF"), _T("默认的selected字体颜色"));
        Add(_T("showshadow"), DATATYPE_BOOL, _T("false"), _T("是否启用窗体阴影"));
        Add(_T("shadowimage"), DATATYPE_STRING , _T(""), _T("阴影图片,使用此属性后自动屏蔽算法阴影(不支持source等属性设置)"));
        Add(_T("shadowcorner"), DATATYPE_RECT, _T("0,0,0,0"), _T("图片阴影的九宫格描述"));
        Add(_T("shadowsize"), DATATYPE_BYTE, _T("0"), _T("算法阴影的宽度(-20到20)"));
        Add(_T("shadowsharpness"), DATATYPE_BYTE, _T("255"), _T("算法阴影的锐度"));
        Add(_T("shadowdarkness"), DATATYPE_BYTE, _T("255"), _T("算法阴影的深度(相当于透明度)"));
        Add(_T("shadowposition"), DATATYPE_SIZE , _T("0,0"), _T("算法阴影的偏移量"));
        Add(_T("shadowcolor"), DATATYPE_DWORD, _T("0x000000"), _T("算法阴影的颜色,RGB格式,不支持透明度,使用shadowdarkness设置透明度"));
        Add(_T("gdiplustext"), DATATYPE_BOOL, _T("false"), _T("是否用gdi+渲染文字(抗锯齿效果更好，默认开启，但是效率低)"));
        Add(_T("textrenderinghint"), DATATYPE_INT, _T("0"), _T("gdi+渲染文字提示(0-5),字体大的时候可以设置为4"));
    }
//////////////////////////////////////////

    CControl::CControl(void)
    {
        Add(_T("pos"), DATATYPE_RECT, _T("0,0,0,0"), _T("位置,如果为float控件则指定位置和大小,否则只指定大小,如(0,0,100,100)"));
        Add(_T("float"), DATATYPE_BOOL_RECT, _T("false或0,0,0,0"), _T("是否使用绝对定位或指定位置,如果为float控件则指定位置和大小,否则只指定大小,如(0,0,100,100)"));
        Add(_T("floatalign"), DATATYPE_FLOAT_ALIGN, _T("null"), _T("绝对定位对齐方式"));
        Add(_T("padding"), DATATYPE_RECT, _T("0,0,0,0"), _T("外边距,如(2,2,2,2)"));
        Add(_T("bkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("背景颜色"));
        Add(_T("bkcolor1"), DATATYPE_DWORD, _T("0x00000000"), _T("背景渐变色1"));
        Add(_T("bkcolor2"), DATATYPE_DWORD, _T("0x00000000"), _T("背景渐变色2"));
        Add(_T("bkcolor3"), DATATYPE_DWORD, _T("0x00000000"), _T("背景渐变色3"));
        Add(_T("forecolor"), DATATYPE_DWORD, _T("0x00000000"), _T("设置文本时的前景色"));
        Add(_T("bordercolor"), DATATYPE_DWORD, _T("0x00000000"), _T("边框颜色"));
        Add(_T("focusbordercolor"), DATATYPE_DWORD, _T("0x00000000"), _T("获得焦点时边框的颜色"));
        Add(_T("colorhsl"), DATATYPE_BOOL, _T("false"), _T("本控件的颜色是否随窗口的hsl变化而变化"));
        Add(_T("bordersize"), DATATYPE_INT_RECT, _T("0或0,0,0,0"), _T("边框大小"));
        Add(_T("leftbordersize"), DATATYPE_INT, _T("0"), _T("左边边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置"));
        Add(_T("rightbordersize"), DATATYPE_INT, _T("0"), _T("右边边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置"));
        Add(_T("topbordersize"), DATATYPE_INT, _T("0"), _T("顶部边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置"));
        Add(_T("bottombordersize"), DATATYPE_INT, _T("0"), _T("底部边框大小,如(1),设置该值大于0,则将忽略bordersize属性的设置"));
        Add(_T("borderstyle"), DATATYPE_INT, _T("0"), _T("边框样式的设置,数值范围0-5"));
        Add(_T("borderround"), DATATYPE_SIZE, _T("0,0"), _T("边框圆角半径,如(2,2)"));
        Add(_T("bkimage"), DATATYPE_STRING, _T(""), _T("背景图片,如(bk.bmp或file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false')"));
        Add(_T("foreimage"), DATATYPE_STRING, _T(""), _T("前景图片"));
        Add(_T("width"), DATATYPE_INT, _T("0"), _T("控件预设的宽度"));
        Add(_T("height"), DATATYPE_INT, _T("0"), _T("控件预设的高度"));
        Add(_T("minwidth"), DATATYPE_INT, _T("0"), _T("控件预设的最小宽度"));
        Add(_T("minheight"), DATATYPE_INT, _T("0"), _T("控件预设的最小高度"));
        Add(_T("maxwidth"), DATATYPE_INT, _T("0"), _T("控件预设的最大宽度"));
        Add(_T("maxheight"), DATATYPE_INT, _T("0"), _T("控件预设的最大高度"));
        Add(_T("name"), DATATYPE_STRING, _T(""), _T("控件名称,同一窗口内必须唯一"));
        Add(_T("drag"), DATATYPE_BOOL , _T("false"), _T("拖拽"));
        Add(_T("resourcetext"), DATATYPE_BOOL, _T("false"), _T("是否支持多语言"));
        Add(_T("text"), DATATYPE_STRING, _T(""), _T("文本内容"));
        Add(_T("tooltip"), DATATYPE_STRING, _T(""), _T("鼠标悬浮提示"));
        Add(_T("userdata"), DATATYPE_STRING, _T(""), _T("用户自定义数据"));
        Add(_T("enabled"), DATATYPE_BOOL, _T("true"), _T("是否可以响应用户操作"));
        Add(_T("mouse"), DATATYPE_BOOL, _T("true"), _T("本控件是否可以响应鼠标操作"));
        Add(_T("keyboard"), DATATYPE_BOOL, _T("true"), _T("非Button类忽略该值，为false时不支持TAB_STOP,且该对象不处理键盘信息"));
        Add(_T("visible"), DATATYPE_BOOL, _T("true"), _T("是否可见"));
        Add(_T("shortcut"), DATATYPE_CHAR, _T(""), _T("对应的快捷键,如(P)"));
        Add(_T("menu"), DATATYPE_BOOL, _T("false"), _T("是否需要右键菜单"));
        Add(_T("cursor"), DATATYPE_CURSOR, _T(""), _T("鼠标光标类型"));
        Add(_T("virtualwnd"), DATATYPE_STRING, _T(""), _T("设置控件的虚拟窗体的名字"));
        Add(_T("innerstyle"), DATATYPE_STRING, _T(""), _T("通过样式表设置控件的样式"));
    }
/////////////////////////////////////////

    CLabel::CLabel(void)
    {
        Add(_T("align"), DATATYPE_ALIGN, _T("left"), _T("文字横向对齐方式"));
        Add(_T("valign"), DATATYPE_VALIGN, _T("vcenter"), _T("文字纵向对齐方式"));
        Add(_T("endellipsis"), DATATYPE_BOOL, _T("false"), _T("句末显示不全是否使用...代替"));
        Add(_T("wordbreak"), DATATYPE_BOOL, _T("false"), _T("是否支持多行文字"));
        Add(_T("noprefix"), DATATYPE_BOOL, _T("false"), _T("是否支持'&'字符转义为带下划线"));
        Add(_T("font"), DATATYPE_INT, _T("-1"), _T("字体id"));
        Add(_T("textcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("字体颜色,0表示使用默认字体颜色"));
        Add(_T("disabledtextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("disable字体颜色,0表示使用默认字体颜色"));
        Add(_T("textpadding"), DATATYPE_RECT, _T("0,0,0,0"), _T("文字显示的边距"));
        Add(_T("showhtml"), DATATYPE_BOOL, _T("false"), _T("是否使用类html富文本绘制"));
        Add(_T("是否使用类html富文本绘制"), DATATYPE_BOOL, _T("false"), _T("是否自动计算宽度"));
    }

////////////////////////////////////////////////
    CScrollBar::CScrollBar(void)
    {
        Add(_T("button1normalimage"), DATATYPE_STRING, _T(""), _T("左或上按钮普通状态图片"));
        Add(_T("button1hotimage"),  DATATYPE_STRING, _T(""), _T("左或上按钮鼠标悬浮状态图片"));
        Add(_T("button1pushedimage"), DATATYPE_STRING, _T(""), _T("左或上按钮鼠标按下状态图片"));
        Add(_T("button1disabledimage"), DATATYPE_STRING, _T(""), _T("左或上按钮禁用状态图片"));
        Add(_T("button2normalimage"),  DATATYPE_STRING, _T(""), _T("右或下按钮普通状态图片"));
        Add(_T("button2hotimage"), DATATYPE_STRING, _T(""), _T("右或下按钮鼠标悬浮状态图片"));
        Add(_T("button2pushedimage"),  DATATYPE_STRING, _T(""), _T("右或下按钮鼠标按下状态图片"));
        Add(_T("button2disabledimage"), DATATYPE_STRING, _T(""), _T("右或下按钮禁用状态图片"));
        Add(_T("thumbnormalimage"), DATATYPE_STRING, _T(""), _T("滑块普通状态图片"));
        Add(_T("thumbhotimage"), DATATYPE_STRING, _T(""), _T("滑块鼠标悬浮状态图片"));
        Add(_T("thumbpushedimage"), DATATYPE_STRING, _T(""), _T("滑块鼠标按下状态图片"));
        Add(_T("thumbdisabledimage"),  DATATYPE_STRING, _T(""), _T("滑块禁用状态图片"));
        Add(_T("railnormalimage"), DATATYPE_STRING, _T(""), _T("滑块中间标识普通状态图片"));
        Add(_T("railhotimage"),  DATATYPE_STRING, _T(""), _T("滑块中间标识鼠标悬浮状态图片"));
        Add(_T("railpushedimage"),  DATATYPE_STRING, _T(""), _T("滑块中间标识鼠标按下状态图片"));
        Add(_T("raildisabledimage"),  DATATYPE_STRING, _T(""), _T("滑块中间标识禁用状态图片"));
        Add(_T("bknormalimage"), DATATYPE_STRING, _T(""), _T("背景普通状态图片"));
        Add(_T("bkhotimage"),  DATATYPE_STRING, _T(""), _T("背景鼠标悬浮状态图片"));
        Add(_T("bkpushedimage"), DATATYPE_STRING, _T(""), _T("背景鼠标按下状态图片"));
        Add(_T("bkdisabledimage"), DATATYPE_STRING, _T(""), _T("背景禁用状态图片"));
        Add(_T("hor"), DATATYPE_BOOL, _T("false"), _T("水平或垂直"));
        Add(_T("linesize"), DATATYPE_INT, _T("8"), _T("滚动一行的大小"));
        Add(_T("range"), DATATYPE_INT, _T("100"), _T("滚动范围"));
        Add(_T("value"), DATATYPE_INT, _T("0"), _T("滚动位置"));
        Add(_T("showbutton1"), DATATYPE_BOOL, _T("true"), _T("是否显示左或上按钮"));
        Add(_T("showbutton2"), DATATYPE_BOOL, _T("true"), _T("是否显示右或下按钮"));
    }

////////////////////////////////////////////////
    CButton::CButton(void)
    {
        Add(_T("normalimage"), DATATYPE_STRING, _T(""), _T("普通状态图片"));
        Add(_T("hotimage"), DATATYPE_STRING, _T(""), _T("鼠标悬浮的状态图片"));
        Add(_T("pushedimage"), DATATYPE_STRING, _T(""), _T("鼠标按下的状态图片"));
        Add(_T("focusedimage"), DATATYPE_STRING, _T(""), _T("获得焦点时的状态图片"));
        Add(_T("disabledimage"), DATATYPE_STRING, _T(""), _T("禁用的状态图片"));
        Add(_T("hotforeimage"), DATATYPE_STRING, _T(""), _T("获得焦点时的状态图片"));
        Add(_T("bindtabindex"), DATATYPE_INT, _T(""), _T("点击后将触发绑定TabLayout控件的对应索引号"));
        Add(_T("bindtablayoutname"), DATATYPE_STRING, _T(""), _T("绑定TabLayout控件的名称"));
        Add(_T("hotbkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("鼠标悬浮背景颜色"));
        Add(_T("pushedbkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("鼠标按下字体颜色"));
        Add(_T("hottextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("鼠标悬浮字体颜色"));
        Add(_T("pushedtextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("鼠标按下的字体颜色"));
        Add(_T("focusedtextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("获得焦点时的字体颜色"));
    }

//////////////////////////////////////////
    COption::COption(void)
    {
        Add(_T("group"), DATATYPE_STRING, _T(""), _T("所属组的名称,可不设"));
        Add(_T("selected"), DATATYPE_BOOL, _T("false"), _T("是否选中"));
        Add(_T("selectedimage"), DATATYPE_STRING, _T(""), _T("选中的状态图片"));
        Add(_T("selectedhotimage"), DATATYPE_STRING, _T(""), _T("选中鼠标悬浮的的状态图片"));
        Add(_T("selectedpushedimage"), DATATYPE_STRING, _T(""), _T("选中鼠标按下的的状态图片"));
        Add(_T("selectedforeimage"), DATATYPE_STRING, _T(""), _T("选中的前景图片"));
        Add(_T("selectedbkcolor"), DATATYPE_DWORD, _T(""), _T("选中状态背景颜色"));
        Add(_T("selectedtextcolor"), DATATYPE_DWORD, _T(""), _T("选中状态字体颜色"));
    }

/////////////////////////////////////////////

    CEdit::CEdit(void)
    {
        Add(_T("readonly"), DATATYPE_BOOL, _T("false"), _T("是否只读"));
        Add(_T("numberonly"), DATATYPE_BOOL, _T("false"), _T("是否只允许数字输入"));
        Add(_T("password"), DATATYPE_BOOL, _T("false"), _T("是否显示密码字符"));
        Add(_T("passwordchar"), DATATYPE_BYTE, _T("*"), _T("设置密码字符"));
        Add(_T("maxchar"), DATATYPE_INT, _T("255"), _T("输入字符最大长度"));
        Add(_T("normalimage"), DATATYPE_STRING, _T(""), _T("普通状态图片"));
        Add(_T("hotimage"), DATATYPE_STRING, _T(""), _T("鼠标悬浮状态图片"));
        Add(_T("focusedimage"), DATATYPE_STRING, _T(""), _T("获得焦点状态图片"));
        Add(_T("disabledimage"), DATATYPE_STRING, _T(""), _T("禁用状态图片"));
        Add(_T("tipvalue"), DATATYPE_STRING, _T(""), _T("文本框内提示文字，当文本框text为空时显示并失去焦点时显示"));
        Add(_T("tipvaluecolor"), DATATYPE_DWORD, _T("0xFFBAC0C5"), _T("文本框内提示文字的颜色"));
        Add(_T("nativetextcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("windows原生edit控件的文字颜色"));
        Add(_T("nativebkcolor"), DATATYPE_DWORD, _T("0x00000000"), _T("windows原生edit控件的背景颜色"));
    }
/////////////////////////////////////////////



    CProgress::CProgress(void)
    {
        Add(_T("hor"), DATATYPE_BOOL, _T("true"), _T("水平或垂直"));
        Add(_T("min"), DATATYPE_INT, _T("0"), _T("进度最小值"));
        Add(_T("max"), DATATYPE_INT, _T("100"), _T("进度最大值"));
        Add(_T("value"), DATATYPE_INT, _T("0"), _T("进度值"));
        Add(_T("isstretchfore"), DATATYPE_BOOL, _T("true"), _T("指定前景图片是否缩放显示"));
        Add(_T("showtext"), DATATYPE_BOOL, _T("false"), _T("是否显示文本内容"));
    }
    //////////////////////////////////////////////
    CSlider::CSlider(void)
    {
        Add(_T("thumbimage"), DATATYPE_STRING, _T(""), _T("拖动滑块普通状态图片"));
        Add(_T("thumbhotimage"), DATATYPE_STRING, _T(""), _T("拖动滑块鼠标悬浮状态图片"));
        Add(_T("thumbpushedimage"), DATATYPE_STRING, _T(""), _T("拖动滑块鼠标按下状态图片"));
        Add(_T("thumbsize"), DATATYPE_SIZE, _T("10,10"), _T("拖动滑块大小"));
        Add(_T("step"), DATATYPE_INT, _T("1"), _T("进度步长"));
        Add(_T("sendmove"), DATATYPE_BOOL, _T("false"), _T("是否发送movevaluechanged消息"));
    }
    /////////////////////////////////////////////////
    CContainer::CContainer(void)
    {
        Add(_T("inset"), DATATYPE_RECT, _T("0,0,0,0"), _T("容器的内边距"));
        Add(_T("mousechild"), DATATYPE_BOOL, _T("true"), _T("本控件的子控件是否可以响应用户操作"));
        Add(_T("vscrollbar"), DATATYPE_BOOL, _T("false"), _T("是否使用纵向滚动条"));
        Add(_T("vscrollbarstyle"), DATATYPE_STRING, _T(""), _T("设置本容器的纵向滚动条的样式"));
        Add(_T("hscrollbar"), DATATYPE_BOOL, _T("false"), _T("是否使用横向滚动条"));
        Add(_T("hscrollbarstyle"), DATATYPE_STRING, _T(""), _T("设置本容器的横向滚动条的样式"));
        Add(_T("childpadding"), DATATYPE_INT, _T("0"), _T("子控件之间的额外距离"));
        Add(_T("childalign"), DATATYPE_ALIGN, _T("left"), _T("子控件横向对齐方式"));
        Add(_T("childvalign"), DATATYPE_VALIGN, _T("top"), _T("子控件纵向对齐方式"));
        Add(_T("scrollstepsize"), DATATYPE_INT, _T("8"), _T("容器的滚动条滚动步长"));
    }
    ////////////////////////////////////////////////////////////////
    CListElement::CListElement(void)
    {
        Add(_T("selected"), DATATYPE_BOOL, _T("false"), _T("是否选中"));
    }
    //////////////////////////////////////////////////////////

    CGif::CGif(void)
    {
        Add(_T("autoplay"), DATATYPE_BOOL, _T("true"), _T("是否自动播放GIF动画"));
        Add(_T("autosize"), DATATYPE_BOOL, _T("false"), _T("是否根据图片自动设置控件大小(开启后width和height属性失效)"));
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