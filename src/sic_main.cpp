#include "sic.h"

void run_with_parameter(Argument arg)
{
	cout << "--------------------------------------------------------------------------------" << endl;
    cout << "dbplace: " << arg.dbplace << endl << "keepnum: " << arg.keepnum << endl
    << " beta:" << arg.beta << endl << "k: " << arg.k << endl << "n: " << arg.n << endl;

    SIC sic(arg.keepnum, arg.k, arg.beta, arg.dbplace, arg.n);
    sic.RealTimeInfluenceMaximization();

	Timer::show();
}

void run(int argc, char **argv)
{
  	Argument arg;
  	for(int i = 0; i < argc; i++)
    {
        if(argv[i] == string("-help") || argv[i] == string("--help") || argc == 1)
        {
            cout << "./sic -keepnum *** -beta *** -k *** -n *** -dbplace ***" << endl;
            return ;
        }
        if(argv[i] == string("-keepnum")) 
            arg.keepnum = atoi(argv[i + 1]);
        if(argv[i] == string("-beta")) 
            arg.beta = atof(argv[i + 1]);
        if(argv[i] == string("-k")) 
            arg.k = atoi(argv[i + 1]);
        if(argv[i] == string("-n")) 
            arg.n = atoi(argv[i + 1]);
        if(argv[i] == string("-dbplace")) 
            arg.dbplace = argv[i + 1];
    }
	ASSERT(arg.dataset != "");
	
	run_with_parameter(arg);
}

int main(int argc, char **argv)
{
  	__head_version = "v0.1";
  	OutputInfo info(argc, argv);

  	run(argc, argv);

  	return 0;
}
