#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

void parseInput(std::vector<unsigned> &crabs)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		getline(input,line);
		size_t pos = 0;
		size_t endpos = 0;
		while(pos<line.size())
		{
			endpos = line.find_first_of(",",pos);
			endpos = (endpos == std::string::npos) ? line.size() : endpos;
			
			crabs.push_back(std::stoi(line.substr(pos,endpos-pos)));
			
			pos = endpos+1;
		}
  }
  input.close();
}

std::pair<uint64_t,uint64_t> alignCrabs(const std::vector<unsigned> &crabs)
{
	std::pair<uint64_t,uint64_t> result{UINT64_MAX,UINT64_MAX};
	
	unsigned maxCrab = 0;
	uint32_t minCrab = UINT32_MAX;
	for(auto crab:crabs)
	{
		maxCrab = (maxCrab < crab) ? crab : maxCrab;
		minCrab = (minCrab > crab) ? crab : minCrab;
	}
	
	for(unsigned i=minCrab; i<=maxCrab; i++)
	{
		unsigned fuelCost = 0;
		unsigned realFuelCost = 0;
		
		for(auto crab:crabs)
		{
			fuelCost += abs(i - crab);
			realFuelCost += (((abs(i-crab)+1)*(abs(i-crab)))/2);
		}
		result.first = (fuelCost < result.first) ? fuelCost : result.first;
		result.second = (realFuelCost < result.second) ? realFuelCost : result.second;
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	
	std::vector<unsigned> crabs;
  
  parseInput(crabs);

 	result = alignCrabs(crabs);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;
 	
  return 0;
}
