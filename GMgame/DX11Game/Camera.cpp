// Camera.cpp
#include "Camera.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_X_CAM		(0.0f)										// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM		(100.0f)									// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM		(-200.0f)									// �J�����̏����ʒu(Z���W)
#define	VIEW_ANGLE		(45.0f)										// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)										// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)									// �r���[���ʂ�FarZ�l

#define CAMERA_MOVE_SPEED	2.0f // �ړ��̑���

// �O���[�o���ϐ�
static CCamera  g_camera;	// �J�����̎��� 

// �R���X�g���N�^
CCamera::CCamera()
{
	Init();
}

// ������
void CCamera::Init()
{
	m_vPos = XMFLOAT3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);	// ���_
	m_vTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);		// ����x�N�g��

	m_fAspectRatio = VIEW_ASPECT;			// �c����
	m_fFovY = VIEW_ANGLE;					// ����p(Degree)
	m_fNearZ = VIEW_NEAR_Z;					// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;					// ����N���b�v����

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fLengthInterval = 
		sqrtf((m_vPos.x - m_vTarget.x)*(m_vPos.x - m_vTarget.x)
		+ (m_vPos.z - m_vTarget.z)*(m_vPos.z - m_vTarget.z));
	
	CalcWorldMatrix();
}

//float dx = 0.0f, dy = 0.0f, dz = 0.0f;
// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CCamera::Update()
{
#ifdef CAMERA_DEBUG
	
#endif

	// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
	UpdateMatrix();
}

void CCamera::UpdateSet()
{


	//�J�����ړ�
	float dx = 0.0f, dy = 0.0f, dz = 0.0f;



	//����Ȃ���E
		if (GetKeyTrigger(VK_I))
		{
			g_nCnt3 = 0;
			g_move3 = true;
		}
		CCamera::CameraMove3(g_move3);
	//
		if (GetKeyTrigger(VK_K))
		{
			g_nCnt4 = 0;
			g_move4 = true;
		}
		CCamera::CameraMove4(g_move4);

		if (GetKeyTrigger(VK_L))
		{
			//�E
			g_nCnt5 = 0;
			g_move5 = true;
		}
		CCamera::CameraMove5(g_move5);
		if (GetKeyTrigger(VK_J))
		{
			//��
			g_nCnt6 = 0;
			g_move6 = true;
		}
		CCamera::CameraMove6(g_move6);
		if (GetKeyPress(VK_Y))
		{
			dy += CAMERA_MOVE_SPEED;
		}
		if (GetKeyPress(VK_N))
		{
			dy -= CAMERA_MOVE_SPEED;
		}
		//����
		if (GetKeyTrigger(VK_U))
		{
			g_nCnt1 = 0;
			g_move1 = true;
		}
		CCamera::CameraMove1(g_move1);

		if (GetKeyTrigger(VK_O))
		{
			g_nCnt2 = 0;
			g_move2 = true;
		}
		CCamera::CameraMove2(g_move2);
	

	//���W += ���x
	m_vPos.x += dx;
	m_vPos.y += dy;
	m_vPos.z += dz;

	//�����_�ړ�
	dx = dy = dz = 0.0f;


	m_vTarget.x += dx;
	m_vTarget.y += dy;
	m_vTarget.z += dz;

	//���Z�b�g
	if (GetKeyTrigger(VK_SPACE))
	{
		m_vPos = XMFLOAT3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		m_vTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}


	// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
	UpdateMatrix();
}


// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CCamera::UpdateMatrix()
{
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}



// ���[���h �}�g���b�N�X�ݒ�
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// ���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}

CCamera * GetCamera()
{
	return &g_camera;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void CCamera::CameraMove1(bool af)
{
	if (af == true)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			if (g_nCnt1 <= 300)
			{
				m_vAngle.y += 0.3f;

				if (m_vAngle.y >= 360.0f)
					m_vAngle.y -= 360.0f;
				m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y)*m_fLengthInterval;
				m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y)*m_fLengthInterval;
				g_nCnt1++;
			}
			else
			{
				g_nCnt5 = 0; g_move5 = true;
				g_move1 = false;
			}
			
		}
		
	}
	//return;
}

void CCamera::CameraMove2(bool af)
{
	if (af == true)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			g_nCnt2++;
			if (g_nCnt2 <= 340)
			{
				m_vAngle.y -= 0.3f;

				if (m_vAngle.y < 0.0f)
					m_vAngle.y += 360.0f;
				m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y)*m_fLengthInterval;
				m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y)*m_fLengthInterval;
			}
			else
			{
				g_nCnt4 = 0; g_move4 = true;
				g_move2 = false;
			}

		}

	}
	//return;
}



void CCamera::CameraMove3(bool af)
{

	if (af == true)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			g_nCnt3++;
			if (g_nCnt3 <= 150)
			{
				m_vPos.x += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y);
				m_vPos.z += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y);
			}
			else
			{
				g_nCnt2 = 0; g_move2 = true;
				g_move3 = false;
			}
		}

	}

	//return;
}

void CCamera::CameraMove4(bool af)
{
	if (af == true)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			g_nCnt4++;
			if (g_nCnt4 <= 200)
			{
				m_vPos.x += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y + 180);
				m_vPos.z += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y + 180);
			}
			else
			{
				g_nCnt1 = 0; g_move1 = true;
				g_move4 = false;
			}
		}


	}
	//return;
}

void CCamera::CameraMove5(bool af)
{
	if (af == true)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			
			if (g_nCnt5 <= 150)
			{
				m_vPos.x += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y + 30);
				m_vPos.z += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y + 30);
				g_nCnt5++;
			}
			else
			{
				g_nCnt6 = 0; g_move6 = true;
				g_move5 = false;
			}
		}

	}
	//return;
}

void CCamera::CameraMove6(bool af)
{
	if (af == true)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			g_nCnt6++;
			if (g_nCnt6 <= 200)
			{
				m_vPos.x += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y - 30);
				m_vPos.z += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y - 30);
			}
			else
			{
				g_nCnt3 = 0; g_move3 = true;
				g_move6 = false;
			}

		}

	}
	//return;
}