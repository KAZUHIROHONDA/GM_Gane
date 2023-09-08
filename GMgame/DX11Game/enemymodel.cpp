//=============================================================================
//
// モデルデータ　[enemymodel.cpp]
//
//=============================================================================
#include "enemymodel.h"
#include "playercs.h"

#include "model.h"

Model enemychimera[10];
XMFLOAT3 enemy[44];


void Enemychimera()
{
	enemychimera[0].SetHead(MODEL_KABU);
	enemychimera[0].SetBody(MODEL_KABU1);
	enemychimera[0].SetMae(MODEL_KABU2);
	enemychimera[0].SetBack(MODEL_KABU3);

	enemy[0]=(XMFLOAT3(8.0f, 8.0f, 8.0f));
	enemy[1]=(XMFLOAT3(0.0f, 0.0f, -2.0f));
	enemy[2]=(XMFLOAT3(0.0f, -0.8f, -1.0f));
	enemy[3]=(XMFLOAT3(0.0f, -0.8f, 0.0f));

	enemychimera[1].SetHead(MODEL_wani);
	enemychimera[1].SetBody(MODEL_wani1);
	enemychimera[1].SetMae(MODEL_wani2);
	enemychimera[1].SetBack(MODEL_wani3);

	enemy[4]=(XMFLOAT3(15.0f, 15.0f, 15.0f));
	enemy[5]=(XMFLOAT3(0.0f, 0.0f, -1.0f));
	enemy[6]=(XMFLOAT3(0.0f, -0.5f, 0.0f));
	enemy[7]=(XMFLOAT3(0.0f, -0.5f, 1.5f));

	enemychimera[2].SetHead(MODEL_PANDA);
	enemychimera[2].SetBody(MODEL_PANDA1);
	enemychimera[2].SetMae(MODEL_PANDA2);
	enemychimera[2].SetBack(MODEL_PANDA3);

	enemy[8]=(XMFLOAT3(16.0f, 16.0f, 16.0f));
	enemy[9]=(XMFLOAT3(0.0f, 0.0f, -1.0f));
	enemy[10]=(XMFLOAT3(0.0f, -0.5f, -0.7f));
	enemy[11]=(XMFLOAT3(0.0f, -0.5f, 0.5f));

	enemychimera[3].SetHead(MODEL_TORI);
	enemychimera[3].SetBody(MODEL_TORI1);
	enemychimera[3].SetMae(MODEL_TORI2);
	enemychimera[3].SetBack(MODEL_TORI3);

	enemy[12]=(XMFLOAT3(7.0f, 7.0f, 7.0f));
	enemy[13]=(XMFLOAT3(0.0f, 1.0f, -0.3f));
	enemy[14]=(XMFLOAT3(0.0f, 0.0f, 0.5f));
	enemy[15]=(XMFLOAT3(0.0f, -1.0f, 0.0f));

	enemychimera[4].SetHead(MODEL_BUTA);
	enemychimera[4].SetBody(MODEL_BUTA1);
	enemychimera[4].SetMae(MODEL_BUTA2);
	enemychimera[4].SetBack(MODEL_BUTA3);

	enemy[16]=(XMFLOAT3(16.0f, 16.0f, 16.0f));
	enemy[17]=(XMFLOAT3(0.0f, 0.0f, -1.0f));
	enemy[18]=(XMFLOAT3(0.0f, -0.3f, -0.5f));
	enemy[19]=(XMFLOAT3(0.0f, -0.3f, 0.44f));

	enemychimera[5].SetHead(MODEL_USI);
	enemychimera[5].SetBody(MODEL_USI1);
	enemychimera[5].SetMae(MODEL_USI2);
	enemychimera[5].SetBack(MODEL_USI3);

	enemy[20]=(XMFLOAT3(7.0f, 7.0f, 7.0f));
	enemy[21]=(XMFLOAT3(0.0f, 0.0f, -1.0f));
	enemy[22]=(XMFLOAT3(0.0f, -1.2f, 0.6f));
	enemy[23]=(XMFLOAT3(0.0f, -1.2f, 3.8f));

	enemychimera[6].SetHead(MODEL_KABU);
	enemychimera[6].SetBody(MODEL_PANDA1);
	enemychimera[6].SetMae(MODEL_USI2);
	enemychimera[6].SetBack(MODEL_TORI3);

	enemy[24]=(XMFLOAT3(8.0f, 8.0f, 8.0f));
	enemy[25]=(XMFLOAT3(0.0f, 0.0f, -2.0f));
	enemy[26]=(XMFLOAT3(0.0f, -0.8f, -1.0f));
	enemy[27]=(XMFLOAT3(0.0f, -0.8f, 0.0f));

	enemychimera[7].SetHead(MODEL_KABU);
	enemychimera[7].SetBody(MODEL_TORI1);
	enemychimera[7].SetMae(MODEL_USI2);
	enemychimera[7].SetBack(MODEL_KABU3);

	enemy[28]=(XMFLOAT3(8.0f, 8.0f, 8.0f));
	enemy[29]=(XMFLOAT3(0.0f, 0.0f, -2.0f));
	enemy[30]=(XMFLOAT3(0.0f, -0.8f, -1.0f));
	enemy[31]=(XMFLOAT3(0.0f, -0.8f, 0.0f));

	enemychimera[8].SetHead(MODEL_KABU);
	enemychimera[8].SetBody(MODEL_KABU1);
	enemychimera[8].SetMae(MODEL_KABU2);
	enemychimera[8].SetBack(MODEL_KABU3);

	enemy[32]=(XMFLOAT3(8.0f, 8.0f, 8.0f));
	enemy[33]=(XMFLOAT3(0.0f, 0.0f, -2.0f));
	enemy[34]=(XMFLOAT3(0.0f, -0.8f, -1.0f));
	enemy[35]=(XMFLOAT3(0.0f, -0.8f, 0.0f));

	enemychimera[9].SetHead(MODEL_KABU);
	enemychimera[9].SetBody(MODEL_KABU1);
	enemychimera[9].SetMae(MODEL_KABU2);
	enemychimera[9].SetBack(MODEL_KABU3);

	enemy[36]=(XMFLOAT3(8.0f, 8.0f, 8.0f));
	enemy[37]=(XMFLOAT3(0.0f, 0.0f, -2.0f));
	enemy[38]=(XMFLOAT3(0.0f, -0.8f, -1.0f));
	enemy[39]=(XMFLOAT3(0.0f, -0.8f, 0.0f));

	enemychimera[10].SetHead(MODEL_KABU);
	enemychimera[10].SetBody(MODEL_KABU1);
	enemychimera[10].SetMae(MODEL_KABU2);
	enemychimera[10].SetBack(MODEL_KABU3);

	enemy[40]=(XMFLOAT3(8.0f, 8.0f, 8.0f));
	enemy[41]=(XMFLOAT3(0.0f, 0.0f, -2.0f));
	enemy[42]=(XMFLOAT3(0.0f, -0.8f, -1.0f));
	enemy[43]=(XMFLOAT3(0.0f, -0.8f, 0.0f));

}

Model * GetEnemychimera(int val)
{
	return &enemychimera[val];
}

XMFLOAT3  EnemyScal(int val)
{
	return enemy[val];
}
