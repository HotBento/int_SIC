#include "sievestream.h"
#define SIEVESTREAM_DEBUG_TIME 0

void SieveStream::Process(pair<int, int> action, bool if_update)
{
  if(!user_list.contains(action.first)) 
  {
    user_list[action.first] = set<int>{action.first};
    rr_user_list[action.first] = set<int>{action.first};
  }
  if(!user_list.contains(action.second))
  {
    user_list[action.second] = set<int>{action.second};
    rr_user_list[action.second] = set<int>{action.second};
    rr_user_single_list[action.second] = set<int>{action.first};
  }
  else rr_user_single_list[action.second].insert(action.first);

  //update user_list
  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t start = clock();
  cout << "---------------------------------" << endl;
  #endif

  set<int> temp = rr_user_list[action.first];
  UpdateUserList(action.first, user_list[action.second], temp);

  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t update_userlist_time = clock();
  cout << "Update userlist time: " << update_userlist_time - start << endl;
  #endif
  
  if(if_update) UpdateStream();

  #if(SIEVESTREAM_DEBUG_TIME==1)
  clock_t update_stream_time = clock();
  cout << "Update stream time: " << update_stream_time - update_userlist_time << endl;
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

void SieveStream::UpdateUserList(int user, set<int> to_append, set<int>& unprocessed)
{
  #if(SIEVESTREAM_DEBUG_TIME==2)
  clock_t start = clock();
  cout << "---------------------------------" << endl;
  #endif
  pair<int, set<int>> temp(user, set<int>());
  set<int>::iterator set_iter = to_append.begin();
  while (set_iter != to_append.end())
  {
    if(!user_list[user].insert(*set_iter).second)
    {
      set<int>::iterator temp_iter = set_iter;
      ++set_iter;
      to_append.erase(temp_iter);
    }
    else
    {
      rr_user_list[*set_iter].insert(user);
      temp.second.insert(*set_iter);
      ++set_iter;
    }
  }
  #if(SIEVESTREAM_DEBUG_TIME==2)
  clock_t update_userlist_time = clock();
  cout << "Update userlist time: " << update_userlist_time - start << endl;
  #endif
  UnprocessedListAppend(temp);
  unprocessed.erase(user);
  #if(SIEVESTREAM_DEBUG_TIME==2)
  clock_t process_append_list_time = clock();
  cout << "Process time: " << process_append_list_time - update_userlist_time << endl;
  for(int i : rr_user_single_list[user])
  {
    if(unprocessed.contains(i));
  }
  clock_t check_time = clock();
  cout << "check time: " << check_time - process_append_list_time << endl;
  #endif
  for(int i : rr_user_single_list[user])
  {
    if(unprocessed.contains(i)) UpdateUserList(i, to_append, unprocessed);
  }
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
