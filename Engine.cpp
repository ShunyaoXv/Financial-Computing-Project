#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "Engine.h"
#include "Menu.h"
#include"multi.h"
#include <chrono>

using namespace std;

void Engine::run()
{
    int option;
    int if_case1 = 0;
    int if_case3 = 0;
    float tdiff = 0;
    
    N = 60; //default
    vector<vector<Vector>> resultShow;
    
    preprocess processor0;
    tradingDayManager calender0;
    Calculation calculation(processor0.getN());
    Menu myMenu(&processor0,&calender0,&calculation);
    
   
    while (true) {
        cout << "--------Primary Menu----------" << endl;
        cout << "Choose from the following options: " << endl;
        cout << "1: Input N to fetch historical price data for stocks " << endl;
        cout << "2: View a specific stock's information " << endl;
        cout << "3: Bootstrap and display AAR, AAR-STD, CAAR, CAAR-STD for a group " << endl;
        cout << "4: Display the gnuplot graph with CAAR for all 3 groups " << endl;
        cout << "5: Display the gnuplot graph with CAAR std for all 3 groups " << endl;
        cout << "6: Terminate the program " << endl;
        cout << "---------Make your selection:--------" << endl;
        cin >> option;
        switch (option)
        {
            // Retrieve historical price data for all stocks
            case 1:
            {
                cout << "--------Please enter N: ---------" << endl;
                cin >> N;
                if(N < 60)
                {
                    cout << "N should be greater or equal to 60. Enter again: " << endl;
                    cin >> N;
                }
                
                auto start = chrono::high_resolution_clock::now();
                
                processor0.setN(N);
                calculation.setN(N);
                myMenu.setN(N);
            
            	myMenu.loadPreProcess();
            	//DownloadDays(tradingDaysFile);
            	//cout<<myMenu.getN()<<endl;
            	myMenu.retrieveData(fetch_allData_multi);
                
                if_case1 = 1;
                if_case3 = 0;
                
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double> duration = end - start;
                double seconds = duration.count();
                cout << "Data Retriever Done. " <<"Time taken: "<<seconds<<" seconds"<< endl<<endl;
                
                break;
            }


            
            case 2:
            {
                if (if_case1 == 0) {
                    cout << "Please retrieve the historical stock data first! Redirecting to Menu..." << endl;
                    //std::this_thread::sleep_for(std::chrono::seconds(5));
                    continue;
                }
                
                cout << "---------Enter and view the stock ticker---------" << endl;
                cin >> ticker;
                
                StockData* found_stock = nullptr;
            	
                // Searching in beatEst map
                auto beat_it = myMenu.getBeatEst().find(ticker);
                if (beat_it != myMenu.getBeatEst().end())
                {
                    found_stock = beat_it->second;
                }
                
                // Searching in meetEst map if not found in beatEst
                if (!found_stock)
                {
                    auto meet_it = myMenu.getMeetEst().find(ticker);
                    if (meet_it != myMenu.getMeetEst().end())
                    {
                        found_stock = meet_it->second;
                    }
                }
                
                // Searching in missEst map if not found in beatEst and meetEst
                if (!found_stock)
                {
                    auto miss_it = myMenu.getMissEst().find(ticker);
                    if (miss_it != myMenu.getMissEst().end())
                    {
                        found_stock = miss_it->second;
                    }
                }
                
                // If a match is found, display attributes and data
                if (found_stock)
                {
                    found_stock->DisplayAttribute();
                    found_stock->DisplayData();
                }
                else
                {
                    cout << "Ticker not found." << endl;
                }
                break;
            }

        
            case 3:
            {
                if (if_case1 == 0) {
                    cout << "Please retrieve the historical stock data first! Redirecting to Menu..." << endl;
                    continue;
                }
                
                auto start = chrono::high_resolution_clock::now();
                
                myMenu.calculate_result();
                resultShow = myMenu.GetResult();
                
                if_case3 = 1; // Switch the bootstrap status to finished;
                
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double> duration = end - start;
                double seconds = duration.count();
                cout << "Bootstrap Finished. Time taken: " << seconds << " seconds" << endl;
                
                string groupNames[] = { "Beat Group", "Meet Group", "Miss Group", "Omit" };
                
                cout << "Please choose a group you would like to view from the following: " << endl;
                for (int i = 0; i < 4; i++)
                    cout << i+1 << ". " << groupNames[i] << endl;
                
                int choice;
                cin >> choice;
                if (choice < 1 || choice > 4) {
                    cout << "N/A. Enter again" << endl;
                    continue;
                }
                if (choice == 4)
                    break;
                
                int i = choice - 1;
                cout << "--------------Result for " << groupNames[i] << "--------------" << endl;
                
                string metricNames[] = { "Avg_AAR", "AAR_STD", "Avg_CAAR", "CAAR_STD" };
                for (int j = 0; j < 4; j++) {
                    cout << metricNames[j] << ":" << endl;
                    for (double val : resultShow[i][j].getVector())
                        cout << val << endl;
                    cout << endl;
                }
                
                ofstream fout;
                fout.open("BootstrapResult.txt");
                for (int i = 0; i < 3; i++) {
                    fout << "--------------Result for " << groupNames[i] << "--------------" << endl;
                    for (int j = 0; j < 4; j++) {
                        fout << metricNames[j] << ":" << endl;
                        for (double val : resultShow[i][j].getVector())
                            fout << val << endl;
                        fout << endl;
                    }
                }
                fout.close();
                
                break;
            }

            
            case 4:
            {
                
                if (if_case1 == 0) {
                    cout << "Please retrieve the historical stock data first! Redirecting to Menu..." << endl << endl;
                    continue;
                }
                if (if_case3 == 0)
                {
                    cout << "Please implement bootstrap first! Redirecting to Menu..." << endl << endl;
                    continue;
                }
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区
                
                gnuplot_func G_Plot(N);
                G_Plot.inityValue_CAAR(resultShow);
                G_Plot.plotResults_CAAR();
                
                break;
            }
            
            
            case 5:
            {
                
                if (if_case1 == 0) {
                    cout << "Please retrieve the historical stock data first! Redirecting to Menu..." << endl << endl;
                    continue;
                }
                if (if_case3 == 0)
                {
                    cout << "Please implement bootstrap first! Redirecting to Menu..." << endl << endl;
                    continue;
                }
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区
                
                gnuplot_func G_Plot2(N);
                G_Plot2.inityValue_CAARstd(resultShow);
                G_Plot2.plotResults_CAARstd();
                
                break;
            }
            
            
            case 6:
            {
                exit(0);
            }
            
            
            default:
                cout << "Please input a valid option." << endl;
        }
    }
}
