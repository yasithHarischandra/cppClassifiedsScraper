#include "Classified_Base.h"

std::string Classified_Base::DateStr() const
{
	std::string date = std::to_string(static_cast<int>(myDate.year()))
		+ "-" + std::to_string(static_cast<unsigned>(myDate.month()))
		+ "-" + std::to_string(static_cast<unsigned>(myDate.day()));

	return date;
}