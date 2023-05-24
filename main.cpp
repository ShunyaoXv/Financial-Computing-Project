#include "Engine.h"

int main() {
    Engine engine;
    engine.run();
    return 0;
}


/*
int main() {
	preprocess processor0;
	processor0.setN(60);
	tradingDayManager calender0;
	Calculation calculation(processor0.getN());
	calculation.setN(60);
	Menu myMenu(&processor0,&calender0,&calculation);
	myMenu.setN(60);

	myMenu.loadPreProcess();
	//DownloadDays(tradingDaysFile);
	//cout<<myMenu.getN()<<endl;
	myMenu.retrieveDataSingleThread();
	myMenu.calculate_result();
	
	vector<vector<Vector>> resultShow = myMenu.GetResult();
	
	
	gnuplot_func G_Plot(60);
    G_Plot.inityValue(resultShow);
    G_Plot.plotResults();
	return 0;
}
*/