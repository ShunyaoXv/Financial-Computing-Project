#include "Menu.h"
#include <algorithm>
#include <iostream>
//#include "gnuplot_linux.h"
#include "configure.h"
#include "multi.h"
#include <chrono>

using namespace std;




void Menu::loadPreProcess() {

	myProcessor->importFromFile(earningAnouncementFile);
	myProcessor->groupBySurprisePercent();
	beatEst = myProcessor->getBeatMap();
	meetEst = myProcessor->getMeetMap();
	missEst = myProcessor->getMissMap();
	//int a = 1;
	
	for (map<string, StockData*>::iterator iter = beatEst.begin(); iter!=beatEst.end();iter++) {
		// if (a % 7 == 0) 
		beatVector.push_back(iter->second);
		// a++;
	}
	//a = 1;
	for (map<string, StockData*>::iterator iter = meetEst.begin(); iter!=meetEst.end();iter++) {
		// if (a % 7 == 0) 
		meetVector.push_back(iter->second);
		// a++;
	}
	//a = 1;
	for (map<string, StockData*>::iterator iter = missEst.begin(); iter!=missEst.end();iter++) {
		// if (a % 7 == 0) 
		missVector.push_back(iter->second);
		// a++;
	}
	//cout<<"vector test"<<beatVector.size()<<endl;
}


void Menu::retrieveData(int (*fetch)(vector<StockData*>&)) {
	cout << "Starting retrieving data: " << endl << endl;
	
	
	// the time our retrieveData program start
	auto start = chrono::high_resolution_clock::now();
	
	myDayManager->LoadData(tradingDaysFile); //Need load data after downloading dates
    //cout<<"Load finished"<<endl;
	// retrieve data setting for 3 groups for download process
	
	// reduce ticker number to test for part 2
	for (vector<StockData*>::iterator iter = beatVector.begin(); iter!=beatVector.end();iter++) {

		(*iter)->RetrieveDataSetting(N, myDayManager);
		
		
	}

	for (vector<StockData*>::iterator iter = meetVector.begin(); iter!=meetVector.end();iter++) {

		(*iter)->RetrieveDataSetting(N, myDayManager);
		
	}

	for (vector<StockData*>::iterator iter = missVector.begin(); iter!=missVector.end();iter++) {

		(*iter)->RetrieveDataSetting(N, myDayManager);
		
	}

	
	vector<StockData*> combined;

// assume you have 3 vectors called v1, v2, and v3
	combined.reserve(beatVector.size() + meetVector.size() + missVector.size()); // optional, but can improve performance
	
	combined.insert(combined.end(), beatVector.begin(), beatVector.end());
	combined.insert(combined.end(), meetVector.begin(), meetVector.end());
	combined.insert(combined.end(), missVector.begin(), missVector.end());
		

	
	fetch(combined);


	// for (auto it : beatEst) {
 //       if (!((it->second)->getFetchSuccess())) {
 //           it = beatEst.erase(it);
 //       } 
 //   }
    
 //   for (auto it : meetEst)  {
 //   	if (!((it->second)->getFetchSuccess())) {
 //           it = meetEst.erase(it);
 //       } 
 //   }
    
 //   for (auto it : missEst)  {
 //   	if (!((it->second)->getFetchSuccess())) {
 //           it = missEst.erase(it);
 //       } 
 //   }
    
	// calculate process
	
	for (auto iter : beatVector)
	{
		//cout << iter->getTicker() << endl;
		iter->RetrieveDataSanityCheck();
		iter->CalDailyReturns();
	}
	for (auto iter : meetVector)
	{
		//cout << iter->getTicker() << endl;
		iter->RetrieveDataSanityCheck();
		iter->CalDailyReturns();
	}
	for (auto iter : missVector)
	{
		//cout << iter->getTicker() << endl;
		iter->RetrieveDataSanityCheck();
		iter->CalDailyReturns();
	}

		// vector
	beatVector.erase(remove_if(beatVector.begin(), beatVector.end(), [](StockData* sd){return !(sd->getFetchSuccess());}), beatVector.end());
	meetVector.erase(remove_if(meetVector.begin(), meetVector.end(), [](StockData* sd){return !(sd->getFetchSuccess());}), meetVector.end());
	missVector.erase(remove_if(missVector.begin(), missVector.end(), [](StockData* sd){return !(sd->getFetchSuccess());}), missVector.end());
	
	// //map
	//map<string, StockData*>
	for (auto it = beatEst.begin(); it != beatEst.end();) {
    	if (!(it->second->getFetchSuccess())) {
        	it = beatEst.erase(it);
    	} else {
        	++it;
    	}
	}
	
	for (auto it = meetEst.begin(); it != meetEst.end();) {
    	if (!(it->second->getFetchSuccess())) {
        	it = meetEst.erase(it);
    	} else {
        	++it;
    	}
	}
	
	for (auto it = missEst.begin(); it != missEst.end();) {
    	if (!(it->second->getFetchSuccess())) {
        	it = missEst.erase(it);
    	} else {
        	++it;
    	}
	}
	

	// the time our retrieveData program end
	auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    double seconds = duration.count();
    cout << "Execution time: " << seconds << " seconds" << endl;

	// calculate process
	
	// for (auto iter : beatVector)
	// {
	// 	iter->DisplayAttribute();
	// 	iter->DisplayData();
	// }
	// for (auto iter : meetVector)
	// {
	// 	iter->DisplayAttribute();
	// 	iter->DisplayData();
	// }
	// for (auto iter : missVector)
	// {
	// 	iter->DisplayAttribute();
	// 	iter->DisplayData();
	// }
	
	
}

void Menu::calculate_result() {
	//calculation->setN(60);
	result = calculation->bootstrap (beatVector, meetVector, missVector);
}

int Menu::getN() const {
	return myProcessor->getN();
}

// int main() {
// 	preprocess processor0;
// 	processor0.setN(60);
// 	tradingDayManager calender0;
// 	Calculation calculation(processor0.getN());
// 	calculation.setN(60);
// 	Menu myMenu(&processor0,&calender0,&calculation);
// 	myMenu.setN(60);

// 	myMenu.loadPreProcess();
// 	//DownloadDays(tradingDaysFile);
// 	//cout<<myMenu.getN()<<endl;
// 	myMenu.retrieveData();
// 	myMenu.calculate_result();
	
// 	// vector<vector<Vector>> resultShow = myMenu.GetResult();
// 	// gnuplot_func G_Plot(60);
//  //   G_Plot.inityValue(resultShow);
//  //   G_Plot.plotResults();
// 	return 0;
// }