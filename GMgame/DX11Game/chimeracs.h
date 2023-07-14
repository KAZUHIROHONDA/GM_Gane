//=============================================================================
//
// フェーズ管理 [chimerah]
//
//=============================================================================
#pragma once

#include "main.h"

enum ChimeraSet 
{
	HEAD,
	BODY,
	FOREFOOT,
	HINDFOOT,
};

class Chimera {
private:

	ChimeraSet nowChimera = HEAD;

public:
	Chimera();
	~Chimera();

	void   ChangeChimera(ChimeraSet state);
	void   headUpdate();
	void   foreUpdate();
	void   bodyUpdate();
	void   hideUpdate();
	void   foreDraw();
	void   headDraw();
	void   bodyDraw();
	void   hideDraw();

	void    Init();
	void	Uninit();
	void	Update();
	void	Draw();
};
