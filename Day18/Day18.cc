#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<std::string> &numbers)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		while(getline(input, line))
		{
			numbers.push_back(line);
		}
  }
  input.close();
}

void reduce(std::string &number)
{
	bool reduced = false;
	
	while(!reduced)
	{
		reduced = true;
		unsigned layer = 0;
		for(unsigned j=0; j<number.size(); j++)
		{
			if(number[j] == '[')
			{
				layer++;
			}
			else if(number[j] == ']')
			{
				layer--;
			}
			if(layer == 5)
			{
				reduced = false;
				size_t pos = number.find_first_of("1234567890", j);
				size_t endpos = number.find_first_of(",",pos);
				unsigned left = std::stoi(number.substr(pos,endpos-pos));
				pos = endpos+1;
				endpos = number.find_first_of("]",pos);
				unsigned right = std::stoi(number.substr(pos,endpos-pos));
				number[j] = '0';
				number.erase(j+1,endpos-j);
				
				endpos = number.find_last_of("1234567890",j-1);
				if(endpos != std::string::npos)
				{
					pos = number.find_last_of("[,",endpos);
					pos++;
					endpos++;
					left += std::stoi(number.substr(pos,endpos-pos));
					number = number.substr(0,pos) + std::to_string(left) + number.substr(endpos);
				}
				
				pos = number.find_first_of("1234567890",j+2);
				if(pos != std::string::npos)
				{
					endpos = number.find_first_of(",]",pos);
					right += std::stoi(number.substr(pos,endpos-pos));
					number = number.substr(0,pos) + std::to_string(right) + number.substr(endpos);
				}
				break;
			}
		}
		if(!reduced)
		{
			continue;
		}
		for(unsigned j=0; j<number.size(); j++)
		{
			size_t pos = number.find_first_of("1234567890",j);
			if(pos == std::string::npos)
			{
				break;
			}
			size_t endpos = number.find_first_of("[],",pos);
			unsigned splitnum = std::stoi(number.substr(pos,endpos-pos));
			if(splitnum >= 10)
			{
				reduced = false;
				
				unsigned left = splitnum/2;
				unsigned right = left + (splitnum%2);
				
				number = number.substr(0,pos) + "[" + std::to_string(left) + "," + std::to_string(right) + "]" + number.substr(endpos);
				break;
			}
			
			j = endpos;
		}
	}
}

uint64_t calculateMagnitude(std::string &number)
{
	size_t endpos = number.find_first_of("]");
	
	while(endpos != std::string::npos)
	{
		size_t midpos = number.find_last_of(",",endpos);
		unsigned right = std::stoi(number.substr(midpos+1,endpos-midpos-1));
		size_t startpos = number.find_last_of("[",midpos);
		unsigned left = std::stoi(number.substr(startpos+1,midpos-startpos-1));
		unsigned mag = (3*left) + (2*right);
		number = number.substr(0,startpos) + std::to_string(mag) + number.substr(endpos+1);
		endpos = number.find_first_of("]");
	}
	
	return std::stoi(number);
}

uint64_t add(const std::vector<std::string> &numbers)
{
	uint64_t result = 0;
	
	std::string number = numbers[0];
	
	for(unsigned i=1; i<numbers.size(); i++)
	{
		number = "[" + number + "," + numbers[i] + "]";
		reduce(number);
	}
	
	result = calculateMagnitude(number);
	
	return result;
}

uint64_t biggestSum(const std::vector<std::string> &numbers)
{
	uint64_t result = 0;
	
	for(auto &num1:numbers)
	{
		for(auto &num2:numbers)
		{
			if(num1 != num2)
			{
				std::string number = "[" + num1 + "," + num2 + "]";
				reduce(number);
				uint64_t newResult = calculateMagnitude(number);
				result = (newResult > result) ? newResult : result;
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> numbers;
  parseInput(numbers);
	
	resultA = add(numbers);
 	resultB = biggestSum(numbers);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
