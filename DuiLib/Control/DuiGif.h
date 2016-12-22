#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiGif
        : public CDuiControl
    {
        DECLARE_DUICONTROL(CDuiGif)
    public:
        CDuiGif(void);
        virtual ~CDuiGif(void);
    public:
        LPCTSTR	GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void PlayGif();
        void PauseGif();
        void StopGif();

        BOOL IsAutoPlay() const;
        void SetAutoPlay(BOOL bIsAuto = TRUE);

        BOOL IsAutoSize() const;
        void SetAutoSize(BOOL bIsAuto = TRUE);

        void SetBkImage(LPCTSTR pStrImage) OVERRIDE;
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void SetVisible(BOOL bVisible = TRUE) OVERRIDE;
        void SetInternVisible(BOOL bVisible = TRUE) OVERRIDE;

        void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;

        void DoInit() OVERRIDE;

        void DoEvent(TEventUI& event) OVERRIDE;
    private:
        void DrawFrame(HDC hDC);		// 绘制GIF每帧
        void InitGifImage();
        void DeleteGif();
    private:
        BOOL m_bIsPlaying;
        BOOL m_bIsAutoPlay;				// 是否自动播放gif
        BOOL m_bIsAutoSize;				// 是否自动根据图片设置大小
        UINT m_nFrameCount;				// gif图片总帧数
        UINT m_nFramePosition;			// 当前放到第几帧
        Gdiplus::PropertyItem* m_pPropertyItem;	// 帧与帧之间间隔时间
        Gdiplus::Image*	m_pGifImage;
    };

}