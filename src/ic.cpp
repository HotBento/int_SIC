#include "ic.h"

IC::IC(int keep_num, int k, double beta, string db_place) : MAX_KEEP_NUM(keep_num), k(k), beta(beta)
{
  int rc = sqlite3_open(db_place.c_str(), &db);
  if(rc != SQLITE_OK)
  {
    cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
    exit(0);
  }
}

void IC::RealTimeInfluenceMaximization()
{
  string sql = "SELECT * FROM PART ORDER BY created_utc;";
  char* errmsg;
  sqlite3_exec(db, sql.c_str(), ic_callback1, this, &errmsg);
}

int ic_callback1(void* void_ic, int argc, char** argv, char** azColName)
{
  IC* ic = (IC*) void_ic;
  string author = argv[1];
  string parent_author = argv[2];
  int current_utc =  atoi(argv[0]);
  ic->sievestream_list.push_back(SieveStream(ic->k, ic->beta));
  if(ic->sievestream_list.size() == ic->k)
  {
    ic->sievestream_list.erase(ic->sievestream_list.begin());
  }
  for(auto& j : ic->sievestream_list)
  {
    //update each SieveStream oracle
    j.Process(pair<string, string>(parent_author, author));
  }

  if(ic->current_utc%100 == 0 && ic->current_utc != current_utc)
  {
    ic->current_utc = current_utc;
    cout << "Current utc: " << ic->current_utc << endl;
    ic->sievestream_list[0].PrintResult();
    cout << "--------------------------------------------------------------------------------" << endl;
  }

  return 0;
}