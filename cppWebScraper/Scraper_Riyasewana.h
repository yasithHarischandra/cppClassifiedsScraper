#pragma once
#include "Scraper_Base.h"
class Scraper_Riyasewana : public Scraper_Base
{
protected:
	void readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage) override;

	std::unique_ptr<Classified_Base> readSingleClassifiedPage(const std::string& aUrl) override;

	bool saveAClassified(const Classified_Base* aclassified) override;

	//the sub string after the last comma is taken as the city
	//if no city is found, throws std::invalid_argument.
	std::string getCityFromTitle(const std::string& aTitle);
	std::chrono::year_month_day getDateFromTitle(const std::string& aTitle);
	std::string getVehicleType(const std::string& aTitle);

public:
	Scraper_Riyasewana(Persistance_Base* aDataSource);

	void ReadSiteFrontToBack() override;
};

