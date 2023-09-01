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

void Player::SetHP(int val)
{
	HP = model[0].GetHP + model[1].GetHP + model[2].GetHP + model[3].GetHP;
}

int Player::GetHP()
{
	return HP;
}

void Player::SetPAat(int val)
{
	PAat = val;
}

int Player::GetPAat()
{
	return PAat;
}

void Player::SetGUat(int val)
{
	GUat = val;
}

int Player::GetGUat()
{
	return GUat;
}

void Player::SetTYOKIat(int val)
{
	TYOKIat = val;
}

int Player::GetTYOKIat()
{
	return TYOKIat;
}

void Player::SetHand(int no,int val)
{
	hand[no] = val;
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
	case 0:	model[0].KABUHead; break;
	case 1:	model[0].WANIHead; break;
	case 2:	model[0].PANDAHead; break;
	case 3:	model[0].TORIHead; break;
	case 4:	model[0].BUTAHead; break;
	case 5:	model[0].USIHead; break;
	case 10:model[1].KABUBody; break;
	case 11:model[1].WANIBody; break;
	case 12:model[1].PANDABody; break;
	case 13:model[1].TORIBody; break;
	case 14:model[1].BUTABody; break;
	case 15:model[1].USIBody; break;
	case 20:model[2].KABUMae; break;
	case 21:model[2].WANIMae; break;
	case 22:model[2].PANDAMae; break;
	case 23:model[2].TORIMae; break;
	case 24:model[2].BUTAMae; break;
	case 25:model[2].USIMae; break;
	case 30:model[3].KABUUsiro; break;
	case 31:model[3].WANIUsiro; break;
	case 32:model[3].PANDAUsiro; break;
	case 33:model[3].TORIUsiro; break;
	case 34:model[3].BUTAUsiro; break;
	case 35:model[3].USIUsiro; break;

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
	std::ostringstream gu;
	std::ostringstream tyo;
	std::ostringstream pa;

	status = "名前:" + Name;
	g_directWrite->DrawString(status, XMFLOAT2(x, y), D2D1_DRAW_TEXT_OPTIONS_NONE);
	hp << HP;
	status = "体力:" + hp.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 50), D2D1_DRAW_TEXT_OPTIONS_NONE);
	gu << GUat;
	status = "グー:" + gu.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 100), D2D1_DRAW_TEXT_OPTIONS_NONE);
	tyo << TYOKIat;
	status = "チョキ:" + tyo.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 150), D2D1_DRAW_TEXT_OPTIONS_NONE);
	pa << PAat;
	status = "パー:" + pa.str();
	g_directWrite->DrawString(status, XMFLOAT2(x, y + 200), D2D1_DRAW_TEXT_OPTIONS_NONE);

}
