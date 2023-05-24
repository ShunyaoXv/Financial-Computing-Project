#include "StockData.h"
#include <iostream>
#include <iomanip>

using namespace std;

// setters
void StockData::RetrieveDataSetting(int N_, tradingDayManager* calendar) {
	N = N_;

	// clear dates and adjclose vectors
	
	vector <string>().swap(dates);
	adjclose.clear();
	vector <string>().swap(dates_benchmark);
	adjclose_benchmark.clear();
	startTime = (*calendar).PrevNDays(announce_day, N);
	endTime = (*calendar).NextNDays(announce_day, N);

}

void StockData::RetrieveDataSanityCheck() {
	fetch_success = false;

	//cout << "Fetch sanity check: " << endl;
	//cout << "fetch: " << ticker << " expected length: " << 2 * N + 1 << ", received length: " << adjclose.size() << endl;
	//cout << "fetch: " << ticker << " benchmark expected length: " << 2 * N + 1 << ", received length: " << adjclose_benchmark.size() << endl;

	if ((2 * N + 1 == adjclose.size()) && (adjclose.size() == adjclose_benchmark.size()))
	{
		// sanity check for fetch data
		//cout << "fetch: sanity check succeed" << endl << endl;
		fetch_success = true;
	}
	else
	{
		cout << ticker<<"fetch: sanity check fail" << endl << endl;
	}
}

void StockData::CalDailyReturns() {
	if (fetch_success) {
		adjreturn = adjclose.pct_change();
		cum_adjreturn = adjreturn.cumsum();
		adjreturn_benchmark = adjclose_benchmark.pct_change();
		cum_adjreturn_benchmark = adjreturn_benchmark.cumsum();
		abnormal_return = adjreturn - adjreturn_benchmark;
		
	}
	
	//else {cout<<"fetch failed: "<<ticker<<endl;}
}

// getters
void StockData::DisplayAttribute() const {
	cout << "Ticker: " << ticker << endl
		<< "Group: " << group << endl
		<< "Announce Date: " << announce_day << endl
		<< "Period End: " << period_end << endl
		<< "EPS Esitmate: " << estimate_eps << endl
		<< "EPS Actual: " << actual_eps << endl
		<< "EPS Surprise: " << eps_surprise << endl
		<< "EPS Surprise Percent: " << eps_surprise_percent << endl
		<< endl;
}

void StockData::DisplayData() const
{

	cout << std::setw(10);
	cout << setw(6) << "Ticker"
		<< setw(15) << "Dates"
		<< setw(15) << "Adjclose"
		<< setw(15) << "Adjreturn"
		<< setw(15) << "Cumadjreturn"
		<< setw(15) << "Benchmarkreturn"
		<< setw(15) << "Abnormalreturn"
		<< endl;

	if ((adjclose.size() > 4) && fetch_success)
	{
		for (int i = 0; i <= 50; i++)
		{
			cout << setw(6) << ticker
				<< setw(15) << dates[i]
				<< setw(15) << adjclose[i]
				<< setw(15) << adjreturn[i]
				<< setw(15) << cum_adjreturn[i]
				<< setw(15) << adjreturn_benchmark[i]
				<< setw(15) << abnormal_return[i]
				<< endl;
		}
	}
	cout << endl;

}