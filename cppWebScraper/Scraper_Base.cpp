#include "Scraper_Base.h"
#include <spdlog/spdlog.h>

std::chrono::year_month_day Scraper_Base::getYesterdayDate()
{
    auto currentTime = std::chrono::system_clock::now();

    // Convert to sys_days to work with days precision
    auto currentSysDays = std::chrono::time_point_cast<std::chrono::days>(currentTime);

    // Subtract one day to get yesterday's date
    auto yesterdaySysDays = currentSysDays - std::chrono::days{ 1 };
    //yesterdaySysDays = currentSysDays; //used for testing purposes
    std::chrono::year_month_day yesterday = yesterdaySysDays;
    return yesterday;
}

bool Scraper_Base::convertFromStringToInt(const std::string& aString, int& anInt)
{
    try 
    {
        std::string str = aString;
        std::erase(str, ',');
        anInt = std::stoi(str);
    }
    catch (const std::invalid_argument& e) {
        spdlog::info( "\"" + aString + "\"" + " is not a number: " + std::string(e.what()));
        anInt = -1;
        return false;
    }
    catch (const std::out_of_range& e) {
        spdlog::error("Out of range: " + std::string(e.what()));
        anInt = -1;
        return false;
    }
	return true;
}

Scraper_Base::Scraper_Base(std::string aUrl, Persistance_Base* aDataSource) : myStartPage(aUrl), myDataSource(aDataSource)
{
	 
}
