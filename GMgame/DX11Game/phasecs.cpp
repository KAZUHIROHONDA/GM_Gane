#include "phasecs.h"
#include "jyanken.h"
#include "player.h"
#include "sceneGame.h"


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
}

void Phase::SetUpdate()
{
	UpdateJyankenSet();
	UpdatePlayer();
}

void Phase::JudgeUpdate()
{
	UpdateJyankenJadge();
	UpdatePlayer();
}

void Phase::BattleUpdate()
{
	UpdateJyankenBattle();
	UpdatePlayer();
}

void Phase::StartDraw()
{
	DrawJyankenStart();
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
}

void Phase::Uninit()
{
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
