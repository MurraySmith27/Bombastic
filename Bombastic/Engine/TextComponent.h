#ifndef TEXTCOMPONENT_HEADER
#define TEXTCOMPONENT_HEADER
#include <string>
#include "IComponent.h"
class TextComponent : IComponent
{

public:

	std::string text;
	TextComponent(std::string text);

};
#endif

