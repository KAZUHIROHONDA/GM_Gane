#include "playercs.h"
#include "MessageManager.h"
#include <string>
#include <sstream>

Player::Player()
{
}

Player::~Player()
{
}

void Player::SetPName(std::string  val)
{
	partsName = val;
}

std::string Player::GetPName()
{
	return  partsName;
}

void Player::SetName(std::string val)
{
	Name = val;
}

std::string Player::GetName()
{
	return  Name;
}

void Player::SetHP()
{
	HP = modeldata[0].GetHP() + modeldata[1].GetHP() + modeldata[2].GetHP() + modeldata[3].GetHP();
}

int Player::GetHP()
{
	return HP;
}

void Player::SetPAat()
{
	PAat = modeldata[0].GetPAat() + modeldata[1].GetPAat() + modeldata[2].GetPAat() + modeldata[3].GetPAat();
}

int Player::GetPAat()
{
	return PAat;
}

void Player::SetGUat()
{
	GUat = modeldata[0].GetGUat() + modeldata[1].GetGUat() + modeldata[2].GetGUat() + modeldata[3].GetGUat();
}

int Player::GetGUat()
{
	return GUat;
}

void Player::SetTYOKIat()
{
	TYOKIat = modeldata[0].GetTYOKIat() + modeldata[1].GetTYOKIat() + modeldata[2].GetTYOKIat() + modeldata[3].GetTYOKIat();
}

int Player::GetTYOKIat()
{
	return TYOKIat;
}

void Player::SetHand()
{
	for (int i = 0; i < 5; i++)
	{
		hand[i] = modeldata[0].GetHand(i);
	}
	for (int i = 5; i < 10; i++)
	{
		hand[i] = modeldata[1].GetHand(i-5);
	}
	for (int i = 10; i < 15; i++)
	{
		hand[i] = modeldata[2].GetHand(i - 10);
	}
	for (int i = 15; i < 20; i++)
	{
		hand[i] = modeldata[3].GetHand(i - 15);
	}
}

int Player::GetHand(int no)
{
	return hand[no];
}

void Player::Setpos(XMFLOAT3 val)
{
	pos = val;
}

XMFLOAT3 Player::Getpos()
{
	return pos;
}

void Player::Setscl(XMFLOAT3 val)
{
	scl = val;
}

XMFLOAT3 Player::Getscl()
{
	return scl;
}

void Player::Setmodel(int val)
{
	switch (val)
	{
	case 0:modeldata[0].KABUHead(); break;
	case 1:modeldata[0].WANIHead(); break;
	case 2:modeldata[0].PANDAHead(); break;
	case 3:modeldata[0].TORIHead(); break;
	case 4:modeldata[0].BUTAHead(); break;
	case 5:modeldata[0].USIHead(); break;
	case 10:modeldata[1].KABUBody(); break;
	case 11:modeldata[1].WANIBody(); break;
	case 12:modeldata[1].PANDABody(); break;
	case 13:modeldata[1].TORIBody(); break;
	case 14:modeldata[1].BUTABody(); break;
	case 15:modeldata[1].USIBody(); break;
	case 20:modeldata[2].KABUMae(); break;
	case 21:modeldata[2].WANIMae(); break;
	case 22:modeldata[2].PANDAMae(); break;
	case 23:modeldata[2].TORIMae(); break;
	case 24:modeldata[2].BUTAMae(); break;
	case 25:modeldata[2].USIMae(); break;
	case 30:modeldata[3].KABUUsiro(); break;
	case 31:modeldata[3].WANIUsiro(); break;
	case 32:modeldata[3].PANDAUsiro(); break;
	case 33:modeldata[3].TORIUsiro(); break;
	case 34:modeldata[3].BUTAUsiro(); break;
	case 35:modeldata[3].USIUsiro(); break;

	default:
		break;
	}
}


void Player::Init()
{
	FontData* fontData = new FontData();
	fontData->fontSize = 20;
	fontData->fontWeight = DWRITE_FONT_WEIGHT_BOLD;
	g_directWrite = new DirectWrite(fontData);
	g_directWrite->Init();
	HP = 0;
	PAat = 0;
	GUat = 0;
	TYOKIat = 0;

}

void Player::Uninit()
{

}

void Player::Update()
{
	SetHP();
	
	SetPAat();
	
	SetGUat();
	
	SetTYOKIat();

	SetHand();

}

void Player::Draw(int x,int y)
{
	std::string status;
	std::ostringstream oss;
	std::ostringstream gu;
	std::ostringstream tyo;
	std::ostringstream pa;

	
	status = "名前:" + Name;
	g_directWrite->DrawString(status,XMFLOAT2(x, y),D2D1_DRAW_TEXT_OPTIONS_NONE);
	gu << GUat;
	status = "グー:" + gu.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
	tyo << TYOKIat;
	status = "チョキ:" + tyo.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 100), D2D1_DRAW_TEXT_OPTIONS_NONE);
	pa << PAat;
	status = "パー:" + pa.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 150), D2D1_DRAW_TEXT_OPTIONS_NONE);

}

void Player::DrawChimera(int x, int y)
{
	std::string status;
	std::ostringstream oss;
	std::ostringstream hp;
	std::ostringstream Gu;
	std::ostringstream Tyo;
	std::ostringstream Pa;
	std::ostringstream percentGu;
	std::ostringstream percentTyoki;
	std::ostringstream percentPa;

	status = "名前:" + Name;
	g_directWrite->DrawString(status, XMFLOAT2(x, y), D2D1_DRAW_TEXT_OPTIONS_NONE);
	hp << HP;
	status = "体力\n";
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 50), D2D1_DRAW_TEXT_OPTIONS_NONE);

	status =  hp.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 100), D2D1_DRAW_TEXT_OPTIONS_NONE);

	status = "攻撃力";
	g_directWrite->DrawString(status, XMFLOAT2(x + 100, y + 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
	Gu << GUat;
	status = "グー　:" + Gu.str();
	g_directWrite->DrawString(status, XMFLOAT2(x + 100, y + 100), D2D1_DRAW_TEXT_OPTIONS_NONE);
	Tyo << TYOKIat;
	status = "チョキ:" + Tyo.str();
	g_directWrite->DrawString(status, XMFLOAT2(x + 100, y + 150), D2D1_DRAW_TEXT_OPTIONS_NONE);
	Pa << PAat;
	status = "パー　:" + Pa.str();
	g_directWrite->DrawString(status, XMFLOAT2(x + 100, y + 200), D2D1_DRAW_TEXT_OPTIONS_NONE);

	percentGu << gu * 100 / 20;
	percentTyoki << tyoki * 100 / 20;
	percentPa << pa * 100 / 20;

	status = "確率";
	g_directWrite->DrawString(status, XMFLOAT2(x + 300, y + 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
	status = "グー　:" + percentGu.str();
	g_directWrite->DrawString(status, XMFLOAT2(x + 300, y + 100), D2D1_DRAW_TEXT_OPTIONS_NONE);
	status = "チョキ:" + percentTyoki.str();
	g_directWrite->DrawString(status, XMFLOAT2(x + 300, y + 150), D2D1_DRAW_TEXT_OPTIONS_NONE);
	status = "パー　:" + percentPa.str();
	g_directWrite->DrawString(status, XMFLOAT2(x + 300, y + 200), D2D1_DRAW_TEXT_OPTIONS_NONE);
}

void Player::Percent()
{
	gu = 0;
	tyoki = 0;
	pa = 0;

	for (int i = 0; i < 20; i++)
	{
		if (hand[i] == 0)
		{
			gu++;
		}
		else if (hand[i] == 1)
		{
			tyoki++;
		}
		else if (hand[i] == 2)
		{
			pa++;
		}
	}
}