// #ifndef fetchData_H
// #define fetchData_H

# pragma once
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream> 
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include <thread>
#include <map>

#include "StockData.h"
#include "tradingDayManager.h"
#include "configure.h"
#include "curl/curl.h"
#include "multi.h"
#include <map>

using namespace std;





int fetch_allData_single(vector<StockData*> &stockList);


void match_benchmark(Vector& benchmark_adjclose, StockData* sd);



int fetch_allData_multi(vector<StockData*> &stockList);