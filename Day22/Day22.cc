#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

struct Cube
{
	bool on = false;
	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;
	int minZ = 0;
	int maxZ = 0;
};

void parseInput(std::vector<Cube> &cubes)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		Cube cube;
  		cube.on = (line[1] == 'n') ? true : false;
  		
  		size_t pos = line.find_first_of("-1234567890");
  		size_t endpos = line.find_first_of(".",pos);
  		cube.minX = std::stoi(line.substr(pos,endpos-pos));
  		
   		pos = line.find_first_of("-1234567890", endpos);
  		endpos = line.find_first_of(",",pos);
  		cube.maxX = std::stoi(line.substr(pos,endpos-pos));
  		
  		pos = line.find_first_of("-1234567890", endpos);
  		endpos = line.find_first_of(".",pos);
  		cube.minY = std::stoi(line.substr(pos,endpos-pos));
  		
   		pos = line.find_first_of("-1234567890", endpos);
  		endpos = line.find_first_of(",",pos);
  		cube.maxY = std::stoi(line.substr(pos,endpos-pos));
  		
  		pos = line.find_first_of("-1234567890", endpos);
  		endpos = line.find_first_of(".",pos);
  		cube.minZ = std::stoi(line.substr(pos,endpos-pos));
  		
   		pos = line.find_first_of("-1234567890", endpos);
  		cube.maxZ = std::stoi(line.substr(pos));
  		
  		cubes.push_back(cube);
  	}
  }
  input.close();
}

uint64_t initialize(std::vector<Cube> &cubes)
{
	uint64_t result = 0;
	
	bool done = false;
	std::array<std::array<std::array<bool,101>,101>,101> cube{false};
	
	while(!done)
	{
		if(cubes[0].minX < -50 || cubes[0].maxX > 50)
		{
			done = true;
			break;
		}
		
		int minX = (cubes[0].minX >= -50) ? cubes[0].minX : -50;
		int maxX = (cubes[0].maxX <= 50) ? cubes[0].maxX : 50;
		int minY = (cubes[0].minY >= -50) ? cubes[0].minY : -50;
		int maxY = (cubes[0].maxY <= 50) ? cubes[0].maxY : 50;
		int minZ = (cubes[0].minZ >= -50) ? cubes[0].minZ : -50;
		int maxZ = (cubes[0].maxZ <= 50) ? cubes[0].maxZ : 50;
		
		for(int i=minX+50; i<=maxX+50; i++)
		{
			for(int j=minY+50; j<=maxY+50; j++)
			{
				for(int k=minZ+50; k<=maxZ+50; k++)
				{
					cube[i][j][k] = cubes[0].on;
				}
			}
		}
		cubes.erase(cubes.begin());
	}
	
	for(auto &plane:cube)
	{
		for(auto &line:plane)
		{
			for(auto &cuboid:line)
			{
				result += cuboid ? 1 : 0;
			}
		}
	}
	
	return result;
}

uint64_t reboot(const std::vector<Cube> &cubes)
{
	uint64_t result = 0;
	
	std::vector<std::vector<Cube>> allCubes;
	allCubes.push_back(std::vector<Cube>());
	for(const auto &cube:cubes)
	{
		std::vector<Cube> intersections;
		
		if(cube.on)
		{
			intersections.push_back(cube);
			uint64_t product = (cube.maxX - cube.minX + 1);
			product *= (cube.maxY - cube.minY + 1);
			product *= (cube.maxZ - cube.minZ + 1);
			result += product;
		}
		
		for(unsigned i=0; i<allCubes.size(); i++)
		{
			unsigned stop = allCubes[i].size();

			for(auto &newInter:intersections)
			{
				allCubes[i].push_back(newInter);
			}

			intersections.clear();
			
			for(unsigned j=0; j<stop; j++)
			{
				Cube* intersection = &allCubes[i][j];
				Cube newIntersection;
				newIntersection.minX = std::max(intersection->minX,cube.minX);
				newIntersection.maxX = std::min(intersection->maxX,cube.maxX);
				newIntersection.minY = std::max(intersection->minY,cube.minY);
				newIntersection.maxY = std::min(intersection->maxY,cube.maxY);
				newIntersection.minZ = std::max(intersection->minZ,cube.minZ);
				newIntersection.maxZ = std::min(intersection->maxZ,cube.maxZ);
				newIntersection.on = cube.on;
				if(newIntersection.minX <= newIntersection.maxX && newIntersection.minY <= newIntersection.maxY && newIntersection.minZ <= newIntersection.maxZ)
				{
					intersections.push_back(newIntersection);
					uint64_t product = (newIntersection.maxX - newIntersection.minX + 1);
					product *= (newIntersection.maxY - newIntersection.minY + 1);
					product *= (newIntersection.maxZ - newIntersection.minZ + 1);

					if(i%2 == 1)
					{
						result += product;
					}
					else
					{
						result -= product;
					}
				}
			}
			
			if(intersections.empty())
			{
				break;
			}
			if(i+1 == allCubes.size())
			{
				allCubes.push_back(std::vector<Cube>());
			}
		}	
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<Cube> cubes;
	
  parseInput(cubes);

	resultA = initialize(cubes);
 	resultB = resultA + reboot(cubes);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
