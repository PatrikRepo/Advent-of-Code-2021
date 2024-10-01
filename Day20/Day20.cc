#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

void parseInput(std::string &table, std::vector<std::string> &map)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	getline(input,table);
  	getline(input,line);
  	
  	while(getline(input,line))
  	{
  		map.push_back(line);
  	}
  }
  input.close();
}

uint64_t enhanceImage(std::vector<std::string> map, const std::string &table, unsigned iterations)
{
	uint64_t result = 0;
	
	for(unsigned count=0; count<iterations; count++)
	{	
		char padChar;
		padChar = (count % 2 == 0) ? '.' : '#';
		
		for(unsigned i=0; i<map.size(); i++)
		{
			map[i] = padChar + map[i] + padChar;
		}
		std::string padding(map[0].size(), padChar);
		
		map.insert(map.begin(),padding);
		map.push_back(padding);
		
		auto newMap = map;
		for(unsigned i=0; i<map.size(); i++)
		{
			for(unsigned j=0; j<map[0].size(); j++)
			{
				std::string key;
				char c;
				c = (i==0 || j==0) ? padChar : map[i-1][j-1];
				key += (c == '.') ? '0' : '1';
				
				c = (i==0) ? padChar : map[i-1][j];
				key += (c == '.') ? '0' : '1';
				
				c = (i==0 || j+1>=map[0].size()) ? padChar : map[i-1][j+1];
				key += (c == '.') ? '0' : '1';
				
				c = (j==0) ? padChar : map[i][j-1];
				key += (c == '.') ? '0' : '1';

				c = map[i][j];
				key += (c == '.') ? '0' : '1';
			
				c = (j+1>=map[0].size()) ? padChar : map[i][j+1];
				key += (c == '.') ? '0' : '1';
				
				c = (i+1>=map.size() || j==0) ? padChar : map[i+1][j-1];
				key += (c == '.') ? '0' : '1';
				
				c = (i+1>=map.size()) ? padChar : map[i+1][j];
				key += (c == '.') ? '0' : '1';

				c = (i+1>=map.size() || j+1>=map[0].size()) ? padChar : map[i+1][j+1];
				key += (c == '.') ? '0' : '1';
				
				std::bitset<9> bits(key);
				newMap[i][j] = table[bits.to_ulong()];
			}
		}
		map = newMap;
	}
	
	for(auto &line:map)
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

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::string table;	
	std::vector<std::string> map;
  parseInput(table, map);

	resultA = enhanceImage(map, table, 2);
 	resultB = enhanceImage(map, table, 50);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
