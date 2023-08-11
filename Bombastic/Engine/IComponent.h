#ifndef ICOMPONENT_HEADER
#define ICOMPONENT_HEADER

#include <string>
/* An interface describing the functionality shared among all components.
*/
class IComponent {
public:


	virtual ~IComponent();

	/* A string identifier for the component type.
	*/
	static std::string componentType;

};
#endif
