// Light.h
#pragma once
#include "main.h"

class CLight
{
private:
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT3 m_direction;
	bool m_bEnable;
	int  m_nCounterDamage;	//ダメージライト用カウンター

public:
	CLight();

	void Init();
	void Update();
	DirectX::XMFLOAT4& GetDiffuse() { return m_diffuse; }
	DirectX::XMFLOAT4& GetAmbient() { return m_ambient; }
	DirectX::XMFLOAT4& GetSpecular() { return m_specular; }
	DirectX::XMFLOAT3& GetDir();
	void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }		//光あり
	void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }	//光なし
	void SetDamageCounter(int num = 8);

};

CLight* GetLight();
