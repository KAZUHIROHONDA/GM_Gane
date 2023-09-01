
// ���f���f�[�^ [modeldata.h]

#pragma once

#include "main.h"
#include "playercs.h"

class Modeldata 
{
public:
	std::string partsName;	//�p�[�c�̖��O
	std::string Name;		//���O
	int	 HP;				//�̗�
	int  PAat;				//�p�[�̍U����
	int  GUat;				//�O�[�̍U����
	int  TYOKIat;			//�`���L�̍U����
	int  hand[5];			//5�ɂ���
	XMFLOAT3 pos;			//�ʒu
	XMFLOAT3 scl;			//�T�C�Y

private:
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
