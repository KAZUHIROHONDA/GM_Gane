//=============================================================================
//
// �G�t�F�N�V�A�̃G�t�F�N�g���� [effect.cpp]
// Shun Miura
//
//=============================================================================
#include "effect.h"
#include "input.h"
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "Camera.h"

// �}�N��
#define	EFFECT_MAX		(E_EFFECT_MAX)			// �G�t�F�N�g�̍ő吔
#define	EFFECT_TYPE_0	(u"data/effect/biiim.efkefc") // �{�ۃR�s����r�[��
#define	EFFECT_TYPE_1	(u"data/effect/Aura01.efkefc")  // ����
#define	EFFECT_TYPE_2	(u"data/effect/Laser03.efkefc")    // �q�[���G���A
#define	EFFECT_TYPE_3	(u"data/effect/biiim.efkefc")    // �q�[���G���A



// �O���[�o���ϐ�
// Effekseer�̏������p�ϐ�
Effekseer::ManagerRef g_efkManager;
EffekseerRendererDX11::RendererRef g_efkRenderer;
Effekseer::Handle g_efcHandle;
Effekseer::EffectRef g_effect[EFFECT_MAX];
// ����������
HRESULT InitEffect( void )
{
	HRESULT hr = S_OK;

	g_efkManager = Effekseer::Manager::Create(8000);

	auto graphicsDevice = EffekseerRendererDX11::CreateGraphicsDevice(
		GetDevice(), GetDeviceContext());// ������11�̃R���e�L�X�g�ƃf�o�C�X

	g_efkRenderer = EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	g_efkManager->SetSpriteRenderer(g_efkRenderer->CreateSpriteRenderer());
	g_efkManager->SetRibbonRenderer(g_efkRenderer->CreateRibbonRenderer());
	g_efkManager->SetRingRenderer(g_efkRenderer->CreateRingRenderer());
	g_efkManager->SetTrackRenderer(g_efkRenderer->CreateTrackRenderer());
	g_efkManager->SetModelRenderer(g_efkRenderer->CreateModelRenderer());

	g_efkManager->SetTextureLoader(g_efkRenderer->CreateTextureLoader());
	g_efkManager->SetModelLoader(g_efkRenderer->CreateModelLoader());
	g_efkManager->SetMaterialLoader(g_efkRenderer->CreateMaterialLoader());
	g_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	// �T�E���h�̓I�[�f�B�I2�炵���̂ŏȗ�
	
	g_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);
	g_effect[0] = Effekseer::Effect::Create(g_efkManager, EFFECT_TYPE_0);// �ǂݍ��ރG�t�F�N�g�̃p�X��������2�ڂ�
	g_effect[1] = Effekseer::Effect::Create(g_efkManager, EFFECT_TYPE_1);
	g_effect[2] = Effekseer::Effect::Create(g_efkManager, EFFECT_TYPE_2);
	g_effect[3] = Effekseer::Effect::Create(g_efkManager, EFFECT_TYPE_3);

	
	return hr;
}

void UninitEffect( void )
{
	g_effect[0].Reset();
	g_effect[1].Reset();
	g_effect[2].Reset();
	g_effect[3].Reset();
	g_efkRenderer.Reset();
	g_efkManager.Reset();
}

void UpdateEffect( void )
{
	static float time = 0.0f;

	g_efkManager->Update();
	g_efkRenderer->SetTime(time);
}

void DrawEffect( void )
{
	Effekseer::Vector3D eye, look, up;
	eye = Effekseer::Vector3D(GetCamera()->GetPos().x, GetCamera()->GetPos().y, GetCamera()->GetPos().z);
	look = Effekseer::Vector3D(GetCamera()->GetTarget().x, GetCamera()->GetTarget().y, GetCamera()->GetTarget().z); 
	up = Effekseer::Vector3D(0, 1, 0);
	g_efkRenderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
	g_efkRenderer->SetProjectionMatrix(
	//Effekseer::Matrix44().PerspectiveFovLH(XMConvertToRadians(45), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f));
	// DirectX�͍�����W�n�H�H
	// �G�t�F�N�V�A�̏����o���ɂ��E��n������n�I�ׂ邩��

	Effekseer::Matrix44().PerspectiveFovLH(45.0f * 3.141592f / 180, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 10000.0f));
	
	// �G�t�F�N�V�A�J����*******************
	//Effekseer::Matrix44 cameraMatrix;
	//cameraMatrix.LookAtRH(viewerPosition,
	//	Eyffekseer::Vector3D(0.0f, 0.0f, 0.0f),
	//	Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
	 //�J�����ړ�������ꏏ�Ɉړ�***********
	//g_efkRenderer->SetProjectionMatrix(projectionMatrix);
	//g_efkRenderer->SetCameraMatrix(cameraMatrix);

	Effekseer::Manager::LayerParameter layerParameter;
	//layerParameter.ViewerPosition = viewerPosition;
	layerParameter.ViewerPosition = eye; // �ǉ�
	g_efkManager->SetLayerParameter(0, layerParameter);

	//Effekseer::Manager::UpdateParameter updateParameter;
	//g_efkManager->Update(updateParameter); // �����͂���Ȃ���������Ȃ�

	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = g_efkRenderer->GetCameraProjectionMatrix();
	g_efkRenderer->BeginRendering();
	g_efkManager->Draw();
	g_efkRenderer->EndRendering();
}

void SetEffect(int no,XMFLOAT3 pos)
{
	g_efkManager->Play(g_effect[no], pos.x, pos.y, pos.z);
}
