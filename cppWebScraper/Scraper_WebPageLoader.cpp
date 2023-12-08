#include "Scraper_WebPageLoader.h"

#include <spdlog/spdlog.h>


Scraper_WebPageLoader::Scraper_WebPageLoader(const std::string& aUrl)
	: myPageHeader({ {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36"} }),
	myUrl(aUrl), myLoadStatus(true), myHtmlDoc(NULL), myContext(NULL)
{
	// make the HTTP request to retrieve the target page
	cpr::Response response = cpr::Get(cpr::Url{ aUrl }, myPageHeader);

	myHtmlDoc = htmlReadMemory(response.text.c_str(), (int)response.text.length(), nullptr, "UNICODE", HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
	if (myHtmlDoc == NULL) {
		spdlog::error("Error parsing the HTML content\n");
		myLoadStatus = false;
		return;
	}
	// set the libxml2 context to the current document
	myContext = xmlXPathNewContext(myHtmlDoc);
	if (myContext == NULL) {
		spdlog::error("Error creating XPath context\n");
		myLoadStatus = false;
		return;
	}
}

Scraper_WebPageLoader::~Scraper_WebPageLoader()
{
	if (myContext != NULL)
		xmlXPathFreeContext(myContext);
	if (myHtmlDoc != NULL)
		xmlFreeDoc(myHtmlDoc);
}

bool Scraper_WebPageLoader::GetLoadStatus()
{
	return myLoadStatus;
}

htmlDocPtr Scraper_WebPageLoader::GetDoc()
{
	return myHtmlDoc;
}

xmlXPathContextPtr Scraper_WebPageLoader::GetContext()
{
	return myContext;
}
