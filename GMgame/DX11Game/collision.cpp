// collision.cpp
// �����蔻���������
#include "collision.h"
#include "bullet.h"
#include "explosion.h"
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
	int nCnt1, nCnt2;	//���[�v�p�ϐ�
	

	////�v���C���[�ƓG�̓����蔻��
	for (nCnt1 = 0; nCnt1 < ENEMY_MAX; nCnt1++)
	{
		if (!IsEnemy(nCnt1)) continue;
		for (nCnt2 = 0; nCnt2 < PLAYER_MAX; nCnt2++)
		{
			if (!IsPlayer(nCnt2)) continue;

			XMFLOAT3 pos1 = GetEnemyPos(nCnt1);
			float radius1 = GetEnemySize(nCnt1).x + 5;
			float height1 = GetEnemySize(nCnt1).y + 10;
			XMFLOAT3 pos2 = GetPlayerPos(nCnt2);
			float radius2 = GetPlayerSize(nCnt2).x;
			float height2 = GetPlayerSize(nCnt2).y;
			if (CollisionBCy(&pos1, height1, radius1, &pos2, height2, radius2))
			{
				//�m�b�N�o�b�N�p�̃x�N�g�����v�Z
				XMFLOAT3 vec;
				vec.x = pos2.x - pos1.x;
				vec.y = 0.0f;
				vec.z = pos2.z - pos1.z;
				float len = sqrtf(vec.x*vec.x + vec.z*vec.z);
				vec.x /= len;	//�P�ʃx�N�g����
				vec.z /= len;
				vec.x *= 10.0f;	//�m�b�N�o�b�N�p���x
				vec.z *= 10.0f;

				DamagePlayer(nCnt2,vec);
				GetLight()->SetDamageCounter(30);
			}
		}
	}

	//�e�ƓG�̓����蔻��
	for (nCnt1 = 0; nCnt1 < ENEMY_MAX; nCnt1++)
	{
		if (!IsEnemy(nCnt1)) continue;
		for (nCnt2 = 0; nCnt2 < BULLET_MAX; nCnt2++)
		{
			if (!IsBullet(nCnt2)) continue;

			XMFLOAT3 pos1 = GetEnemyPos(nCnt1);
			float radius1 = GetEnemySize(nCnt1).x;
			XMFLOAT3 pos2 = GetBulletPos(nCnt2);
			float radius2 = GetBulletSize(nCnt2).x;
			if (CollisionBS(&pos1, radius1, &pos2, radius2))
			{
				DestroyEnemy(nCnt1);
			}
		}
	}


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

