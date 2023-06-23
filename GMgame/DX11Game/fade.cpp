//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
//
//=============================================================================
#include "fade.h"
#include "polygon.h"	// �|���S���g�p

// �}�N����`
#define FADE_POS_X		(0)		// �\���ʒuX
#define FADE_POS_Y		(0)		// �\���ʒuY
#define FADE_SIZE_X		(SCREEN_WIDTH)	// �T�C�YX
#define FADE_SIZE_Y		(SCREEN_HEIGHT)	// �T�C�YY

#define FADE_RED		(1.0f)
#define FADE_GREEN		(1.0f)
#define FADE_BLUE		(1.0f)

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static float			g_fAlpha = 1.0f;	// �����x
static E_FADE			g_nFade = E_FADE_IN;	// �t�F�[�h�̏��
static E_TYPE_SCENE		g_stateNext = SCENE_TITLE;	// �t�F�[�h��̏��
static int				g_nFrame = 30;	// �t�F�[�h�t���[����

// ����������
HRESULT InitFade( void )
{
	g_fAlpha = 1.0f;				// �ŏ��̓t�F�[�h�C������
	g_nFade = E_FADE_IN;
	g_stateNext = SCENE_TITLE;
	g_nFrame = 30;

	return S_OK;
}

void UninitFade( void )
{
}

void UpdateFade( void )
{
	// �P�t���[�����Ƃ̍X�V����
	switch( g_nFade )
	{
	case E_FADE_OUT:	// �t�F�[�h�A�E�g
		
		//���l�𑝂₵�Ă���
		g_fAlpha += 1.0f / g_nFrame;
		
		//�t�F�[�h�A�E�g�̏I���̃`�F�b�N
		if (g_fAlpha >= 1.0f)
		{
			//�t�F�[�h�C���J�n
			g_fAlpha = 1.0f;
			g_nFade = E_FADE_IN;
			//��ʂ��B�ꂽ���ŃV�[���̕ύX
			StartSceneChange(g_stateNext);
		}

		break;
	case E_FADE_IN:	// �t�F�[�h�C��
		
		//���l�����炵�Ă���
		g_fAlpha -= 1.0f / g_nFrame;

		//�t�F�[�h�C���̏I���̃`�F�b�N
		if (g_fAlpha <= 0.0f)
		{
			//�t�F�[�h�I��
			g_fAlpha = 0.0f;
			g_nFade = E_FADE_NONE;
		}
		
		break;
	}

}

void DrawFade( void )
{
	// �`��p�Ƀf�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pDeviceContext
		= GetDeviceContext();

	// �ݒ�
	SetPolygonTexture( nullptr);		// �e�N�X�`���Z�b�g
	SetPolygonSize( FADE_SIZE_X, FADE_SIZE_Y );	// �T�C�Y���Z�b�g
	SetPolygonPos(FADE_POS_X, FADE_POS_Y );	// �ʒu���Z�b�g
	SetPolygonColor(FADE_RED, FADE_GREEN, FADE_BLUE);	// �|���S���̐F���Z�b�g
	SetPolygonAlpha(g_fAlpha);	// �����x���Z�b�g

	// �`��
	DrawPolygon( pDeviceContext );

	// ���Ƃɖ߂�
	SetPolygonColor( 1.0f, 1.0f, 1.0f );
	SetPolygonAlpha( 1.0f );
}

// �t�F�[�h�J�n
// stateNext:
// nFrame:
void StartFade( E_TYPE_SCENE stateNext, int nFrame /*= 30*/ )
{
	if( GetFade() != E_FADE_NONE ) return;
	g_nFade = E_FADE_OUT;
	g_stateNext = stateNext;
		// 0���Z���
	g_nFrame = nFrame;
	
}

// ���݃t�F�[�h���ł����H
E_FADE GetFade()
{
	return g_nFade;
}

