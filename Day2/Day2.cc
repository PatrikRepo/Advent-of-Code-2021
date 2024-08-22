#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

void parseInput(std::vector<std::pair<char,unsigned>> &instructions)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		instructions.push_back(std::make_pair(line[0],line.back()-'0'));
  	}
  }
  input.close();

}

uint64_t travel(const std::vector<std::pair<char,unsigned>> &instructions)
{
	uint64_t result = 0;
	
	size_t x = 0;
	size_t y = 0;
	
	for(auto& instruction:instructions)
	{
		switch(instruction.first)
		{
			case 'f':
			{
				x += instruction.second;
				break;
			}
			case 'd':
			{
				y += instruction.second;
				break;
			}
			case 'u':
			{
				y -= instruction.second;
			}
		}
	}
	
	result = x*y;
	
	return result;
}

uint64_t aimedTravel(const std::vector<std::pair<char,unsigned>> &instructions)
{
	uint64_t result = 0;
	
	size_t x = 0;
	size_t y = 0;
	int aim = 0;
	
	for(auto& instruction:instructions)
	{
		switch(instruction.first)
		{
			case 'f':
			{
				x += instruction.second;
				y += instruction.second*aim;
				break;
			}
			case 'd':
			{
				aim += instruction.second;
				break;
			}
			case 'u':
			{
				aim -= instruction.second;
			}
		}
	}
	
	result = x*y;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::pair<char,unsigned>> instructions;
  
  parseInput(instructions);

 	resultA = travel(instructions);
 	resultB = aimedTravel(instructions);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
