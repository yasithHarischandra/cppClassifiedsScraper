#include "Scraper_Riyasewana.h"
#include <spdlog/spdlog.h>
#include "cpr/cpr.h"
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"




void Scraper_Riyasewana::readClassifiedListPage(const std::string& aUrl, std::vector<std::string>& aListOfUrl, std::string& nextPage)
{
	spdlog::info("Starting - " + aUrl);

	// define the user agent for the GET request
	cpr::Header headers = { {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36"} };
	// make the HTTP request to retrieve the target page
	cpr::Response response = cpr::Get(cpr::Url{ aUrl }, headers);

	htmlDocPtr doc = htmlReadMemory(response.text.c_str(), (int)response.text.length(), nullptr, nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
	if (doc == NULL) {
		spdlog::error("Error parsing the HTML content\n");
		return;
	}
	// set the libxml2 context to the current document
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	if (context == NULL) {
		spdlog::error("Error creating XPath context\n");
		return;
	}

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
		
		xmlXPathFreeContext(context);
		xmlFreeDoc(doc);
		return;
	}

	xmlNodePtr nextPageUrlElement = nextPageElement->nodesetval->nodeTab[0];
	nextPage = "https:" + std::string(reinterpret_cast<char*>(xmlGetProp(nextPageUrlElement, (xmlChar*)"href")));
	
	
	
	xmlXPathFreeObject(nextPageElement);
	
	xmlXPathFreeContext(context);
	xmlFreeDoc(doc);
	return;
}

Scraper_Riyasewana::Scraper_Riyasewana() : Scraper_Base(std::string{ "https://riyasewana.com/search" })
{
	
}

void Scraper_Riyasewana::ReadSiteFrontToBack()
{
	std::string nextPage = myStartPage;
	std::vector<std::string> urlListOfClassifieds;
	readClassifiedListPage(myStartPage, urlListOfClassifieds, nextPage);
}
