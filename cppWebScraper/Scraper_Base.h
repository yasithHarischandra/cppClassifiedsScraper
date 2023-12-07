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

class Scraper_Base
{
protected:
	//TODO - add data source

	//starting page of the site to be scraped.
	std::string myStartPage;

	std::chrono::year_month_day getYesterdayDate();

	std::chrono::year_month_day getLatestSavedClassifiedDate();

	virtual std::tuple<std::vector<std::string>, std::string> readClassifiedListPage(const std::string& aUrl) = 0;

public:
	virtual void ReadSiteFrontToBack() = 0;
	Scraper_Base(std::string aUrl) : myStartPage(aUrl) {};
};

