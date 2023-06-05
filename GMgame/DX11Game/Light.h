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
	int  m_nCounterDamage;	//�_���[�W���C�g�p�J�E���^�[

public:
	CLight();

	void Init();
	void Update();
	DirectX::XMFLOAT4& GetDiffuse() { return m_diffuse; }
	DirectX::XMFLOAT4& GetAmbient() { return m_ambient; }
	DirectX::XMFLOAT4& GetSpecular() { return m_specular; }
	DirectX::XMFLOAT3& GetDir();
	void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }		//������
	void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }	//���Ȃ�
	void SetDamageCounter(int num = 8);

};

CLight* GetLight();
