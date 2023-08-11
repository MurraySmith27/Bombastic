#ifndef MAPLOADER_HEADER
#define MAPLOADER_HEADER
#include <string>
#include <vector>
class MapLoader
{
public:

	static std::vector<std::vector<char>> LoadMap(std::string path);

};
#endif 

