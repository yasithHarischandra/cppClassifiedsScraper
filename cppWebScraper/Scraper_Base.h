/*  This is the abstract base class of all the scrapers for different web sites
*   Have a start web page and a data source connection to send data.
*   ---
*   Written by - Yasith Harischandra
*   This file contains main function
*/
#pragma once

#include <string>
#include <chrono>
#include <tuple>
#include <vector>
#include <memory>

#include "Classified_Base.h"
#include "Persistance_Base.h"

class Scraper_Base
{
protected:
	//TODO - add data source
	Persistance_Base* myDataSource;

	//starting page of the site to be scraped.
	std::string myStartPage;

	std::chrono::year_month_day getYesterdayDate();

	virtual void readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage) = 0;

	virtual std::unique_ptr<Classified_Base> readSingleClassifiedPage(const std::string& aUrl) = 0;

	bool convertFromStringToInt(const std::string& aString, int& anInt);

	virtual bool saveAClassified(const Classified_Base* aclassified) = 0;

public:
	virtual void ReadSiteFrontToBack() = 0;
	Scraper_Base(std::string aUrl, Persistance_Base* aDataSource);

	
};

