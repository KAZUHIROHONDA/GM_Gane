//=============================================================================
//
// フェード処理 [fade.cpp]
//
//=============================================================================
#include "fade.h"
#include "polygon.h"	// ポリゴン使用

// マクロ定義
#define FADE_POS_X		(0)		// 表示位置X
#define FADE_POS_Y		(0)		// 表示位置Y
#define FADE_SIZE_X		(SCREEN_WIDTH)	// サイズX
#define FADE_SIZE_Y		(SCREEN_HEIGHT)	// サイズY

#define FADE_RED		(1.0f)
#define FADE_GREEN		(1.0f)
#define FADE_BLUE		(1.0f)

// 構造体定義
// プロトタイプ宣言
// グローバル変数
static float			g_fAlpha = 1.0f;	// 透明度
static E_FADE			g_nFade = E_FADE_IN;	// フェードの状態
static E_TYPE_SCENE		g_stateNext = SCENE_TITLE;	// フェード後の状態
static int				g_nFrame = 30;	// フェードフレーム数

// 初期化処理
HRESULT InitFade( void )
{
	g_fAlpha = 1.0f;				// 最初はフェードインから
	g_nFade = E_FADE_IN;
	g_stateNext = SCENE_TITLE;
	g_nFrame = 30;

	return S_OK;
}

void UninitFade( void )
{
}

void UpdateFade( void )
{
	// １フレームごとの更新処理
	switch( g_nFade )
	{
	case E_FADE_OUT:	// フェードアウト
		
		//α値を増やしていく
		g_fAlpha += 1.0f / g_nFrame;
		
		//フェードアウトの終了のチェック
		if (g_fAlpha >= 1.0f)
		{
			//フェードイン開始
			g_fAlpha = 1.0f;
			g_nFade = E_FADE_IN;
			//画面が隠れた所でシーンの変更
			StartSceneChange(g_stateNext);
		}

		break;
	case E_FADE_IN:	// フェードイン
		
		//α値を減らしていく
		g_fAlpha -= 1.0f / g_nFrame;

		//フェードインの終了のチェック
		if (g_fAlpha <= 0.0f)
		{
			//フェード終了
			g_fAlpha = 0.0f;
			g_nFade = E_FADE_NONE;
		}
		
		break;
	}

}

void DrawFade( void )
{
	// 描画用にデバイスコンテキストを取得
	ID3D11DeviceContext* pDeviceContext
		= GetDeviceContext();

	// 設定
	SetPolygonTexture( nullptr);		// テクスチャセット
	SetPolygonSize( FADE_SIZE_X, FADE_SIZE_Y );	// サイズをセット
	SetPolygonPos(FADE_POS_X, FADE_POS_Y );	// 位置をセット
	SetPolygonColor(FADE_RED, FADE_GREEN, FADE_BLUE);	// ポリゴンの色をセット
	SetPolygonAlpha(g_fAlpha);	// 透明度をセット

	// 描画
	DrawPolygon( pDeviceContext );

	// もとに戻す
	SetPolygonColor( 1.0f, 1.0f, 1.0f );
	SetPolygonAlpha( 1.0f );
}

// フェード開始
// stateNext:
// nFrame:
void StartFade( E_TYPE_SCENE stateNext, int nFrame /*= 30*/ )
{
	if( GetFade() != E_FADE_NONE ) return;
	g_nFade = E_FADE_OUT;
	g_stateNext = stateNext;
		// 0除算回避
	g_nFrame = nFrame;
	
}

// 現在フェード中ですか？
E_FADE GetFade()
{
	return g_nFade;
}

