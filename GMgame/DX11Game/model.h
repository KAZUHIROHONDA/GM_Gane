#pragma once

#include "main.h"

//�}�N����`
#define MODEL_MAX	(1)

#define MODEL_KABU		"data/model/kabu2.obj"//������
#define MODEL_KABU1		"data/model/kabuBody.obj"
#define MODEL_KABU2		"data/model/kabuAM.obj"//�O��
#define MODEL_KABU3		"data/model/kabuLG.obj"//���

#define MODEL_wani		"data/model/wani1.obj"
#define MODEL_wani1		"data/model/wani2.obj"
#define MODEL_wani2		"data/model/wani3.obj"
#define MODEL_wani3		"data/model/wani4.obj"


//���f���f�[�^�ǉ�
void	SetHead(std::string val);
std::string	GetHead();
void	SetBody(std::string val);
std::string	GetBody();
void	SetMae(std::string val);
std::string	GetMae();
void	SetBack(std::string val);
std::string	GetBack();

