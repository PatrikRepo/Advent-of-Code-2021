#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Beacon
{
	Beacon(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
	int16_t x = 0;
	int16_t y = 0;
	int16_t z = 0;
};

struct Scanner
{
	int16_t x = 0;
	int16_t y = 0;
	int16_t z = 0;
	std::vector<Beacon> beacons;
};

void parseInput(std::vector<Scanner> &scanners)
{
	std::string line;
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	Scanner scanner;
  	getline(input,line);
  	
		while(getline(input,line))
		{
			if(line == "")
			{
				getline(input,line);
				scanners.push_back(scanner);
				scanner = Scanner();
				continue;
			}
			size_t pos = 0;
			size_t endpos = line.find_first_of(",");
			int x = std::stoi(line.substr(0,endpos));
			
			pos = endpos+1;
			endpos = line.find_first_of(",", pos);
			int y = std::stoi(line.substr(pos,endpos-pos));
			
			pos = endpos+1;
			int z = std::stoi(line.substr(pos));
			
			scanner.beacons.emplace_back(x,y,z);
		}
		scanners.push_back(scanner);
  }
  input.close();
}

bool placeScanner(Scanner &newScanner, const Scanner &foundScanner)
{
	bool result = false;
	
	int alignBeaconFound = -1;
	int alignBeaconNew = -1;
	
	for(unsigned i=0; i<foundScanner.beacons.size(); i++)
	{
		for(unsigned j=0; j<newScanner.beacons.size(); j++)
		{
			unsigned xDist = newScanner.beacons[j].x - foundScanner.beacons[i].x;
			unsigned yDist = newScanner.beacons[j].y - foundScanner.beacons[i].y;
			unsigned zDist = newScanner.beacons[j].z - foundScanner.beacons[i].z;
			
			auto beacons = newScanner.beacons;
			const std::vector<Beacon>* foundBeacons = &foundScanner.beacons;
			unsigned pairsFound = 0;
			
			for(auto &beacon:beacons)
			{
				beacon.x -= xDist;
				beacon.y -= yDist;
				beacon.z -= zDist;
				if(std::find_if(foundBeacons->begin(), foundBeacons->end(), [beacon](Beacon fBeacon) { return beacon.x == fBeacon.x && beacon.y == fBeacon.y && beacon.z == fBeacon.z; }) != std::end(*foundBeacons))
				{
					pairsFound++;
					if(pairsFound == 12)
					{
						alignBeaconFound = i;
						alignBeaconNew = j;
						result = true;
						break;
					}
				}
			}
			if(result)
			{
				break;
			}
		}
		if(result)
		{
			break;
		}
	}
	
	if(result)
	{
		newScanner.x = (newScanner.beacons[alignBeaconNew].x - foundScanner.beacons[alignBeaconFound].x) + foundScanner.x;
		newScanner.y = (newScanner.beacons[alignBeaconNew].y - foundScanner.beacons[alignBeaconFound].y) + foundScanner.y;
		newScanner.z = (newScanner.beacons[alignBeaconNew].z - foundScanner.beacons[alignBeaconFound].z) + foundScanner.z;
	}
	
	return result;
}

bool findScanner(Scanner &newScanner, const Scanner &foundScanner)
{
	bool result = false;
	
	for(unsigned i=0; i<4; i++)
	{
		for(unsigned j=0; j<4; j++)
		{
			for(unsigned k=0; k<4; k++)
			{
				if(placeScanner(newScanner, foundScanner))
				{
					return true;
				}
				for(auto &beacon : newScanner.beacons)
				{
					unsigned temp = beacon.y;
					beacon.y = beacon.x;
					beacon.x = -temp;
				}
			}
			for(auto &beacon : newScanner.beacons)
			{
				unsigned temp = beacon.x;
				beacon.x = beacon.z;
				beacon.z = -temp;
			}
		}
		for(auto &beacon : newScanner.beacons)
		{
			unsigned temp = beacon.z;
			beacon.z = beacon.y;
			beacon.y = -temp;
		}
	}
	
	return result;
}

uint64_t countBeacons(std::vector<Scanner> &scanners)
{
	uint64_t result = 0;
	
	unsigned scannersLeft = scanners.size()-1;
	std::vector<unsigned> scannersFound;
	scannersFound.push_back(0);
	
	while(scannersLeft > 0)
	{
		for(unsigned i=1; i<scanners.size(); i++)
		{
			if(std::find(scannersFound.begin(), scannersFound.end(), i) != std::end(scannersFound))
			{
				continue;
			}
			
			for(auto foundScanner : scannersFound)
			{
				if(findScanner(scanners[i], scanners[foundScanner]))
				{
					scannersLeft--;
					scannersFound.push_back(i);
					break;
				}
			}
		}
	}
	
	std::vector<Beacon> allBeacons;
	
	for(auto &scanner:scanners)
	{
		for(auto &beacon:scanner.beacons)
		{
			if(std::find_if(allBeacons.begin(), allBeacons.end(), [scanner, beacon](Beacon fBeacon) { return (beacon.x-scanner.x) == fBeacon.x && (beacon.y-scanner.y) == fBeacon.y && (beacon.z-scanner.z) == fBeacon.z; }) == std::end(allBeacons))
			{
				allBeacons.emplace_back(beacon.x - scanner.x, beacon.y - scanner.y, beacon.z - scanner.z);
			}
		}
	}
	
	result = allBeacons.size();
	
	return result;
}

uint64_t largestDistance(const std::vector<Scanner> &scanners)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<scanners.size()-1; i++)
	{
		for(unsigned j=i+1; j<scanners.size(); j++)
		{
			uint64_t tempResult = abs(scanners[j].x - scanners[i].x) + abs(scanners[j].y - scanners[i].y) + abs(scanners[j].z - scanners[i].z);
			result = (tempResult > result) ? tempResult : result;
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<Scanner> scanners;
  parseInput(scanners);

	resultA = countBeacons(scanners);
 	resultB = largestDistance(scanners);

  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  
  return 0;
}
