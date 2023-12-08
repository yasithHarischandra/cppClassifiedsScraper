#include "Scraper_Base.h"

std::chrono::year_month_day Scraper_Base::getYesterdayDate()
{
	return std::chrono::year_month_day();
}

std::chrono::year_month_day Scraper_Base::getLatestSavedClassifiedDate()
{
	return std::chrono::year_month_day();
}

Scraper_Base::Scraper_Base(std::string aUrl) : myStartPage(aUrl)
{
	 
}
