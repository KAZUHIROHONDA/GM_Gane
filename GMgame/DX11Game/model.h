#pragma once

#include "main.h"

//マクロ定義
#define MODEL_MAX	(1)

#define MODEL_KABU		"data/model/kabu2.obj"//あたま
#define MODEL_KABU1		"data/model/kabuBody.obj"//体
#define MODEL_KABU2		"data/model/kabuAM.obj"//前足
#define MODEL_KABU3		"data/model/kabuLG.obj"//後ろ

#define MODEL_wani		"data/model/wani1.obj"
#define MODEL_wani1		"data/model/wani2.obj"
#define MODEL_wani2		"data/model/wani3.obj"
#define MODEL_wani3		"data/model/wani4.obj"

#define MODEL_PANDA		"data/model/pandaHD.obj"
#define MODEL_PANDA1	"data/model/pandaBD.obj"
#define MODEL_PANDA2	"data/model/pandaAM.obj"
#define MODEL_PANDA3	"data/model/pandaLG.obj"

#define MODEL_TORI		"data/model/toriatama.obj"
#define MODEL_TORI1		"data/model/torikarada.obj"
#define MODEL_TORI2		"data/model/toriude.obj"
#define MODEL_TORI3		"data/model/toriasi.obj"


//モデルデータ追加
void	SetHead(std::string val);
std::string	GetHead();
void	SetBody(std::string val);
std::string	GetBody();
void	SetMae(std::string val);
std::string	GetMae();
void	SetBack(std::string val);
std::string	GetBack();

