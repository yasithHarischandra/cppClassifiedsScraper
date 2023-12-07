#pragma once
#include "Scraper_Base.h"
class Scraper_Riyasewana : public Scraper_Base
{
protected:
	void readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage) override;


public:
	Scraper_Riyasewana();

	void ReadSiteFrontToBack() override;
};

