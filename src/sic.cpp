#include "sic.h"
#define SIC_DEBUG_TIME 0

SIC::SIC(int k, double beta, string db_place, int N, int L, int u, string logplace) : k(k), beta(beta), N(N), L(L), U(u)
{
  timer_on = false;
  // create log file
  int slash_place = db_place.rfind("/");
  int point_place = db_place.rfind(".");
  string db_type = db_place.substr(slash_place+1, point_place-slash_place-1);
  time_t now = time(0);
  tm *ltm = localtime(&now);
  logplace = logplace + "/" + db_type + to_string(ltm->tm_year+1900) + "_" + to_string(ltm->tm_mon+1) + "_"
  + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + "_" + to_string(ltm->tm_min) + ".txt";
  log_file.open(logplace, ios::out);
  if (!log_file.is_open())
	{
		cout << "log文件打开失败" << endl;
		exit(0);
	}
  // open database
  db.open(db_place, ios::in);
  if (!db.is_open())
	{
		cout << "db文件打开失败" << endl;
		exit(0);
	}
}

void SIC::RealTimeInfluenceMaximization()
{
  string line;
  int cnt = 0;
  while(getline(db, line) && cnt != U)
  {
    #if(SIC_DEBUG_TIME==1)
    clock_t start = clock();
    cout << "---------------------------------" << endl;
    #endif
    if(cnt%L == 0) cout << cnt << endl;

    // get and handle data
    vector<string> line_arg;
    boost::split(line_arg, line, boost::is_any_of(" "), boost::token_compress_on);
    int author = atoi(line_arg.at(0).c_str());
    int parent_author = atoi(line_arg.at(1).c_str());
    int time = atoi(line_arg.at(2).c_str());

    
    #if(SIC_DEBUG_TIME==1)
    clock_t preprocess_time = clock();
    cout << "Preprocess time: " << preprocess_time - start << endl;
    #endif

    // init part
    // cout << available.size() << ", " << N/L << endl;
    if(available.size() != N/L)
    {
      #if(SIC_DEBUG_TIME==2)
      clock_t before = clock();
      clock_t now = clock();
      #endif

      // window slide, part 1, append list
      if(cnt%L == 0)
      {
        // cout << available.size() << ", " << N/L << endl;
        sievestream_list.push_back(SieveStream(k, beta));
        available.push_back(true);
      }

      //update each SieveStream oracle
      // cout << "Update" << cnt << endl;
      vector<thread> thread_list;
      for(auto& j : sievestream_list)
      {
        // j.Process(pair<int, int>(parent_author, author));
        thread_list.emplace_back(thread(callback, ref(j), pair<int, int>(parent_author, author), (cnt%L == 0)));
        #if(SIC_DEBUG_TIME==2)
        now = clock();
        cout << "Preprocess time: " << now - before << endl;
        before = now;
        #endif
      }
      for(auto& i : thread_list) i.join();
      if(cnt%L == 0)
      {
        // prune list
        int end = available.size()-2;
        while(end > 0 && available.at(end) == false) --end;
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
                // cout << sievestream_list.at(i).seed_influence_value << ", " << sievestream_list.at(j).seed_influence_value << ", " << sievestream_list.at(next_availale).seed_influence_value << endl;
                available[j] = false;
              }
              else break;
            }
          }
        }
        // cout << "Checkpoints num: " << count(available.begin(), available.end(), true) << endl;
        // cout << available.size() << endl;
        // for(bool i : available)
        // {
        //   cout << i << ", ";
        // }
        // cout << endl;
      }
      
      ++cnt;
      #if(SIC_DEBUG_TIME==1)
      clock_t process_time = clock();
      cout << "Process time: " << process_time - preprocess_time << endl;
      #endif
      continue;
    }

    // cout << "successfully init" << endl;

    // window slide, part 1, append list
    if(cnt%L == 0)
    {
      sievestream_list.push_back(SieveStream(k, beta));
      available.push_back(true);
    }
    
    //update each SieveStream oracle
    vector<thread> thread_list;
    for(int i = 0; i < available.size(); ++i)
    {
      // if(available.at(i)) sievestream_list.at(i).Process(pair<int, int>(parent_author, author)); 
      if(available.at(i)) thread_list.emplace_back(thread(callback, ref(sievestream_list.at(i)), pair<int, int>(parent_author, author), (cnt%L==0)));
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

      // prune list
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

      // delete element
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
      int result_indx = 0;
      for(int i = 1; i < N/L; ++i)
      {
        if(available[i])
        {
          result_indx = i;
          break;
        }
      }
      // for(bool i : available)
      // {
      //   cout << i << ", ";
      // }
      #if(SIC_DEBUG_TIME==1)
      clock_t afterprocess_time = clock();
      cout << "After_process time: " << afterprocess_time - process_time << endl;
      #endif

      // print result
      cout << "Checkpoints num: " << count(available.begin(), available.end(), true) << endl;
      cout << "Current data time: " << time << endl;
      log_file << "Checkpoints num: " << count(available.begin(), available.end(), true) << endl;
      log_file << "Current data time: " << time << endl;
      sievestream_list[result_indx].PrintResult();
      cout << "--------------------------------------------------------------------------------" << endl;

      // cnt = 0;
    }
    ++cnt;
  }
  cout << "successfully ended" << endl;
  
}

void callback(SieveStream& sievestream, pair<int, int> action, bool if_update)
{
  sievestream.Process(action, if_update);
}