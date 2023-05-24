#ifndef tradingDayManager_H
#define tradingDayManager_H

# pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "configure.h"


#include <string>
#include <vector>
#include <sstream> 
#include <fstream>
#include <iostream>
#include <algorithm>


#include <stdio.h>
#include <string> 
#include <cstring>
#include <iostream>
#include <vector>
#include <locale>
#include <iomanip>

//#include <thread>

#include "curl/curl.h" // need to adjust it when fetchData.h is completed

using namespace std;



//int DownloadDays(const char* &tradingDaysFile);


extern int DownloadDays(std::string &tradingDaysFile);
extern int write_data2(void* ptr, size_t size, size_t nmemb, void* data);
extern void* myrealloc(void* ptr, size_t size);
struct MemoryStruct {
	char* memory;
	size_t size;
};


class tradingDayManager
{
private:
	vector<string> trading_days;

public:
	tradingDayManager() {};
	~tradingDayManager() {};

	void LoadData(string CALENDAR_FILE)// load data from local csv file
	{

		ifstream inFile(CALENDAR_FILE, ios::in);
		string line;

		if (inFile.is_open()){
			while (getline(inFile, line)) {

				trading_days.push_back(line.substr(0, line.find_first_of(',')));
			}
		}
		inFile.close();
		//cout<<"finished loading"<<endl;
	}
	

	vector<string>::iterator CalendarFind(string target)
	{
		vector<string>::iterator iter = trading_days.begin();

		for (; iter != trading_days.end(); iter++)

		{	// some firms release report on weekends (eg: CE, ON), so we will need to adjust the day0 to next trading day.
			if (*iter >= target) return iter; //already in lexicographical order
		}
		return iter;
	}

	string NextNDays(string day0, int N)
	{
		auto iter = CalendarFind(day0); //day t
		int font = distance(begin(trading_days), iter); //
		int back = trading_days.size() - font;
		if (iter == trading_days.end())
		{
			cout << "can not find trading day" << endl;
			return "";
		}

		if (back > N)
		{
			return trading_days[font + N];
		}
		else
		{
			cout << "N too large for future days" << endl;
			return trading_days[trading_days.size() - 1];
		}
	}

	string PrevNDays(string day0, int N)
	{
		auto iter = CalendarFind(day0);
		int font = distance(begin(trading_days), iter);
		//int back = trading_days.size() - font;
		if (iter == trading_days.end())
		{
			cout << "can not find trading day" << endl;
			return "";
		}

		if (font > N)
		{
			return trading_days[font - N];
		}
		else
		{
			cout << "N too large for previous days" << endl;
			return trading_days[0];
		}
	}

	void DisplayDates()
	{
		for (auto iter : trading_days) cout << iter << endl;
	}
};


#endif

