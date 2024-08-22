#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <bitset>

void parseInput(std::vector<std::string> &numbers)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		numbers.push_back(line);
  	}
  }
  input.close();
}

uint64_t powerConsumption(const std::vector<std::string> &numbers)
{
	uint64_t result = 0;
	
	std::array<unsigned,12> ones{0};
	std::array<unsigned,12> zeros{0};
	std::bitset<12> gamma;
	std::bitset<12> epsilon;
	
	for(auto& number:numbers)
	{
		for(unsigned i=0; i<number.size(); i++)
		{
			if(number[i] == '1')
			{
				ones[number.size()-1-i] += 1;
			}
			else
			{
				zeros[number.size()-1-i] += 1;
			}
		}
	}
		
	for(unsigned i=0; i<ones.size(); i++)
	{
		if(ones[i] > zeros[i])
		{
			gamma[i] = 1;
			epsilon[i] = 0;
		}
		else
		{
			epsilon[i] = 1;
			gamma[i] = 0;
		}
	}
	
	result = gamma.to_ullong();
	result *= epsilon.to_ullong();
	
	return result;
}

uint64_t lifeSupport(const std::vector<std::string> &numbers)
{
	uint64_t result = 0;
	
	auto oxygenNumbers = numbers;
	auto CO2Numbers = numbers;
	
	for(unsigned i=0; i<12; i++)
	{
		unsigned ones = 0;
		unsigned zeros = 0;
		for(unsigned j=0; j<oxygenNumbers.size(); j++)
		{
			if(oxygenNumbers[j][i] == '1')
			{
				ones++;
			}
			else
			{
				zeros++;
			}
		}
		char remove = (ones >= zeros) ? '0' : '1';
		for(unsigned j=0; j<oxygenNumbers.size();)
		{
			if(oxygenNumbers[j][i] == remove)
			{
				oxygenNumbers.erase(oxygenNumbers.begin()+j);
			}
			else
			{
				j++;
			}
		}
		
		ones = 0;
		zeros = 0;
		for(unsigned j=0; j<CO2Numbers.size(); j++)
		{
			if(CO2Numbers[j][i] == '1')
			{
				ones++;
			}
			else
			{
				zeros++;
			}
		}
		remove = (ones < zeros) ? '0' : '1';
		for(unsigned j=0; j<CO2Numbers.size();)
		{
			if(CO2Numbers[j][i] == remove)
			{
				CO2Numbers.erase(CO2Numbers.begin()+j);
			}
			else
			{
				j++;
			}
		}
	}
	
	std::bitset<12> oxygen(oxygenNumbers[0]);
	std::bitset<12> CO2(CO2Numbers[0]);
	
	result = oxygen.to_ullong();
	result *= CO2.to_ullong();
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> numbers;
  
  parseInput(numbers);

 	resultA = powerConsumption(numbers);
 	resultB = lifeSupport(numbers);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
