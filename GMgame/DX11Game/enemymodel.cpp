//=============================================================================
//
// モデルデータ　[enemymodel.cpp]
//
//=============================================================================
#include "enemymodel.h"

#include "model.h"

Model enemychimera[10];

void Enemychimera()
{
	enemychimera[0].SetHead(MODEL_KABU);
	enemychimera[0].SetBody(MODEL_KABU1);
	enemychimera[0].SetMae(MODEL_KABU2);
	enemychimera[0].SetBack(MODEL_KABU3);

	enemychimera[1].SetHead(MODEL_wani);
	enemychimera[1].SetBody(MODEL_wani1);
	enemychimera[1].SetMae(MODEL_wani2);
	enemychimera[1].SetBack(MODEL_wani3);

	enemychimera[2].SetHead(MODEL_PANDA);
	enemychimera[2].SetBody(MODEL_PANDA1);
	enemychimera[2].SetMae(MODEL_PANDA2);
	enemychimera[2].SetBack(MODEL_PANDA3);

	enemychimera[3].SetHead(MODEL_TORI);
	enemychimera[3].SetBody(MODEL_TORI1);
	enemychimera[3].SetMae(MODEL_TORI2);
	enemychimera[3].SetBack(MODEL_TORI3);

	enemychimera[4].SetHead(MODEL_BUTA);
	enemychimera[4].SetBody(MODEL_BUTA1);
	enemychimera[4].SetMae(MODEL_BUTA2);
	enemychimera[4].SetBack(MODEL_BUTA3);

	enemychimera[5].SetHead(MODEL_USI);
	enemychimera[5].SetBody(MODEL_USI1);
	enemychimera[5].SetMae(MODEL_USI2);
	enemychimera[5].SetBack(MODEL_USI3);

	enemychimera[6].SetHead(MODEL_KABU);
	enemychimera[6].SetBody(MODEL_PANDA1);
	enemychimera[6].SetMae(MODEL_USI2);
	enemychimera[6].SetBack(MODEL_TORI3);

	enemychimera[7].SetHead(MODEL_KABU);
	enemychimera[7].SetBody(MODEL_TORI1);
	enemychimera[7].SetMae(MODEL_USI2);
	enemychimera[7].SetBack(MODEL_KABU3);

	enemychimera[8].SetHead(MODEL_KABU);
	enemychimera[8].SetBody(MODEL_KABU1);
	enemychimera[8].SetMae(MODEL_KABU2);
	enemychimera[8].SetBack(MODEL_KABU3);

	enemychimera[9].SetHead(MODEL_KABU);
	enemychimera[9].SetBody(MODEL_KABU1);
	enemychimera[9].SetMae(MODEL_KABU2);
	enemychimera[9].SetBack(MODEL_KABU3);

	enemychimera[10].SetHead(MODEL_KABU);
	enemychimera[10].SetBody(MODEL_KABU1);
	enemychimera[10].SetMae(MODEL_KABU2);
	enemychimera[10].SetBack(MODEL_KABU3);

}

Model * GetEnemychimera(int val)
{
	return &enemychimera[val];
}
