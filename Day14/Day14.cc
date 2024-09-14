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

uint64_t expandPolymer(const std::string &polymer, const std::unordered_map<std::string,char> &map, unsigned iterations)
{
	uint64_t result = 0;
	
	std::unordered_map<std::string, uint64_t> pairs;
	for(unsigned i=0; i<polymer.size()-1; i++)
	{
		pairs[polymer.substr(i,2)]++;
	}
	
	for(unsigned count=0; count<iterations; count++)
	{
		std::unordered_map<std::string, uint64_t> newPairs;
		for(auto it=pairs.begin(); it!=pairs.end(); it++)
		{
			uint64_t count = it->second;
			std::string key1 = it->first;
			std::string key2 = key1;
			key1[1] = map.at(it->first);
			key2[0] = map.at(it->first);
			newPairs[key1] += count;
			newPairs[key2] += count;
		}
		pairs = newPairs;
	}
	
	std::unordered_map<char,uint64_t> freq;
	for(auto it=pairs.begin(); it!=pairs.end(); it++)
	{
		freq[it->first[0]] += it->second;
	}
	freq[polymer.back()]++;
	
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

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::string polymer;
	std::unordered_map<std::string,char> map;
  
  parseInput(polymer, map);

 	resultA = expandPolymer(polymer, map, 10);
 	resultB = expandPolymer(polymer, map, 40);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
