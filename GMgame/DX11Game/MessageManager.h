#pragma once
//MessageManager.h
//メッセージを管理する

//列挙型
enum MESSAGE_STATE
{
	E_NONE,		//何も出ていない
	E_NOW,		//表示中
	E_COMPLETE,	//表示完了
	E_WAIT,		//ENTER入力待ち
	E_TIMER,	//時間待ち
	E_MAX
};

enum MESSAGE_TYPE
{
	E_TYPE_NONE,		//表示なし
	E_TYPE_NORMAL,		//出っぱなし
	E_TYPE_ENTER,		//ENTER入力待ち
	E_TYPE_TIMER,		//時間制限あり
	E_TYPE_MAX
};

//構造体
typedef struct _tMessage
{
	wchar_t			szMessage[256]; //メッセージ
	MESSAGE_TYPE	type;			//タイプ
	unsigned long long int	param;			//汎用
}tMessage;

// プロトタイプ宣言
void InitMessage();
void UninitMessage();
void UpdateMessage();
void DrawMessage();
bool IsMessage();
void SetMessage(tMessage* pMessage);