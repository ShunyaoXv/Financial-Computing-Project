#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "Vector.h"
#include "configure.h"

using namespace std;

    typedef vector<double> Vec;
    // typedef vector<Vector> Matrix;

    class gnuplot_func{
        private:
            int N; // the range of the plot is from day -(n-1) to day n, with announcement day as day 0
            double* xData; // x axis value for the plot
            double* yData; // CAAR mean for Beat
            double *yData2; // CAAR mean for Miss
            double* yData3; // CAAR mean for Meet
            int nIntervals;
        
        public:
            // constructor with parameter
            gnuplot_func(int N_):N(N_){
                cout<<"in gnuplot N = "<<N<<endl;
                nIntervals = 2*N;
                int startDate = -(N-1);
                int endDate = N;
                xData = (double*) malloc((nIntervals+1)*sizeof(double));
                
                int count = 0;
                for (int i = startDate; i <= endDate; i++) {
                    xData[count] = i;
                    count++;
                }
            
            }
            void inityValue_CAAR(vector<vector<Vector>> V);
            void inityValue_CAARstd(vector<vector<Vector>> V);
            void plotResults_CAAR();
            void plotResults_CAARstd();
            
            ~gnuplot_func() {
                delete[] xData;
                delete[] yData;
                delete[] yData2;
                delete[] yData3;
                
                xData = nullptr;
                yData = nullptr;
                yData2 = nullptr;
                yData3 = nullptr;
            }
            
    };



