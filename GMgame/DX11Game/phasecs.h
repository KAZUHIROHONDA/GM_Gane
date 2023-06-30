//=============================================================================
//
// フェーズ管理 [phasecs.h]
//
//=============================================================================
#pragma once

#include "main.h"

enum PhaseSet 
{
	STARTPHASE,
	SETPHASE,
	JUDGEPHASE,
	BATTLEPHASE,
};

class Phase {
private:

	PhaseSet nowPhase;
	int pCnt;

public:
	Phase();
	~Phase();

	void   ChangePhase(PhaseSet state);
	void   StartUpdate();
	void   SetUpdate();
	void   JudgeUpdate();
	void   BattleUpdate();
	void   StartDraw();
	void   SetDraw();
	void   JudgeDraw();
	void   BattleDraw();

	void    Init();
	void	Uninit();
	void	Update();
	void	Draw();
};
