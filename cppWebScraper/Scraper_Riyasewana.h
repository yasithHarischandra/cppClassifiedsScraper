#pragma once
#include "Scraper_Base.h"
class Scraper_Riyasewana : public Scraper_Base
{
protected:
	std::tuple<std::vector<std::string>, std::string> readClassifiedListPage(const std::string& aUrl) override;


public:
	Scraper_Riyasewana();

	void ReadSiteFrontToBack() override;
};

