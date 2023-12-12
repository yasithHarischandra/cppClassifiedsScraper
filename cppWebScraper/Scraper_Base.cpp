#include "Scraper_Base.h"
#include <spdlog/spdlog.h>

std::chrono::year_month_day Scraper_Base::getYesterdayDate()
{
    auto currentTime = std::chrono::system_clock::now();

    // Convert to sys_days to work with days precision
    auto currentSysDays = std::chrono::time_point_cast<std::chrono::days>(currentTime);

    // Subtract one day to get yesterday's date
    auto yesterdaySysDays = currentSysDays - std::chrono::days{ 1 };
    //yesterdaySysDays = currentSysDays; used for testing purposes
    std::chrono::year_month_day yesterday = yesterdaySysDays;
    return yesterday;
}

std::chrono::year_month_day Scraper_Base::getLatestSavedClassifiedDate()
{
    //TODO - add code to get date from data source
    //temporary date is returned for testing
    auto currentTime = std::chrono::system_clock::now();

    // Convert to sys_days to work with days precision
    auto currentSysDays = std::chrono::time_point_cast<std::chrono::days>(currentTime);

    // Subtract one day to get yesterday's date
    auto yesterdaySysDays = currentSysDays - std::chrono::days{ 2 };
    std::chrono::year_month_day yesterday = yesterdaySysDays;
    return yesterday;
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

Scraper_Base::Scraper_Base(std::string aUrl, Persistance_Base* aDataSource) : myStartPage(aUrl), myDataSource(aDataSource)
{
	 
}
