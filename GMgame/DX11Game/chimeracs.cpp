#include "chimeracs.h"
#include "jyanken.h"
#include "player.h"
#include "sceneGame.h"
#include "AssimpModel.h"
#include "partsmenu.h"
#include "Bpartsmenu.h"
#include "Mpartsmenu.h"
#include "Upartsmenu.h"


Chimera::Chimera()
{
}

Chimera::~Chimera()
{
}

void Chimera::ChangeChimera(ChimeraSet state)
{
	nowChimera = state;
}

void Chimera::headUpdate()
{
	UpdateParts();
}

void Chimera::foreUpdate()
{
	UpdateMParts();
}

void Chimera::bodyUpdate()
{
	UpdateBParts();
}

void Chimera::hideUpdate()
{
	UpdateUParts();
}

void Chimera::foreDraw()
{
	DrawMParts();
}

void Chimera::headDraw()
{
	DrawParts();
}

void Chimera::bodyDraw()
{
	DrawBParts();
}

void Chimera::hideDraw()
{
	DrawUParts();
}



void Chimera::Init()
{
	InitParts();
	InitBParts();
	InitMParts();
	InitUParts();
}

void Chimera::Uninit()
{
	UninitParts();
	UninitBParts();
	UninitMParts();
	UninitUParts();
}

void Chimera::Update()
{
	switch (nowChimera)
	{
	case HEAD:
	{
		headUpdate();
	}break;
	case BODY:
	{
		bodyUpdate();
	}break;
	case FOREFOOT:
	{
		foreUpdate();
	}break;
	case HINDFOOT:
	{
		hideUpdate();
	}break;
	default:
		break;
	}
}

void Chimera::Draw()
{
	switch (nowChimera)
	{
	case HEAD:
	{
		headDraw();
	}break;
	case BODY:
	{
		bodyDraw();
	}break;
	case FOREFOOT:
	{
		foreDraw();
	}break;
	case HINDFOOT:
	{
		hideDraw();
	}break;
	default:
		break;
	}
}
