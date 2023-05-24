# pragma once
# include <algorithm>
# include "preprocess.h"
# include "StockData.h"
# include "tradingDayManager.h"
# include "fetchData.h"
# include "Vector.h"

# include "configure.h"
# include <time.h>
# include "Calculation.h"
# include "multi.h"

using namespace std;

class Menu
{
public:
	preprocess* myProcessor;
	//Calculation* calculation;
	tradingDayManager* myDayManager;
	map<string, StockData*> beatEst;
	map<string, StockData*> meetEst;
	map<string, StockData*> missEst;
	vector<StockData*> beatVector;
	vector<StockData*> meetVector;
	vector<StockData*> missVector;
	vector<vector<Vector>> result;

	int N;
public:
	Calculation* calculation;
	// constructor
	//Menu(): {}
	Menu(preprocess* pre,tradingDayManager* calender, Calculation* calculation_): myProcessor(pre),
		myDayManager(calender),N(pre->getN()),calculation(calculation_) {
		calculation->setN(pre->getN());
	}
	~Menu() {}
	
	void setN(int N_){N = N_;}
	void loadPreProcess();
	void retrieveData(int (*fetch)(vector<StockData*>&));
	void calculate_result();
	const std::map<std::string, StockData*>& getBeatEst() const { return beatEst; }
    const std::map<std::string, StockData*>& getMeetEst() const { return meetEst; }
    const std::map<std::string, StockData*>& getMissEst() const { return missEst; }
	int getN() const;
	vector<vector<Vector>> GetResult(){return result;}
};