//=============================================================================
//
// スコア画像表示処理 [enemyhp.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "enemyhp.h"
#include"enemy.h"
#include "polygon.h"
#include "Texture.h"
#include "number.h"	// 数字描画に必要
#include"player.h"
#ifdef _DEBUG
#include "debugproc.h"
#endif
#pragma warning(disable:4996)	//おまじない
//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define ENEMYHP_TEXTURENAME	_T("data/texture/frame_score.png")

#define ENEMYHP_SIZE_X	(340)	// 横幅
#define ENEMYHP_SIZE_Y	(80)	// 縦幅
#define ENEMYHP_POS_X	(SCREEN_WIDTH/2-ENEMYHP_SIZE_X/2)	// 初期位置X
#define ENEMYHP_POS_Y	(SCREEN_HEIGHT/2-ENEMYHP_SIZE_Y/2)	// 初期位置Y

#define ENEMYHP_FRAME_X	(1)	// アニメの横の数
#define ENEMYHP_FRAME_Y	(1)	// アニメの縦の数

#define ENEMYHP_DIGIT		(8)	// 桁数

#define NUMBER_POS_X	(ENEMYHP_POS_X-ENEMYHP_DIGIT/2*NUMBER_SIZE_X)
#define NUMBER_POS_Y	(ENEMYHP_POS_Y+NUMBER_SIZE_Y/2-8)

// 構造体定義
typedef struct {
	int	nPatNo;		// 表示パターン番号
	int	nCount;		// 表示フレーム数
}ANIM_PAT;

// スコア構造体
typedef struct _tEnemyhp
{
	XMFLOAT3	Position;	// 座標
	XMFLOAT2	Vel;		// 移動速度
	XMFLOAT2	Size;		// サイズ
	float		Angle;		// 角度
	XMFLOAT2	Scale;		// 拡大率
	int			nDir;		// 方向
	XMFLOAT4	col;

	int			nAnimNo;	// 表示情報No
	int			nCount;		// 残りフレーム数
	bool		bUse;		// 使用フラグ

	int			nEnemyhp;		// 
}tEnemyhp;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static ANIM_PAT g_animPat[][2] = {
	{{0,6},{-1,-1}},
	{{0,6},{-1,-1}},
	{{0,6},{-1,-1}},
	{{0,6},{-1,-1}},
};
tEnemyhp g_enemyhp[ENEMYHP_MAX];	// スコア
//int g_nHienemyhp = 1000;	//ハイスコア

//=============================================================================
// スコア画像表示の初期化処理
//=============================================================================
HRESULT InitEnemyhp()
{
	// テクスチャ読み込み
	HRESULT hr = 
		CreateTextureFromFile(GetDevice(),
		ENEMYHP_TEXTURENAME, &g_pTexture);
	if (FAILED(hr)) return hr;

	// 変数初期化
	for (int i = 0; i < ENEMYHP_MAX; i++)
	{
		g_enemyhp[i].Position = XMFLOAT3(ENEMYHP_POS_X,
			ENEMYHP_POS_Y, 0.0f);
		g_enemyhp[i].Size = XMFLOAT2(ENEMYHP_SIZE_X,
			ENEMYHP_SIZE_Y);
		g_enemyhp[i].Angle = 0.0f;
		g_enemyhp[i].Scale = XMFLOAT2(1.0f, 1.0f);
		g_enemyhp[i].nDir = 0;	// 最初は下

		g_enemyhp[i].nAnimNo = 0;
		g_enemyhp[i].nCount = g_animPat[g_enemyhp[i].nDir][g_enemyhp[i].nAnimNo].nCount;
		// 最初の向きの最初のアニメ表示時間

		g_enemyhp[i].Vel = XMFLOAT2(0.0f, 0.0f);
		g_enemyhp[i].bUse = true;

		g_enemyhp[i].nEnemyhp = 0;
	}

	///g_nHienemyhp = 1000;	//デフォルト値
	//ファイルから読み込み
	FILE* fp;
	if ((fp = fopen("savedata.dat", "r")) != NULL)
	{//オーペン成功
		//fread(&g_nHienemyhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
	return hr;
}

//=============================================================================
// スコア画像表示の終了処理
//=============================================================================
void UninitEnemyhp()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	//ファイルから読み込み
	FILE* fp;
	if ((fp = fopen("savedata.dat", "w")) != NULL)
	{//オーペン成功
		//fwrite(&g_nHienemyhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
}

//=============================================================================
// スコア画像表示の更新処理
//=============================================================================
void UpdateEnemyhp()
{	// 更新処理

	for (int i = 0; i < ENEMYHP_MAX; i++)
	{
		// 使われてなければ何もしない
		// 出ていないスコアは何もしない
		if (g_enemyhp[i].bUse == false) continue;

		g_enemyhp[i].nEnemyhp = GetEnemyHp(i);

	}
}

//=============================================================================
// スコア画像表示処理
//=============================================================================
void DrawEnemyhp()
{
	for (int i = 0; i < ENEMYHP_MAX; i++)
	{
		// 未発射のスコアは表示しない
		if (g_enemyhp[i].bUse == false)	continue;

		// スコア画像描画
		SetPolygonPos(g_enemyhp[i].Position.x, g_enemyhp[i].Position.y);	// 座標
		SetPolygonSize(g_enemyhp[i].Size.x * g_enemyhp[i].Scale.x,
			g_enemyhp[i].Size.y * g_enemyhp[i].Scale.y);	// 大きさ
		SetPolygonAngle(g_enemyhp[i].Angle);				// 角度
		SetPolygonColor(0.0f, 1.0f, 1.0f);	// 色(RGB)
		SetPolygonAlpha(0.0f);				// α値(透明度)

		int u, v;
		u = g_animPat[g_enemyhp[i].nDir][g_enemyhp[i].nAnimNo].nPatNo
			% ENEMYHP_FRAME_X;
		v = g_animPat[g_enemyhp[i].nDir][g_enemyhp[i].nAnimNo].nPatNo
			/ ENEMYHP_FRAME_X;
		SetPolygonUV(
			u / (float)ENEMYHP_FRAME_X,
			v / (float)ENEMYHP_FRAME_Y);			// 0番のテクスチャ座標
		SetPolygonFrameSize(
			1.0f / ENEMYHP_FRAME_X,
			1.0f / ENEMYHP_FRAME_Y);	// テクスチャの横幅,縦幅
		SetPolygonTexture(g_pTexture);		// テクスチャ指定

		// ポリゴンの描画処理
		DrawPolygon(GetDeviceContext());

		// 数字(number)の描画
		XMFLOAT2 pos = XMFLOAT2(
			NUMBER_POS_X,NUMBER_POS_Y);
		
		SetPolygonColor(100.0f, 00.0f, 1.0f);	// 色(RGB)
		SetPolygonAlpha(1.0f);
		DrawNumber(&pos,		// 座標
			g_enemyhp[i].nEnemyhp,	// 数値
			3,					// 桁数
			NUMBER_SIZE_X, NUMBER_SIZE_Y);// サイズ
	}

	//ハイスコア
	//XMFLOAT2 pos = XMFLOAT2(
	//	NUMBER_POS_X, NUMBER_POS_Y - 64);
	//DrawNumber(&pos, g_nHienemyhp, 8);

	// もとに戻す
	SetPolygonUV(0.0f, 0.0f);			// 0番のテクスチャ座標
	SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャの横幅,縦幅
}

/*void AddEnemyhp(int num)
{
	g_enemyhp[0].nEnemyhp += num;
	if (g_enemyhp[0].nEnemyhp < 0)
		g_enemyhp[0].nEnemyhp = 0;
	if (g_enemyhp[0].nEnemyhp >
		pow(10, ENEMYHP_DIGIT) - 1)
	{	// カンスト
		g_enemyhp[0].nEnemyhp =
			pow(10, ENEMYHP_DIGIT) - 1;
	}
	//if (g_enemyhp[0].nEnemyhp > g_nHienemyhp)
	//{  //ハイスコア更新
	//	g_nHienemyhp = g_enemyhp[0].nEnemyhp;

	//}
}*/
