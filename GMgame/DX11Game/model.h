#pragma once

#include "main.h"

//�}�N����`
#define MODEL_MAX	(1)

#define MODEL_KABU		"data/model/kabu2.obj"//������
#define MODEL_KABU1		"data/model/kabuBody.obj"//��
#define MODEL_KABU2		"data/model/kabuAM.obj"//�O��
#define MODEL_KABU3		"data/model/kabuLG.obj"//���

#define MODEL_wani		"data/model/wani1.obj"
#define MODEL_wani1		"data/model/wani2.obj"
#define MODEL_wani2		"data/model/wani3.obj"
#define MODEL_wani3		"data/model/wani4.obj"

#define MODEL_PANDA		"data/model/pandaHD.obj"
#define MODEL_PANDA1	"data/model/pandaBD.obj"
#define MODEL_PANDA2	"data/model/pandaAM.obj"
#define MODEL_PANDA3	"data/model/pandaLG.obj"

#define MODEL_TORI		"data/model/toriHD.obj"
#define MODEL_TORI1		"data/model/toriBD.obj"
#define MODEL_TORI2		"data/model/toriAM.obj"
#define MODEL_TORI3		"data/model/toriLG.obj"

#define MODEL_BUTA		"data/model/butaHD.obj"
#define MODEL_BUTA1		"data/model/butaBD.obj"
#define MODEL_BUTA2		"data/model/butaAM.obj"
#define MODEL_BUTA3		"data/model/butaLG.obj"

#define MODEL_USI		"data/model/ushi_head.obj"
#define MODEL_USI1		"data/model/ushi_body.obj"
#define MODEL_USI2		"data/model/ushi_maeAshi.obj"
#define MODEL_USI3		"data/model/ushi_ushiroAshi.obj"

//���f���f�[�^�ǉ�
void	SetHead(std::string val);
std::string	GetHead();
void	SetBody(std::string val);
std::string	GetBody();
void	SetMae(std::string val);
std::string	GetMae();
void	SetBack(std::string val);
std::string	GetBack();

