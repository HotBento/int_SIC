#pragma once

#include "sievestream.h"
// #include "infgraph.h"
#include <sqlite3.h>

class Argument
{
public:
    int k;
    string dataset;
    double epsilon;
    double T;
};



class IC
{
private:
	void update(pair<string, string> action);
    friend int ic_callback1(void* void_sic, int argc, char** argv, char** azColName);
    vector<SieveStream> sievestream_list;
    const int MAX_KEEP_NUM;
    const int k;//max size of seedset
    const double beta;
    int current_utc;
    sqlite3* db;
public:
	void RealTimeInfluenceMaximization();
	IC(int keep_num, int k, double beta, string db_place);
};

int ic_callback1(void* not_used, int argc, char** argv, char** azColName);