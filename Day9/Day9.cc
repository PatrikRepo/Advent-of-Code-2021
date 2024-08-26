#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

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

uint64_t calculateRisk(const std::vector<std::string> &map)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[i].size(); j++)
		{
			bool isLowest = true;
			char point = map[i][j];
			
			if(i>0 && map[i-1][j] <= point)
			{
				isLowest = false;
			}
			if(isLowest && j+1<map[i].size() && map[i][j+1] <= point)
			{
				isLowest = false;
			}
			if(isLowest && i+1<map.size() && map[i+1][j] <= point)
			{
				isLowest = false;
			}
			if(isLowest && j>0 && map[i][j-1] <= point)
			{
				isLowest = false;
			}
			if(isLowest)
			{
				result += 1 + point - '0';
			}
		}
	}
	
	return result;
}

unsigned fill(std::vector<std::string> &map, size_t y, size_t x)
{
	unsigned result = 1;
	map[y][x] = '9';
	
	if(y>0 && map[y-1][x] != '9')
	{
		result += fill(map,y-1,x);
	}
	if(x+1<map[y].size() && map[y][x+1] != '9')
	{
		result += fill(map,y,x+1);
	}
	if(y+1<map.size() && map[y+1][x] != '9')
	{
		result += fill(map,y+1,x);
	}
	if(x>0 && map[y][x-1] != '9')
	{
		result += fill(map,y,x-1);
	}
	
	return result;
}

uint64_t findBasins(std::vector<std::string> &map)
{
	uint64_t result = 0;
	unsigned largest = 0;
	unsigned second = 0;
	unsigned third = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[i].size(); j++)
		{
			if(map[i][j] != '9')
			{
				unsigned basin = fill(map, i, j);
				if(basin > largest)
				{
					third = second;
					second = largest;
					largest = basin;
				}
				else if(basin > second)
				{
					third = second;
					second = basin;
				}
				else if(basin > third)
				{
					third = basin;
				}
			}
		}
	}
	result = (largest*second)*third;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> map;
  
  parseInput(map);

 	resultA = calculateRisk(map);
	resultB = findBasins(map);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
