//=============================================================================
//
// ƒ‚ƒfƒ‹ [model.cpp]
//
//=============================================================================
#include "AssimpModel.h"
#include "model.h"
#include "main.h"
#include "playercs.h"


std::string modelhead =  MODEL_TORI;
std::string modelbody = MODEL_TORI1;
std::string modelmae = MODEL_TORI2;
std::string modelusiro = MODEL_TORI3;

void SetHead(std::string val)
{
	modelhead = val;
}

std::string GetHead()
{
	return modelhead;
}

void SetBody(std::string val)
{
	modelbody = val;
}

std::string GetBody()
{
	return modelbody;
}

void SetMae(std::string val)
{
	modelmae = val;
}

std::string GetMae()
{
	return modelmae;
}

void SetBack(std::string val)
{
	modelusiro = val;
}

std::string GetBack()
{
	return modelusiro;
}
