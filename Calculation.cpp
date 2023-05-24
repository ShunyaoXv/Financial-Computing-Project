#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include "StockData.h"
#include "Calculation.h"
#include "Vector.h"
using namespace std;

    void Calculation::setN(int N_){N = N_;}
    
    int Calculation::GetN(){return N;}

    vector<Vector> Calculation::calculate_AR(vector<StockData*> &estVector){
        std::random_device rd;
        std::mt19937 gen(rd());
        shuffle(estVector.begin(), estVector.end(), gen);

        vector<Vector> ARt;
        
        for (int i = 0; i < 80; i++){
            auto it = estVector.begin();
            if (it != estVector.end() && (*it)->getAdjreturn().size() == (*it)->getAdjreturnBenchmark().size()){
                ARt.push_back(((*it)->getAbnormalReturn()).data);
            }
            else {
                std::cerr << "Abnormal situation at index " << i << ": either the key was not found or the sizes do not match." << std::endl;
                // Continue with the next iteration
                continue;
            }
        }
        
        return ARt;
    }

    Vector Calculation::calculate_AAR (vector<Vector> &stockAR){
        Vector AARt;
        for (int n = 0; n < 2*N ; n++){
            double res = 0.0;
            for (int i = 0; i < 80; i++){
                res = res + stockAR[i][n];
            }
            AARt.push_back(res/80);
        }
        
        return AARt;
    }

    Vector Calculation::calculate_CAAR (Vector &stockAAR){
        Vector CAAR;
        double res = 0.0;
        for (int n = 0; n < 2*N; n++){
            res = res + stockAAR[n];
            CAAR.push_back(res);
        }
        
        return CAAR;
    }
    
    vector<vector<Vector>> Calculation::bootstrap (vector<StockData*> &beat, vector<StockData*> &meet, vector<StockData*> &miss){
        // Initialize DisplayFinal
        vector<vector<Vector>> DisplayFinal;
        DisplayFinal.resize(3);
        for (int i = 0; i < 3; i++) {
            DisplayFinal[i].resize(4);
        }

        Vector beat_Avg_AAR(2 * N);
        Vector meet_Avg_AAR(2 * N);
        Vector miss_Avg_AAR(2 * N);
        
        Vector beat_AAR_square(2 * N);
        Vector meet_AAR_square(2 * N);
        Vector miss_AAR_square(2 * N);
    
        Vector beat_Avg_CAAR(2 * N);
        Vector meet_Avg_CAAR(2 * N);
        Vector miss_Avg_CAAR(2 * N);
        
        Vector beat_CAAR_square(2 * N);
        Vector meet_CAAR_square(2 * N);
        Vector miss_CAAR_square(2 * N);
        
        // Repeat 400 times
        for (int i = 0; i < 400; i++) {
            
            vector<Vector> beat_AR = calculate_AR(beat);
            vector<Vector> meet_AR = calculate_AR(meet);
            vector<Vector> miss_AR = calculate_AR(miss);
            
            Vector beat_AAR = calculate_AAR(beat_AR);
            Vector meet_AAR = calculate_AAR(meet_AR);
            Vector miss_AAR = calculate_AAR(miss_AR);
            
            Vector beat_CAAR = calculate_CAAR(beat_AAR);
            Vector meet_CAAR = calculate_CAAR(meet_AAR);
            Vector miss_CAAR = calculate_CAAR(miss_AAR);
            
            beat_Avg_AAR += beat_AAR; //overloading
            meet_Avg_AAR += meet_AAR;
            miss_Avg_AAR += miss_AAR;
            
            beat_Avg_CAAR += beat_CAAR;
            meet_Avg_CAAR += meet_CAAR;
            miss_Avg_CAAR += miss_CAAR;
            
            // Using Variance[x] = E[x^2] - E[x]^2 to calculate standard deviation
            beat_AAR_square += beat_AAR * beat_AAR; //overloading
            meet_AAR_square += meet_AAR * meet_AAR;
            miss_AAR_square += miss_AAR * miss_AAR;
            
            beat_CAAR_square += beat_CAAR * beat_CAAR;
            meet_CAAR_square += meet_CAAR * meet_CAAR;
            miss_CAAR_square += miss_CAAR * miss_CAAR;
    
        }
        
        /*
         fill in the matrix
         DisplayFinal[0][0] = beat Avg AAR; DisplayFinal[0][1] = beat AAR STD; DisplayFinal[0][2] = beat Avg CAAR; DisplayFinal[0][3] = beat CAAR STD;
         DisplayFinal[1][0] = meet Avg AAR; DisplayFinal[1][1] = meet AAR STD; DisplayFinal[1][2] = meet Avg CAAR; DisplayFinal[1][3] = meet CAAR STD;
         DisplayFinal[2][0] = miss Avg AAR; DisplayFinal[2][1] = miss AAR STD; DisplayFinal[2][2] = miss Avg CAAR; DisplayFinal[1][3] = miss CAAR STD;
         */
        DisplayFinal[0][0] = beat_Avg_AAR / 400.0;
        DisplayFinal[1][0] = meet_Avg_AAR / 400.0;
        DisplayFinal[2][0] = miss_Avg_AAR / 400.0;
        
        // Using Variance[x] = E[x^2] - E^2[x] to calculate standard deviation
        DisplayFinal[0][1] = DisplayFinal[0][1].mysqrt(beat_AAR_square / 400.0 - DisplayFinal[0][0] * DisplayFinal[0][0]); 
        DisplayFinal[1][1] = DisplayFinal[1][1].mysqrt(meet_AAR_square / 400.0 - DisplayFinal[1][0] * DisplayFinal[1][0]);
        DisplayFinal[2][1] = DisplayFinal[2][1].mysqrt(miss_AAR_square / 400.0 - DisplayFinal[2][0] * DisplayFinal[2][0]);
    
        DisplayFinal[0][2] = beat_Avg_CAAR / 400.0;
        DisplayFinal[1][2] = meet_Avg_CAAR / 400.0;
        DisplayFinal[2][2] = miss_Avg_CAAR / 400.0;
    
        DisplayFinal[0][3] = DisplayFinal[0][3].mysqrt(beat_CAAR_square / 400.0 - DisplayFinal[0][2] * DisplayFinal[0][2]);
        DisplayFinal[1][3] = DisplayFinal[1][3].mysqrt(meet_CAAR_square / 400.0 - DisplayFinal[1][2] * DisplayFinal[1][2]);
        DisplayFinal[2][3] = DisplayFinal[2][3].mysqrt(miss_CAAR_square / 400.0 - DisplayFinal[2][2] * DisplayFinal[2][2]);
    
        return DisplayFinal;
    }
