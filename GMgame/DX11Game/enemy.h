//=============================================================================
//
// 敵処理 [enemy.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//マクロ定義
#define ENEMY_MAX	(10)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
HRESULT InitSelectEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void UpdateSelectEnemy(void);
void DrawEnemy(void);
float ECheckCollisionRay(XMVECTOR pos, float fRadius);

int SetEnemy(XMFLOAT3 pos, XMFLOAT3 rot);

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
void EAction12(bool af);
void EAction13(bool af);
void EAction14(bool af);
bool GetEendflag();
void SetEendflag();