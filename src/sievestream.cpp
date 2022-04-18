#include "sievestream.h"
#define SIEVESTREAM_DEBUG_TIME 0

void SieveStream::Process(pair<int, int> action, bool if_update)
{
  // unprocessed_user_list.clear();
  if(!user_list.contains(action.first)) 
  {
    user_list[action.first] = set<int>{action.first};
    rr_user_list[action.first] = set<int>{action.first};
  }
  if(!user_list.contains(action.second))
  {
    user_list[action.second] = set<int>{action.second};
    rr_user_list[action.second] = set<int>{action.second};
  }

  //update user_list
  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t start = clock();
  cout << "---------------------------------" << endl;
  #endif
  for(auto i : rr_user_list[action.first])
  {
    if(user_list[i].contains(action.second)) continue;
    // user_list[i].merge(user_list[action.second]);
    pair<int, set<int>> temp(i, set<int>());
    for(auto j : user_list[action.second])
    {
      if(!user_list[i].contains(j))
      {
        user_list[i].insert(j);
        temp.second.insert(j);
      }
    }
    UnprocessedListAppend(temp);
  }
  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t update_userlist_time = clock();
  cout << "Update userlist time: " << update_userlist_time - start << endl;
  #endif
  //update rr_user_list
  for(auto i : user_list[action.second])
  {
    if(rr_user_list[i].contains(action.first)) continue;
    rr_user_list[i].merge(rr_user_list[action.first]);
  }

  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t update_rruserlist_time = clock();
  cout << "Update rruserlist time: " << update_rruserlist_time - update_userlist_time << endl;
  #endif
  
  if(if_update) UpdateStream();

  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t update_stream_time = clock();
  cout << "Update stream time: " << update_stream_time - update_rruserlist_time << endl;
  #endif

}

void SieveStream::UpdateStream()
{
  vector<SieveStreamInstance> instance_set;
  
  double opt = 1.0;
  while(opt < m) opt *= 1.0 + beta;
  while (opt < 2 * k * m)
  {
    SieveStreamInstance tmp(opt, seed_user, seed_influence_set);
    instance_set.push_back(tmp);
    opt *= 1 + beta;
  }
  
  for(auto unprocessed_user : unprocessed_user_list)
  {
    // if(seed_user.size() == k) return;
    for(auto& i : instance_set)
    {
      if(i.seed_set.size() == k) continue;
      int score = 0;
      set<int> add_list;
      if(i.influence_set.contains(unprocessed_user.first)) continue;
      for(auto& j : unprocessed_user.second)
      {
        if(!i.influence_set.contains(j))
        {
          ++score;
          add_list.insert(j);
        }
      }
      if(score >= (i.opt/2-i.influence_set.size())/(k-i.seed_set.size()))
      {
        i.seed_set.insert(unprocessed_user.first);
        i.influence_set.merge(add_list);
      }
    }
  }
  set<int> tmp_seed_set;
  set<int> tmp_influence_set;
  for(auto& i : instance_set)
  {
    if(i.influence_set.size() > tmp_influence_set.size()
      || (i.influence_set.size() == tmp_influence_set.size() && i.seed_set.size() < tmp_seed_set.size()))
    {
      tmp_seed_set = i.seed_set;
      tmp_influence_set = i.influence_set;
    }
  }
  seed_user = tmp_seed_set;
  seed_influence_set = tmp_influence_set;
  seed_influence_value = tmp_influence_set.size();

  unprocessed_user_list.clear();
}

void SieveStream::UnprocessedListAppend(pair<int, set<int>> unprocessed_user)
{
  if(!seed_user.contains(unprocessed_user.first))
    unprocessed_user_list[unprocessed_user.first] = user_list[unprocessed_user.first];
  else
  {
    seed_influence_set.merge(unprocessed_user.second);
  }

  if(m < user_list[unprocessed_user.first].size()) m = user_list[unprocessed_user.first].size();
}

SieveStream::SieveStream(int k, double beta): k(k), seed_influence_value(0), m(0), beta(beta)
{

}

void SieveStream::PrintResult()
{
  if(seed_user.size() == 0)
  {
    cout << "Error: No seed set found." << endl;
    return;
  }
  cout << "Influence spread: " << seed_influence_value << endl;
  cout << "Seed set: ";
  bool first_time = true;
  for(int i : seed_user)
  {
    if(first_time) first_time = false;
    else cout << ", ";
    cout << "\"" << i << "\"";
  }
  cout << ";" << endl;
}

SieveStreamInstance::SieveStreamInstance(double opt, set<int> seed_set, set<int> influence_set):
  opt(opt), seed_set(seed_set), influence_set(influence_set)
{
  
}
