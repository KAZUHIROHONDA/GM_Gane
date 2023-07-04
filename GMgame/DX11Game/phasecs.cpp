#include "phasecs.h"
#include "jyanken.h"
#include "player.h"
#include "sceneGame.h"
#include "AssimpModel.h"
#include "VS.h"


Phase::Phase()
{
}

Phase::~Phase()
{
}

void Phase::ChangePhase(PhaseSet state)
{
	nowPhase = state;
}

void Phase::StartUpdate()
{
	
	UpdateJyankenStart();
	UpdateStart();
	UpdateVS();
	// カメラ更新
	GetCamera()->Update();
}

void Phase::SetUpdate()
{
	UpdateJyankenSet();
	UpdatePlayer();
	// カメラ更新
	GetCamera()->UpdateSet();
}

void Phase::JudgeUpdate()
{
	UpdateJyankenJadge();
	UpdatePlayer();
	// カメラ更新
	GetCamera()->Update();
}

void Phase::BattleUpdate()
{
	UpdateJyankenBattle();
	UpdatePlayer();
	// カメラ更新
	GetCamera()->Update();
}

void Phase::StartDraw()
{
	DrawJyankenStart();
	DrawVS();
}

void Phase::SetDraw()
{
	DrawJyankenSet();
}

void Phase::JudgeDraw()
{
	DrawJyankenJadge();
}

void Phase::BattleDraw()
{
}

void Phase::Init()
{
	InitVS();
}

void Phase::Uninit()
{
	UninitVS();
}

void Phase::Update()
{
	switch (nowPhase)
	{
	case STARTPHASE:
	{
		StartUpdate();
		break;
	}
	case SETPHASE:
	{
		SetUpdate();
		break;
	}
	case JUDGEPHASE:
	{
		JudgeUpdate();
		break;
	}
	case BATTLEPHASE:
	{
		BattleUpdate();
		break;
	}
	default:
		break;
	}
}

void Phase::Draw()
{
	switch (nowPhase)
	{
	case STARTPHASE:
	{
		StartDraw();
		break;
	}
	case SETPHASE:
	{
		SetDraw();
		break;
	}
	case JUDGEPHASE:
	{
		JudgeDraw();
		break;
	}
	case BATTLEPHASE:
	{
		BattleDraw();
		break;
	}
	default:
		break;
	}
}
