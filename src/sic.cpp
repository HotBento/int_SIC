#include "sic.h"
#define SIC_DEBUG_TIME 0

SIC::SIC(int k, double beta, string db_place, int N, int L) : k(k), beta(beta), N(N), L(L)
{
  timer_on = false;
  available = vector<bool>(N/L, false);
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
    if(cnt%1000 == 0) cout << cnt << endl;

    // get and handle data
    vector<string> line_arg;
    boost::split(line_arg, line, boost::is_any_of(" "), boost::token_compress_on);
    int author = atoi(line_arg.at(0).c_str());
    int parent_author = atoi(line_arg.at(1).c_str());
    int time = atoi(line_arg.at(2).c_str());

    // window slide, part 1, append list
    if(cnt%L == 0)
    {
      sievestream_list.push_back(SieveStream(k, beta));
      available.push_back(true);
    }
    #if(SIC_DEBUG_TIME==1)
    clock_t preprocess_time = clock();
    cout << "Preprocess time: " << preprocess_time - start << endl;
    #endif

    // init part
    if(available.at(0) == false)
    {
      #if(SIC_DEBUG_TIME==2)
      clock_t before = clock();
      clock_t now = clock();
      #endif
      //update each SieveStream oracle
      vector<thread> thread_list;
      for(auto& j : sievestream_list)
      {
        // j.Process(pair<int, int>(parent_author, author));
        thread_list.emplace_back(thread(callback, ref(j), pair<int, int>(parent_author, author)));
        #if(SIC_DEBUG_TIME==2)
        now = clock();
        cout << "Preprocess time: " << now - before << endl;
        before = now;
        #endif
      }
      for(auto& i : thread_list) i.join();
      if(cnt%L == 0) available.erase(available.begin());
      ++cnt;
      #if(SIC_DEBUG_TIME==1)
      clock_t process_time = clock();
      cout << "Process time: " << process_time - preprocess_time << endl;
      #endif
      continue;
    }
    
    //update each SieveStream oracle
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

    // window slide, part 2, prune list and delete element
    if(cnt%L == 0)
    {
      if(timer_on == false) timer_on = true;
      else
      {
        clock_t temp = clock();
        cout << "Time cost: " << 1000.0 * (temp - timer) / CLOCKS_PER_SEC << "ms" << endl;
        cout << "Throughput: " << L * CLOCKS_PER_SEC * 0.001 / (temp - timer) << "K/s" << endl;
      }
      timer = clock();
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

      // print result
      cout << "Current data time: " << time << endl;
      sievestream_list[0].PrintResult();
      cout << "--------------------------------------------------------------------------------" << endl;

      cnt = 0;
    }
    ++cnt;
  }
  
}

void callback(SieveStream& sievestream, pair<int, int> action)
{
  sievestream.Process(action);
}