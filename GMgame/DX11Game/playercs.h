//=============================================================================
//
// プレイヤのステータス [playercs.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "DirectWrite.h"
#include "modeldata.h"

class Player {
private:

protected:
	std::string partsName;	//パーツの名前
	std::string Name;		//名前
	int	 HP;				//体力
	int  PAat;				//パーの攻撃力
	int  GUat;				//グーの攻撃力
	int  TYOKIat;			//チョキの攻撃力
	int  hand[20];			//5個にする
	XMFLOAT3 pos;			//位置
	XMFLOAT3 scl;			//サイズ


	Modeldata modeldata[4];

	DirectWrite* g_directWrite;



public:
	Player();
	~Player();
	void	SetPName(std::string val);
	std::string	GetPName();
	void	SetName(std::string val);
	std::string	GetName();
	void	SetHP();
	int		GetHP();
	void	SetPAat();
	int		GetPAat();
	void	SetGUat();
	int		GetGUat();
	void	SetTYOKIat();
	int		GetTYOKIat();
	void	SetHand();
	int		GetHand(int no);
	void Setpos(XMFLOAT3 val);
	XMFLOAT3 Getpos();
	void Setscl(XMFLOAT3 val);
	XMFLOAT3 Getscl();

	void    Setmodel(int val);

	void    Init();
	void	Uninit();
	void	Update();
	void	Draw(int x,int y);
	void	DrawChimera(int x, int y);
};
