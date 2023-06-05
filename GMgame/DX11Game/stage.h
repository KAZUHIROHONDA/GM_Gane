//=============================================================================
//
// ステージ情報を管理する [stage.h]
// Author : KOUITI FUZIMURA
//
//=============================================================================
#pragma once

#ifndef ___STAGE_H___
#define ___STAGE_H___

#include "main.h"

//構造体定義
typedef struct _tStage
{
	int			nFrame;  //出現フレーム数
	XMFLOAT3	pos;	 //座標
	int			nType;	 //種類
	int			nData;	 //汎用整数データ
	float	    rot;	 //
}tStage;

//種類
enum  //P273 列挙型
{
	E_OBJECT_ENEMY01,

	//追加するならここ
	E_OBJECT_MAX		 //これは必ず最後
};

//プロトタイプ宣言
void InitStage();
void UpdateStage();
void SetStageNo(int no);//ステージ番号設定

//外部ファイルの変数を使用させる	P210
extern const tStage g_stage01[];


		//stage02以降を作成した場合ここに追加

#endif