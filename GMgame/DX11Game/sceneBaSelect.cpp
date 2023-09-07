//=============================================================================
//
// �^�C�g���V�[��[sceneBaSelect.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneBaSelect.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "fade.h"
#include "bg.h"
#include "selectstage.h"
#include "player.h"
#include "Camera.h"
#include "pchimera.h"
#include "Light.h"
#include "enemy.h"
#include "enemyselect.h"
#include "enemymodel.h"
#include "sceneTitle.h"
#include "selecttitle.h"
#include "backtex.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneBaSelect()
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

	InitSelTitle();

	hr = InitSelectStage();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�Z���N�g�摜�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�v���C���[�̏���������
	hr = InitPChimera();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	Enemychimera();

	InitBackTex();

	//�G�̏���������
	hr = InitSelectEnemyselect();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}
	
	SetEnemyselect(XMFLOAT3(0, -15, -150), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, -50), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 50), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 150), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 250), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 350), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 450), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 550), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 650), XMFLOAT3(0, 90, 0));
	SetEnemyselect(XMFLOAT3(0, -15, 750), XMFLOAT3(0, 90, 0));

	// �J�����X�V
	GetCamera()->Init();

	GetEnemy()->Init();

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneBaSelect()
{
	//�w�i�̏I������
	UninitBg();

	UninitSelTitle();

	UninitBackTex();

	//�Z���N�g�̏I������
	UninitSelectStage();

	UninitPChimera();

	UninitEnemyselect();

	GetEnemy()->Uninit();
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneBaSelect()
{
	//�w�i�̍X�V����
	UpdateBg();

	//�I������
	UpdateSelectStage();

	// �J�����X�V
	GetCamera()->Update();

	UpdatePChimera();

	UpdateSelTitle();

	UpdateBackTex();

	UpdateSelectEnemyselect();

	GetEnemy()->Update();

	if (GetMouseTrigger(1))
	{
		StartFade(SCENE_SELECT);
	}

}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneBaSelect()
{

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	//�������I�u�W�F�N�g(3D�̒��ł����ɂ���)
	//SetZWrite(true);

	//�w�i
	DrawBg1();
	DrawSelTitle1();
	DrawBackTex1();
	// Z�o�b�t�@�L��
	SetZBuffer(true);

	//�v���C���[
	DrawPChimera();
	DrawEnemyselect();

}
