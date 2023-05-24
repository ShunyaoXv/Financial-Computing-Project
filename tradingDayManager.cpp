#include "tradingDayManager.h"
#include "curl/curl.h" 
#include "configure.h"
//#include "fetchData.h"


int write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    }
    return realsize;
}


//#include "fetchData.h"

void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

int DownloadDays(std::string &tradingDaysFile) //download trading days from API
{
    CURL* handle;

	CURLcode result;

	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();

	// if everything's all right with the easy handle... 
	if (handle)
	{
		string url_common = "https://eodhistoricaldata.com/api/eod/";
		string start_date = "2018-11-01";
		string end_date = "2023-05-03";
		string api_token = "644340fc70c400.57231198";  // You must replace this API token with yours

		struct MemoryStruct data;
		data.memory = NULL;
		data.size = 0;

		string symbol = BENCHMARK_TICKER;
		string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
		curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

		//adding a user agent
		curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

		// check for errors 


		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
		
		result = curl_easy_perform(handle);
		if (result != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return -1;
		}
		stringstream sData;
		ofstream datefile(tradingDaysFile);
		sData.str(data.memory);
		string sDate;
		string line;
		//cout << symbol << endl;
		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				cout << line << endl;
				sDate = line.substr(0, line.find_first_of(','));
                if (datefile.is_open()) {

                datefile << sDate << "," << "\n";
                cout << sDate << " " << endl;
                } 
				
			}
		}
		datefile.close();

		free(data.memory);
		data.size = 0;
	
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return -1;
	}

	// cleanup since you've used curl_easy_init  
	curl_easy_cleanup(handle);

	// release resources acquired by curl_global_init() 
	curl_global_cleanup();

	return 0;
}