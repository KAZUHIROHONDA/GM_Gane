//=============================================================================
//
// 敵処理 [enemyselect.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//マクロ定義
#define ENEMYSEL_MAX	(10)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSelectEnemyselect(void);
void UninitEnemyselect(void);
void UpdateSelectEnemyselect(void);
void DrawEnemyselect(void);
float ECheckCollisionRay1(XMVECTOR pos, float fRadius);

int SetEnemyselect(XMFLOAT3 pos, XMFLOAT3 rot);

bool IsEnemyselect(int no);
XMFLOAT3 GetEnemyselectPos(int no);
XMFLOAT3 GetEnemyselectSize(int no);

int Enemyno();
void SelectEnemy(int val);
