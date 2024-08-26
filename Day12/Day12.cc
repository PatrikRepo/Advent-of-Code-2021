#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <tuple>

void parseInput(std::unordered_map<std::string,std::vector<std::string>> &caves)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		while(getline(input,line))
		{
			size_t pos = line.find_first_of("-");
			std::string from = line.substr(0,pos);
			std::string to = line.substr(pos+1);
			
			if(caves.count(from) == 0)
			{
				caves[from] = std::vector<std::string>();
			}
			caves[from].push_back(to);
			
			if(caves.count(to) == 0)
			{
				caves[to] = std::vector<std::string>();
			}
			caves[to].push_back(from);
		}
  }
  input.close();
}

uint64_t findPaths(const std::unordered_map<std::string,std::vector<std::string>> &caves)
{
	uint64_t result = 0;
	
	std::deque<std::pair<std::string,std::vector<std::string>>> openList;
	
	openList.push_back(std::make_pair("start",std::vector<std::string>()));
	
	while(!openList.empty())
	{
		auto cave = openList.front();
		openList.pop_front();
		if(cave.first == "end")
		{
			result++;
			continue;
		}
		if(cave.first[0] >= 'a' && cave.first[0] <= 'z')
		{
			cave.second.push_back(cave.first);
		}
		
		for(auto& nCave : caves.at(cave.first))
		{
			if(std::find_if(cave.second.begin(),cave.second.end(),[nCave](std::string cCave) { return nCave == cCave; }) == std::end(cave.second))
			{
				openList.push_back(std::make_pair(nCave,cave.second));
			}
		}
	}
	
	return result;
}

uint64_t morePaths(const std::unordered_map<std::string,std::vector<std::string>> &caves)
{
	uint64_t result = 0;
	
	std::deque<std::tuple<std::string,std::vector<std::string>,bool>> openList;
	
	openList.push_back(std::make_tuple("start",std::vector<std::string>(),false));
	
	while(!openList.empty())
	{
		auto cave = openList.front();
		openList.pop_front();
		if(std::get<0>(cave) == "end")
		{
			result++;
			continue;
		}
		if(std::get<0>(cave)[0] >= 'a' && std::get<0>(cave)[0] <= 'z')
		{
			std::get<1>(cave).push_back(std::get<0>(cave));
		}
		
		for(auto& nCave : caves.at(std::get<0>(cave)))
		{
			if(nCave != "start" && !std::get<2>(cave) && std::count_if(std::get<1>(cave).begin(),std::get<1>(cave).end(),[nCave](std::string cCave) { return nCave == cCave; }) == 1)
			{
				openList.push_back(std::make_tuple(nCave,std::get<1>(cave),true));
			}
			if(nCave != "start" && std::count_if(std::get<1>(cave).begin(),std::get<1>(cave).end(),[nCave](std::string cCave) { return nCave == cCave; }) == 0)
			{
				openList.push_back(std::make_tuple(nCave,std::get<1>(cave),std::get<2>(cave)));
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::unordered_map<std::string,std::vector<std::string>> caves;
  
  parseInput(caves);

 	resultA = findPaths(caves);
 	resultB = morePaths(caves);
 	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
