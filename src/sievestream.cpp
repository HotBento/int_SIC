#include "sievestream.h"

void SieveStream::Process(pair<int, int> action)
{
  //update user_list
  unprocessed_user_list.clear();
  auto iter_tmp0 = find(user_list.begin(), user_list.end(), action.first);
  if(iter_tmp0 == user_list.end())
  {
    vector<int> tmp;
    tmp.push_back(action.first);
    auto iter_tmp1 = find(user_list.begin(), user_list.end(), action.second);
    if(iter_tmp1 == user_list.end())
    {
      tmp.push_back(action.second);
      auto tmp_pair = pair<int, vector<int>>(action.second, vector<int>(1, action.second));
      user_list.push_back(tmp_pair);
      UnprocessedListAppend(tmp_pair);
    }
    else
    {
      for(auto i : (*iter_tmp1).second)
      {
        tmp.push_back(i);
      }
    }
    auto tmp_pair = pair<int, vector<int>>(action.first, tmp);
    user_list.push_back(tmp_pair);
    UnprocessedListAppend(tmp_pair);
  }
  else
  {
    if(find((*iter_tmp0).second.begin(), (*iter_tmp0).second.end(), action.second)==(*iter_tmp0).second.end())
    {
      (*iter_tmp0).second.push_back(action.second);
      UnprocessedListAppend(*iter_tmp0);
    }
    auto iter_tmp1 = find(user_list.begin(), user_list.end(), action.second);
    if(iter_tmp1 == user_list.end())
    {
      auto tmp_pair = pair<int, vector<int>>(action.second, vector<int>(1, action.second));
      user_list.push_back(tmp_pair);
      UnprocessedListAppend(tmp_pair);
    }
  }
  //update rr_user_list
  bool is_first_in_rr = false;
  bool is_second_in_rr = false;
  for(pair<int, vector<int>>& i : rr_user_list)
  {
    if(i.first == action.first) is_first_in_rr = true;
    else if(i.first == action.second) is_second_in_rr = true;
    if(find(i.second.begin(), i.second.end(), action.second)!=i.second.end()
    && find(i.second.begin(), i.second.end(), action.first)==i.second.end())
    {
      i.second.push_back(action.first);
    }
  }
  if(!is_first_in_rr)
  {
    rr_user_list.push_back(pair<int, vector<int>>(action.first, vector<int>(1, action.first)));
  }
  if(!is_second_in_rr)
  {
    rr_user_list.push_back(pair<int, vector<int>>(action.second, vector<int>(1, action.second)));
    rr_user_list.back().second.push_back(action.first);
  }
  
  UpdateStream();

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
      vector<int> add_list;
      if(find(i.influence_set.begin(), i.influence_set.end(), unprocessed_user.first) == i.influence_set.end())
      {
        ++score;
        add_list.push_back(unprocessed_user.first);
      }
      for(auto& j : unprocessed_user.second)
      {
        if(find(i.influence_set.begin(), i.influence_set.end(), j) == i.influence_set.end())
          ++score;
          add_list.push_back(j);
      }
      if(score >= (i.opt/2-i.influence_set.size())/(k-i.seed_set.size()))
      {
        i.seed_set.push_back(unprocessed_user.first);
        i.influence_set.insert(i.influence_set.end(), add_list.begin(), add_list.end());
      }
    }
  }
  vector<int> tmp_seed_set;
  vector<int> tmp_influence_set;
  for(auto i : instance_set)
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
}

void SieveStream::UnprocessedListAppend(pair<int, vector<int>> unprocessed_user)
{
  if(find(seed_user.begin(), seed_user.end(), unprocessed_user.first) == seed_user.end())
    unprocessed_user_list.push_back(unprocessed_user);
  else
  {
    for(auto i : unprocessed_user.second)
    {
      if(find(seed_influence_set.begin(), seed_influence_set.end(), i) == seed_influence_set.end())
        seed_influence_set.push_back(i);
    }
  }

  if(m < unprocessed_user.second.size()) m = unprocessed_user.second.size();
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
  cout << "Seed set: \"" << seed_user[0] << "\"";
  for(int i : seed_user)
  {
    if(i == seed_user[0]) continue;
    cout << ", \"" << i << "\"";
  }
  cout << ";" << endl;
}

SieveStreamInstance::SieveStreamInstance(double opt, vector<int> seed_set, vector<int> influence_set):
  opt(opt), seed_set(seed_set), influence_set(influence_set)
{
  
}

