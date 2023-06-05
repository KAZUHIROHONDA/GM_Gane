//=============================================================================
//
// �X�e�[�W�����Ǘ����� [stage.h]
// Author : KOUITI FUZIMURA
//
//=============================================================================
#pragma once

#ifndef ___STAGE_H___
#define ___STAGE_H___

#include "main.h"

//�\���̒�`
typedef struct _tStage
{
	int			nFrame;  //�o���t���[����
	XMFLOAT3	pos;	 //���W
	int			nType;	 //���
	int			nData;	 //�ėp�����f�[�^
	float	    rot;	 //
}tStage;

//���
enum  //P273 �񋓌^
{
	E_OBJECT_ENEMY01,

	//�ǉ�����Ȃ炱��
	E_OBJECT_MAX		 //����͕K���Ō�
};

//�v���g�^�C�v�錾
void InitStage();
void UpdateStage();
void SetStageNo(int no);//�X�e�[�W�ԍ��ݒ�

//�O���t�@�C���̕ϐ����g�p������	P210
extern const tStage g_stage01[];


		//stage02�ȍ~���쐬�����ꍇ�����ɒǉ�

#endif