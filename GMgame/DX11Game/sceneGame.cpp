//=============================================================================
//
// �Q�[���V�[�� [sceneGame.cpp]
// Author : TAKUYA KATOU
//
//=============================================================================
#include "sceneGame.h"
#include "stage.h"
#include "AssimpModel.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"	
#include "player.h"
#include "field3D.h"
#include "player.h"
#include "sceneGame.h"
#include "shadow.h"
#include "gauge.h"
#include "Egauge.h"
#include "wall.h"
#include "enemy.h"
#include "fade.h"
#include "bg.h"
#include "pause.h"
#include "MessageManager.h"
#include "playercs.h"
#include "playerhp.h"
#include "number.h"
#include"enemyhp.h"
#include"jyanken.h"
#include "phasecs.h"
#include "sceneTitle.h"
#include "select.h"
#include "gameover.h"
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static bool g_bPause = false;	//ture�Ń|�[�Y��
static bool g_bClear = false;
static bool g_bOver = false;
int			eCnt;			//�G��|����
int			sTime;			//�s���s�\���ԃJ�E���g


Phase  phase;


//=============================================================================
//�@����������
//=============================================================================
HRESULT InitSceneGame()
{
	HRESULT hr = S_OK;
	HWND hWnd = GetMainWnd();
	

	phase.Init();

	eCnt = 0;
	sTime = 0;

	//�w�i�̏���������
	InitBg();
	//�ۉe����������(�ق��̃��f�����O)
	hr = InitShadow();
	if (FAILED(hr))
		return hr;

	//�v���C���[�̏���������
	hr = InitPlayer();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�v���C���[�̏���������
	hr = InitJyanken();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("����񂯂񏉊��������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �G����������
	hr = InitEnemy();

	// tairyoku�̏���������
	hr = InitPlayerhp();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[HP�����������G���["),
			_T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	// tairyoku�̏���������
	hr = InitEnemyhp();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�v���C���[HP�����������G���["),
			_T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}


	// �����̏���������
	hr = InitNumber();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("���������������G���["),
			_T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	//serect�̏���������
	hr = InitSelect();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("serect�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;
	}

	hr = InitGauge();
	if (FAILED(hr))
		return hr;
	
	hr = InitEGauge();
	if (FAILED(hr))
		return hr;

	//�n�ʏ���������
	hr = InitField3D();
	if (FAILED(hr))
		return hr;

	//��
	hr = InitWall();
	if (FAILED(hr))
		return hr;

	//�|�[�Y����������
	hr = InitPause();
	if (FAILED(hr))
	{
		MessageBox(hWnd, _T("�|�[�Y�����������G���["), _T("�G���["), MB_OK | MB_ICONSTOP);
		return hr;

	}
	g_bPause = false;
	g_bClear = false;

	//SetEnemy(XMFLOAT3(0.0f, 70.0f, 200.0f),0);

	//�X�e�[�W�̏�����
	InitStage();

	//��������������
	InitMessage();

	SetEnemy(XMFLOAT3(0.0f, 20.0f, 100.0f), XMFLOAT3(0.0f, 180.0f, 0.0f));

	InitGameover();

	return hr;
}

//=============================================================================
//�@�I������
//=============================================================================
void UninitSceneGame()
{
	//�����I������
	UninitMessage();

	//�w�i�̏I������
	UninitBg();
	//�G
	UninitEnemy();

	UninitGauge();

	UninitEGauge();

	// tairyoku�̏I������
	UninitPlayerhp();

	// tairyoku�̏I������
	UninitEnemyhp();

	// �����̏I������
	UninitNumber();

	//�n�ʏI������
	UninitField3D();
	//�n�ʏI������
	UninitWall();
	//�v���C���[�̏I������
	UninitPlayer();

	//�v���C���[�̏I������
	UninitJyanken();

	//�e
	UninitShadow();
	//�|�[�Y
	UninitPause();
	//�Z���N�g
	UninitSelect();

	UninitGameover();

	//�f�o�b�N�p

	phase.Uninit();
}

//=============================================================================
//�@�X�V����
//=============================================================================
void UpdateSceneGame()
{
	if (GetKeyTrigger(VK_RETURN))
	{
		g_bClear = true;
	}
	if (g_bClear == true || g_bOver == true)
	{
		GetCamera()->Update();
		UpdateClear();

		UpdateSelect();

		E_FADE fadeState = GetFade();
		if (fadeState == E_FADE_NONE)
		{
			if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0) || GetMouseTrigger(0))
			{
				//�I�𒆂̂��̂ɂ�蕪��
				SELECT_MENU menu = GetSelectMenu();
				switch (menu)
				{
				case SELECT_MENU_CONTINUE:
					StartFade(SCENE_GAME);
					GetPhase()->ChangePhase(STARTPHASE);
					//PlaySound(SOUND_LABEL_SE_DECIDE);
					//sCnt++;
					//SetStageNo(sCnt);
					//sNO();
					break;
					//���g���C
				case SELECT_MENU_RETRY:
					StartFade(SCENE_SELECT);
					GetPhase()->ChangePhase(STARTPHASE);
					//PlaySound(SOUND_LABEL_SE_DECIDE);
					break;
					//�I��(�^�C�g����)
				case SELECT_MENU_QUIT:
					StartFade(SCENE_TITLE);
					//PlaySound(SOUND_LABEL_SE_DECIDE);
					//sCnt = 0;
					break;
				}
			}
		}
	}
	else 
	{
		if (g_bPause == true)
		{	//�|�[�Y�X�V
			UpdatePause();
			UpdateGameover();
		}
		else if (g_bPause == false)
		{
			//�f�o�b�N
			phase.Update();

			//�w�i
			UpdateBg();
			//�G
			UpdateEnemy();

			//�n�ʍX�V����
			UpdateField3D();


			UpdatePlayerhp();
			UpdateEnemyhp();

			//�n�ʍX�V����
			UpdateWall();
			//�����蔻��

			//�e
			UpdateShadow();
			//�X�e�[�W
			UpdateStage();

		}

		//�|�[�Y�̃I���I�t
		if ( GetJoyTrigger(0, 1) || GetMouseTrigger(2))
		{
			//�t�F�[�h���͔������Ȃ��p�ɂ���
			E_FADE fadeState = GetFade();
			if (fadeState == E_FADE_NONE)
			{
				//[�t���O�̐؂�ւ�]
				//g_bPause��"true"�Ȃ�"false"��"false"�Ȃ�"true"�ɂ���
				g_bPause = !g_bPause;

				//�|�[�Y�J�n���ɂ�邱��
				if (g_bPause == true)
				{
					ResetPauseMenu();
				}
			}
		}

		//�|�[�Y���̍��ڌ���
		if (g_bPause == true)
		{
			E_FADE fadeState = GetFade();
			if (fadeState == E_FADE_NONE)
			{
				if (GetKeyTrigger(VK_RETURN) || GetJoyTrigger(0, 0) || GetMouseTrigger(0))
				{
					//�I�𒆂̂��̂ɂ�蕪��
					PAUSE_MENU menu = GetPauseMenu();
					switch (menu)
					{
						//�Q�[���ɖ߂�
					case PAUSE_MENU_CONTINUE:
						g_bPause = !g_bPause;
						break;
						//���g���C
					case PAUSE_MENU_RETRY:
						StartFade(SCENE_GAME);
						GetPhase()->ChangePhase(STARTPHASE);
						break;
						//�I��(�^�C�g����)
					case PAUSE_MENU_QUIT:
						StartFade(SCENE_SELECT);
						GetPhase()->ChangePhase(STARTPHASE);
						break;
					}
				}
			}

		}
	}
	
		
	//�����X�V����
	UpdateMessage();
}

//=============================================================================
//�@�\������
//=============================================================================
void DrawSceneGame()
{	

	//�������I�u�W�F�N�g(3D�̒��ł����ɂ���)
	SetZWrite(false);
	//�w�i
	DrawBg();

	// Z�o�b�t�@�L��
	SetZBuffer(true);
	//�v���C���[
	DrawPlayer();

	//�G
	DrawEnemy();
	//�n��
	DrawField3D();

	//������������
	GetLight()->SetDisable();
	//�n��
	DrawWall();
	//���������L��
	GetLight()->SetEnable();

	

	// Z�o�b�t�@����
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);


	//������������
	GetLight()->SetDisable();
	if (g_bClear == true)
	{
		DrawSelect();
		if (g_bOver == true)
		{
			DrawGameover();
		}
		else
		{
			DrawGameclear();
		}
		
	}
	//�r���{�[�h�e�т傤��
	SetZWrite(false);	

	DrawShadow();

	if (g_bClear == false)
	{//�Q�[�W
		DrawPlayerGauge();

		DrawEnemyEGauge();
	}

	//���������L��
	GetLight()->SetEnable();

	if (g_bClear == false)
	{
		GetPlayer()->Draw(50, 100);
		GetEnemy()->Draw(1100, 100);
		phase.Draw();

		DrawPlayerhp();
		DrawEnemyhp();
	}

	//�|�[�Y���
	if (true == g_bPause)
	{
		DrawPause();
	}


}

Phase* GetPhase()
{
	return &phase;
}

void Clearflag()
{
	g_bClear = true;
}
void Overflag()
{
	g_bClear = true;
	g_bOver = true;
}
