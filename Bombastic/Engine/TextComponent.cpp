#include "stdafx.h"
#include "TextComponent.h"

TextComponent::TextComponent(std::string text) : text(text) {
	this->componentType = "text";
}