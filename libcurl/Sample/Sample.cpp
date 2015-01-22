#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <curl/curl.h>


//
//  Case-insensitive string comparison
//

#ifdef _MSC_VER
#define COMPARE(a, b) (!stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

//
//  libxml callback context structure
//

struct Context
{
	Context(): addTitle(false) { }

	bool addTitle;
	std::string title;
};

//
//  libcurl variables for error strings and returned data

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

//
//  libcurl write callback function
//

static int writer(char *data, size_t size, size_t nmemb,
				  std::string *writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

//
//  libcurl connection initialization
//

static bool init(CURL *&conn, char *url)
{
	CURLcode code;

	conn = curl_easy_init();

	if (conn == NULL)
	{
		fprintf(stderr, "Failed to create CURL connection\n");

		exit(EXIT_FAILURE);
	}

	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);

		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);

		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);

		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);

		return false;
	}

	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);

		return false;
	}

	return true;
}

//
//  libxml start element callback function
//


//
//  libxml end element callback function
//

//  Text handling helper function
//

//
//  libxml PCDATA callback function
//


//
//  libxml CDATA callback function
//

static void parseHtml(const std::string &html,
					  std::string &title)
{


}

int main(int argc, char *argv[])
{
	CURL *conn = NULL;
	CURLcode code;
	std::string title;

	// Ensure one argument is given



	curl_global_init(CURL_GLOBAL_DEFAULT);

	// Initialize CURL connection

	if (!init(conn, "http://www.google.com/#q=test"))
	{
		fprintf(stderr, "Connection initializion failed\n");

		exit(EXIT_FAILURE);
	}

	// Retrieve content for the URL

	code = curl_easy_perform(conn);
	curl_easy_cleanup(conn);

	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", argv[1], errorBuffer);

		exit(EXIT_FAILURE);
	}

	// Parse the (assumed) HTML code

	parseHtml(buffer, title);

	// Display the extracted title

	printf("Title: %s\n", title.c_str());

	return EXIT_SUCCESS;
}