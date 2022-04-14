#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class SieveStream
{
private:
  vector<pair<int, vector<int>>> user_list;//include user
  vector<pair<int, vector<int>>> rr_user_list;
  vector<pair<int, vector<int>>> unprocessed_user_list;
  vector<int> seed_user;
  vector<int> seed_influence_set;
  // int max_cardinality;
  const int k;//max size of seedset
  int m;//max influence value of a single ingluence
  // vector<pair<int, int>> user;//<user_id, influence_value>
  const double beta;

  void UnprocessedListAppend(pair<int, vector<int>> unprocessed_user);
  void UpdateStream();

public:
  int seed_influence_value;
  void Process(pair<int, int> action);
  void PrintResult();

  SieveStream(int k, double beta);
  ~SieveStream();
};

struct SieveStreamInstance
{
  double opt;
  vector<int> seed_set;
  vector<int> influence_set;

  SieveStreamInstance(double opt, vector<int> seed_set, vector<int> influence_set);
};
