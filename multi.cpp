#define _CRT_SECURE_NO_WARNINGS

#include "multi.h"

using namespace std;
mutex display_data_mutex;
mutex queue_not_empty_mutex;
mutex curl_perform;
deque<MYDATA> q;
condition_variable cond;

int count_now = 0;
static int all_num;

extern int write_data2(void* ptr, size_t size, size_t nmemb, void* data);

void thread_producer(MYDATA* md)
{
	int count = md->size;
	all_num = count;
	for (int i = 0; i < count; i++)
	{
		unique_lock<mutex> unique(queue_not_empty_mutex);
		q.push_front(md[i]);
		//this_thread::sleep_for(100ms);

		unique.unlock();
		// cout << "producer a value: " << md[i].sd->getTicker()<< endl;
		cond.notify_all();
	}

	// poison pill to terminate consumers
	MYDATA pill = md[0];
	pill.size = 0;

	unique_lock<mutex> unique(queue_not_empty_mutex);
	q.push_front(pill);
	unique.unlock();
	//cout << "producer a pill" << endl;
	cond.notify_all();
	//this_thread::sleep_for(chrono::seconds(10));
}


int thread_consumer()
{
	MYDATA mydt;

	CURL* handle;

	CURLcode result;

	// curl_easy_init() returns a CURL easy handle 
	handle = curl_easy_init();
	
	string url_common = "https://eodhistoricaldata.com/api/eod/";
	string api_token = "644340fc70c400.57231198"; 

	if (handle)
	{
		while (1)
		{
			unique_lock<mutex> unique(queue_not_empty_mutex);
			while (q.empty())
				cond.wait(unique);
			mydt = q.back(); //mydt is one stock
			if (mydt.size == 0)
			{
				// receive poison pill: finish all tasks, terminate current thread
				// cout << "thread receive poison pill" << endl;
				unique.unlock();
				curl_easy_cleanup(handle);
				return 0;
			}
			q.pop_back();
			unique.unlock();

			StockData* sd = mydt.sd;
			string sCrumb = mydt.sCrumb;
			string sCookies = mydt.sCookies;


			struct MemoryStruct data;
			data.memory = NULL;
			data.size = 0;


			
			string startTime = sd->getStartTime();
			string endTime = sd->getEndTime();
			string symbol = sd->getTicker();

			//cout << "fetch: fetching " << symbol << " from " << sd->getStartTime() << " to " << sd->getEndTime() << endl;


            string url_request = url_common + symbol + ".US?" + "from=" 
                + startTime + "&to=" + endTime + "&api_token=" 
                + api_token + "&period=d";
            
            curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
            
            const char* cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);
        	curl_easy_setopt(handle, CURLOPT_ENCODING, "");
            
            //adding a user agent
            curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			
			
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
            result = curl_easy_perform(handle);

			if (result != CURLE_OK)
			{
				// if errors have occurred, tell us what is wrong with 'result'
				fprintf(stderr, "curl_easy_perform() failed: % s\n", curl_easy_strerror(result));
				return 1;
			}

			//cout << "fetch: storing " << symbol << " from " << sd->startTime << " to " << sd->endTime << endl;

			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;
			string line;
			getline(sData, line);

			//cout << "fetch: finish storing " << symbol << " from " << sd->startTime << " to " << sd->endTime << endl;

			try
			{
				while (getline(sData, line))
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sValue = line.substr(line.find_last_of(',') + 1);
					dValue = strtod(sValue.c_str(), NULL);

                    ////use our version
					sd->getDates().push_back(sDate);
					sd->getAdjclose().push_back(dValue);

				}
				
			count_now += 1;
			if (count_now == (int) all_num/3) cout<<"Retrieved 1/3 tickers."<<endl;
			
			else if ( count_now == (int) all_num/3*2) cout<<"Retrieved 2/3 tickers."<<endl;
			
			//else if ( count_now == all_num) cout<<"Finished retrieving all tickers."<<endl;
				
				
			}
			catch (out_of_range& exc)
			{
				cout << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
			}
			catch (...)
			{
				cout << "other error." << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
			}
			free(data.memory);
			data.size = 0;
		}
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
	//return 0;

}