//=============================================================================
//
// �^�C�g���V�[��[sceneTitle.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneTitle.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "Title.h"
#include "enter.h"
#include "stage.h"
#include "sceneClear.h"
#include "Bpartsmenu.h"
#include "partsmenu.h"
#include "Upartsmenu.h"
#include "Mpartsmenu.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

Player g_player;
Model  player;
Player g_enemy;
Model  enemy;

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneTitle()
{
	HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//�w�i�̏���������
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�w�i�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	g_player.Init();
	g_player.SetName("�J�u�g���V");
	g_player.SetHP(300);
	g_player.SetPAat(60);
	g_player.SetGUat(70);
	g_player.SetTYOKIat(50);
	g_player.Setscl(XMFLOAT3(8.0f, 8.0f, 8.0f));
	PartsGet()->Setpos(XMFLOAT3(0.0f, 0.0f, -2.0f));
	MPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, -1.0f));
	UPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, 0.0f));
	PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	g_player.SetHand(0, 2);g_player.SetHand(1, 1);
	g_player.SetHand(2, 2);g_player.SetHand(3, 1);
	g_player.SetHand(4, 2);g_player.SetHand(5, 1);
	g_player.SetHand(6, 2);g_player.SetHand(7, 1);
	g_player.SetHand(8, 2);g_player.SetHand(9, 1); 
	g_player.SetHand(10, 2);g_player.SetHand(11, 1);
	g_player.SetHand(12, 2); g_player.SetHand(13, 1);
	g_player.SetHand(14, 2); g_player.SetHand(15, 1);
	g_player.SetHand(16, 2); g_player.SetHand(17, 1);
	g_player.SetHand(18, 2); g_player.SetHand(19, 1);
	player.SetHead(MODEL_KABU);
	player.SetBody(MODEL_KABU1);
	player.SetMae(MODEL_KABU2);
	player.SetBack(MODEL_KABU3);

	//�f�o�b�N
	g_enemy.Init();
	g_enemy.SetName("���");
	g_enemy.SetHP(50);
	g_enemy.SetPAat(15);
	g_enemy.SetGUat(20);
	g_enemy.SetTYOKIat(25);
	g_enemy.SetHand(0, 0); g_enemy.SetHand(1, 1);
	g_enemy.SetHand(2, 0); g_enemy.SetHand(3, 1);
	g_enemy.SetHand(4, 0); g_enemy.SetHand(5, 1);
	g_enemy.SetHand(6, 0); g_enemy.SetHand(7, 1);
	g_enemy.SetHand(8, 0); g_enemy.SetHand(9, 1);
	g_enemy.SetHand(10, 0);g_enemy.SetHand(11, 2);
	g_enemy.SetHand(12, 1);g_enemy.SetHand(13, 2);
	g_enemy.SetHand(14, 2);g_enemy.SetHand(15, 2);
	g_enemy.SetHand(16, 1); g_enemy.SetHand(17, 2);
	g_enemy.SetHand(18, 0);g_enemy.SetHand(19, 2);
	enemy.SetHead(MODEL_wani);
	enemy.SetBody(MODEL_wani1);
	enemy.SetMae(MODEL_wani2);
	enemy.SetBack(MODEL_wani3);
	
	//�^�C�g��
	InitTitle();

	InitEnter();

	//�X�e�[�W�̏�����
	SetStageNo(0);

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneTitle()
{

	//�w�i�̏I������
	UninitBg();

	//�^�C�g��
	UninitTitle();

	UninitEnter();

	g_player.Uninit();
	g_enemy.Uninit();

}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneTitle()
{

	//�w�i�̍X�V����
	UpdateBg();

	//�^�C�g��
	UpdateTitle();

	g_player.Update();
	g_enemy.Update();

	UpdateEnter();

	if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0)|| GetMouseTrigger(0))
	{

		StartFade(SCENE_SELECT);
	}


}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneTitle()
{
	SetZWrite(false);
	//�w�i�̕`�揈��
	DrawBg();

	// Z�o�b�t�@�L��
	SetZBuffer(true);

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	

	//�^�C�g��
	DrawTitle();
	DrawEnter();
	
}

Player* GetPlayer()
{
	return &g_player;
}

Player* GetEnemy()
{
	return &g_enemy;
}

Model* GetPlayerModel()
{
	return &player;
}

Model* GetEnemyModel()
{
	return &enemy;
}