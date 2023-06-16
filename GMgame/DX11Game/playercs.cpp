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
	HP = val;
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
	oss << HP;
	status = "体力:"+ oss.str();
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
