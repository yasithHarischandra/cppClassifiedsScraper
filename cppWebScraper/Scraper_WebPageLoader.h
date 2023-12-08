#pragma once

#include <string>
#include "cpr/cpr.h"
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

class Scraper_WebPageLoader
{
protected:
	//setting user agent for browser
	const cpr::Header myPageHeader;

	const std::string myUrl;

	bool myLoadStatus;
	htmlDocPtr myHtmlDoc;
	xmlXPathContextPtr myContext;

public:
	Scraper_WebPageLoader(const std::string& aUrl);
	~Scraper_WebPageLoader();

	bool GetLoadStatus();
	htmlDocPtr GetDoc();
	xmlXPathContextPtr GetContext();
};

