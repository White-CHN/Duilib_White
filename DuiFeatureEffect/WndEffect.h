#pragma once
#include "Effect.h"

#define ANIMATIONNUM	85

// �������ڲ��������֣�˳���ܵ���
//enum AnimationEffect
//{
//	EFFECT_FLIPLEFT = 2,					//	����ת
//	EFFECT_FLIPRIGHT,						//	���ҷ�ת
//	EFFECT_FLIPBOTTOM,						//	���·�ת
//	EFFECT_FLIPUP = 5,						//	���Ϸ�ת
//	EFFECT_MOSAIC,							//	������
//	EFFECT_SLIDELEFT,						//	���󻬶�
//	EFFECT_SLIDERIGHT,						//	���һ���
//	EFFECT_SLIDEUP,							//	���ϻ���
//	EFFECT_SLIDEBOTTOM = 10,				//	���»���
//	EFFECT_BANDSLIDEH,						//	ˮƽ��״����
//	EFFECT_BANDSLIDEV,						//	��ֱ��״����
//	EFFECT_BLINDSH,							//	ˮƽ��Ҷ
//	EFFECT_BLINDSV ,						//	��ֱ��Ҷ
//	EFFECT_SEPCIALBLINDSH= 15, 				//  ���ȸ�ˮƽ��Ҷ
//	EFFECT_SEPCIALBLINDSV,					//  ���ȿ�ֱ��Ҷ
//	EFFECT_SCANLEFT,						//	����ɨ��
//	EFFECT_SCANRIGHT,						//	����ɨ��
//	EFFECT_SCANUP,							//	����ɨ��
//	EFFECT_SCANBOTTOM = 20,					//	����ɨ��
//	EFFECT_INERTIASLIDELEFT,				//  ������Ի���
//  EFFECT_INERTIASLIDERIGHT,				//  ���ҹ��Ի���
//	EFFECT_INERTIASLIDEUP,					//  ���Ϲ��Ի���
//	EFFECT_INERTIASLIDEBOTTOM,				//  ���¹��Ի���
//	EFFECT_FADE = 25,						//	����/����
//	EFFECT_ZOOM,							//	�Ŵ�
//	EFFECT_SHRINK,							//	��С
//	EFFECT_SWING,							//	�ڳ�/����
//	EFFECT_FUNNEL,							//	©��
//	EFFECT_NOISE = 30,						//	����
//	EFFECT_CUBELEFT,						//	��������
//	EFFECT_CUBERIGHT,						//	��������
//	EFFECT_CUBEUP,							//	��������
//	EFFECT_CUBEBOTTOM ,						//	��������
//	EFFECT_GRADUALERASELEFT = 35,			//	���󽥱����
//	EFFECT_GRADUALERASERIGHT,				//	���ҽ������
//	EFFECT_GRADUALERASEUP,					//	���Ͻ������
//	EFFECT_GRADUALERASEBOTTOM,				//	���½������
//	EFFECT_PUSHLEFT ,						//	�����Ƽ�
//	EFFECT_PUSHRIGHT = 40,					//	�����Ƽ�
//	EFFECT_PUSHUP,							//	�����Ƽ�
//	EFFECT_PUSHBOTTOM,						//	�����Ƽ�
//	EFFECT_DISSOLVE,						//  �ܽ�
//	EFFECT_CURTAIN,							//  Ļ
//	EFFECT_TRIANGULARPRISMRIGHT = 45,		//  ��������
//	EFFECT_TRIANGULARPRISMLEFT,				//  ��������
//	EFFECT_TRIANGULARPRISMUP,				//  ��������
//	EFFECT_TRIANGULARPRISMBOTTOM,			//  ��������
//	EFFECT_CUBEINSIDELEFT ,					//  ����������
//	EFFECT_CUBEINSIDERIGHT = 50,			//  ����������
//	EFFECT_CUBEINSIDEUP,					//  ����������
//	EFFECT_CUBEINSIDEBOTTOM,				//  ����������
//	EFFECT_SPATIALMOTIONLEFT,				//  �ռ�����
//	EFFECT_SPATIALMOTIONRIGHT,				//  �ռ�����
//	EFFECT_SPATIALMOTIONUP= 55,				//  �ռ�����
//	EFFECT_SPATIALMOTIONBOTTOM,				//  �ռ�����
//	EFFECT_PATCHRANDOMFLIP,					//  �ֿ鷭ת
//	EFFECT_SPATIALROTATELEFT,				//  �ռ�������ת
//	EFFECT_SPATIALROTATERIGHT ,				//  �ռ�������ת
//	EFFECT_SPATIALROTATEUP = 60,			//  �ռ�������ת
//	EFFECT_SPATIALROTATEBOTTOM,				//  �ռ�������ת
//	EFFECT_DOOR,							//  ��/����
//	EFFECT_WHIRLPOOL,						//	����
//	EFFECT_SCATTER,							//	��ɢ
//	EFFECT_COLORDADE= 65 ,					//	��ɫ
//	EFFECT_DAMPSLIDE_BOTTOMLEFT,			//  ���½����Ử��
//	EFFECT_DAMPSLIDE_BOTTOMRIGHT,			//  ���½����Ử��
//	EFFECT_DAMPSLIDE_TOPLEFT,				//  ���Ͻ����Ử��
//	EFFECT_DAMPSLIDE_TOPRIGHT,				//  ���Ͻ����Ử��
//	EFFECT_DAMPSLIDELEFT = 70,				//	�������Ử��
//	EFFECT_DAMPSLIDERIGHT,					//	�������Ử��
//	EFFECT_DAMPSLIDEUP,						//	�������Ử��
//	EFFECT_DAMPSLIDEBOTTOM,					//	�������Ử��
//	EFFECT_CLOCKWIPE,						//	���β���
//	EFFECT_PINWHEEL = 75,					//	�糵
//	EFFECT_PAGEPEEL,						//	��ҳ
//	EFFECT_GAUSSBLUR,						//	ģ������/��
//	EFFECT_WAVE,							//	���˻�
//	EFFECT_ZOOMBLUR ,						//	��β
//  EFFECT_FOLDV = 80,						//  ��ֱ�۵�
//	EFFECT_FOLDH,							//	ˮƽ�۵�
//	EFFECT_STACKLEFT,						//	����ѻ�
//	EFFECT_STACKRIGHT,						//	���Ҷѻ�
//	EFFECT_STACKUP,							//	���϶ѻ�
//	EFFECT_STACKBOTTOM = 85,				//	���¶ѻ�
//};

class CWndEffect : public IUIEffect
{
public:
    CWndEffect(void);
    virtual ~CWndEffect(void);
public:
    //IUIEffect
    BOOL AppendAnimation(AnimationParam& aParam) OVERRIDE;
    BOOL DependAnimation(WPARAM effectKey) OVERRIDE;
    BOOL ClearAllAnimation() OVERRIDE;
    BOOL Animation(IUIEffectCallBack* iDrawEffect, DWORD frameSpin) OVERRIDE;
    // ��ʼ��֧�ֵĶ���
    static DWORD InitSurportAnimationType(const char*& strAnimationType);
private:
    // ���㶯������
    void ComputeAnimation(IUIEffectCallBack* iDrawEffect, DWORD timeElapse);
    // ����ִ�н����Ķ���
    void CleanFinishedAnimation(IUIEffectCallBack* iDrawEffect);
private:
    std::vector<InternalAnimationParam>				m_animationContainer;				// ������������
    std::vector<InternalAnimationParam>::iterator	m_itAnimation;						// ö����
    static const char* 								m_animation;						// �������ƣ���\0�ָ�,\0\0����
    ULONG_PTR m_gdiplusToken;															// gdiplus
};

