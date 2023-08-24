//=============================================================================
//
//  ステージ情報を管理する[stage.cpp]
// Author : KOUITI FUZIMURA
//
//=============================================================================
#include "stage.h"
#include "enemy.h"
#include "wall.h"

//グローバル宣言
static int g_nStageNo = 0;	//ステージ番号
static int g_nStageIdx = 0; //ステージ内番号
static int g_nStageCnt = 0; //出現用カウンター

//ステージ情報
const tStage* stage[] =
{
	g_stage01,

	//ステージが追加されたら配列名をここに追加
};

void InitStage()
{
	 g_nStageIdx = 0;
	 g_nStageCnt = 0;
}

void UpdateStage()
{
	if (stage[g_nStageNo][g_nStageIdx].nFrame == -1)return;

	//フレームを進める
	g_nStageCnt++;


	//登場フレーム数になったら
	while(g_nStageCnt >= stage[g_nStageNo][g_nStageIdx].nFrame)
	{
		//これ以上オブジェクトが出ない
		if (stage[g_nStageNo][g_nStageIdx].nFrame == -1)return;

		//種類からオブジェクト出現
		switch (stage[g_nStageNo][g_nStageIdx].nType)
		{
		case	E_OBJECT_ENEMY01:
		{
			const tStage& temp = stage[g_nStageNo][g_nStageIdx];
			SetEnemy(temp.pos);
		}
		break;
	
		}
		//次のオブジェクト設定
		g_nStageIdx++;
	}
}

void SetStageNo(int no)
{
	if (no < 0)return;
	 g_nStageNo = no;
	 g_nStageIdx = 0;
	 g_nStageCnt = 0;
}
