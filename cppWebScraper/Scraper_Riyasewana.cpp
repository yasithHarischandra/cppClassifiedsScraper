#include "Scraper_Riyasewana.h"
#include "Classified_Vehicle.h"
#include "Scraper_WebPageLoader.h"

#include <spdlog/spdlog.h>
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

#include <sstream>
#include <regex>

void Scraper_Riyasewana::readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage)
{
	spdlog::info("Reading List Page - " + aUrl);

	Scraper_WebPageLoader pageLoader(aUrl);
	if (!pageLoader.GetLoadStatus())
		return;

	auto doc = pageLoader.GetDoc();
	auto context = pageLoader.GetContext();

	xmlXPathObjectPtr classified_elements = xmlXPathEvalExpression((xmlChar*)"//li[contains(@class, 'item round')]", context);

	for (int i = 0; i < classified_elements->nodesetval->nodeNr; ++i)
	{
		// get the current element of the loop
		xmlNodePtr classified_html_element = classified_elements->nodesetval->nodeTab[i];
		// set the libxml2 context to the current element
		// to limit the XPath selectors to its children
		xmlXPathSetContextNode(classified_html_element, context);
		//skip promoted classifieds
		if (std::string(reinterpret_cast<char*>(xmlGetProp(classified_html_element, (xmlChar*)"class"))) == "item round promoted")
			continue;

		//individual classified URL
		xmlNodePtr url_html_element = xmlXPathEvalExpression((xmlChar*)".//h2/a", context)->nodesetval->nodeTab[0];
		std::string url = std::string(reinterpret_cast<char*>(xmlGetProp(url_html_element, (xmlChar*)"href")));
		std::string title = std::string(reinterpret_cast<char*>(xmlGetProp(url_html_element, (xmlChar*)"title")));
		aListOfUrl.emplace_back(url);

	}
	xmlXPathFreeObject(classified_elements);
	
	//find the URL of next page
	xmlXPathObjectPtr nextPageElement = xmlXPathEvalExpression((xmlChar*)"//a[text()='Next']", context);
	if (nextPageElement == NULL)
	{
		spdlog::error("No next page found\n");
		return;
	}

	xmlNodePtr nextPageUrlElement = nextPageElement->nodesetval->nodeTab[0];
	nextPage = "https:" + std::string(reinterpret_cast<char*>(xmlGetProp(nextPageUrlElement, (xmlChar*)"href")));
	
	xmlXPathFreeObject(nextPageElement);
	
	return;
}

std::unique_ptr<Classified_Base> Scraper_Riyasewana::readSingleClassifiedPage(const std::string& aUrl)
{
	spdlog::info("Reading classified Page - " + aUrl);

	Scraper_WebPageLoader pageLoader(aUrl);
	if (!pageLoader.GetLoadStatus())
	{
		spdlog::error("Failed opening web page - " + aUrl);
		return nullptr;
	}

	auto doc = pageLoader.GetDoc();
	auto context = pageLoader.GetContext();


	///////////////find the titles of the page
	xmlXPathObjectPtr titleDivNode = xmlXPathEvalExpression((xmlChar*)"//div[contains(@id, 'content')]", context);
	if (titleDivNode == NULL)
	{
		spdlog::error("Page title not found\n");
		return nullptr;
	}
	// get the current element of the loop
	xmlNodePtr classified_html_element = titleDivNode->nodesetval->nodeTab[0];
	// set the libxml2 context to the current element
	// to limit the XPath selectors to its children
	xmlXPathSetContextNode(classified_html_element, context);

	
	xmlNodePtr h1Node = xmlXPathEvalExpression((xmlChar*)".//h1", context)->nodesetval->nodeTab[0];
	std::string pageMainTitle = std::string(reinterpret_cast<char*>(xmlNodeGetContent(h1Node)));
	xmlNodePtr h2Node = xmlXPathEvalExpression((xmlChar*)".//h2", context)->nodesetval->nodeTab[0];
	std::string pageSubTitle = std::string(reinterpret_cast<char*>(xmlNodeGetContent(h2Node)));
	xmlXPathFreeObject(titleDivNode);
	///////////////

	//////////////////traverse through properties table
	std::string contactNo, price, Make, Model, yomStr, mileageStr, gear, fuelType, options, engineCapacity;
	std::string details, startType;
	xmlXPathObjectPtr tableRowNodes = xmlXPathEvalExpression((xmlChar*)"//tr", context);
	for (int i = 0; i < tableRowNodes->nodesetval->nodeNr; ++i)
	{
		
		// get the current element of the loop
		xmlNodePtr rowElement = tableRowNodes->nodesetval->nodeTab[i];
		// set the libxml2 context to the current element
		// to limit the XPath selectors to its children
		xmlXPathSetContextNode(rowElement, context);

		//read tr->td->p tag and see if it is "Contact"
		xmlXPathObjectPtr labelColumns = xmlXPathEvalExpression((xmlChar*)".//td/p", context);
		if (labelColumns == nullptr || labelColumns->nodesetval->nodeNr <= 0)
			continue;

		xmlNodePtr firstColumn = xmlXPathEvalExpression((xmlChar*)".//td/p", context)->nodesetval->nodeTab[0];
		if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "Contact")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td/span", context);
			contactNo = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[0])));
			price = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			xmlXPathFreeObject(values);
		}
		else if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "Make")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td", context);
			Make = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			Model = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[3])));
			xmlXPathFreeObject(values);
		}
		else if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "YOM")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td", context);
			yomStr = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			mileageStr = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[3])));
			xmlXPathFreeObject(values);
		}
		else if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "Gear")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td", context);
			gear = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			fuelType = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[3])));
			xmlXPathFreeObject(values);
		}
		else if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "Options")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td", context);
			options = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			engineCapacity = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[3])));
			xmlXPathFreeObject(values);
		}
		// for motor bikes
		else if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "Engine (cc)")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td", context);
			engineCapacity = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			startType = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[3])));
			xmlXPathFreeObject(values);
		}
		else if (std::string(reinterpret_cast<char*>(xmlNodeGetContent(firstColumn))) == "Details")
		{
			xmlXPathObjectPtr values = xmlXPathEvalExpression((xmlChar*)".//td", context);
			xmlChar* unidet = xmlNodeGetContent(values->nodesetval->nodeTab[1]);
			details = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			
			xmlXPathFreeObject(values);
		}
		else
		{
			continue;
		}
		

	}

	std::string city = getCityFromTitle(pageSubTitle);
	if (city == "")
	{
		spdlog::error("Error reading city from title - " + pageSubTitle);
		return nullptr;
	}
	std::chrono::year_month_day date = getDateFromTitle(pageSubTitle);
	std::string vehicleType = getVehicleType(pageMainTitle);
	int yom, mileage;
	if (!convertFromStringToInt(yomStr, yom))
	{
		spdlog::error("Error converting YOM to integer - " + yomStr);
		return nullptr;
	}
		
	if (!convertFromStringToInt(mileageStr, mileage))
	{
		spdlog::error("Error converting mileage to integer - " + mileageStr);
		return nullptr;
	}

	std::unique_ptr<Classified_Base> classifiedPtr = std::make_unique<Classified_Vehicle>(price, city, contactNo, details, aUrl, date, Make, Model, yom, mileage, gear, fuelType, vehicleType, startType, options);

	return classifiedPtr;
}

std::string Scraper_Riyasewana::getCityFromTitle(const std::string& aTitle)
{
	//city name is aftere the last comma,
	std::string s;
	std::stringstream ss(aTitle);
	std::vector<std::string> v;
	while (getline(ss, s, ',')) {

		// store token string in the vector
		v.push_back(s);
	}
	if(v.empty())
		throw std::invalid_argument("Could not find a valid city in string - " + aTitle);

	return v.back().erase(0, 1);	//remove space at the start
}

std::chrono::year_month_day Scraper_Riyasewana::getDateFromTitle(const std::string& aTitle)
{
	std::chrono::year_month_day date;

	// Define the regular expression pattern to match the date
	std::regex pattern("\\b\\d{4}-\\d{2}-\\d{2}\\b");

	// Search for matches in the input string
	std::smatch match;
	if (std::regex_search(aTitle, match, pattern)) 
	{
		std::string s;
		std::stringstream ss(match[0]);
		std::vector<std::string> v;
		while (getline(ss, s, '-'))
			// store token string in the vector
			v.push_back(s);
		date = std::chrono::year_month_day(static_cast<std::chrono::year>(atoi(v[0].c_str())), static_cast<std::chrono::month>(atoi(v[1].c_str())), static_cast<std::chrono::day>(atoi(v[2].c_str())) );
	}
	else {
		// No match found
		spdlog::error("Could not extract date from title - " + aTitle);
		throw std::invalid_argument("Could not find a valid date in string - " + aTitle);
	}
	return date;
}

std::string Scraper_Riyasewana::getVehicleType(const std::string& aTitle)
{
	std::string vType = "";
	if (aTitle.find(" Heavy-Duty ") != std::string::npos)
	{
		vType = "heavyduty";
	}
	else if (aTitle.find(" Lorry ") != std::string::npos)
	{
		vType = "lorry";
	}
	else if (aTitle.find(" Motorbike ") != std::string::npos)
	{
		vType = "motorbike";
	}
	else if (aTitle.find(" Three Wheel ") != std::string::npos)
	{
		vType = "threewheel";
	}
	else if (aTitle.find(" Van ") != std::string::npos)
	{
		vType = "van";
	}
	else if (aTitle.find(" SUV ") != std::string::npos)
	{
		vType = "suv";
	}
	else if (aTitle.find(" Crew Cab ") != std::string::npos)
	{
		vType = "crewcab";
	}
	else if (aTitle.find(" Pickup ") != std::string::npos)
	{
		vType = "pickup";
	}
	else if (aTitle.find(" Bus ") != std::string::npos)
	{
		vType = "bus";
	}
	else if (aTitle.find(" Bicycle ") != std::string::npos)
	{
		vType = "bicycle";
	}
	else if (aTitle.find(" Car ") != std::string::npos)
	{
		vType = "car";
	}
	else if (aTitle.find(" Other ") != std::string::npos)
	{
		vType = "other";
	}
	else
	{
		vType = "other";
	}
	
	return vType;
}

Scraper_Riyasewana::Scraper_Riyasewana() : Scraper_Base(std::string{ "https://riyasewana.com/search" })
{
	
}

void Scraper_Riyasewana::ReadSiteFrontToBack()
{
	auto yesterday = getYesterdayDate();
	auto lastClassifiedDay = getLatestSavedClassifiedDate();

	std::string currentPage = myStartPage;
	std::string nextPage = myStartPage;
	

	while (nextPage != "")
	{
		std::vector<std::string> urlListOfClassifieds;
		std::vector < std::shared_ptr<Classified_Base> > classifiedData;
		bool reachedDataSource = false;
		readClassifiedListPage(currentPage, urlListOfClassifieds, nextPage);

		
		for (auto aUrl : urlListOfClassifieds)
		{
			std::shared_ptr<Classified_Base> aClassified = readSingleClassifiedPage(aUrl);

			//skip classifieds newer than yesterday
			if (aClassified->GetDate() > yesterday)
				continue;

			//no need to go through classifieds already in database
			if (aClassified->GetDate() <= lastClassifiedDay)
			{
				spdlog::info("Finished reading classifieds from riyasewana.com.");
				reachedDataSource = true;
				break;
			}

			classifiedData.push_back(aClassified);
		}

		while (!classifiedData.empty())
		{
			saveAClassified(classifiedData.back().get());
			classifiedData.pop_back();
		}
		
	}


	//readClassifiedListPage(myStartPage, urlListOfClassifieds, nextPage);
	//readSingleClassifiedPage(urlListOfClassifieds[0]);
	//readSingleClassifiedPage("https://riyasewana.com/buy/bajaj-4-stroke-sale-kurunegala-7346452");
}
