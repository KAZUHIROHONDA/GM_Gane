//=============================================================================
//
// スコア画像表示処理 [playerhp.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "playerhp.h"
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
#define PLAYERHP_TEXTURENAME	_T("data/texture/frame_score.png")

#define PLAYERHP_SIZE_X	(340)	// 横幅
#define PLAYERHP_SIZE_Y	(80)	// 縦幅
#define PLAYERHP_POS_X	(-SCREEN_WIDTH/2+PLAYERHP_SIZE_X/2)	// 初期位置X
#define PLAYERHP_POS_Y	(-SCREEN_HEIGHT/2+PLAYERHP_SIZE_Y/2)	// 初期位置Y

#define PLAYERHP_FRAME_X	(1)	// アニメの横の数
#define PLAYERHP_FRAME_Y	(1)	// アニメの縦の数

#define PLAYERHP_DIGIT		(8)	// 桁数

#define NUMBER_POS_X	(PLAYERHP_POS_X-PLAYERHP_DIGIT/2*NUMBER_SIZE_X)
#define NUMBER_POS_Y	(PLAYERHP_POS_Y+NUMBER_SIZE_Y/2-8)

// 構造体定義
typedef struct {
	int	nPatNo;		// 表示パターン番号
	int	nCount;		// 表示フレーム数
}ANIM_PAT;

// スコア構造体
typedef struct _tPlayerhp
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

	int			nPlayerhp;		// 
}tPlayerhp;

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
tPlayerhp g_playerhp[PLAYERHP_MAX];	// スコア
//int g_nHiplayerhp = 1000;	//ハイスコア

//=============================================================================
// スコア画像表示の初期化処理
//=============================================================================
HRESULT InitPlayerhp()
{
	// テクスチャ読み込み
	HRESULT hr = 
		CreateTextureFromFile(GetDevice(),
		PLAYERHP_TEXTURENAME, &g_pTexture);
	if (FAILED(hr)) return hr;

	// 変数初期化
	for (int i = 0; i < PLAYERHP_MAX; i++)
	{
		g_playerhp[i].Position = XMFLOAT3(PLAYERHP_POS_X,
			PLAYERHP_POS_Y, 0.0f);
		g_playerhp[i].Size = XMFLOAT2(PLAYERHP_SIZE_X,
			PLAYERHP_SIZE_Y);
		g_playerhp[i].Angle = 0.0f;
		g_playerhp[i].Scale = XMFLOAT2(1.0f, 1.0f);
		g_playerhp[i].nDir = 0;	// 最初は下

		g_playerhp[i].nAnimNo = 0;
		g_playerhp[i].nCount = g_animPat[g_playerhp[i].nDir][g_playerhp[i].nAnimNo].nCount;
		// 最初の向きの最初のアニメ表示時間

		g_playerhp[i].Vel = XMFLOAT2(0.0f, 0.0f);
		g_playerhp[i].bUse = true;

		g_playerhp[i].nPlayerhp = 0;
	}

	///g_nHiplayerhp = 1000;	//デフォルト値
	//ファイルから読み込み
	FILE* fp;
	if ((fp = fopen("savedata.dat", "r")) != NULL)
	{//オーペン成功
		//fread(&g_nHiplayerhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
	return hr;
}

//=============================================================================
// スコア画像表示の終了処理
//=============================================================================
void UninitPlayerhp()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
	//ファイルから読み込み
	FILE* fp;
	if ((fp = fopen("savedata.dat", "w")) != NULL)
	{//オーペン成功
		//fwrite(&g_nHiplayerhp, sizeof(int), 1, fp);
		//fclose(fp);
	}
}

//=============================================================================
// スコア画像表示の更新処理
//=============================================================================
void UpdatePlayerhp()
{	// 更新処理

	for (int i = 0; i < PLAYERHP_MAX; i++)
	{
		// 使われてなければ何もしない
		// 出ていないスコアは何もしない
		if (g_playerhp[i].bUse == false) continue;

		g_playerhp[i].nPlayerhp = GetPlayerHp(i);

	}
}

//=============================================================================
// スコア画像表示処理
//=============================================================================
void DrawPlayerhp()
{
	for (int i = 0; i < PLAYERHP_MAX; i++)
	{
		// 未発射のスコアは表示しない
		if (g_playerhp[i].bUse == false)	continue;

		// スコア画像描画
		SetPolygonPos(g_playerhp[i].Position.x, g_playerhp[i].Position.y);	// 座標
		SetPolygonSize(g_playerhp[i].Size.x * g_playerhp[i].Scale.x,
			g_playerhp[i].Size.y * g_playerhp[i].Scale.y);	// 大きさ
		SetPolygonAngle(g_playerhp[i].Angle);				// 角度
		SetPolygonColor(1.0f, 1.0f, 1.0f);	// 色(RGB)
		SetPolygonAlpha(0.0f);				// α値(透明度)

		int u, v;
		u = g_animPat[g_playerhp[i].nDir][g_playerhp[i].nAnimNo].nPatNo
			% PLAYERHP_FRAME_X;
		v = g_animPat[g_playerhp[i].nDir][g_playerhp[i].nAnimNo].nPatNo
			/ PLAYERHP_FRAME_X;
		SetPolygonUV(
			u / (float)PLAYERHP_FRAME_X,
			v / (float)PLAYERHP_FRAME_Y);			// 0番のテクスチャ座標
		SetPolygonFrameSize(
			1.0f / PLAYERHP_FRAME_X,
			1.0f / PLAYERHP_FRAME_Y);	// テクスチャの横幅,縦幅
		SetPolygonTexture(g_pTexture);		// テクスチャ指定

		// ポリゴンの描画処理
		DrawPolygon(GetDeviceContext());

		// 数字(number)の描画
		XMFLOAT2 pos = XMFLOAT2(
			NUMBER_POS_X,NUMBER_POS_Y);
		
		SetPolygonColor(0.0f, 50.0f, 1.0f);	// 色(RGB)
		SetPolygonAlpha(1.0f);
		DrawNumber(&pos,		// 座標
			g_playerhp[i].nPlayerhp,	// 数値
			3,					// 桁数
			NUMBER_SIZE_X, NUMBER_SIZE_Y);// サイズ
	}

	//ハイスコア
	//XMFLOAT2 pos = XMFLOAT2(
	//	NUMBER_POS_X, NUMBER_POS_Y - 64);
	//DrawNumber(&pos, g_nHiplayerhp, 8);

	// もとに戻す
	SetPolygonUV(0.0f, 0.0f);			// 0番のテクスチャ座標
	SetPolygonFrameSize(1.0f, 1.0f);	// テクスチャの横幅,縦幅
}

/*void AddPlayerhp(int num)
{
	g_playerhp[0].nPlayerhp += num;
	if (g_playerhp[0].nPlayerhp < 0)
		g_playerhp[0].nPlayerhp = 0;
	if (g_playerhp[0].nPlayerhp >
		pow(10, PLAYERHP_DIGIT) - 1)
	{	// カンスト
		g_playerhp[0].nPlayerhp =
			pow(10, PLAYERHP_DIGIT) - 1;
	}
	//if (g_playerhp[0].nPlayerhp > g_nHiplayerhp)
	//{  //ハイスコア更新
	//	g_nHiplayerhp = g_playerhp[0].nPlayerhp;

	//}
}*/
