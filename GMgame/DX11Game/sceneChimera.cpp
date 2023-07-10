//=============================================================================
//
// �^�C�g���V�[��[sceneChimera.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneChimera.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"		
#include "bg.h"
#include "collision.h"
#include "fade.h"
#include "chimeracs.h"


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

Chimera player;//�v���C��

//=============================================================================
// �^�C�g���\���̏���������
//=============================================================================
HRESULT InitSceneChimera()
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr =
		CreateTextureFromFile(GetDevice(),
			SAMPLE_TEXTURENAME, &g_pTexture);

	//HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();

	
	player.Init();

	return hr;
}

//=============================================================================
// �^�C�g���\���̏I������
//=============================================================================
void UninitSceneChimera()
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	//�w�i�̏I������
	UninitBg();
	player.Uninit();
}

//=============================================================================
// �^�C�g���\���̍X�V����
//=============================================================================
void UpdateSceneChimera()
{
	
	player.Update();
	
}

//=============================================================================
// �^�C�g���\������
//=============================================================================
void DrawSceneChimera()
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

	player.Draw();

}
