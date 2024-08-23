#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

struct Line
{
	size_t startX = 0;
	size_t endX = 0;
	size_t startY = 0;
	size_t endY = 0;
};

void parseInput(std::vector<Line> &lines)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		while(getline(input,line))
		{
			Line vline;
			size_t pos = 0;
			size_t endpos = line.find_first_of(",");
			vline.startX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = endpos+1;
			endpos = line.find_first_of(" ",pos);
			vline.startY = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("1234567890",endpos);
			endpos = line.find_first_of(",",pos);
			vline.endX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = endpos+1;
			vline.endY = std::stoi(line.substr(pos));
			
			lines.push_back(vline);
  	}
  }
  input.close();
}

uint64_t findCrossings(const std::vector<Line> &lines)
{
	uint64_t result = 0;
	
	std::array<std::array<unsigned,1000>,1000> map{0};
	
	for(unsigned i=0; i<lines.size(); i++)
	{
		if(lines[i].startX != lines[i].endX && lines[i].startY != lines[i].endY)
		{
			continue;
		}
		
		bool horizontal = false;
		
		if(lines[i].startY == lines[i].endY)
		{
			horizontal = true;
		}

		if(horizontal)
		{
			unsigned startX = std::min(lines[i].startX,lines[i].endX);
			unsigned endX = std::max(lines[i].startX,lines[i].endX);
			for(unsigned j=startX; j<=endX; j++)
			{
				map[lines[i].startY][j]++;
			}
		}
		else
		{
			unsigned startY = std::min(lines[i].startY,lines[i].endY);
			unsigned endY = std::max(lines[i].startY,lines[i].endY);
			for(unsigned j=startY; j<=endY; j++)
			{
				map[j][lines[i].startX]++;
			}
		}
	}
	
	for(auto& line:map)
	{
		for(auto num:line)
		{
			result += (num > 1) ? 1 : 0;
		}
	}
	
	return result;
}	

uint64_t withDiagonals(const std::vector<Line> &lines)
{
	uint64_t result = 0;
	
	std::array<std::array<unsigned,1000>,1000> map{0};
	
	for(unsigned i=0; i<lines.size(); i++)
	{
		bool horizontal = false;
		bool vertical = false;
		bool positiveDiag = false;
		
		if(lines[i].startY == lines[i].endY)
		{
			horizontal = true;
		}
		else if(lines[i].startX == lines[i].endX)
		{
			vertical = true;
		}
		else if((lines[i].startX > lines[i].endX && lines[i].startY > lines[i].endY) || (lines[i].startX < lines[i].endX && lines[i].startY < lines[i].endY))
		{
			positiveDiag = true;
		}
		if(horizontal)
		{
			unsigned startX = std::min(lines[i].startX,lines[i].endX);
			unsigned endX = std::max(lines[i].startX,lines[i].endX);
			for(unsigned j=startX; j<=endX; j++)
			{
				map[lines[i].startY][j]++;
			}
		}
		else if(vertical)
		{
			unsigned startY = std::min(lines[i].startY,lines[i].endY);
			unsigned endY = std::max(lines[i].startY,lines[i].endY);
			for(unsigned j=startY; j<=endY; j++)
			{
				map[j][lines[i].startX]++;
			}
		}
		else if(positiveDiag)
		{
			unsigned startX = std::min(lines[i].startX,lines[i].endX);
			unsigned endX = std::max(lines[i].startX,lines[i].endX);
			unsigned startY = std::min(lines[i].startY,lines[i].endY);
			for(unsigned j=0; j<=endX-startX; j++)
			{
				map[startY+j][startX+j]++;
			}
		}
		else
		{
			unsigned startX = std::min(lines[i].startX,lines[i].endX);
			unsigned endX = std::max(lines[i].startX,lines[i].endX);
			unsigned startY = std::max(lines[i].startY,lines[i].endY);
			for(unsigned j=0; j<=endX-startX; j++)
			{
				map[startY-j][startX+j]++;
			}
		}
	}
	
	for(auto& line:map)
	{
		for(auto num:line)
		{
			result += (num > 1) ? 1 : 0;
		}
	}
	
	return result;
}	

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<Line> lines;
  
  parseInput(lines);

 	resultA = findCrossings(lines);
 	resultB = withDiagonals(lines);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
