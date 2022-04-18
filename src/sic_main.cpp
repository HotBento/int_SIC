#include "sic.h"
#include "head.h"

void run_with_parameter(Argument arg)
{
	cout << "--------------------------------------------------------------------------------" << endl;
    cout << "dbplace: " << arg.dbplace << endl << " beta:" << arg.beta << endl
    << "k: " << arg.k << endl << "n: " << arg.n << endl << "u: " << arg.u << endl;

    SIC sic(arg.k, arg.beta, arg.dbplace, arg.n, arg.l, arg.u);
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
            cout << "./sic -u *** -beta *** -k *** -n *** -l *** -dbplace ***" << endl;
            return ;
        }
        if(argv[i] == string("-u")) 
            arg.u = atoi(argv[i + 1]);
        if(argv[i] == string("-l")) 
            arg.l = atoi(argv[i + 1]);
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
