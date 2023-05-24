#include "fetchData.h"
#define _CRT_SECURE_NO_WARNINGS

#include "curl/curl.h"

using namespace std;



extern void thread_producer(MYDATA* md);

extern int thread_consumer();

void match_benchmark( map<string,double>& benchmark_data, StockData* sd)
{
    //match benchmark data to each stock
    int n = sd->getN();
    
    string start_date = sd->getStartTime();
    
    map<string,double>::iterator iter = benchmark_data.begin();
    for (; iter != benchmark_data.end(); iter++)
    {	
    	if (iter->first == start_date) break; //already in lexicographical order
    }
    int count = 0;
    while (count <= 2*n ){
        (sd->getAdjcloseBenchmark()).push_back(iter->second);
        iter++;
        count++;
    }
    
}


int fetch_allData_single(vector<StockData*>& stockList) {
	// declaration of an object CURL 
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
		string api_token = "644340fc70c400.57231198";  
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


		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
		
		result = curl_easy_perform(handle);
		if (result != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return -1;
		}

    	// fetch for benchmark
		

		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		
		map<string,double> benchmark_data;

		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				sDate = line.substr(0, line.find_first_of(','));
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',') + 1);
				dValue = strtod(sValue.c_str(), NULL);
                
                benchmark_data.insert(make_pair(sDate,dValue));
                
			}
		}
		
		free(data.memory);
        data.size = 0;
        
        // fetch for stocks
		vector<StockData*>::iterator itr = stockList.begin();
		for (; itr != stockList.end(); itr++)
		{
			struct MemoryStruct data;
			data.memory = NULL;
			data.size = 0;

	
			start_date = (*itr)->getStartTime();
			end_date = (*itr)->getEndTime();
		

			symbol = (*itr)->getTicker();
			

			string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
			curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

			//adding a user agent
			curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			// perform, then store the expected code in result
			result = curl_easy_perform(handle);

			// check for errors 
			if (result != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}
			
            stringstream sData;
			sData.str(data.memory);

            dValue = 0;
			//string line;

			while (getline(sData, line)) {
				size_t found = line.find('-');
				if (found != std::string::npos)
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sValue = line.substr(line.find_last_of(',') + 1);
					dValue = strtod(sValue.c_str(), NULL);
					//cout<<"sDate: "<<sDate<<endl;
					//cout<<"dValue: "<<dValue<<endl;

					((*itr)->getDates()).push_back(sDate);
					((*itr)->getAdjclose()).push_back(dValue);
				}
				
			}
			match_benchmark(benchmark_data,*itr);

			free(data.memory);
			data.size = 0;
		}
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

int fetch_allData_multi(vector<StockData*>& stockList)
{
	CURL* handle;

	CURLcode result;

	// set up the program environment that libcurl needs 
	curl_global_init(CURL_GLOBAL_ALL);

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();
	//FILE* fp;
	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	string sCookies;
	string sCrumb;
	
	// if everything's all right with the easy handle... 
	if (handle)
	{   
	    
	    string url_common = "https://eodhistoricaldata.com/api/eod/";
	    string start_date = "2018-11-01";
		string end_date = "2023-05-03";
		string api_token = "644340fc70c400.57231198";  


		string symbol = BENCHMARK_TICKER;
		string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
		curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

		//adding a user agent
		curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
		//const char* cURL = url_request.c_str();
		const char* cookies = sCookies.c_str();
		curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
		
		result = curl_easy_perform(handle);
		if (result != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return -1;
		}

    	// fetch for benchmark
		

		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		
		map<string,double> benchmark_data;

		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				sDate = line.substr(0, line.find_first_of(','));
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',') + 1);
				dValue = strtod(sValue.c_str(), NULL);
                
                benchmark_data.insert(make_pair(sDate,dValue));
                
			}
		}
		
		free(data.memory);
        data.size = 0;
		curl_easy_cleanup(handle);

		MYDATA* mydt = new MYDATA[stockList.size()];
		for (int i = 0; i < stockList.size(); i++) {

			mydt[i].sd = stockList[i];
			mydt[i].size = stockList.size();
			mydt[i].sCrumb = sCrumb;
			mydt[i].sCookies = sCookies;
		}

		thread t1(thread_producer, mydt);

		thread* consumer_threads = new thread[THREAD_NUM];
		for (int i = 0; i < THREAD_NUM; i++)
		{
			consumer_threads[i] = thread(thread_consumer);
		}

		t1.join();
		for (int i = 0; i < THREAD_NUM; i++)
		{
			//consumer_threads[i].detach();
			consumer_threads[i].join();
		}
		
		// match benchmark data for each stock
		for (vector<StockData*>::iterator itr = stockList.begin(); itr != stockList.end(); itr++)
		{
			match_benchmark(benchmark_data,*itr);
		}
		
		//delete[] mydt;
		//mydt = nullptr;
		//delete consumer_threads;
		//consumer_threads = nullptr;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	// cleanup since you've used curl_easy_init
	//curl_easy_cleanup(handle);
	// release resources acquired by curl_global_init()
	curl_global_cleanup();
	
	
	return 0;
}