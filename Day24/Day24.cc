#include <iostream>
#include <fstream>
#include <vector>
#include <array>

struct Instruction
{
	char operation = 0;
	char param1 = 0;
	char param2 = 0;
};

void parseInput(std::vector<Instruction> &program)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
			Instruction instruction;
			if(line[0] == 'm')
			{
				instruction.operation = line[1];
			}
			else
			{
				instruction.operation = line[0];
			}
			
			if(instruction.operation == 'i')
			{
				instruction.param1 = line.back();
			}
			else
			{
				size_t pos = line.find_first_of(" ") + 1;
				instruction.param1 = line[pos];
				pos += 2;
				if(line[pos] == 'x' || line[pos] == 'w' || line[pos] == 'z' || line[pos] == 'y')
				{
					instruction.param2 = line[pos];
				}
				else
				{
					instruction.param2 = std::stoi(line.substr(pos));
				}
			}
			program.push_back(instruction);
  	}
  }
  input.close();
}

void aluAdd(int64_t &a, int64_t b)
{
	a += b;
}

void aluMul(int64_t &a, int64_t b)
{
	a *= b;
}

void aluDiv(int64_t &a, int64_t b)
{
	a /= b;
}

void aluMod(int64_t &a, int64_t b)
{
	a %= b;
}

void aluEql(int64_t &a, int64_t b)
{
	a = (a == b) ? 1 : 0;
}

void aluInp(int64_t &a, int64_t b)
{
	a = b;
}

bool validateModelNumber(const std::vector<Instruction> &program, const std::string &modelNumber)
{
	bool result = false;
		
	int64_t* reg = 0;
	int64_t val = 0;
	std::array<int64_t, 4> registers{0};

	unsigned index = 0;
	
	for(auto &instruction:program)
	{
		switch(instruction.param1)
		{
			case 'w':
			{
				reg = &registers[0];
				break;
			}
			case 'x':
			{
				reg = &registers[1];
				break;
			}
			case 'y':
			{
				reg = &registers[2];
				break;
			}
			case 'z':
			{
				reg = &registers[3];
				break;
			}
		}
		
		switch(instruction.param2)
		{
			case 'w':
			{
				val = registers[0];
				break;
			}
			case 'x':
			{
				val = registers[1];
				break;
			}
			case 'y':
			{
				val = registers[2];
				break;
			}
			case 'z':
			{
				val = registers[3];
				break;
			}
			default:
			{
				val = instruction.param2;
			}
		}
		
		switch(instruction.operation)
		{
			case 'i':
			{
				aluInp(*reg,modelNumber[index] - '0');
				index++;
				break;
			}
			case 'a':
			{
				aluAdd(*reg,val);
				break;
			}
			case 'u':
			{
				aluMul(*reg,val);
				break;
			}
			case 'd':
			{
				aluDiv(*reg,val);
				break;
			}
			case 'o':
			{
				aluMod(*reg,val);
				break;
			}
			case 'e':
			{
				aluEql(*reg,val);
				break;
			}
		}
	}

	if(registers[3] == 0)
	{
		result = true;
	}
	
	return result;
}

uint64_t findLargestModelNumber(const std::vector<Instruction> &program)
{
	uint64_t result = 0;
	
	for(unsigned i=99; i>=11; i--)
	{
		bool done = false;
		for(unsigned j=99; j>=11; j--)
		{
			for(unsigned k=9999; k>=1111; k--)
			{
				std::string modelNumber = std::to_string(i) + "92" + std::to_string(j) + "9597" + std::to_string(k);
				bool zero = false;
				for(auto c:modelNumber)
				{
					if(c == '0')
					{
						zero = true;
						break;
					}
				}
				if(zero)
				{
					continue;
				}
				
				if(validateModelNumber(program, modelNumber))
				{
					result = std::stoll(modelNumber);
					done = true;
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
	
	return result;
}

uint64_t findSmallestModelNumber(const std::vector<Instruction> &program)
{
	uint64_t result = 0;
	
	for(unsigned i=11; i<=99; i++)
	{
		bool done = false;
		for(unsigned j=11; j<=99; j++)
		{
			for(unsigned k=1111; k<=9999; k++)
			{
				std::string modelNumber = std::to_string(i) + "81" + std::to_string(j) + "5131" + std::to_string(k);
				bool zero = false;
				for(auto c:modelNumber)
				{
					if(c == '0')
					{
						zero = true;
						break;
					}
				}
				if(zero)
				{
					continue;
				}
				
				if(validateModelNumber(program, modelNumber))
				{
					result = std::stoll(modelNumber);
					done = true;
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
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<Instruction> program;
	
  parseInput(program);

	resultA = findLargestModelNumber(program);
 	resultB = findSmallestModelNumber(program);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
