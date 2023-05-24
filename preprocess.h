# pragma once
# include <string>
# include <time.h>
# include "Vector.h"
# include <map>
# include <ctime>
# include "configure.h"
# include "StockData.h"

using namespace std;


class preprocess
{
private:
	int N;
	map<string, StockData*> beatEst;
	map<string, StockData*> meetEst;
	map<string, StockData*> missEst;
	vector<string> tickers;
	vector<string> announce_day;
	vector<string> period_end;
	Vector estimate_eps;
	Vector reported_eps;
	Vector eps_surprise;
	Vector eps_surprise_percent;

public:
	// constructor
	preprocess(): N(60) {
		srand(time(NULL));
	}
	preprocess(int N_) : N(N_) {
		srand(time(NULL));
	}
	~preprocess() {
	// Delete all elements in the map
    	for (auto& pair : beatEst) {
        	delete pair.second; // pair.second is a pointer to the value of each element
        	pair.second = nullptr;
    	}
    	beatEst.clear(); 
    	
		for (auto& pair : meetEst) {
    		delete pair.second; 
    		pair.second = nullptr;
    	}
    	meetEst.clear(); 
	
	
	    for (auto& pair : missEst) {
        	delete pair.second; 
        	pair.second = nullptr;
    	}
    	missEst.clear(); 
	}
		
	// setters
	void setN(int N_);
	void importFromFile(string earningAnouncementFile);
	// void quickSortBySurprisePercent(int i, int j);
	Vector sortBystdsort();
	// int partition(int low, int high);
	string convertDateFormat(string inputDate);
	void groupBySurprisePercent();
	// void swapAll(int i, int j);
	// void swap(vector<string>* V, int i, int j);
	// void swap(Vector* V, int i, int j);
	// getters
	map<string, StockData*>& getBeatMap() {
		return beatEst;
	}
	map<string, StockData*>& getMeetMap() {
		return meetEst;
	}
	map<string, StockData*>& getMissMap() {
		return missEst;
	}
	int getN();
};
