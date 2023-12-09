#include "Scraper_Base.h"
#include <spdlog/spdlog.h>

std::chrono::year_month_day Scraper_Base::getYesterdayDate()
{
	return std::chrono::year_month_day();
}

std::chrono::year_month_day Scraper_Base::getLatestSavedClassifiedDate()
{
	return std::chrono::year_month_day();
}

bool Scraper_Base::convertFromStringToInt(const std::string& aString, int& anInt)
{
    try 
    {
        anInt = std::stoi(aString);
    }
    catch (const std::invalid_argument& e) {
        spdlog::info( aString + "is not a number: " + std::string(e.what())); 
        anInt = -1;
    }
    catch (const std::out_of_range& e) {
        spdlog::error("Out of range: " + std::string(e.what()));
        return false;
    }
	return true;
}

Scraper_Base::Scraper_Base(std::string aUrl) : myStartPage(aUrl)
{
	 
}
