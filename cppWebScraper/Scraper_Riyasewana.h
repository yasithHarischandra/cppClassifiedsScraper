#pragma once
#include "Scraper_Base.h"
class Scraper_Riyasewana : public Scraper_Base
{
protected:
	void readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage) override;

	std::unique_ptr<Classified_Base> readSingleClassifiedPage(const std::string& aUrl) override;

	std::string getCityFromTitle(const std::string& aTitle);

public:
	Scraper_Riyasewana();

	void ReadSiteFrontToBack() override;
};

