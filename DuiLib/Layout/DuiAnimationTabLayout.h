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

        void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) OVERRIDE;
        void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) OVERRIDE;
        void OnAnimationStop(int nAnimationID) OVERRIDE;

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void DoEvent(CDuiEvent& event) OVERRIDE;
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
