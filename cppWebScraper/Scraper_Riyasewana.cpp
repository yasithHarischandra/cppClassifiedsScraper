#include "Scraper_Riyasewana.h"
#include "Classified_Vehicle.h"
#include "Scraper_WebPageLoader.h"

#include <spdlog/spdlog.h>
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

#include <sstream>

void Scraper_Riyasewana::readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage)
{
	spdlog::info("Starting List Page - " + aUrl);

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
		spdlog::info("No next page found\n");
		return;
	}

	xmlNodePtr nextPageUrlElement = nextPageElement->nodesetval->nodeTab[0];
	nextPage = "https:" + std::string(reinterpret_cast<char*>(xmlGetProp(nextPageUrlElement, (xmlChar*)"href")));
	
	xmlXPathFreeObject(nextPageElement);
	
	return;
}

std::unique_ptr<Classified_Base> Scraper_Riyasewana::readSingleClassifiedPage(const std::string& aUrl)
{
	spdlog::info("Starting List Page - " + aUrl);

	Scraper_WebPageLoader pageLoader(aUrl);
	if (!pageLoader.GetLoadStatus())
		return nullptr;

	auto doc = pageLoader.GetDoc();
	auto context = pageLoader.GetContext();


	///////////////find the titles of the page
	xmlXPathObjectPtr titleDivNode = xmlXPathEvalExpression((xmlChar*)"//div[contains(@id, 'content')]", context);
	if (titleDivNode == NULL)
	{
		spdlog::info("Page title not found\n");
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
	std::string contactNo, price, Make, Model, YoM, mileage, gear, fuelType, options, engineCapacity;
	std::string details, startType, city;
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
			YoM = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[1])));
			mileage = std::string(reinterpret_cast<char*>(xmlNodeGetContent(values->nodesetval->nodeTab[3])));
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

	city = getCityFromTitle(pageSubTitle);

	return std::unique_ptr<Classified_Base>();
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
	return v.back().erase(0, 1);	//remove space at the start
}

Scraper_Riyasewana::Scraper_Riyasewana() : Scraper_Base(std::string{ "https://riyasewana.com/search" })
{
	
}

void Scraper_Riyasewana::ReadSiteFrontToBack()
{
	std::string nextPage = myStartPage;
	std::vector<std::string> urlListOfClassifieds;
	//readClassifiedListPage(myStartPage, urlListOfClassifieds, nextPage);
	//readSingleClassifiedPage(urlListOfClassifieds[0]);
	readSingleClassifiedPage("https://riyasewana.com/buy/bajaj-4-stroke-sale-kurunegala-7346452");
}
