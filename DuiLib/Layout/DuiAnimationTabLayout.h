#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiAnimationTabLayout
        : public CDuiTabLayout
        , public IAnimation
    {
        DECLARE_DUICONTROL(CDuiAnimationTabLayout)
    public:
        CDuiAnimationTabLayout(void);
        virtual ~CDuiAnimationTabLayout(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        void AnimationSwitch();
        BOOL SelectItem(int iIndex) OVERRIDE;

        void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) OVERRIDE;
        void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID) OVERRIDE;
        void OnAnimationStop(INT nAnimationID) OVERRIDE;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void DoEvent(TEventUI& event) OVERRIDE;
    private:
        enum
        {
            TAB_ANIMATION_ELLAPSE = 10,
            TAB_ANIMATION_FRAME_COUNT = 15,
        };
    private:
        BOOL m_bIsVerticalDirection;
        BOOL m_bControlVisibleFlag;
        int m_nPositiveDirection;
        CDuiControl* m_pCurrentControl;
        CDuiRect m_rcCurPos;
        CDuiRect m_rcItemOld;

    };


}
