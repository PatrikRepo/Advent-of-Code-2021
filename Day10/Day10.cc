#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <utility>
#include <algorithm>

void parseInput(std::vector<std::string> &chunks)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		while(getline(input,line))
		{
			chunks.push_back(line);
		}
  }
  input.close();
}

std::pair<uint64_t,uint64_t> findChunks(std::vector<std::string> &chunks)
{
	std::pair<uint64_t,uint64_t> result;
	
	std::vector<char> corruptBrackets;
	std::vector<uint64_t> scores;
	for(unsigned i=0; i<chunks.size(); i++)
	{
		std::deque<char> openBrackets;
		size_t pos = 0;
		bool corrupt = false;
		uint64_t chunkResult = 0;
		
		while(pos < chunks[i].size())
		{
			pos = chunks[i].find_first_of("()[]{}<>", pos);
			switch(chunks[i][pos])
			{
				case '(':
				{
					openBrackets.push_back(')');
					break;
				}
				case '[':
				{
					openBrackets.push_back(']');
					break;
				}
				case '{':
				{
					openBrackets.push_back('}');
					break;
				}
				case '<':
				{
					openBrackets.push_back('>');
					break;
				}
				case ')':
				case ']':
				case '}':
				case '>':
				{
					if(openBrackets.back() == chunks[i][pos])
					{
						openBrackets.pop_back();
					}
					else
					{
						corruptBrackets.push_back(chunks[i][pos]);
						corrupt = true;
						pos = chunks[i].size();
						break;
					}
				}
			}
			pos++;
		}
		if(!corrupt)
		{
			while(!openBrackets.empty())
			{
				chunkResult *= 5;
				switch(openBrackets.back())
				{
					case ')':
					{
						chunkResult += 1;
						break;
					}
					case ']':
					{
						chunkResult += 2;
						break;
					}
					case '}':
					{
						chunkResult += 3;
						break;
					}
					case '>':
					{
						chunkResult += 4;
						break;
					}
				}
				openBrackets.pop_back();
			}
			scores.push_back(chunkResult);
		}
	}
	
	for(auto bracket:corruptBrackets)
	{
		switch(bracket)
		{
			case ')':
			{
				result.first += 3;
				break;
			}
			case ']':
			{
				result.first += 57;
				break;
			}
			case '}':
			{
				result.first += 1197;
				break;
			}
			case '>':
			{
				result.first += 25137;
				break;
			}
		}
	}
	
	std::sort(scores.begin(),scores.end());
	result.second = scores[scores.size()/2];
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	
	std::vector<std::string> chunks;
  
  parseInput(chunks);

 	result = findChunks(chunks);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;
 	
  return 0;
}
