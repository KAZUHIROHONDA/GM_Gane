// Camera.h
#pragma once
#include "main.h"

#define CAMERA_DEBUG	//
#define CAMERA_MAX (1)

class CCamera
{
private:
	DirectX::XMFLOAT3 m_vPos;		// 視点
	DirectX::XMFLOAT3 m_vTarget;	// 注視点
	DirectX::XMFLOAT3 m_vUp;		// 上方ベクトル

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ワールド マトリックス
	DirectX::XMFLOAT4X4 m_mtxView;	// ビュー マトリックス
	DirectX::XMFLOAT4X4 m_mtxProj;	// プロジェクション マトリックス

	XMFLOAT3 m_vAngle;		// カメラの角度
	float m_fLengthInterval;// カメラの視点と注視点の距離


	int		g_nCnt1;
	int		g_nCnt2;
	int		g_nCnt3;
	int		g_nCnt4;
	int		g_nCnt5;
	int		g_nCnt6;
	
	bool	g_move1;
	bool	g_move2;
	bool	g_move3;
	bool	g_move4;
	bool	g_move5;
	bool	g_move6;

public:
	CCamera();

	void Init();
	void Update();

	void SetPos(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetPos() { return m_vPos; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	void UpdateMatrix();
	DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	void CameraMove1(bool af);
	void CameraMove2(bool af);
	void CameraMove3(bool af);
	void CameraMove4(bool af);
	void CameraMove5(bool af);
	void CameraMove6(bool af);

};

CCamera* GetCamera();
