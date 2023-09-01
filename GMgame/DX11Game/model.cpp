//=============================================================================
//
// ƒ‚ƒfƒ‹ [model.cpp]
//
//=============================================================================
#include "AssimpModel.h"
#include "model.h"
#include "main.h"
#include "playercs.h"


Model::Model()
{
}

Model::~Model()
{
}

void Model::SetHead(std::string val)
{
	modelhead = val;
}

std::string Model::GetHead()
{
	return modelhead;
}

void Model::SetBody(std::string val)
{
	modelbody = val;
}

std::string Model::GetBody()
{
	return modelbody;
}

void Model::SetMae(std::string val)
{
	modelmae = val;
}

std::string Model::GetMae()
{
	return modelmae;
}

void Model::SetBack(std::string val)
{
	modelusiro = val;
}

std::string Model::GetBack()
{
	return modelusiro;
}
