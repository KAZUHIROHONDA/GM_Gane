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

	//�G�̏���������
	hr = InitSelectEnemy();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}
	
	SetEnemy(XMFLOAT3(-50, -5, -150), XMFLOAT3(0, 90, 0));
	SetEnemy(XMFLOAT3(-50, -5, -50), XMFLOAT3(0, 90, 0));
	SetEnemy(XMFLOAT3(-50, -5, 50), XMFLOAT3(0, 90, 0));

	// �J�����X�V
	GetCamera()->Init();

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneBaSelect()
{
	//�w�i�̏I������
	UninitBg();

	//�Z���N�g�̏I������
	UninitSelectStage();

	UninitPChimera();

	UninitEnemy();
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

	UpdateSelectEnemy();

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

	// Z�o�b�t�@�L��
	SetZBuffer(true);

	//�v���C���[
	DrawPChimera();
	DrawEnemy();


}
