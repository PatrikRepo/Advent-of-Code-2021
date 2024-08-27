#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <utility>

void parseInput(std::array<std::array<char,1331>,895> &map, std::vector<std::pair<char,unsigned>> &instructions)
{
	for(auto& line:map)
	{
		for(auto& c:line)
		{
			c = ' ';
		}
	}
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		getline(input,line);
		while(line != "")
		{
			size_t pos = line.find_first_of(",");
			map[std::stoi(line.substr(pos+1))][std::stoi(line.substr(0,pos))] = '#';
			getline(input,line);
		}
		
		while(getline(input,line))
		{
			std::pair<char,unsigned> instruction;
			size_t pos = line.find_first_of("xy");
			instruction.first = line[pos];
			instruction.second = std::stoi(line.substr(pos+2));
			instructions.push_back(instruction);
		}
  }
  input.close();
}

uint64_t foldMap(std::array<std::array<char,1331>,895> &map, const std::pair<char,unsigned> &instruction)
{
	uint64_t result = 0;

	if(instruction.first == 'x')
	{
		for(unsigned i=0; i<map.size(); i++)
		{
			for(unsigned j=0; j<instruction.second; j++)
			{
				if(map[i][instruction.second*2-j] == '#')
				{
					map[i][j] = '#';
					map[i][instruction.second*2-j] = ' ';
				}
			}
		}
	}
	else
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			for(unsigned i=0; i<instruction.second; i++)
			{
				if(map[instruction.second*2-i][j] == '#')
				{
					map[i][j] = '#';
					map[instruction.second*2-i][j] = ' ';
				}
			}
		}
	}
	
	for(auto& line:map)
	{
		for(auto c:line)
		{
			if(c == '#')
			{
				result++;
			}
		}
	}
	
	return result;
}

void followInstructions(std::array<std::array<char,1331>,895> &map, const std::vector<std::pair<char,unsigned>> &instructions)
{
	for(auto& instruction:instructions)
	{
		foldMap(map,instruction);
	}	
	
	for(unsigned i=0; i<6; i++)
	{
		for(unsigned j=0; j<40; j++)
		{
			std::cout << map[i][j];
		}
		std::cout << '\n';
	}
}

int main()
{
	uint64_t resultA = 0;
	
	std::array<std::array<char,1331>,895> map;
  std::vector<std::pair<char,unsigned>> instructions;
  
  parseInput(map, instructions);

 	resultA = foldMap(map, instructions[0]);
 	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << '\n';
  followInstructions(map, instructions);
  
  return 0;
}
