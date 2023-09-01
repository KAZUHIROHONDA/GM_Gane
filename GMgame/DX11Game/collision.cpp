// collision.cpp
// �����蔻���������
#include "collision.h"
#include "Light.h"
#include "enemy.h"
#include "fade.h"
#include "player.h"
#include "stage.h"
#include "sceneGame.h"
#include "flame.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

// �����蔻��܂Ƃ�
void Collision()
{
	
	
}

// �o�E���f�B���O�{�b�N�X
// ��`���m�̓����蔻��
bool CollisionBB( XMFLOAT2 * pCenter1, XMFLOAT2 * pSize1, XMFLOAT2 * pCenter2, XMFLOAT2 * pSize2 )
{
	if( pCenter1->x + pSize1->x >= 
		pCenter2->x - pSize2->x && 
		pCenter1->x - pSize1->x <=
		pCenter2->x + pSize2->x )
	{	// X�������������Ă���
		if( pCenter1->y + pSize1->y >=
			pCenter2->y - pSize2->y &&
			pCenter1->y - pSize1->y <=
			pCenter2->y + pSize2->y )
		{	// Y�������������Ă���
			return true;
		}
	}
	// ������Ȃ�����
	return false;
}

// �o�E���f�B���O�T�[�N��
// �~���m�̓����蔻��
bool CollisionBC( XMFLOAT2 * pCenter1, float fRadius1, XMFLOAT2 * pCenter2, float fRadius2 )
{
	float fx = pCenter1->x - pCenter2->x;
	float fy = pCenter1->y - pCenter2->y;
	float len = fx * fx + fy * fy;
	float fr = fRadius1 + fRadius2;
	if( len < fr * fr )
	{
		return true;	// ��������
	}

	return false;		// �������ĂȂ�
}

bool CollisionBB3D(XMFLOAT3 * pCenter1, XMFLOAT3 * pSize1, XMFLOAT3 * pCenter2, XMFLOAT3 * pSize2)
{
	if (pCenter1->x + pSize1->x >=
		pCenter2->x - pSize2->x &&
		pCenter1->x - pSize1->x <=
		pCenter2->x + pSize2->x)
	{	// X�������������Ă���
		if (pCenter1->y + pSize1->y >=
			pCenter2->y - pSize2->y &&
			pCenter1->y - pSize1->y <=
			pCenter2->y + pSize2->y)
		{	// Y�������������Ă���
			if (pCenter1->z + pSize1->z >=
				pCenter2->z - pSize2->z &&
				pCenter1->z - pSize1->z <=
				pCenter2->z + pSize2->z)
			{	//Z�������������Ă���
				return true;
			}
		}
	}
	// ������Ȃ�����
	return false;
}

bool CollisionBS(XMFLOAT3 * pCenter1, float fRadius1, XMFLOAT3 * pCenter2, float fRadius2)
{
	float fx = pCenter1->x - pCenter2->x;
	float fy = pCenter1->y - pCenter2->y;
	float fz = pCenter1->z - pCenter2->z;
	float len = fx * fx + fy * fy + fz * fz;
	float fr = fRadius1 + fRadius2;
	if (len < fr * fr)
	{
		return true;	// ��������
	}

	return false;		//�������ĂȂ�
}

bool CollisionBCy(XMFLOAT3 * pCenter1, float height1, float fRadius1, XMFLOAT3 * pCenter2, float height2, float fRadius2)
{
	//y�����̔���

	if (pCenter1->y + height1 >=
		pCenter2->y - height2 &&
		pCenter1->y - height1 <=
		pCenter2->y + height2)
	{
		//xz���ʂ̉~�̔���
		float fx = pCenter1->x - pCenter2->x;
		float fz = pCenter1->z - pCenter2->z;
		float len = fx * fx + fz * fz;
		float fr = fRadius1 + fRadius2;
		if (len < fr * fr)
		{
			return true;
		}
	}
	return false;
}

