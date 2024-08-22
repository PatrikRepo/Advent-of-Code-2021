#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <bitset>

struct Card
{
	std::array<std::array<int,5>,5> cardNumbers{0};
	std::array<unsigned,5> rows{0};
	std::array<unsigned,5> columns{0};
};	

void parseInput(std::vector<int> &numbers, std::vector<Card> &cards)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	getline(input,line);
  	size_t pos = 0;
  	size_t endpos = 0;
  	
  	while(pos < line.size())
  	{
  		endpos = line.find_first_of(",",pos);
  		endpos = (endpos == std::string::npos) ? line.size() : endpos;
  		numbers.push_back(std::stoi(line.substr(pos,endpos-pos)));
  		pos = endpos+1;
  	}
  	
  	getline(input,line);
  	
  	while(getline(input,line))
  	{
  		Card card;
  		
  		for(unsigned i=0; i<5; i++)
  		{
  	 		pos = 0;
  			endpos = 0;
  			for(unsigned j=0; j<5; j++)
  			{
  				pos = line.find_first_of("1234567890",endpos);
  				endpos = line.find_first_of(" ", pos);
  		 		endpos = (endpos == std::string::npos) ? line.size() : endpos;
  				card.cardNumbers[i][j] = std::stoi(line.substr(pos,endpos-pos));
  			}
  			getline(input,line);
  		}
  		cards.push_back(card);
  	}
  }
  input.close();
}

uint64_t playBingo(const std::vector<int> &numbers, std::vector<Card> cards)
{
	uint64_t result = 0;
	
	Card* winningCard = nullptr;
	unsigned winningNum = 0;
	bool done = false;
	
	for(auto number:numbers)
	{
		for(auto& card:cards)
		{
			bool cardDone = false;
			for(unsigned i=0; i<5; i++)
			{
				for(unsigned j=0; j<5; j++)
				{
					if(card.cardNumbers[i][j] == number)
					{
						card.cardNumbers[i][j] = -1;
						card.rows[i]++;
						card.columns[j]++;
						cardDone = true;
						if(card.rows[i] == 5 || card.columns[j] == 5)
						{
							winningCard = &card;
							winningNum = number;
							done = true;
						}
						break;
					}
				}
				if(done || cardDone)
				{
					break;
				}
			}
			if(done)
			{
				break;
			}
		}
		if(done)
		{
			break;
		}
	}
	
	if(winningCard != nullptr)
	{
		for(auto& line:winningCard->cardNumbers)
		{
			for(auto num:line)
			{
				result += (num != -1) ? num : 0;
			}
		}
	}

	result *= winningNum;
	
	return result;
}

uint64_t loseBingo(const std::vector<int> &numbers, std::vector<Card> &cards)
{
	uint64_t result = 0;
	
	Card* winningCard = nullptr;
	unsigned winningNum = 0;
	bool done = false;
	
	for(auto number:numbers)
	{
		for(unsigned c=0; c<cards.size(); c++)
		{
			bool cardDone = false;
			for(unsigned i=0; i<5; i++)
			{
				for(unsigned j=0; j<5; j++)
				{
					if(cards[c].cardNumbers[i][j] == number)
					{
						cards[c].cardNumbers[i][j] = -1;
						cards[c].rows[i]++;
						cards[c].columns[j]++;

						if(cards[c].rows[i] == 5 || cards[c].columns[j] == 5)
						{
							if(cards.size() > 1)
							{
								cards.erase(cards.begin()+c);
								cardDone = true;
								c--;
								break;
								
							}
							else
							{
								winningCard = &cards[c];
								winningNum = number;
								done = true;
								break;
							}
						}
					}
				}
				if(done || cardDone)
				{
					break;
				}
			}
			if(done)
			{
				break;
			}
		}
		if(done)
		{
			break;
		}
	}
	
	if(winningCard != nullptr)
	{
		for(auto& line:winningCard->cardNumbers)
		{
			for(auto num:line)
			{
				result += (num != -1) ? num : 0;
			}
		}
	}

	result *= winningNum;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int> numbers;
	std::vector<Card> cards;
  
  parseInput(numbers, cards);

 	resultA = playBingo(numbers, cards);
 	resultB = loseBingo(numbers, cards);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
