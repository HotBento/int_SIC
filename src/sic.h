#pragma once

#include "sievestream.h"
// #include "infgraph.h"
#include <sqlite3.h>
#include <fstream>
#include <boost/algorithm/string.hpp>

class Argument
{
public:
    int k;
    string dataset;
    double epsilon;
    double T;
};



class SIC
{
private:
	void update(pair<int, int> action);
    friend int sic_callback1(void* void_sic, int argc, char** argv, char** azColName);
    vector<SieveStream> sievestream_list;
    vector<bool> available;
    const int MAX_KEEP_NUM;
    const int k;//max size of seedset
    const int N;//max keep number of checkpoints
    const double beta;
    // const double approximation;
    int current_time;
    ifstream db;
public:
	void RealTimeInfluenceMaximization();
	SIC(int keep_num, int k, double beta, string db_place, int N);
};

int sic_callback1(void* void_sic, int argc, char** argv, char** azColName);