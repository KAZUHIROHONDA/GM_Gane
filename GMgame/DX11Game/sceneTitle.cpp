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

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

Player g_player;
Player g_enemy;

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
	g_player.SetHP(100);
	g_player.SetPAat(60);
	g_player.SetGUat(70);
	g_player.SetTYOKIat(50);

	//�f�o�b�N
	g_enemy.Init();
	g_enemy.SetName("���");
	g_enemy.SetHP(10);
	g_enemy.SetPAat(15);
	g_enemy.SetGUat(20);
	g_enemy.SetTYOKIat(25);

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
