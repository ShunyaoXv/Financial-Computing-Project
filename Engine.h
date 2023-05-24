#pragma once

#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "StockData.h"
#include "Menu.h"
#include "Vector.h"
#include "gnuplot_linux.h"

using namespace std;


class Engine {
private:
    int N;
    std::string ticker;
    std::map<std::string, StockData*> Data;

public:
    void run();
};

/*
class Engine {
private:
    map<string, StockData*> Data;
    int if_case1 = 0;
    vector<double> surprise;
    vector<string> ValidSymbols;
    map<string, StockData*> Beat, Meet, Miss;
    int N;
    string ticker;
    vector<vector<vector<double>>> result;
    int if_case3 = 0;

public:
    void run();

};
*/