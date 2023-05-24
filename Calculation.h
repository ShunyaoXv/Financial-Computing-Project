#include <iostream>
#include <vector>
#include <map>
#include "StockData.h"
#include "Vector.h"
using namespace std;


    class Calculation
    {
    private:
        //const map<string, StockData *> group;
        int N;
    public:
        Calculation(int N_) : N(N_) {}
        void setN(int N_);
        int GetN();
        //vector<string> get_Ticker();
        vector<Vector> calculate_AR (vector<StockData*> &estVector);
        Vector calculate_AAR (vector<Vector> &stockAR);
        Vector calculate_CAAR (Vector &stockAAR);
        vector<vector<Vector>> bootstrap (vector<StockData*> &beat, vector<StockData*> &meet, vector<StockData*> &miss);
    
        ~Calculation() {}
    };
