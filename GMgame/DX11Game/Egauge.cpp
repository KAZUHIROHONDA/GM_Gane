//=============================================================================
//
// �Q�[�W���� [gauge.cpp]
//
//=============================================================================
#include "Egauge.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"

// �}�N����`
#define EGAUGE_TEXTURENAME	_T("data/texture/gauge001.png")
#define EGAUGE_SIZE_X	(256)	// �T�C�YX
#define EGAUGE_SIZE_Y	(32)	// �T�C�YY
	    
#define EGAUGE1_TEXTURENAME	_T("data/texture/gaugeframe.png") 
#define EGAUGE1_SIZE_X	(256)	// �T�C�YX
#define EGAUGE1_SIZE_Y	(32)	// �T�C�YY

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static ID3D11ShaderResourceView*	g_pTexture1;				// �e�N�X�`��

// ����������
HRESULT InitEGauge( void )
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr = CreateTextureFromFile(GetDevice(),
		EGAUGE_TEXTURENAME, &g_pTexture);
	// �e�N�X�`���ǂݍ���
	      hr = CreateTextureFromFile(GetDevice(),
		EGAUGE1_TEXTURENAME, &g_pTexture1);

	return hr;
}

void UninitEGauge( void )
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture1);
}

void UpdateEGauge( void )
{
	// �P�t���[�����Ƃ̍X�V����
}

//in:pos:���[���W�w��
//�@	persent:100%��1.0�Ƃ��Ď󂯎��
void DrawEGauge(XMFLOAT2 pos, float percent)
{
	//�`��p�Ƀf�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetBlendState(BS_ALPHABLEND);

	//�ݒ�
	SetPolygonTexture(g_pTexture);
	SetPolygonSize(EGAUGE_SIZE_X * percent, EGAUGE_SIZE_Y);
	SetPolygonPos(pos.x + EGAUGE_SIZE_X * percent / 2, pos.y);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(percent, 1.0f);

	//�`��
	DrawPolygon(pDeviceContext);
	//���ɖ߂�
	SetPolygonFrameSize(1.0f,1.0f);
	//�ݒ�
	SetPolygonTexture(g_pTexture1);
	SetPolygonSize(EGAUGE1_SIZE_X , EGAUGE1_SIZE_Y);
	SetPolygonPos(252.0f, 300.0f);
	SetPolygonUV(0.0f, 0.0f);

	
	//�`��
	DrawPolygon(pDeviceContext);
	//���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);

}


