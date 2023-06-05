// light.cpp
#include "Light.h"
#include "input.h"

using namespace DirectX;

// マクロ定義
#define LIGHT0_DIRECTION	XMFLOAT3(-1.0f, -1.0f, 1.0f)
#define LIGHT0_DIFFUSE		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_AMBIENT		XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f)
#define LIGHT0_SPECULAR		XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f)

// グローバル変数
static CLight g_light;

// コンストラクタ
CLight::CLight()
{
	Init();
}

// 初期化
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	m_bEnable = true;
	m_nCounterDamage = 0;

}

void CLight::Update()
{
	//
	//XMFLOAT3 vDir = LIGHT0_DIRECTION;
	//static float rad;
	//rad += 0.1f;
	//vDir.x = sinf(rad);
	//XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));

	////ボタン押下で色を変える
	//if (GetKeyPress(VK_R))
	//{
	//	m_diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//}
	//else
	//{
	//	m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//}

	//ダメージライト
	if (m_nCounterDamage > 0)
	{
		m_nCounterDamage--;      //左は右の倍の値
		if (m_nCounterDamage % 8 / 4 == 0)
		{//白
			m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{//赤
			m_diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}

	}
}

// 光源方向取得
XMFLOAT3& CLight::GetDir()
{
	if (m_bEnable) return m_direction;
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

void CLight::SetDamageCounter(int num)
{
	if (num < 0) num = 0;
	m_nCounterDamage = num;
}


CLight * GetLight()
{
	return &g_light;
}
