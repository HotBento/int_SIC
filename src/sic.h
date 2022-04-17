#pragma once

#include "sievestream.h"
#include <time.h>
// #include "infgraph.h"

// #include <sqlite3.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <thread>

class Argument
{
public:
    int k;
    int n;
    int l;
    string dbplace;
    double beta;
};



class SIC
{
private:
	void update(pair<int, int> action);
    // friend int sic_callback1(void* void_sic, int argc, char** argv, char** azColName);
    vector<SieveStream> sievestream_list;
    vector<bool> available;
    int MAX_KEEP_NUM;
    int k;//max size of seedset
    int N;//max keep number of checkpoints
    double beta;
    // const double approximation;
    int current_time;
    int print_time;
    ifstream db;
public:
    friend void callback(SieveStream& sievestream, pair<int, int> action);
	void RealTimeInfluenceMaximization();
	SIC(int k, double beta, string db_place, int N, int print_time);
};

void callback(SieveStream& sievestream, pair<int, int> action);