#include "sic.h"
#define SIC_DEBUG_TIME 0

SIC::SIC(int k, double beta, string db_place, int N, int print_time) : k(k), beta(beta), N(N), print_time(print_time)
{
  available = vector<bool>(N, false);
  db.open(db_place, ios::in);
  if (!db.is_open())
	{
		cout << "文件打开失败" << endl;
		exit(0);
	}
}

void SIC::RealTimeInfluenceMaximization()
{
  string line;
  int cnt = 0;
  // cout << 1 << endl;
  while(getline(db, line))
  {
    #if(SIC_DEBUG_TIME==1)
    clock_t start = clock();
    cout << "---------------------------------" << endl;
    #endif
    cout << cnt << endl;
    cnt++;
    vector<string> line_arg;
    boost::split(line_arg, line, boost::is_any_of(" "), boost::token_compress_on);
    int author = atoi(line_arg.at(0).c_str());
    int parent_author = atoi(line_arg.at(1).c_str());
    int time = atoi(line_arg.at(2).c_str());
    sievestream_list.push_back(SieveStream(k, beta));
    available.push_back(true);
    #if(SIC_DEBUG_TIME==1)
    clock_t preprocess_time = clock();
    cout << "Preprocess time: " << preprocess_time - start << endl;
    #endif
    if(available.at(0) == false)
    {
      #if(SIC_DEBUG_TIME==2)
      clock_t before = clock();
      clock_t now = clock();
      #endif
      vector<thread> thread_list;
      // thread t1(callback, ref(sievestream_list.at(0)), pair<int, int>(parent_author, author));
      for(auto& j : sievestream_list)
      {
        //update each SieveStream oracle
        // j.Process(pair<int, int>(parent_author, author));
        thread_list.emplace_back(thread(callback, ref(j), pair<int, int>(parent_author, author)));
        current_time = time;
        #if(SIC_DEBUG_TIME==2)
        now = clock();
        cout << "Preprocess time: " << now - before << endl;
        before = now;
        #endif
      }
      for(auto& i : thread_list) i.join();
      available.erase(available.begin());
      #if(SIC_DEBUG_TIME==1)
      clock_t process_time = clock();
      cout << "Process time: " << process_time - preprocess_time << endl;
      #endif
      continue;
    }
    
    //to check
    vector<thread> thread_list;
    for(int i = 0; i < available.size(); ++i)
    {
      // if(available.at(i)) sievestream_list.at(i).Process(pair<int, int>(parent_author, author)); 
      if(available.at(i)) thread_list.emplace_back(thread(callback, ref(sievestream_list.at(i)), pair<int, int>(parent_author, author)));
    }
    for(auto& i : thread_list) i.join();
    #if(SIC_DEBUG_TIME==1)
    clock_t process_time = clock();
    cout << "Process time: " << process_time - preprocess_time << endl;
    #endif
    int end = available.size()-2;
    while(available.at(end) == false) --end;
    for(int i = 0; i < end; ++i)
    {
      if(available.at(i))
      {
        for(int j = i+1; j < end; ++j)
        {
          if(available.at(j) == false) continue;
          int next_availale = j + 1;
          while(available.at(next_availale) == false) ++next_availale;
          if(sievestream_list.at(j).seed_influence_value >= (1-beta) * sievestream_list.at(i).seed_influence_value
          && sievestream_list.at(next_availale).seed_influence_value >= (1-beta) * sievestream_list.at(i).seed_influence_value)
          {
            available[j] = false;
          }
          else break;
        }
      }
    }
    if(available.at(1))
    {
      available.erase(available.begin());
      sievestream_list.erase(sievestream_list.begin());
    }
    else
    {
      available.erase(available.begin()+1);
      sievestream_list.erase(sievestream_list.begin()+1);
    }
    #if(SIC_DEBUG_TIME==1)
    clock_t afterprocess_time = clock();
    cout << "After_process time: " << afterprocess_time - process_time << endl;
    #endif

    if(current_time%print_time == 0 && current_time != time)
    {
      current_time = time;
      cout << "Current time: " << current_time << endl;
      sievestream_list[0].PrintResult();
      cout << "--------------------------------------------------------------------------------" << endl;
    }

    
  }
  
}

void callback(SieveStream& sievestream, pair<int, int> action)
{
  sievestream.Process(action);
}