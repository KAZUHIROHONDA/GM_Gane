//=============================================================================
//
// �v���C���̃X�e�[�^�X [playercs.h]
//
//=============================================================================
#pragma once

#include "main.h"
#include "DirectWrite.h"
#include "modeldata.h"

class Player {
private:

protected:
	std::string partsName;	//�p�[�c�̖��O
	std::string Name;		//���O
	int	 HP;				//�̗�
	int  PAat;				//�p�[�̍U����
	int  GUat;				//�O�[�̍U����
	int  TYOKIat;			//�`���L�̍U����
	int  hand[20];			//5�ɂ���
	XMFLOAT3 pos;			//�ʒu
	XMFLOAT3 scl;			//�T�C�Y


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
