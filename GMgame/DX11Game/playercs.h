//=============================================================================
//
// プレイヤのステータス [playercs.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "DirectWrite.h"

class Player {
private:
	std::string partsName;	//パーツの名前
	std::string Name;		//名前
	int	 HP;				//体力
	int  PAat;				//パーの攻撃力
	int  GUat;				//グーの攻撃力
	int  TYOKIat;			//チョキの攻撃力
	int  hand[20];			//5個にする

	DirectWrite* g_directWrite;

public:
	Player();
	~Player();
	void	SetPName(std::string val);
	std::string	GetPName();
	void	SetName(std::string val);
	std::string	GetName();
	void	SetHP(int val);
	int		GetHP();
	void	SetPAat(int val);
	int		GetPAat();
	void	SetGUat(int val);
	int		GetGUat();
	void	SetTYOKIat(int val);
	int		GetTYOKIat();
	void	SetHand(int *val);
	int		GetHand();

	void    Init();
	void	Uninit();
	void	Update();
	void	Draw(int x,int y);
	void	DrawChimera(int x, int y);
};
