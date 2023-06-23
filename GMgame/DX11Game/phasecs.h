//=============================================================================
//
// フェーズ管理 [phasecs.h]
//
//=============================================================================
#pragma once

#include "main.h"

enum PhaseSet 
{
	SETPHASE,
	JUDGEPHASE,
	BATTLEPHASE,
};

class Phase {
private:

	PhaseSet nowPhase;

public:
	Phase();
	~Phase();

	void   ChangePhase(PhaseSet state);
	void   SetUpdate();
	void   JudgeUpdate();
	void   BattleUpdate();
	void   SetDraw();
	void   JudgeDraw();
	void   BattleDraw();

	void    Init();
	void	Uninit();
	void	Update();
	void	Draw();
};
