#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

class CSVLoader
{
public:
	static std::vector<std::string> TextLoad(std::string& filepath);
};

