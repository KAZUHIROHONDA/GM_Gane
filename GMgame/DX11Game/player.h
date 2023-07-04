//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//マクロ定義
#define PLAYER_MAX	(1)

enum
{
	E_CAMERA_VIEW_FIXED,		//固定
	E_CAMERA_VIEW_BIRD,			//バードビュー
	E_CAMERA_VIEW_BEHIND,		//ビハインドビュー
	E_CAMERA_VIEW_FPS,			//FPSビュー
	E_CAMERA_VIEW_CAMERA_DEBUG,	//CAMERA_DEBUG用
	E_CAMERA_VIEW_DIAGONAL,
	E_CAMERA_VIEW_MAX,		//
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void UpdateStart(void);

bool IsPlayer(int no);
void DestroyPlayer(int no);
XMFLOAT3 GetPlayerPos(int no);
XMFLOAT3 GetPlayerSize(int no);

void DamagePlayer(int damage);
void DrawPlayerGauge();

int GetPlayerHp(int no);
void ResetPos(int no);

void Action(bool af);
void Action();
void Action2(bool af);
void Action2();
void Action3(bool af);

void Action4(bool af);

void CameraFIXED();
void CameraDEBUG();