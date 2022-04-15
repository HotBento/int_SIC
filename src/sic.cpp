#include "sic.h"

SIC::SIC(int keep_num, int k, double beta, string db_place, int N) : MAX_KEEP_NUM(keep_num), k(k), beta(beta), N(N)
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
  while(getline(db, line))
  {
    vector<string> line_arg;
    boost::split(line_arg, line, boost::is_any_of(" "), boost::token_compress_on);
    int author = atoi(line_arg.at(0).c_str());
    int parent_author = atoi(line_arg.at(1).c_str());
    int time = atoi(line_arg.at(2).c_str());
    sievestream_list.push_back(SieveStream(k, beta));
    available.push_back(true);
    if(available.at(0) == false)
    {
      for(auto& j : sievestream_list)
      {
        //update each SieveStream oracle
        j.Process(pair<int, int>(parent_author, author));
        current_time = time;
      }
      available.erase(available.begin());
      return;
    }
  }
  
}

// int sic_callback1(void* void_sic, int argc, char** argv, char** azColName)
// {
//   SIC* sic = (SIC*) void_sic;
//   string author = argv[1];
//   string parent_author = argv[2];
//   int current_utc =  atoi(argv[0]);
//   sic->sievestream_list.push_back(SieveStream(sic->k, sic->beta));
//   sic->available.push_back(true);
//   if(sic->available.at(0) == false)
//   {
//     for(auto& j : sic->sievestream_list)
//     {
//       //update each SieveStream oracle
//       j.Process(pair<int, int>(parent_author, author));
//     }
//     sic->available.erase(sic->available.begin());
//     return 0;
//   }
//   for(int i = 0; i < sic->available.size(); ++i)
//   {
//     if(sic->available.at(i)) sic->sievestream_list.at(i).Process(pair<int, int>(parent_author, author)); 
//   }
//   int end = sic->available.size()-2;
//   while(sic->available.at(end) == false) --end;
//   for(int i = 0; i < end; ++i)
//   {
//     if(sic->available.at(i))
//     {
//       for(int j = i+1; j < end; ++j)
//       {
//         if(sic->available.at(j) == false) continue;
//         int next_availale = j + 1;
//         while(sic->available.at(next_availale) == false) ++next_availale;
//         if(sic->sievestream_list.at(j).seed_influence_value >= (1-sic->beta) * sic->sievestream_list.at(i).seed_influence_value
//         && sic->sievestream_list.at(next_availale).seed_influence_value >= (1-sic->beta) * sic->sievestream_list.at(i).seed_influence_value)
//         {
//           sic->available[j] = false;
//         }
//         else break;
//       }
//     }
//   }

//   if(sic->available.at(1))
//   {
//     sic->available.erase(sic->available.begin());
//     sic->sievestream_list.erase(sic->sievestream_list.begin());
//   }
//   else
//   {
//     sic->available.erase(sic->available.begin()+1);
//     sic->sievestream_list.erase(sic->sievestream_list.begin()+1);
//   }

//   if(sic->current_utc%100 == 0 && sic->current_utc != current_utc)
//   {
//     sic->current_utc = current_utc;
//     cout << "Current utc: " << sic->current_utc << endl;
//     sic->sievestream_list[0].PrintResult();
//     cout << "--------------------------------------------------------------------------------" << endl;
//   }

//   return 0;
// }