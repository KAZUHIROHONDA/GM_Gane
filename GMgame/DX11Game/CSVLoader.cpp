#include "CSVLoader.h"

using namespace std;

vector<string> CSVLoader::TextLoad(std::string& input)
{
	std::string str_buf;
	std::string str_conma_buf;
	
	ifstream ifs_csv_file(input);
		
	vector<string> result;

	while (getline(ifs_csv_file, str_buf))
	{
		std::istringstream i_stream(str_buf);

		while (getline(i_stream, str_conma_buf, ','))
		{
			result.push_back(str_conma_buf);
		}
	}

	ifs_csv_file.close();

	return result;
}
