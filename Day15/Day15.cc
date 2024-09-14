#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>

struct Walker
{
	size_t x = 0;
	size_t y = 0;
	unsigned cost = 0;
};

struct SortWalkers
{
	bool operator()(const Walker walker1, const Walker walker2) const { return walker1.cost < walker2.cost; }
};

void parseInput(std::vector<std::string> &map)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		while(getline(input,line))
		{
			map.push_back(line);
		}
  }
  input.close();
}

uint64_t traverseMap(const std::vector<std::string> &map, unsigned multiplier)
{
	uint64_t result = 0;
	
	const unsigned mapSizeY = map.size();
	const unsigned mapSizeX = map[0].size();
	const size_t goalX = (mapSizeX*multiplier) -1;
	const size_t goalY = (mapSizeY*multiplier) -1;
	
	std::deque<Walker> openList;
	std::vector<Walker> closedList;
	Walker walker;
	openList.push_back(walker);
	
	while(!openList.empty())
	{
		walker = openList.front();
		openList.pop_front();
		
		if(walker.x == goalX && walker.y == goalY)
		{
			result = walker.cost;
			break;
		}
		
		closedList.push_back(walker);
		
		Walker newWalker;
		std::deque<Walker> newWalkers;
		if(walker.x > 0)
		{
			newWalker = walker;
			newWalker.x--;
			unsigned addedCost = map[newWalker.y % mapSizeY][newWalker.x % mapSizeX] - '0';
			addedCost += (newWalker.x/mapSizeX) + (newWalker.y/mapSizeY);
			addedCost = (addedCost > 9) ? addedCost-9 : addedCost;
			newWalker.cost += addedCost;
			newWalkers.push_back(newWalker);
		}
		if(walker.x < goalX)
		{
			newWalker = walker;
			newWalker.x++;
			unsigned addedCost = map[newWalker.y % mapSizeY][newWalker.x % mapSizeX] - '0';
			addedCost += (newWalker.x/mapSizeX) + (newWalker.y/mapSizeY);
			addedCost = (addedCost > 9) ? addedCost-9 : addedCost;
			newWalker.cost += addedCost;
			newWalkers.push_back(newWalker);
		}
		if(walker.y > 0)
		{
			newWalker = walker;
			newWalker.y--;
			unsigned addedCost = map[newWalker.y % mapSizeY][newWalker.x % mapSizeX] - '0';
			addedCost += (newWalker.x/mapSizeX) + (newWalker.y/mapSizeY);
			addedCost = (addedCost > 9) ? addedCost-9 : addedCost;
			newWalker.cost += addedCost;
			newWalkers.push_back(newWalker);
		}
		if(walker.y < goalY)
		{
			newWalker = walker;
			newWalker.y++;
			unsigned addedCost = map[newWalker.y % mapSizeY][newWalker.x % mapSizeX] - '0';
			addedCost += (newWalker.x/mapSizeX) + (newWalker.y/mapSizeY);
			addedCost = (addedCost > 9) ? addedCost-9 : addedCost;
			newWalker.cost += addedCost;
			newWalkers.push_back(newWalker);
		}
		
		while(!newWalkers.empty())
		{
			newWalker = newWalkers.front();
			newWalkers.pop_front();
			auto it = std::find_if(openList.begin(), openList.end(), [newWalker](Walker oWalker){ return newWalker.x == oWalker.x && newWalker.y == oWalker.y; });
			if(it != std::end(openList))
			{
				it->cost = (it->cost <= newWalker.cost) ? it->cost : newWalker.cost;
			}
			else
			{
				if(std::find_if(closedList.begin(), closedList.end(), [newWalker](Walker cWalker){ return newWalker.x == cWalker.x && newWalker.y == cWalker.y; }) == std::end(closedList))
				{
					openList.push_back(newWalker);
				}
			}
		}
		
		std::sort(openList.begin(), openList.end(), SortWalkers());
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> map;
  
  parseInput(map);

 	resultA = traverseMap(map, 1);
 	resultB = traverseMap(map, 5);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
