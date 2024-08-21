#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<unsigned> &depths)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		depths.push_back(std::stoi(line));
  	}
  }
  input.close();

}

uint64_t measureDepths(const std::vector<unsigned> &depths)
{
	uint64_t result = 0;
	
	for(unsigned i=1; i<depths.size(); i++)
	{
		if(depths[i] > depths[i-1])
		{
			result++;
		}
	}
	
	return result;
}

uint64_t windowDepths(const std::vector<unsigned> &depths)
{
	uint64_t result = 0;
	
	for(unsigned i=3; i<depths.size(); i++)
	{
		unsigned prev = depths[i-3] + depths[i-2] + depths[i-1];
		unsigned current = depths[i-2] + depths[i-1] + depths[i];
		if(prev < current)
		{
			result++;
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<unsigned> depths;
  
  parseInput(depths);

 	resultA = measureDepths(depths);
 	resultB = windowDepths(depths);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
