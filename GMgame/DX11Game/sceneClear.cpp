//=============================================================================
//
// �N���A�V�[��[sceneClear.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneClear.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "fade.h"
#include "sceneGameover.h"
#include "sceneSelect.h"
#include"select.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SAMPLE_TEXTURENAME	_T("data/texture/GC.jpg")


#define SAMPLE_POS_X	(0)	// �����ʒuX
#define SAMPLE_POS_Y	(0)	// �����ʒuY
#define SAMPLE_SIZE_X	(SCREEN_WIDTH)	// ����
#define SAMPLE_SIZE_Y	(SCREEN_HEIGHT)	// �c��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static XMFLOAT3 g_Position;	// ���W

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneClear()
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	if (FAILED(hr)) return hr;
	//return E_NOTIMPL;
	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	//�w�i�̏���������
	hr = InitBg();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�w�i�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}
	
	//serect�̏���������
	hr = InitSelect();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("serect�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}


	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneClear()
{

	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	//�w�i�̏I������
	UninitBg();
	//�w�i�̏I������
	UninitSelect();
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneClear()
{
	//�w�i�̍X�V����
	UpdateBg();
	UpdateSelect();
	E_FADE fadeState = GetFade();
	if (fadeState == E_FADE_NONE)
	{
		if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0))
		{
			//�I�𒆂̂��̂ɂ�蕪��
			SELECT_MENU menu = GetSelectMenu();
			switch (menu)
			{
				//�Q�[���ɖ߂�
			case SELECT_MENU_CONTINUE:
				StartFade(SCENE_GAME);
				//PlaySound(SOUND_LABEL_SE_DECIDE);
				//sCnt++;
				//SetStageNo(sCnt);
				//sNO();
				break;
				//���g���C
			case SELECT_MENU_RETRY:
				StartFade(SCENE_GAME);
				//PlaySound(SOUND_LABEL_SE_DECIDE);
				break;
				//�I��(�^�C�g����)
			case SELECT_MENU_QUIT:
				StartFade(SCENE_TITLE);
				//PlaySound(SOUND_LABEL_SE_DECIDE);
				//sCnt = 0;
				break;
			}
		}
	}

	/*if (GetKeyTrigger(VK_RETURN))
	{
		//�G���^�[�L�[�����ŃQ�[����ʂ�
		StartFade(SCENE_TITLE, 60);
	}*/
}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneClear()
{

	// �T���v���摜�`��
	SetPolygonPos(g_Position.x, g_Position.y);	// ���W
	SetPolygonSize(SAMPLE_SIZE_X, SAMPLE_SIZE_Y);	// �傫��
	SetPolygonAngle(0.0f);				// �p�x
	SetPolygonColor(1.0f, 1.0f, 1.0f);	// �F(RGB)
	SetPolygonAlpha(1.0f);				// ���l(�����x)

	SetPolygonUV(0.0f, 0.0f);			// 0�Ԃ̃e�N�X�`�����W
	SetPolygonFrameSize(1.0f, 1.0f);	// �e�N�X�`���̉���,�c��
	SetPolygonTexture(g_pTexture);		// �e�N�X�`���w��


	
	 //�|���S���̕`�揈��
	DrawPolygon(GetDeviceContext());
	// Z�o�b�t�@�L��
	SetZBuffer(true);

	//�w�i�̕`�揈��
	DrawBg();
	

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
	DrawSelect();


}
