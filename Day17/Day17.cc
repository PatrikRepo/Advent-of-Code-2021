#include <iostream>
#include <fstream>
#include <utility>

struct Target
{
	unsigned xMin = 0;
	unsigned xMax = 0;
	int yMin = 0;
	int yMax = 0;
};

void parseInput(Target &target)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		getline(input, line);
		
		size_t pos = line.find_first_of("-1234567890");
		size_t endpos = line.find_first_of(".", pos);
		target.xMin = std::stoi(line.substr(pos, endpos-pos));
		
		pos = line.find_first_of("-1234567890", endpos);
		endpos = line.find_first_of(",", pos);
		target.xMax = std::stoi(line.substr(pos, endpos-pos));
		
		pos = line.find_first_of("-1234567890", endpos);
		endpos = line.find_first_of(".", pos);
		target.yMin = std::stoi(line.substr(pos, endpos-pos));
		
		pos = line.find_first_of("-1234567890", endpos);
		target.yMax = std::stoi(line.substr(pos));
  }
  input.close();
}

uint64_t findHighest(const Target &target)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<100; i++)
	{
		int64_t height = (i*(i+1))/2;
		uint64_t tempResult = height;
		unsigned step = 1;
		while(height >= target.yMin)
		{
			height -= step;
			step++;
			if(height <= target.yMax && height >= target.yMin)
			{
				result = tempResult;
				break;
			}
		}
	}
	
	return result;
}

uint64_t findAllVelocities(const Target &target)
{
	uint64_t result = 0;
	
	for(unsigned i=1; i<=target.xMax; i++)
	{
		unsigned xDist = 0;
		unsigned steps = 0;
		unsigned xStep = i;
		unsigned from = 0;
		unsigned to = 0;
		while(xStep > 0)
		{
			xDist += xStep;
			steps++;
			xStep--;
			if(xDist >= target.xMin && xDist <= target.xMax)
			{
				if(from == 0)
				{
					from = steps;
				}
				to = (xStep == 0) ? 1000 : steps;
			}
			if(xDist >= target.xMax)
			{
				break;
			}
		}
		
		if(from == 0)
		{
			continue;
		}
		
		for(int j=target.yMin; j<100; j++)
		{
			int height = 0;
			steps = 0;
			int yStep = j;
			
			while(steps < to)
			{
				height += yStep;
				steps++;
				yStep--;
				if(steps >= from && height <= target.yMax && height >= target.yMin)
				{
					result++;
					break;
				}
				if(height < target.yMin)
				{
					break;
				}
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	Target target;
  parseInput(target);
	
	resultA = findHighest(target);
 	resultB = findAllVelocities(target);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
