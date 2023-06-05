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

// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CCamera::Update()
{
#ifdef CAMERA_DEBUG
	//CAMERA_��Camera.h�Œ�`����Ă���Ǝ��s�����

	//�J�����ړ�
	float dx = 0.0f, dy = 0.0f, dz = 0.0f;

	if (GetKeyPress(VK_I))
	{
		//�܂�
		dx += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y);
		dz += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y);
	}
	if (GetKeyPress(VK_K))
	{
		//��
		dx += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y+180);
		dz += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y+180);
	}
	if (GetKeyPress(VK_L))
	{
		//�E
		dx += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y + 90);
		dz += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y + 90);
	}
	if (GetKeyPress(VK_J))
	{
		//��
		dx += CAMERA_MOVE_SPEED * SinDeg(m_vAngle.y - 90);
		dz += CAMERA_MOVE_SPEED * CosDeg(m_vAngle.y - 90);
	}
	if (GetKeyPress(VK_Y))
	{
		dy += CAMERA_MOVE_SPEED;
	}
	if (GetKeyPress(VK_N))
	{
		dy -= CAMERA_MOVE_SPEED;
	}
	//����
	if (GetKeyPress(VK_U))
	{
		m_vAngle.y += 1.0f;

		if (m_vAngle.y >= 360.0f)
			m_vAngle.y -= 360.0f;
			m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y)*m_fLengthInterval;
			m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y)*m_fLengthInterval;	
	}
	if (GetKeyPress(VK_O))
	{
		m_vAngle.y -= 1.0f;

		if (m_vAngle.y < 0.0f)
			m_vAngle.y += 360.0f;
		m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y)*m_fLengthInterval;
		m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y)*m_fLengthInterval;

	}

	//���W += ���x
	m_vPos.x += dx;
	m_vPos.y += dy;
	m_vPos.z += dz;

	//�����_�ړ�
	dx = dy = dz = 0.0f;

	if (GetKeyPress(VK_T))
	{
		dy += CAMERA_MOVE_SPEED;
	}
	if (GetKeyPress(VK_B))
	{
		dy -= CAMERA_MOVE_SPEED;
	}

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


	// �f�o�b�O�\��
	PrintDebugProc("[��ײ� : (%7.2f, %7.2f, %7.2f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	PrintDebugProc("[������: (%7.2f, %7.2f, %7.2f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	PrintDebugProc("[���ѷ : (%7.2f)]\n", m_vAngle.y);
	PrintDebugProc("\n");

	PrintDebugProc("*** ��� ��� ***\n");
	PrintDebugProc("ϴ   ��޳ : I\n");
	PrintDebugProc("���  ��޳ : K\n");
	PrintDebugProc("���� ��޳ : J\n");
	PrintDebugProc("з�  ��޳ : L\n");
	PrintDebugProc("��   ��޳ : Y\n");
	PrintDebugProc("��   ��޳ : N\n");
	PrintDebugProc("���� �ݶ� : U\n");
	PrintDebugProc("з�  �ݶ� : O\n");
	PrintDebugProc("\n");

	PrintDebugProc("*** ������ ��� ***\n");
	PrintDebugProc("��   ��޳ : T\n");
	PrintDebugProc("��   ��޳ : B\n");
	PrintDebugProc("\n");

	PrintDebugProc("ؾ��      : SPACE\n");
	PrintDebugProc("\n");

#endif

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
