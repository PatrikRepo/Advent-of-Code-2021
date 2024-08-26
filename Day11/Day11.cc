#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <array>
#include <utility>

void parseInput(std::array<std::array<unsigned,10>,10> &dumbos)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	unsigned i=0;
		while(getline(input,line))
		{
			for(unsigned j=0; j<line.size(); j++)
			{
				dumbos[i][j] = line[j]-'0';
			}
			i++;
		}
  }
  input.close();
}

void flash(size_t x, size_t y, std::array<std::array<unsigned,10>,10> &dumbos, std::deque<std::pair<size_t,size_t>> &flashes)
{
	if(x>0 && y>0)
	{
		dumbos[y-1][x-1]++;
		if(dumbos[y-1][x-1] == 10)
		{
			flashes.push_back(std::make_pair(x-1,y-1));
		}
	}
	if(y>0)
	{
		dumbos[y-1][x]++;
		if(dumbos[y-1][x] == 10)
		{
			flashes.push_back(std::make_pair(x,y-1));
		}
	}
	if(x+1<dumbos[y].size() && y>0)
	{
		dumbos[y-1][x+1]++;
		if(dumbos[y-1][x+1] == 10)
		{
			flashes.push_back(std::make_pair(x+1,y-1));
		}
	}
	if(x>0)
	{
		dumbos[y][x-1]++;
		if(dumbos[y][x-1] == 10)
		{
			flashes.push_back(std::make_pair(x-1,y));
		}
	}
	if(x+1<dumbos[y].size())
	{
		dumbos[y][x+1]++;
		if(dumbos[y][x+1] == 10)
		{
			flashes.push_back(std::make_pair(x+1,y));
		}
	}
	if(x>0 && y+1<dumbos.size())
	{
		dumbos[y+1][x-1]++;
		if(dumbos[y+1][x-1] == 10)
		{
			flashes.push_back(std::make_pair(x-1,y+1));
		}
	}
	if(y+1<dumbos.size())
	{
		dumbos[y+1][x]++;
		if(dumbos[y+1][x] == 10)
		{
			flashes.push_back(std::make_pair(x,y+1));
		}
	}
	if(x+1<dumbos[y].size() && y+1<dumbos.size())
	{
		dumbos[y+1][x+1]++;
		if(dumbos[y+1][x+1] == 10)
		{
			flashes.push_back(std::make_pair(x+1,y+1));
		}
	}
}

uint64_t countFlashes(std::array<std::array<unsigned,10>,10> dumbos)
{
	uint64_t result = 0;
	
	std::deque<std::pair<size_t,size_t>> flashes;
	
	for(unsigned count=0; count<100; count++)
	{
		for(unsigned i=0; i<dumbos.size(); i++)
		{
			for(unsigned j=0; j<dumbos[i].size(); j++)
			{
				dumbos[i][j]++;
				if(dumbos[i][j] == 10)
				{
					flashes.push_back(std::make_pair(j,i));
				}
			}
		}
		
		while(!flashes.empty())
		{
			std::pair<size_t,size_t> dumbo = flashes.front();
			flashes.pop_front();
			
			flash(dumbo.first, dumbo.second, dumbos, flashes);
		}	
		
		for(unsigned i=0; i<dumbos.size(); i++)
		{
			for(unsigned j=0; j<dumbos[i].size(); j++)
			{
				if(dumbos[i][j] > 9)
				{
					dumbos[i][j] = 0;
					result++;
				}
			}
		}
	}
	
	return result;
}

uint64_t synchronizedFlash(std::array<std::array<unsigned,10>,10> &dumbos)
{
	uint64_t result = 0;
	
	std::deque<std::pair<size_t,size_t>> flashes;
	bool done = false;
	unsigned step = 1;
	
	while(!done)
	{
		for(unsigned i=0; i<dumbos.size(); i++)
		{
			for(unsigned j=0; j<dumbos[i].size(); j++)
			{
				dumbos[i][j]++;
				if(dumbos[i][j] == 10)
				{
					flashes.push_back(std::make_pair(j,i));
				}
			}
		}
		
		while(!flashes.empty())
		{
			std::pair<size_t,size_t> dumbo = flashes.front();
			flashes.pop_front();
			
			flash(dumbo.first, dumbo.second, dumbos, flashes);
		}	
		
		unsigned flashNumber = 0;
		for(unsigned i=0; i<dumbos.size(); i++)
		{
			for(unsigned j=0; j<dumbos[i].size(); j++)
			{
				if(dumbos[i][j] > 9)
				{
					dumbos[i][j] = 0;
					flashNumber++;
				}
			}
		}
		if(flashNumber == 100)
		{
			result = step;
			done = true;
		}
		step++;
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::array<std::array<unsigned,10>,10> dumbos;
  
  parseInput(dumbos);

 	resultA = countFlashes(dumbos);
 	resultB = synchronizedFlash(dumbos);
 	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
