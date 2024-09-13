#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <utility>

void parseInput(std::string &polymer, std::unordered_map<std::string,char> &map)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	getline(input,polymer);
  	getline(input,line);
  	
		while(getline(input,line))
		{
			map[line.substr(0,2)] = line.back();
		}
  }
  input.close();
}

uint64_t expandPolymer(const std::string &polymerString, const std::unordered_map<std::string,char> &map)
{
	uint64_t result = 0;
	
	std::list<char> polymer;
	
	for(auto c:polymerString)
	{
		polymer.push_back(c);
	}
	
	for(unsigned count=0; count<10; count++)
	{	
		for(auto it=polymer.begin(); std::next(it,1)!=polymer.end(); it++)
		{
			std::string key;
			key += (*it);
			it++;
			key += (*it);
			it = polymer.insert(it, map.at(key));
		}
	}
	
	std::unordered_map<char,unsigned> freq;
	
	for(auto& c:polymer)
	{
		freq[c]++;
	}
	
	uint64_t max = 0;
	uint64_t min = UINT64_MAX;
	
	for(auto pair:freq)
	{
		max = (pair.second > max) ? pair.second : max;
		min = (pair.second < min) ? pair.second : min;
	}
	
	result = max - min;
	
	return result;
}

//This can be optimized by taking it in steps of 10 instead of 20, then expand each step 3 times. At least I believe that would be faster. You never know until you try though
uint64_t expandLongPolymer(const std::string &polymerString, const std::unordered_map<std::string,char> &map)
{
	uint64_t result = 0;
	
	std::unordered_map<std::string,std::pair<uint64_t,uint64_t>> cache;
	
	for(auto mapIt=map.begin(); mapIt!=map.end(); mapIt++)
	{
		std::list<char> polymer;
		std::string mapKey = mapIt->first;
		polymer.push_back(mapKey[0]);
		polymer.push_back(mapKey[1]);
		for(unsigned count=0; count<20; count++)
		{	
			for(auto it=polymer.begin(); std::next(it,1)!=polymer.end(); it++)
			{
				std::string key;
				key += (*it);
				it++;
				key += (*it);
				it = polymer.insert(it, map.at(key));
			}
		}
		std::unordered_map<char,unsigned> freq;
		
		for(auto it=polymer.begin(); std::next(it,1)!=polymer.end(); it++)
		{
			freq[(*it)]++;
		}
		
		cache[mapKey] = std::make_pair(freq['C'],freq['B']);
	}
	
	std::list<char> polymer;
	for(auto c:polymerString)
	{
		polymer.push_back(c);
	}
	
	for(unsigned count=0; count<20; count++)
	{	
		for(auto it=polymer.begin(); std::next(it,1)!=polymer.end(); it++)
		{
			std::string key;
			key += (*it);
			it++;
			key += (*it);
			it = polymer.insert(it, map.at(key));
		}
	}
	
	for(auto it=polymer.begin(); std::next(it,1)!=polymer.end(); it++)
	{
		std::string key;
		key += *it;
		key += *(std::next(it,1));
		result += cache.at(key).first - cache.at(key).second;
	}
	
	if(polymerString.back() == 'C')
	{
		result++;
	}
	else if(polymerString.back() == 'B')
	{
		result--;
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::string polymer;
	std::unordered_map<std::string,char> map;
  
  parseInput(polymer, map);

 	resultA = expandPolymer(polymer, map);
 	resultB = expandLongPolymer(polymer, map);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
