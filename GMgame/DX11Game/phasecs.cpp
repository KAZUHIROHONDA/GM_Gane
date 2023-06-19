#include "phasecs.h"
#include "jyanken.h"

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

void Phase::SetUpdate()
{
	//UpdateJyankenSet();
}

void Phase::JudgeUpdate()
{
	//UpdateJyankenJadge();
}

void Phase::BattleUpdate()
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
}
