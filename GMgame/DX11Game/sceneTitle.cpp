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
	g_player.Setmodel(0);
	g_player.Setmodel(10);
	g_player.Setmodel(20);
	g_player.Setmodel(30);
	g_player.Setscl(XMFLOAT3(8.0f, 8.0f, 8.0f));
	PartsGet()->Setpos(XMFLOAT3(0.0f, 0.0f, -2.0f));
	MPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, -1.0f));
	UPartsGet()->Setpos(XMFLOAT3(0.0f, -0.8f, 0.0f));
	PartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	MPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	UPartsGet()->Setscl(XMFLOAT3(1.0f, 1.0f, 1.0f));
	player.SetHead(MODEL_KABU);
	player.SetBody(MODEL_KABU1);
	player.SetMae(MODEL_KABU2);
	player.SetBack(MODEL_KABU3);

	//�f�o�b�N
	g_enemy.Init();
	g_enemy.SetName("���");
	g_enemy.Setmodel(1);
	g_enemy.Setmodel(11);
	g_enemy.Setmodel(21);
	g_enemy.Setmodel(31);
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

