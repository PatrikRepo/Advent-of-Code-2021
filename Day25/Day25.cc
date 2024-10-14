#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<std::string> &map)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
			map.push_back(line);
  	}
  }
  input.close();
}

uint64_t findSpace(std::vector<std::string> &map)
{
	uint64_t result = 0;
	
	bool done = false;
	
	while(!done)
	{
		result++;
		
		auto eastMap = map;
		
		for(unsigned i=0; i<map.size(); i++)
		{
			for(unsigned j=0; j<map[0].size(); j++)
			{
				if(map[i][j] == '>' && map[i][(j+1)%(map[0].size())] == '.')
				{
					eastMap[i][(j+1)%(map[0].size())] = '>';
					eastMap[i][j] = '.';
				}
			}
		}
		
		auto southMap = eastMap;
		
		for(unsigned i=0; i<eastMap.size(); i++)
		{
			for(unsigned j=0; j<eastMap[0].size(); j++)
			{
				if(eastMap[i][j] == 'v' && eastMap[(i+1)%(eastMap.size())][j] == '.')
				{
					southMap[(i+1)%(eastMap.size())][j] = 'v';
					southMap[i][j] = '.';
				}
			}
		}
		
		done = true;
		for(unsigned i=0; i<map.size(); i++)
		{
			if(map[i] != southMap[i])
			{
				done = false;
				break;
			}
		}
		map = southMap;
	}
	
	return result;
}

int main()
{
	uint64_t result = 0;

	std::vector<std::string> map;
	
  parseInput(map);

	result = findSpace(map);

  std::cout << "result: " << result << std::endl;;
  
  return 0;
}
