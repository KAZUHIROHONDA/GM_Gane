//=============================================================================
//
// �v���C���̃X�e�[�^�X [playercs.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "DirectWrite.h"

class Player {
private:
	std::string partsName;	//�p�[�c�̖��O
	std::string Name;		//���O
	int	 HP;				//�̗�
	int  PAat;				//�p�[�̍U����
	int  GUat;				//�O�[�̍U����
	int  TYOKIat;			//�`���L�̍U����
	int  hand[20];			//5�ɂ���

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
