#include <iostream>
#include <fstream>
#include <array>
#include <unordered_map>

void parseInput(std::array<unsigned,2> &startPos)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	getline(input,line);
  	size_t pos = line.find_first_of("1234567890");
   	pos = line.find_first_of("1234567890", pos+1);
  	startPos[0] = std::stoi(line.substr(pos));
  	
  	getline(input,line);
		pos = line.find_first_of("1234567890");
   	pos = line.find_first_of("1234567890", pos+1);
  	startPos[1] = std::stoi(line.substr(pos));
  }
  input.close();
}

uint64_t playPracticeGame(const std::array<unsigned,2> &startPos)
{
	uint64_t result = 0;
	
	auto pos = startPos;
	std::array<unsigned,2> score = {0,0};
	bool done = false;
	unsigned i=0;
	unsigned steps = 6;
	unsigned rolls = 0;
	
	while(!done)
	{
		pos[i] += steps;
		pos[i] = (pos[i] > 10) ? pos[i] - 10 : pos[i];
		score[i] += pos[i];
		if(score[i] >= 1000)
		{
			done = true;
		}
		steps = (steps>0) ? steps-1 : 9;
		i = (i+1)%2;
		rolls += 3;
	}
	
	result = score[i]*rolls;
	
	return result;
}

std::array<uint64_t,2> playRealGame(std::array<unsigned,2> pos, std::array<unsigned,2> scores, unsigned player, std::unordered_map<uint64_t,std::array<uint64_t,2>> &cache)
{
	std::array<uint64_t,2> result = {0,0};

	for(unsigned i=1; i<=3; i++)
	{
		for(unsigned j=1; j<=3; j++)
		{
			for(unsigned k=1; k<=3; k++)
			{
				auto oldPos = pos[player];
				auto oldScore = scores[player];
				pos[player] += (i+j+k);
				pos[player] = (pos[player] > 10) ? pos[player]-10 : pos[player];
				scores[player] += pos[player];

				if(scores[player] < 21)
				{
					uint64_t key = pos[0] + (pos[1]*100) + (scores[0]*10000) + (scores[1]*1000000) + player*100000000;
					if(cache.count(key) == 0)
					{
						cache[key] = playRealGame(pos, scores, (player+1)%2, cache);
					}
					result[0] += cache[key][0];
					result[1] += cache[key][1];
				}
				else
				{
					result[player] += 1;
				}
				
				scores[player] = oldScore;
				pos[player] = oldPos;
			}
		}
	}
	
	return result;
}

uint64_t playRealGame(const std::array<unsigned,2> &startPos)
{
	uint64_t result = 0;
	
	auto pos = startPos;
	std::array<unsigned,2> scores = {0,0};
	std::unordered_map<uint64_t,std::array<uint64_t,2>> cache;
	
	std::array<uint64_t,2> wins = playRealGame(pos, scores, 0, cache);
	
	result = (wins[0] > wins[1]) ? wins[0] : wins[1];
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::array<unsigned,2> startPos{0};
	
  parseInput(startPos);

	resultA = playPracticeGame(startPos);
 	resultB = playRealGame(startPos);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
