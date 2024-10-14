#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <unordered_map>

struct Universe
{
	unsigned cost = 0;
	unsigned heuristic = 0;
	unsigned placed = 0;
	std::vector<std::string> map;
};

struct orderUniverse
{
	bool operator()(const Universe uni1, const Universe uni2) const { if(uni1.placed != uni2.placed) return uni1.placed > uni2.placed; else return uni1.cost + uni1.heuristic < uni2.cost + uni2.heuristic; }
};

void parseInput(std::vector<std::string> &map)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	getline(input,line);
  	while(getline(input,line))
  	{
			map.push_back(line);
  	}
  }
  input.close();
}

std::pair<unsigned,unsigned> stepsToHome(const std::vector<std::string> &map, char type, unsigned startColumn, unsigned homeColumn)
{
	std::pair<unsigned,unsigned> result = {0,0};
	
	bool open = true;
	for(unsigned j=1; j<map.size()-1; j++)
	{
		if(map[j][homeColumn] != '.' && map[j][homeColumn] != type)
		{
			open = false;
			break;
		}
	}
	if(open)
	{
		bool obstructed = false;
		unsigned place = startColumn;
		place += (place < homeColumn) ? 1 : -1;
		while(place != homeColumn)
		{
			if(map[0][place] != '.')
			{
				obstructed = true;
			}
			place += (place < homeColumn) ? 1 : -1;
		}
		if(!obstructed)
		{
			result.first = abs(startColumn - homeColumn); 
			unsigned row = 1;
			while(map[row][homeColumn] == '.')
			{
				result.second++;
				row++;
			}
		}
	}
	
	return result;
}

unsigned getMultiplier(char type)
{
	unsigned result = 1;
	
	switch(type)
	{
		case 'B':
		{
			result = 10;
			break;
		}
		case 'C':
		{
			result = 100;
			break;
		}
		case 'D':
		{
			result = 1000;
			break;
		}
	}
	
	return result;
}

unsigned getHomeColumn(char type)
{
	unsigned result = 3;
	
	switch(type)
	{
		case 'B':
		{
			result = 5;
			break;
		}
		case 'C':
		{
			result = 7;
			break;
		}
		case 'D':
		{
			result = 9;
			break;
		}
	}
	
	return result;
}

unsigned calculateHeuristic(const std::vector<std::string> &map)
{
	unsigned result = 0;
	
	for(unsigned i=1; i<map[0].size()-1; i++)
	{
		if(map[0][i] != '.')
		{
			unsigned temp = abs(i-getHomeColumn(map[0][i]));
			temp *= getMultiplier(map[0][i]);
			result += temp;
		}
	}
	for(unsigned i=3; i<=9; i+=2)
	{
		for(unsigned j=1; j<map.size()-1; j++)
		{
			if(map[j][i] != '.' && i != getHomeColumn(map[j][i]))
			{
				unsigned temp = j + j + abs(i-getHomeColumn(map[j][i]));
				temp *= getMultiplier(map[j][i]);
				result += temp;
			}
		}
	}
	
	return result;
}

uint64_t leastEnergy(const std::vector<std::string> &map, unsigned target)
{
	uint64_t result = UINT64_MAX;
	
	Universe universe;
	universe.map = map;
	std::deque<Universe> universes;
	universes.push_back(universe);
	
	std::unordered_map<std::string, unsigned> cache;
	
	uint64_t countdown = UINT64_MAX;
	
	while(countdown > 0 && !universes.empty())
	{
		countdown--;
		universe = universes.front();
		universes.pop_front();
		
		if(universe.cost > result)
		{
			continue;
		}
		
		if(universe.placed == target)
		{
			
			result = universe.cost;
			if(countdown > 2000)
			{
				countdown = 3000;
			}
			else
			{
				countdown += 1000;
			}

			continue;
		}
		
		std::string key;
		for(auto &line:universe.map)
		{
			key += line;
		}
		cache[key] = universe.cost;
		
		for(unsigned i=1; i<universe.map[0].size()-1; i++)
		{
			if(universe.map[0][i] != '.')
			{
				unsigned energy = 0;
				std::pair<unsigned, unsigned> steps = {0,0};
				unsigned homeColumn = getHomeColumn(universe.map[0][i]);
				steps = stepsToHome(universe.map, universe.map[0][i], i, homeColumn);

				energy = steps.first + steps.second;
				energy *= getMultiplier(universe.map[0][i]);						
				
				if(energy > 0)
				{
					Universe newUniverse = universe;
					newUniverse.placed += getMultiplier(universe.map[0][i]);
					newUniverse.cost += energy;
					newUniverse.map[steps.second][homeColumn] = newUniverse.map[0][i];
					newUniverse.map[0][i] = '.';
					newUniverse.heuristic = calculateHeuristic(newUniverse.map);

					
					if(newUniverse.cost >= result)
					{
						continue;
					}
						
					std::string newKey;
					for(auto &line:newUniverse.map)
					{
						newKey += line;
					}
					if(cache.count(newKey) == 1)
					{
						if(cache.at(newKey) <= newUniverse.cost)
						{
							continue;
						}
					}
					auto it = std::find_if(universes.begin(), universes.end(), [newUniverse](Universe &oUniverse) { 
																																																			bool found = true; 
																																																			for(unsigned i=0; i<oUniverse.map.size(); i++)
																																																				{ if(oUniverse.map[i] != newUniverse.map[i]) 
																																																					{ found = false; break; }} 
																																																			return found;});
					if(it == std::end(universes))
					{
						universes.push_back(newUniverse);

					}
					else
					{
						if(it->cost > newUniverse.cost)
						{
							*it = newUniverse;
						}
					}
				}
			}
		}
		
		for(unsigned i=3; i<=9; i+=2)
		{
			for(unsigned j=1; j<map.size()-1; j++)
			{
				if(universe.map[j][i] != '.')
				{
					bool home = true;
					
					if(i != getHomeColumn(universe.map[j][i]))
					{
						home = false;
					}
					if(home)
					{
						for(unsigned k=j+1; k<universe.map.size()-1; k++)
						{
							if(universe.map[j][i] != universe.map[k][i])
							{
								home = false;
								break;
							}
						}
					}
					if(home)
					{
						break;
					}

					for(unsigned k=i-1; k>0; k--)
					{
						if(universe.map[0][k] != '.')
						{
							break;
						}
						if(universe.map[1][k] != '#')
						{
							continue;
						}
						Universe newUniverse = universe;
						unsigned energy = j+i-k;
						energy *= getMultiplier(universe.map[j][i]);
						newUniverse.cost += energy;
						newUniverse.map[0][k] = universe.map[j][i];
						newUniverse.map[j][i] = '.';
						newUniverse.heuristic = calculateHeuristic(newUniverse.map);
						
						if(newUniverse.cost >= result)
						{
							continue;
						}
						
						std::string newKey;
						for(auto &line:newUniverse.map)
						{
							newKey += line;
						}
						if(cache.count(newKey) == 1)
						{
							if(cache.at(newKey) <= newUniverse.cost)
							{
								continue;
							}
						}
						
						auto it = std::find_if(universes.begin(), universes.end(), [newUniverse](Universe &oUniverse) { 
																																																				bool found = true; 
																																																				for(unsigned i=0; i<oUniverse.map.size(); i++)
																																																					{ if(oUniverse.map[i] != newUniverse.map[i]) 
																																																						{ found = false; break; }} 
																																																				return found;});
						if(it == std::end(universes))
						{
							universes.push_back(newUniverse);
						}
						else
						{
							if(it->cost > newUniverse.cost)
							{
								*it = newUniverse;
							}
						}
					}
					for(unsigned k=i+1; k<map[0].size(); k++)
					{
						if(universe.map[0][k] != '.')
						{
							break;
						}
						if(universe.map[1][k] != '#')
						{
							continue;
						}
						Universe newUniverse = universe;
						unsigned energy = j+k-i;
						energy *= getMultiplier(universe.map[j][i]);
						newUniverse.cost += energy;
						newUniverse.map[0][k] = universe.map[j][i];
						newUniverse.map[j][i] = '.';
						newUniverse.heuristic = calculateHeuristic(newUniverse.map);


						if(newUniverse.cost >= result)
						{
							continue;
						}						

						std::string newKey;
						for(auto &line:newUniverse.map)
						{
							newKey += line;
						}
						if(cache.count(newKey) == 1)
						{
							if(cache.at(newKey) <= newUniverse.cost)
							{
								continue;
							}
						}
							
						auto it = std::find_if(universes.begin(), universes.end(), [newUniverse](Universe &oUniverse) { 
																																																				bool found = true; 
																																																				for(unsigned i=0; i<oUniverse.map.size(); i++)
																																																					{ if(oUniverse.map[i] != newUniverse.map[i]) 
																																																						{ found = false; break; }} 
																																																				return found;});
						if(it == std::end(universes))
						{
							universes.push_back(newUniverse);
						}
						else
						{
							if(it->cost > newUniverse.cost)
							{
								*it = newUniverse;
							}
						}
					}
					break;
				}
			}
		}
		
		std::sort(universes.begin(), universes.end(), orderUniverse());
	}
	
	return result;
}

uint64_t leastEnergy(const std::vector<std::string> &map)
{
	uint64_t result = 0;
	unsigned target = 2222;
	
	result = leastEnergy(map, target);
	
	return result;
}

uint64_t foldedMap(std::vector<std::string> &map)
{
	uint64_t result = 0;
	unsigned target = 4444;
	
	std::string line2 = "  #D#C#B#A#";
	std::string line3 = "  #D#B#A#C#";
	
	map.insert(map.begin()+2, line2);
	map.insert(map.begin()+3, line3);
	
	result = leastEnergy(map, target);
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<std::string> map;
	
  parseInput(map);

	resultA = leastEnergy(map);
 	resultB = foldedMap(map);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
