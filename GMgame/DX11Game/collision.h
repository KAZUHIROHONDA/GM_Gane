// collision.h
// �����蔻��p

#pragma once

#include "main.h"

void Collision();	// �����蔻��܂Ƃ�

// �o�E���f�B���O�{�b�N�X(BB)
bool CollisionBB(
	XMFLOAT2* pCenter1, XMFLOAT2* pSize1,
	XMFLOAT2* pCenter2, XMFLOAT2* pSize2 );

// �o�E���f�B���O�T�[�N��(BC)
bool CollisionBC(
	XMFLOAT2* pCenter1, float fRadius1,
	XMFLOAT2* pCenter2, float fRadius2 );

// �o�E���f�B���O�{�b�N�X(BB3D)
bool CollisionBB3D(
	XMFLOAT3* pCenter1, XMFLOAT3* pSize1,
	XMFLOAT3* pCenter2, XMFLOAT3* pSize2);

// �o�E���f�B���O�T�[�N��(BS)
bool CollisionBS(
	XMFLOAT3* pCenter1, float fRadius1,
	XMFLOAT3* pCenter2, float fRadius2);

// �o�E���f�B���O�T�[�N��(BCy)
bool CollisionBCy(
	XMFLOAT3* pCenter1, float height1, float fRadius1,
	XMFLOAT3* pCenter2, float height2, float fRadius2);
