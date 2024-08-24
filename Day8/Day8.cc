#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

void parseInput(std::vector<std::array<std::string,10>> &signals, std::vector<std::array<std::string,4>> &displays)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		while(getline(input,line))
		{
			size_t pos = 0;
			size_t endpos = 0;
			unsigned i=0;
			
			std::array<std::string,10> signal;
			std::array<std::string,4> display;
			
			while(i<10)
			{
				endpos = line.find_first_of(" ",pos);
				signal[i] = line.substr(pos,endpos-pos);
				std::sort(signal[i].begin(),signal[i].end());
				pos = endpos+1;
				i++;
			}
			
			pos = endpos+3;
			i = 0;
			while(i<4)
			{
				endpos = line.find_first_of(" ",pos);
				endpos = (endpos == std::string::npos) ? line.size() : endpos;
				display[i] = line.substr(pos,endpos-pos);
				std::sort(display[i].begin(),display[i].end());
				pos = endpos+1;
				i++;
			}
			
			signals.push_back(signal);
			displays.push_back(display);
		}
  }
  input.close();
}

uint64_t findDigits(const std::vector<std::array<std::string,4>> &displays)
{
	uint64_t result = 0;
	
	for(auto& display:displays)
	{
		for(auto& digit:display)
		{
			if(digit.size() <= 4 || digit.size() == 7)
			{
				result++;
			}
		}
	}
	
	return result;
}

void mapDigits(const std::array<std::string,10> &patterns, std::array<std::string,10> &digits)
{
	std::array<std::string,3> fivePatterns;
	std::array<std::string,3> sixPatterns;
	unsigned fiveCount = 0;
	unsigned sixCount = 0;
	for(auto& pattern:patterns)
	{
		unsigned patternSize = pattern.size();
		if(patternSize == 2)
		{
			digits[1] = pattern;
		}
		else if(patternSize == 3)
		{
			digits[7] = pattern;
		}
		else if(patternSize == 4)
		{
			digits[4] = pattern;
		}
		else if(patternSize == 7)
		{
			digits[8] = pattern;
		}
		else if(patternSize == 6)
		{
			sixPatterns[sixCount] = pattern;
			sixCount++;
		}
		else
		{
			fivePatterns[fiveCount] = pattern;
			fiveCount++;
		}
	}
	
	for(auto& sixPattern:sixPatterns)
	{
		unsigned count = 0;
		auto it = std::find(sixPattern.begin(),sixPattern.end(),digits[1][0]);
		if(it != std::end(sixPattern))
		{
			count++;
		}
		it = std::find(sixPattern.begin(),sixPattern.end(),digits[1][1]);
		if(it != std::end(sixPattern))
		{
			count++;
		}
		if(count == 1)
		{
			digits[6] = sixPattern;
			break;
		}
	}
	
	std::string fourPattern;
	for(auto c:digits[4])
	{
		auto it = std::find(digits[1].begin(),digits[1].end(),c);
		if(it == std::end(digits[1]))
		{
			fourPattern += c;
		}
	}
	
	for(auto& sixPattern:sixPatterns)
	{
		unsigned count = 0;
		auto it = std::find(sixPattern.begin(),sixPattern.end(),fourPattern[0]);
		if(it != std::end(sixPattern))
		{
			count++;
		}
		it = std::find(sixPattern.begin(),sixPattern.end(),fourPattern[1]);
		if(it != std::end(sixPattern))
		{
			count++;
		}
		if(count == 1)
		{
			digits[0] = sixPattern;
			break;
		}
	}
	
	for(auto& sixPattern:sixPatterns)
	{
		if(sixPattern != digits[0] && sixPattern != digits[6])
		{
			digits[9] = sixPattern;
			break;
		}
	}
	
	for(auto& fivePattern:fivePatterns)
	{
		unsigned count = 0;
		auto it = std::find(fivePattern.begin(),fivePattern.end(),digits[6][0]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[6][1]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[6][2]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[6][3]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[6][4]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[6][5]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		if(count == 5)
		{
			digits[5] = fivePattern;
			break;
		}
	}
	
	for(auto& fivePattern:fivePatterns)
	{
		unsigned count = 0;
		auto it = std::find(fivePattern.begin(),fivePattern.end(),digits[5][0]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[5][1]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[5][2]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[5][3]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		it = std::find(fivePattern.begin(),fivePattern.end(),digits[5][4]);
		if(it != std::end(fivePattern))
		{
			count++;
		}
		if(count == 3)
		{
			digits[2] = fivePattern;
		}
		else if(count == 4)
		{
			digits[3] = fivePattern;
		}
	}
}

uint64_t sumDisplays(const std::vector<std::array<std::string,10>> &signals, const std::vector<std::array<std::string,4>> &displays)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<displays.size(); i++)
	{
		std::array<std::string,10> digits;
		mapDigits(signals[i], digits);
		
		std::string displayNumber;
		for(unsigned j=0; j<displays[i].size(); j++)
		{
			for(unsigned k=0; k<digits.size(); k++)
			{
				if(displays[i][j] == digits[k])
				{
					displayNumber += std::to_string(k);
					break;
				}
			}
		}

		result += std::stoi(displayNumber);
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::array<std::string,10>> signals;
	std::vector<std::array<std::string,4>> displays;
  
  parseInput(signals, displays);

 	resultA = findDigits(displays);
	resultB = sumDisplays(signals, displays);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
