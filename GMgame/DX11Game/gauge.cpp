//=============================================================================
//
// �Q�[�W���� [gauge.cpp]
//
//=============================================================================
#include "gauge.h"
#include "polygon.h"	// �|���S���g�p
#include "Texture.h"

// �}�N����`
#define GAUGE_TEXTURENAME	_T("data/texture/gauge000.png")
#define GAUGE_SIZE_X	(256)	// �T�C�YX
#define GAUGE_SIZE_Y	(32)	// �T�C�YY

#define GAUGE1_TEXTURENAME	_T("data/texture/gaugeframe.png")
#define GAUGE1_SIZE_X	(256)	// �T�C�YX
#define GAUGE1_SIZE_Y	(32)	// �T�C�YY

// �\���̒�`
// �v���g�^�C�v�錾
// �O���[�o���ϐ�
static ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`��
static ID3D11ShaderResourceView*	g_pTexture1;				// �e�N�X�`��

// ����������
HRESULT InitGauge( void )
{
	// �e�N�X�`���ǂݍ���
	HRESULT hr = CreateTextureFromFile(GetDevice(),
		GAUGE_TEXTURENAME, &g_pTexture);
	// �e�N�X�`���ǂݍ���
	      hr = CreateTextureFromFile(GetDevice(),
		GAUGE1_TEXTURENAME, &g_pTexture1);

	return hr;
}

void UninitGauge( void )
{
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture);
	// �e�N�X�`���J��
	SAFE_RELEASE(g_pTexture1);
}

void UpdateGauge( void )
{
	// �P�t���[�����Ƃ̍X�V����
}

//in:pos:���[���W�w��
//�@	persent:100%��1.0�Ƃ��Ď󂯎��
void DrawGauge(XMFLOAT2 pos, float percent)
{
	//�`��p�Ƀf�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	SetBlendState(BS_ALPHABLEND);

	//�ݒ�
	SetPolygonTexture(g_pTexture);
	SetPolygonSize(GAUGE_SIZE_X * percent, GAUGE_SIZE_Y);
	SetPolygonPos(pos.x + GAUGE_SIZE_X * percent / 2, pos.y);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(percent, 1.0f);

	//�`��
	DrawPolygon(pDeviceContext);
	//���ɖ߂�
	SetPolygonFrameSize(1.0f,1.0f);
	//�ݒ�
	SetPolygonTexture(g_pTexture1);
	SetPolygonSize(GAUGE1_SIZE_X , GAUGE1_SIZE_Y);
	SetPolygonPos(3.0f, 300.0f);
	SetPolygonUV(0.0f, 0.0f);

	
	//�`��
	DrawPolygon(pDeviceContext);
	//���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);

}


