//=============================================================================
//
//  �X�e�[�W�����Ǘ�����[stage.cpp]
// Author : KOUITI FUZIMURA
//
//=============================================================================
#include "stage.h"
#include "enemy.h"
#include "wall.h"

//�O���[�o���錾
static int g_nStageNo = 0;	//�X�e�[�W�ԍ�
static int g_nStageIdx = 0; //�X�e�[�W���ԍ�
static int g_nStageCnt = 0; //�o���p�J�E���^�[

//�X�e�[�W���
const tStage* stage[] =
{
	g_stage01,

	//�X�e�[�W���ǉ����ꂽ��z�񖼂������ɒǉ�
};

void InitStage()
{
	 g_nStageIdx = 0;
	 g_nStageCnt = 0;
}

void UpdateStage()
{
	if (stage[g_nStageNo][g_nStageIdx].nFrame == -1)return;

	//�t���[����i�߂�
	g_nStageCnt++;


	//�o��t���[�����ɂȂ�����
	while(g_nStageCnt >= stage[g_nStageNo][g_nStageIdx].nFrame)
	{
		//����ȏ�I�u�W�F�N�g���o�Ȃ�
		if (stage[g_nStageNo][g_nStageIdx].nFrame == -1)return;

		//��ނ���I�u�W�F�N�g�o��
		switch (stage[g_nStageNo][g_nStageIdx].nType)
		{
		case	E_OBJECT_ENEMY01:
		{
			const tStage& temp = stage[g_nStageNo][g_nStageIdx];
			SetEnemy(temp.pos);
		}
		break;
	
		}
		//���̃I�u�W�F�N�g�ݒ�
		g_nStageIdx++;
	}
}

void SetStageNo(int no)
{
	if (no < 0)return;
	 g_nStageNo = no;
	 g_nStageIdx = 0;
	 g_nStageCnt = 0;
}
