#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <time.h>
using namespace std;

class SieveStream
{
private:
  // vector<pair<int, vector<int>>> user_list;//include user
  // vector<pair<int, vector<int>>> rr_user_list;
  // vector<pair<int, vector<int>>> unprocessed_user_list;
  // vector<int> seed_user;
  // vector<int> seed_influence_set;
  map<int, set<int>> user_list;//include user
  map<int, set<int>> rr_user_list;
  // map<int, set<int>> user_single_list;
  map<int, set<int>> rr_user_single_list;
  map<int, set<int>> unprocessed_user_list;
  set<int> seed_user;
  set<int> seed_influence_set;
  // int max_cardinality;
  int k;//max size of seedset
  int m;//max influence value of a single ingluence
  // vector<pair<int, int>> user;//<user_id, influence_value>
  double beta;

  void UpdateUserList(vector<int> user, set<int>to_append, set<int>&unprocessed);
  // void UpdateRRUserList(const int& user, set<int>to_append, set<int>&unprocessed);
  void UnprocessedListAppend(pair<int, set<int>> unprocessed_user);
  void UpdateStream();

public:
  int seed_influence_value;
  //action.first influences action.second
  void Process(pair<int, int> action, bool if_update);
  void PrintResult();

  SieveStream(int k, double beta);
};

struct SieveStreamInstance
{
  double opt;
  set<int> seed_set;
  set<int> influence_set;

  SieveStreamInstance(double opt, set<int> seed_set, set<int> influence_set);
};

inline void SieveStream::UnprocessedListAppend(pair<int, set<int>> unprocessed_user)
{
  if(!seed_user.contains(unprocessed_user.first))
    unprocessed_user_list[unprocessed_user.first] = user_list[unprocessed_user.first];
  else
  {
    seed_influence_set.merge(unprocessed_user.second);
  }

  if(m < user_list[unprocessed_user.first].size()) m = user_list[unprocessed_user.first].size();
}