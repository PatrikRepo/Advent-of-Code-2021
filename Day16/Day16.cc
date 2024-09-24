#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <numeric>

void parseInput(std::string &message)
{
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
		getline(input, message);
  }
  input.close();
}

void decodeMessage(std::string &message)
{
	std::string decodedMessage;
	
	for(auto c:message)
	{
		switch(c)
		{
			case '0':
			{
				decodedMessage += "0000";
				break;
			}
			case '1':
			{
				decodedMessage += "0001";
				break;
			}			
			case '2':
			{
				decodedMessage += "0010";
				break;
			}
			case '3':
			{
				decodedMessage += "0011";
				break;
			}
			case '4':
			{
				decodedMessage += "0100";
				break;
			}
			case '5':
			{
				decodedMessage += "0101";
				break;
			}			
			case '6':
			{
				decodedMessage += "0110";
				break;
			}
			case '7':
			{
				decodedMessage += "0111";
				break;
			}
			case '8':
			{
				decodedMessage += "1000";
				break;
			}
			case '9':
			{
				decodedMessage += "1001";
				break;
			}			
			case 'A':
			{
				decodedMessage += "1010";
				break;
			}
			case 'B':
			{
				decodedMessage += "1011";
				break;
			}
			case 'C':
			{
				decodedMessage += "1100";
				break;
			}
			case 'D':
			{
				decodedMessage += "1101";
				break;
			}			
			case 'E':
			{
				decodedMessage += "1110";
				break;
			}
			case 'F':
			{
				decodedMessage += "1111";
				break;
			}
		}
	}
	
	message = decodedMessage;
}

uint64_t addVersionNumbers(const std::string &message, unsigned &index, unsigned count)
{
	uint64_t result = 0;
	
	bool type1 = (count == 0) ? false : true;
	
	if(type1)
	{
		while(count > 0)
		{
			std::bitset<3> bits(message.substr(index,3));
			result += bits.to_ulong();
			index+=3;
			std::string packetType = message.substr(index,3);
			index+=3;

			if(packetType == "100")
			{
				while(message[index] == '1')
				{
					index+=5;
				}
				index+=5;
			}
			else
			{
				if(message[index] == '0')
				{
					index++;
					std::bitset<15> subBits(message.substr(index,15));
					index += 15;
					unsigned newIndex = index + subBits.to_ulong();
					result += addVersionNumbers(message.substr(0,index + 1 + subBits.to_ulong()), index, 0);
					index = newIndex;
				}
				else
				{
					index++;
					std::bitset<11> subBits(message.substr(index,11));
					index += 11;
					result += addVersionNumbers(message, index, subBits.to_ulong());
				}
			}
			count--;
		}
	}
	else
	{
		while(index < message.size()-10)
		{
			std::bitset<3> bits(message.substr(index,3));
			result += bits.to_ulong();
			index+=3;
			std::string packetType = message.substr(index,3);
			index+=3;

			if(packetType == "100")
			{
				while(message[index] == '1')
				{
					index+=5;
				}
				index+=5;
			}
			else
			{
				if(message[index] == '0')
				{
					index++;
					std::bitset<15> subBits(message.substr(index,15));
					index += 15;
					unsigned newIndex = index + subBits.to_ulong();
					result += addVersionNumbers(message.substr(0,index + 1 + subBits.to_ulong()), index, 0);
					index = newIndex;
				}
				else
				{
					index++;
					std::bitset<11> subBits(message.substr(index,11));
					index += 11;
					result += addVersionNumbers(message, index, subBits.to_ulong());
				}
			}
		}
	}
	
	return result;
}

uint64_t addVersionNumbers(const std::string &message)
{
	uint64_t result = 0;
	unsigned index = 0;
	
	result = addVersionNumbers(message, index, 0);
	
	return result;
}

uint64_t calculatePackets(const std::string &message, unsigned &index, unsigned count, unsigned opCode)
{
	uint64_t result = 0;
	
	std::vector<uint64_t> numbers;
	bool type1 = (count == 0) ? false : true;
	
	if(type1)
	{
		while(count > 0)
		{
			index+=3;
			std::bitset<3> bits(message.substr(index,3));
			unsigned packetType = bits.to_ulong();
			index+=3;

			if(packetType == 4)
			{
				std::string numS;
				while(message[index] == '1')
				{
					numS += message.substr(index+1,4);
					index+=5;
				}
				numS += message.substr(index+1,4);
				index+=5;
				std::bitset<64> bits(numS);
				numbers.push_back(bits.to_ulong());
			}
			else
			{
				if(message[index] == '0')
				{
					index++;
					std::bitset<15> subBits(message.substr(index,15));
					index += 15;
					unsigned newIndex = index + subBits.to_ulong();
					numbers.push_back(calculatePackets(message.substr(0,index + 1 + subBits.to_ulong()), index, 0, packetType));
					index = newIndex;
				}
				else
				{
					index++;
					std::bitset<11> subBits(message.substr(index,11));
					index += 11;
					numbers.push_back(calculatePackets(message, index, subBits.to_ulong(), packetType));
				}
			}
			count--;
		}
	}
	else
	{
		while(index < message.size()-10)
		{
			index+=3;
			std::bitset<3> bits(message.substr(index,3));
			unsigned packetType = bits.to_ulong();
			index+=3;

			if(packetType == 4)
			{
				std::string numS;
				while(message[index] == '1')
				{
					numS += message.substr(index+1,4);
					index+=5;
				}
				numS += message.substr(index+1,4);
				index+=5;
				std::bitset<64> bits(numS);
				numbers.push_back(bits.to_ulong());
			}
			else
			{
				if(message[index] == '0')
				{
					index++;
					std::bitset<15> subBits(message.substr(index,15));
					index += 15;
					unsigned newIndex = index + subBits.to_ulong();
					numbers.push_back(calculatePackets(message.substr(0,index + 1 + subBits.to_ulong()), index, 0, packetType));
					index = newIndex;
				}
				else
				{
					index++;
					std::bitset<11> subBits(message.substr(index,11));
					index += 11;
					numbers.push_back(calculatePackets(message, index, subBits.to_ulong(), packetType));
				}
			}
		}
	}
	
	switch(opCode)
	{
		case 0:
		{
			result = std::accumulate(numbers.begin(), numbers.end(), static_cast<uint64_t>(0), std::plus<uint64_t>());
			break;
		}
		case 1:
		{
			result = std::accumulate(numbers.begin(), numbers.end(), static_cast<uint64_t>(1), std::multiplies<uint64_t>());
			break;
		}
		case 2:
		{
			result = *std::min_element(numbers.begin(), numbers.end());
			break;
		}
		case 3:
		{
			result = *std::max_element(numbers.begin(), numbers.end());
			break;
		}
		case 5:
		{
			result = (numbers[0] > numbers[1]) ? 1 : 0;
			break;
		}
		case 6:
		{
			result = (numbers[0] < numbers[1]) ? 1 : 0;
			break;
		}
		case 7:
		{
			result = (numbers[0] == numbers[1]) ? 1 : 0;
		}
	}
	
	return result;
}

uint64_t calculatePackets(const std::string &message)
{
	uint64_t result = 0;
	unsigned index = 0;
	
	result = calculatePackets(message, index, 0, 0);
	
	return result;
}


int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::string message;
  parseInput(message);
  decodeMessage(message);
	
	resultA = addVersionNumbers(message);
 	resultB = calculatePackets(message);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
