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
    int u;
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
    int L;//data num for each window slide
    int U;//max data num
    bool timer_on;
    clock_t timer;
    double beta;
    ifstream db;
public:
    friend void callback(SieveStream& sievestream, pair<int, int> action, bool if_update);
	void RealTimeInfluenceMaximization();
	SIC(int k, double beta, string db_place, int N, int L, int u);
};

void callback(SieveStream& sievestream, pair<int, int> action, bool if_update);