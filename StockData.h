#pragma once
#include <string>
#include "Vector.h"
#include "configure.h"
#include "tradingDayManager.h"

using namespace std;

class StockData
{
private:
	string ticker;
	string group;
	string announce_day;
	string period_end;
	double estimate_eps;
	double actual_eps;
	double eps_surprise;
	double eps_surprise_percent;
	
	vector<string> dates;
	Vector adjclose;
	Vector adjreturn;
	Vector cum_adjreturn;

	vector<string> dates_benchmark;
	Vector adjclose_benchmark;
	Vector adjreturn_benchmark;
	Vector cum_adjreturn_benchmark;

	string startTime;
	string endTime;
	int N;

	bool fetch_success;
	
	Vector abnormal_return;

public:
	// getters
	string getTicker() const { return ticker; }
	string getGroup() const { return group; }
	string getAnnounceDay() const { return announce_day; }
	string getPeriodEnd() const { return period_end; }
	double getEstimateEps() const { return estimate_eps; }
	double getActualEps() const { return actual_eps; }
	double getEpsSurprise() const { return eps_surprise; }
	double getEpsSurprisePercent() const { return eps_surprise_percent; }

	vector<string>& getDates() { return dates; }
	Vector& getAdjclose() { return adjclose; }
	Vector& getAdjreturn() { return adjreturn; }
	Vector& getCumAdjreturn() { return cum_adjreturn; }

	vector<string>& getDatesBenchmark() { return dates_benchmark; }
	Vector& getAdjcloseBenchmark() { return adjclose_benchmark; }
	Vector& getAdjreturnBenchmark() { return adjreturn_benchmark; }
	Vector& getCumAdjreturnBenchmark() { return cum_adjreturn_benchmark; }
	string getStartTime() const { return startTime; }
	string getEndTime() const { return endTime; }
	int getN() const { return N; }
	bool getFetchSuccess() const { return fetch_success; }
	Vector& getAbnormalReturn() { return abnormal_return; }

	// setters
	void setTicker(string ticker_){ ticker = ticker_; }
	void setGroup(string group_) { group = group_; }
	void setAnnounceDay(string announce_day_) { announce_day = announce_day_; }
	void setPeriodEnd(string period_end_) { period_end = period_end_; }
	void setEstimateEps(double estimate_eps_) { estimate_eps = estimate_eps_; }
	void setActualEps(double actual_eps_) { actual_eps = actual_eps_; }
	void setEpsSurprise(double eps_surprise_) { eps_surprise = eps_surprise_; }
	void setEpsSurprisePercent(double eps_surprise_percent_) { eps_surprise_percent = eps_surprise_percent_; }

	void setStartTime(string startTime_) { startTime = startTime_; }
	void setEndTime(string endTime_) { endTime = endTime_; }
	void setN(int N_) { N = N_; }
	void setFetchSuccess(bool fetch_success_) { fetch_success = fetch_success_; }

	// constructor
	StockData() {}
	StockData(string ticker_): ticker(ticker_) {}
	StockData(string ticker_, string group_, string announce_day_, string period_end_, double estimate_eps_,
		double actual_eps_, double eps_surprise_, double eps_surprise_percent_, int N_) {
		ticker = ticker_;
		group = group_;
		announce_day = announce_day_;
		period_end = period_end_;
		estimate_eps = estimate_eps_;
		actual_eps = actual_eps_;
		eps_surprise = eps_surprise_;
		eps_surprise_percent = eps_surprise_percent_;
		N = N_;
	}
	~StockData() {}

	// setters
	void RetrieveDataSetting(int N_, tradingDayManager* calendar);
	void RetrieveDataSanityCheck();
	void CalDailyReturns();

	// getters
	void DisplayAttribute() const;
	void DisplayData() const;


};
