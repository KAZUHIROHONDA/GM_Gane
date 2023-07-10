#include "chimeracs.h"
#include "jyanken.h"
#include "player.h"
#include "sceneGame.h"
#include "AssimpModel.h"


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
}

void Chimera::foreUpdate()
{
}

void Chimera::bodyUpdate()
{
}

void Chimera::hideUpdate()
{
}

void Chimera::foreDraw()
{
}

void Chimera::headDraw()
{
}

void Chimera::bodyDraw()
{
}

void Chimera::hideDraw()
{
}



void Chimera::Init()
{

}

void Chimera::Uninit()
{

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
