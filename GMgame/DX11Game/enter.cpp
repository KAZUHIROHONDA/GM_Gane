//=============================================================================
//
// エンター表示処理 [enter.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "enter.h"
#include "polygon.h"
#include "Texture.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャファイル名
#define ENTER_TEXTURENAME	_T("data/texture/select.png")

#define ENTER_POS_X	(0)					//初期位置X
#define ENTER_POS_Y	(-200)					//初期位置Y
#define ENTER_SIZE_X	(360)		//横幅
#define ENTER_SIZE_Y	(80)		//縦幅

#define ENTER_FRAME_X (1)			//アニメの横の数
#define ENTER_FRAME_Y (1)			//アニメの縦の数

//構造体定義
typedef struct {
	int nPatNo;		//表示パターン番号
	int nCount;		//表示フレーム数
}ANIM_PAT;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture; // テクスチャ

static ANIM_PAT g_animPat[] = {
	{0,6},
    {-1,-1} //番兵
};

static XMFLOAT3 g_Position;//位置
static float    g_Angle;   //角度
static XMFLOAT2 g_Size;    //サイズ
static XMFLOAT2 g_Scale;   //拡大率 
static float	g_Alpha;   //透過

static int g_nAnimNo;		//表示情報No
static int g_nCount;		//残りフレーム数
static int g_nCnt;

//=============================================================================
// エンター表示の初期化処理
//=============================================================================
HRESULT InitEnter()
{
	HRESULT hr;

	// テクスチャ読み込み
	hr = CreateTextureFromFile(GetDevice(), ENTER_TEXTURENAME, &g_pTexture);
	if (FAILED(hr))return hr;
	
	//変数初期化
	g_Position = XMFLOAT3(ENTER_POS_X, ENTER_POS_Y, 0.0f);
	g_Size = XMFLOAT2(ENTER_SIZE_X, ENTER_SIZE_Y);
	g_Angle = 0.0f;
	g_Scale = XMFLOAT2(1.0f, 1.0f);

	g_nAnimNo = 0;
	g_nCount = g_animPat[g_nAnimNo].nCount;
	g_Alpha = 1.0f;
				//最初のアニメ表示時間

	return hr;
}

//=============================================================================
// エンター表示の終了処理
//=============================================================================
void UninitEnter()
{
	// テクスチャ開放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// エンター表示の更新処理
//=============================================================================
void UpdateEnter()
{
	g_nCnt++;

	if (g_Alpha > 0)
	{
		g_Alpha -= 0.01;
	}
	else if (g_Alpha < 0)
	{
		g_Alpha = 1.0;
	}
	

	// アニメーション情報の更新
	g_nCount--;		//残りカウントを減らす
	if (g_nCount <= 0)
	{
		//今の絵は終わり
		g_nAnimNo++;		//次の絵に進める
		if (g_animPat[g_nAnimNo].nPatNo < 0)
		{
			//番兵を参照したので
			g_nAnimNo = 0;	//先頭に戻す
		}
		//カウンタを次の絵の表示フレーム数に設定
		g_nCount = g_animPat[g_nAnimNo].nCount;
	}


}

//=============================================================================
// エンター表示処理
//=============================================================================
void DrawEnter()
{
	// エンター描画
	//ポリゴン情報設定
	SetPolygonPos(g_Position.x, g_Position.y);			//座標
	SetPolygonSize(g_Size.x*g_Scale.x, g_Size.y*g_Scale.y);		//大きさ
	SetPolygonAngle(g_Angle);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(g_Alpha);				//α値(透明度)

	int u, v;
	u = g_animPat[g_nAnimNo].nPatNo % ENTER_FRAME_X;
	v = g_animPat[g_nAnimNo].nPatNo / ENTER_FRAME_X;
	SetPolygonUV(u / (float)ENTER_FRAME_X,v / (float)ENTER_FRAME_Y);	
	SetPolygonFrameSize(1.0f/ENTER_FRAME_X, 1.0f/ ENTER_FRAME_Y);	//テクスチャ
	SetPolygonTexture(g_pTexture);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

	//元に戻す
	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ

}

void DrawEnter1()
{
	// エンター描画
	//ポリゴン情報設定
	SetPolygonPos(0, -200);			//座標
	SetPolygonSize(g_Size.x*g_Scale.x, g_Size.y*g_Scale.y);		//大きさ
	SetPolygonAngle(g_Angle);				//角度
	SetPolygonColor(1.0f, 1.0f, 1.0f);	//色(RGB)
	SetPolygonAlpha(g_Alpha);				//α値(透明度)

	int u, v;
	u = g_animPat[g_nAnimNo].nPatNo % ENTER_FRAME_X;
	v = g_animPat[g_nAnimNo].nPatNo / ENTER_FRAME_X;
	SetPolygonUV(u / (float)ENTER_FRAME_X, v / (float)ENTER_FRAME_Y);
	SetPolygonFrameSize(1.0f / ENTER_FRAME_X, 1.0f / ENTER_FRAME_Y);	//テクスチャ
	SetPolygonTexture(g_pTexture);		//テクスチャ

	//ポリゴンの描画処理
	DrawPolygon(GetDeviceContext());

	//元に戻す
	SetPolygonUV(0.0f, 0.0f);			//0番のテクスチャ
	SetPolygonFrameSize(1.0f, 1.0f);	//テクスチャ
}


// エンター描画までの手順
// ① enter.h にenter.cppにある関数のプロトタイプ宣言を書く
// ② main.cpp でenter.cppの初期化、終了、更新、描画を呼び出す
// ③ enter.cpp に座標とサイズとテクスチャーファイル名の定数を書く
// ④ InitENTER()関数を実装する
// ⑤ DrawENTER()関数を実装する
