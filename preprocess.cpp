#include <stdio.h>
#include <cstring>
#include <string> 
#include <iostream>
#include <algorithm>    // std::sort
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include <cmath>
#include "curl/curl.h"

#include "preprocess.h"

using namespace std;

void preprocess::setN(int N_) {
	N = N_;
}

int preprocess::getN() {
	return N;
}

string preprocess::convertDateFormat(string inputDate) {
    // Convert input date string to struct tm
    struct tm timeStruct = {0};
    strptime(inputDate.c_str(), "%d-%b-%Y", &timeStruct);
    
    // Convert struct tm to output date string
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeStruct);
    
    return buffer;
}

void preprocess::importFromFile(string earningAnouncementFile) {
	ifstream fin(earningAnouncementFile);
	string line, ticker, announce, periodEnd, estimate, reported, surprise, surprise_percent;
	double dEstimate, dReported, dSurprise, dSurprise_percent;
	getline(fin, line);
	if (fin.is_open()) {
	while (getline(fin,line)) {
		if (!fin.eof()) {
		stringstream sin(line);
		getline(sin, ticker, ',');
		getline(sin, announce, ',');
		getline(sin, periodEnd, ',');
		getline(sin, estimate, ',');
		getline(sin, reported, ',');
		getline(sin, surprise, ',');
		getline(sin, surprise_percent);
		dEstimate = strtod(estimate.c_str(), NULL);
		dReported = strtod(reported.c_str(), NULL);
		dSurprise = strtod(surprise.c_str(), NULL);
		dSurprise_percent = strtod(surprise_percent.c_str(), NULL);
		
		tickers.push_back(ticker);
		announce_day.push_back(convertDateFormat(announce));
		period_end.push_back(periodEnd);
		estimate_eps.push_back(dEstimate);
		reported_eps.push_back(dReported);
		eps_surprise.push_back(dSurprise);
		eps_surprise_percent.push_back(dSurprise_percent);
		}
		
		else break;
	  }
	}
	else cout<<"no file"<<endl;
	fin.close();
}



Vector preprocess::sortBystdsort() {
	Vector sortVector = eps_surprise_percent;
	std::sort (sortVector.data.begin(), sortVector.data.end());
	return sortVector;
}



// version2: 

void preprocess::groupBySurprisePercent() {
	int len = eps_surprise_percent.size();
	int subLen = len / 3;
	//cout<<subLen<<endl;
	// if (subLen * 3 < len) {
	// 	subLen++;
	// }

	Vector sortVector = sortBystdsort();
	double spot1 = sortVector.data[subLen-1];
	double spot2 = sortVector.data[subLen*2-1];
	int count1 = 0, count2 = 0, count3 = 0;
	for (int i = 0; i < len; i++) {
		if (eps_surprise_percent[i] <= spot1 && count1 <= subLen) {
			missEst[tickers[i]] = new StockData(tickers[i], "miss", announce_day[i], period_end[i],
				estimate_eps[i], reported_eps[i], eps_surprise[i], eps_surprise_percent[i], N);
			count1++;
		} else if(count2 <= subLen) {
			meetEst[tickers[i]] = new StockData(tickers[i], "meet", announce_day[i], period_end[i],
				estimate_eps[i], reported_eps[i], eps_surprise[i], eps_surprise_percent[i], N);
			count2++;
		} else {
			beatEst[tickers[i]] = new StockData(tickers[i], "beat", announce_day[i], period_end[i],
				estimate_eps[i], reported_eps[i], eps_surprise[i], eps_surprise_percent[i], N);
			count3++;
		}
	}

	
}