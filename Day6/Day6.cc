#include <iostream>
#include <fstream>
#include <array>

void parseInput(std::array<uint64_t,7> &fish)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		getline(input,line);
		size_t pos = 0;

		while(pos<line.size())
		{
			fish[line[pos]-'0']++;
			pos += 2;
		}
  }
  input.close();
}

uint64_t countFish(std::array<uint64_t,7> fish, unsigned days)
{
	uint64_t result = 0;
	
	uint64_t newlyHatched = 0;
	uint64_t dayOld = 0;
	unsigned index = 0;
	
	for(unsigned count=0; count<days; count++)
	{
		uint64_t newFish = fish[index];
		fish[index] += dayOld;
		dayOld = newlyHatched;
		newlyHatched = newFish;
		index = (index+1 < 7) ? index+1 : 0;
	}
	
	for(auto num:fish)
	{
		result += num;
	}
	result += dayOld;
	result += newlyHatched;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::array<uint64_t,7> fish{0};
  
  parseInput(fish);

 	resultA = countFish(fish, 80);
 	resultB = countFish(fish, 256);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
