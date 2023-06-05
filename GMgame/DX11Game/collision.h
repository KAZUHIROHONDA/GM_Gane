// collision.h
// 当たり判定用

#pragma once

#include "main.h"

void Collision();	// 当たり判定まとめ

// バウンディングボックス(BB)
bool CollisionBB(
	XMFLOAT2* pCenter1, XMFLOAT2* pSize1,
	XMFLOAT2* pCenter2, XMFLOAT2* pSize2 );

// バウンディングサークル(BC)
bool CollisionBC(
	XMFLOAT2* pCenter1, float fRadius1,
	XMFLOAT2* pCenter2, float fRadius2 );

// バウンディングボックス(BB3D)
bool CollisionBB3D(
	XMFLOAT3* pCenter1, XMFLOAT3* pSize1,
	XMFLOAT3* pCenter2, XMFLOAT3* pSize2);

// バウンディングサークル(BS)
bool CollisionBS(
	XMFLOAT3* pCenter1, float fRadius1,
	XMFLOAT3* pCenter2, float fRadius2);

// バウンディングサークル(BCy)
bool CollisionBCy(
	XMFLOAT3* pCenter1, float height1, float fRadius1,
	XMFLOAT3* pCenter2, float height2, float fRadius2);
