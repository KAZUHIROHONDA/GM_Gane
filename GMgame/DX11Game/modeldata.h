
// モデルデータ [modeldata.h]

#pragma once

#include "main.h"
#include "DirectWrite.h"

class Modeldata 
{
private:
	std::string partsName;	//パーツの名前
	std::string Name;		//名前
	int	 HP;				//体力
	int  PAat;				//パーの攻撃力
	int  GUat;				//グーの攻撃力
	int  TYOKIat;			//チョキの攻撃力
	int  hand[5];			//5個にする
	XMFLOAT3 pos;			//位置
	XMFLOAT3 scl;			//サイズ

public:
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
	void	SetHand(int no, int val);
	int		GetHand(int no);

	void KABUHead();
	void WANIHead();
	void PANDAHead();
	void TORIHead();
	void BUTAHead();
	void USIHead();

	void KABUBody();
	void WANIBody();
	void PANDABody();
	void TORIBody();
	void BUTABody();
	void USIBody();

	void KABUMae();
	void WANIMae();
	void PANDAMae();
	void TORIMae();
	void BUTAMae();
	void USIMae();

	void KABUUsiro();
	void WANIUsiro();
	void PANDAUsiro();
	void TORIUsiro();
	void BUTAUsiro();
	void USIUsiro();
};
