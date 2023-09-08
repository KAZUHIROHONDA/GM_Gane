//=============================================================================
//
// �^�C�g���V�[��[sceneGameover.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneGameover.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "stage.h"
#include "fade.h"
#include "gameover.h"
#include "select 2.h"
#include "player.h"
#include "sound.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/GO1.jpg")


#define SAMPLE_POS_X	(0)	// �����ʒuX
#define SAMPLE_POS_Y	(0)	// �����ʒuY
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// ����
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// �c��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static XMFLOAT3 g_Position;	// ���W

int sNo = 0;


//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneGameover()
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//�w�i�̏���������
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�w�i�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�v���C���[�̏���������
	hr = InitPlayer();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//serect�̏���������
	hr = InitSelect2();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("serect�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}
	PlaySound(SOUND_LABEL_BGM_5);

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneGameover()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	//�w�i�̏I������
	UninitBg();
	//�v���C���[�̏I������
	UninitPlayer();
	//�w�i�̏I������
	UninitSelect2();

	StopSound();
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneGameover()
{
	//�Q�[���I�[�o�[�e�N�X�`��
	UpdateGameover();
	UpdateSelect2();
	//�v���C���[
	UpdatePlayer();

	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN)|| GetJoyTrigger(0, 0) || GetMouseTrigger(0))
		{
			//�I�𒆂̂��̂ɂ�蕪��
			SELECT2_MENU menu = GetSelect2Menu();
			switch (menu)
			{
				//�Q�[���ɖ߂�
			case SELECT2_MENU_CONTINUE:
				StartFade(SCENE_GAME);
				SetStageNo(sNo);
				break;
				//���g���C
			case SELECT2_MENU_RETRY:
				StartFade(SCENE_TITLE);
				break;
			}
		}
	}
}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneGameover()
{

	//�T���v���摜�`��
	SetPolygonPos(g_Position.x, g_Position.y);	// ���W
	SetPolygonSize(SAMPLE_SIZE_X, SAMPLE_SIZE_Y);	// �傫��
	SetPolygonAngle(0.0f);				// �p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	// �F(RGB)
	SetPolygonAlpha(1.0f);				// ���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			// 0�Ԃ̃e�N�X�`�����W
	SetPolygonFrameSize(1.0f, 1.0f);	// �e�N�X�`���̉���,�c��
	SetPolygonTexture(g_pTexture);		//

	
	 //�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());
	//�w�i�̕`�揈��
	DrawBg();
	// Z�o�b�t�@�L��
	SetZBuffer(true);
	
	//�v���C���[
	DrawPlayer();


	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	DrawSelect2();

}
