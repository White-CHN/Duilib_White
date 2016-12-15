#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiRing
        : public CDuiLabel
    {
        DECLARE_DUICONTROL(CDuiRing)
    public:
        CDuiRing(void);
        virtual ~CDuiRing(void);
    public:
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

        virtual void SetBkImage(LPCTSTR pStrImage);

        virtual void PaintBkImage(HDC hDC);

        virtual void DoEvent(TEventUI& event);
    private:
        void InitImage();
        void DeleteImage();
    private:
        float m_fCurAngle;
        Gdiplus::Image* m_pBkimage;
    };

}