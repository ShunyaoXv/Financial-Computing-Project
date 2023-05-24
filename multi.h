#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <map>
#include <sstream> 
#include <fstream>
#include <iostream>
#include <random>  
#include <time.h>  
#include <mutex> 
#include <deque>
#include <condition_variable>
#include "StockData.h"
#include "configure.h"
#include "curl/curl.h"
#include "tradingDayManager.h"

using namespace std;


typedef struct
{
	StockData* sd;
	CURL* handle;
	int size;
	string sCrumb;
	string sCookies;

}MYDATA;

void thread_producer(MYDATA* md);

int thread_consumer();