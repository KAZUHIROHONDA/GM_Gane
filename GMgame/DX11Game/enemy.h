//=============================================================================
//
// 敵処理 [enemy.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//マクロ定義
#define ENEMY_MAX	(1)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

//int SetEnemy(XMFLOAT3 pos,int no);

bool IsEnemy(int no);
void DestroyEnemy(int no);
XMFLOAT3 GetEnemyPos(int no);
XMFLOAT3 GetEnemySize(int no);

void DamageEnemy(int damage);

int GetEnemyHp(int no);

void DrawEnemyEGauge();

void ResetEPos(int no);

void EAction(bool af);
void EAction();
void EAction2(bool af);
void EAction2();
void EAction3(bool af);
void EAction4(bool af);
void EAction5(bool af);
void EAction6(bool af);
void EAction7(bool af);
void EAction8(bool af);
void EAction9(bool af);
void EAction10(bool af);
void EAction11(bool af);