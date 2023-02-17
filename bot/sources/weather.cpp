#include <iostream>
#include <string>
#include <curl/curl.h>
#include <map>
#include <cstdlib>
#include "../includes/Bot.hpp"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string parse_string(std::string readBuffer, std::string key)
{
	std::string temp = readBuffer.substr(readBuffer.find(key) + key.length() + 3, readBuffer.length());
	temp = temp.substr(0, temp.find("\""));
	return temp;
}

std::string parse_int(std::string readBuffer, std::string key)
{
	std::string temp = readBuffer.substr(readBuffer.find(key) + key.length() + 2, readBuffer.length());
	temp = temp.substr(0, temp.find(","));
	return temp;
}

std::map<std::string, std::string> Bot::weather(std::string city)
{
	std::string readBuffer;
	std::string tmp = city;
	std::string temp;
	// std::string city;
	std::string is_day;
	std::string condition;
	std::string wind;
	std::string humidity;
	std::string feelslike;

	for (long unsigned int i = 0; i < tmp.length(); i++)
	{
		if (tmp[i] == ' ')
		{
			tmp[i] = '%';
			tmp.insert(i + 1, "20");
		}
	}
	std::string q = "https://weatherapi-com.p.rapidapi.com/current.json?q=" + tmp;
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_URL, q.c_str());

	struct curl_slist *headers = NULL;
	std::string api_key = "X-RapidAPI-Key: ";
	api_key += std::getenv("RAPIDAPI_KEY");
	headers = curl_slist_append(headers, api_key.c_str());
	headers = curl_slist_append(headers, "X-RapidAPI-Host: weatherapi-com.p.rapidapi.com");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

	CURLcode res = curl_easy_perform(hnd);
	if (res != CURLE_OK)
	{
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
	}
	else
	{
		temp = parse_int(readBuffer, "temp_c");
		city = parse_string(readBuffer, "name");
		is_day = parse_string(readBuffer, "is_day");
		is_day = (is_day == "1" ? "day" : "night");
		condition = parse_string(readBuffer, "text");
		wind = parse_int(readBuffer, "wind_kph");
		humidity = parse_int(readBuffer, "humidity");
		feelslike = parse_int(readBuffer, "feelslike_c");
	}

	curl_slist_free_all(headers);
	curl_easy_cleanup(hnd);
	curl_global_cleanup();

	std::map<std::string, std::string> ret;
	ret.insert(std::pair<std::string, std::string>("temp", temp));
	ret.insert(std::pair<std::string, std::string>("city", city));
	ret.insert(std::pair<std::string, std::string>("is_day", is_day));
	ret.insert(std::pair<std::string, std::string>("condition", condition));
	ret.insert(std::pair<std::string, std::string>("wind", wind));
	ret.insert(std::pair<std::string, std::string>("humidity", humidity));
	ret.insert(std::pair<std::string, std::string>("feelslike", feelslike));
	return ret;
}
