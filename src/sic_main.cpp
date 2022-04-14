#include "sic.h"

void run_with_parameter(Argument arg)
{
	cout << "--------------------------------------------------------------------------------" << endl;
    cout << arg.dataset << " k=" << arg.k << " epsilon=" << arg.epsilon << endl;

    // SIC::RealTimeInfluenceMaximization(arg);

	Timer::show();
}

void run(int argc, char **argv)
{
  	Argument arg;
  	for(int i = 0; i < argc; i++)
    {
        if(argv[i] == string("-help") || argv[i] == string("--help") || argc == 1)
        {
            cout << "./sic -dataset *** -epsilon *** -k ***" << endl;
            return ;
        }
        if(argv[i] == string("-dataset")) 
            arg.dataset = argv[i + 1];
        if(argv[i] == string("-epsilon")) 
            arg.epsilon = atof(argv[i + 1]);
        if(argv[i] == string("-T")) 
            arg.T = atof(argv[i + 1]);
        if(argv[i] == string("-k")) 
            arg.k = atoi(argv[i + 1]);
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
