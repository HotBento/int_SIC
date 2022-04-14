#include "sfmt/SFMT.h"
#include "head.h"
#include "graph.h"

typedef pair<double,int> dipair;


#include "iheap.h"
#include <queue>	
#include <utility>  

struct CompareBySecond {
	bool operator()(pair<int, int> a, pair<int, int> b)
	{
		return a.second < b.second;
	}
};


class InfGraph: public Graph
{
private:
    vector<bool> visit;
    vector<int> visit_mark;
public:
    vector<vector<int> > hyperG;
    vector<vector<int> > hyperGT;

    InfGraph(string folder, string graph_file): Graph(folder, graph_file)
    {
        sfmt_init_gen_rand(&sfmtSeed , 95082);
        init_hyper_graph();
        visit = vector<bool> (n);
        visit_mark = vector<int> (n);
    }


    void init_hyper_graph(){
        hyperG.clear();
        for (int i = 0; i < n; i++)
            hyperG.push_back(vector<int>());
        hyperGT.clear();
    }
    void build_hyper_graph_r(int64 R, const Argument & arg)
    {
        if( R > INT_MAX ){
            cout<<"Error:R too large"<<endl;
            exit(1);
        }
        //INFO("build_hyper_graph_r", R);



        int prevSize = hyperGT.size();
        while ((int)hyperGT.size() <= R)
            hyperGT.push_back( vector<int>() );



        vector<int> random_number;
        for (int i = 0; i < R; i++)
        {
            random_number.push_back(  sfmt_genrand_uint32(&sfmtSeed) % n);
        }

        //trying BFS start from same node
        
        //构建可达集
        for (int i = prevSize; i < R; i++)
        {
#ifdef CONTINUOUS
            BuildHypergraphNode(random_number[i], i, arg );
#endif
#ifdef DISCRETE
            BuildHypergraphNode(random_number[i], i );
#endif
        }

        //转化为反向可达集
        int totAddedElement = 0;
        for (int i = prevSize; i < R; i++)
        {
            for (int t : hyperGT[i])
            {
                hyperG[t].push_back(i);
                //hyperG.addElement(t, i);
                totAddedElement++;
            }
        }
    }

#ifdef DISCRETE
int BuildHypergraphNode(int uStart, int hyperiiid)
{
    int n_visit_edge = 1;
        ASSERT((int)hyperGT.size() > hyperiiid);
        hyperGT[hyperiiid].push_back(uStart);

    int n_visit_mark = 0;

    q.clear();
    q.push_back(uStart);
    ASSERT(n_visit_mark < n);
    visit_mark[n_visit_mark++] = uStart;
    visit[uStart] = true;
    while (!q.empty())
    {

        int expand = q.front();
        q.pop_front();
        if (influModel == IC)
        {
            int i = expand;
            for (int j = 0; j < (int)gT[i].size(); j++)
            {
                //int u=expand;
                int v = gT[i][j];
                n_visit_edge++;
                double randDouble = sfmt_genrand_real1(&sfmtSeed);
                if (randDouble > probT[i][j])
                    continue;
                if (visit[v])
                    continue;
                if (!visit[v])
                {
                    ASSERT(n_visit_mark < n);
                    visit_mark[n_visit_mark++] = v;
                    visit[v] = true;
                }
                q.push_back(v);
                    ASSERT((int)hyperGT.size() > hyperiiid);
                    hyperGT[hyperiiid].push_back(v);
            }
        }
        else if (influModel == LT)
        {
            if (gT[expand].size() == 0)
                continue;
            ASSERT(gT[expand].size() > 0);
            n_visit_edge += gT[expand].size();
            double randDouble = sfmt_genrand_real1(&sfmtSeed);
            for (int i = 0; i < (int)gT[expand].size(); i++)
            {
                ASSERT( i < (int)probT[expand].size());
                randDouble -= probT[expand][i];
                if (randDouble > 0)
                    continue;
                //int u=expand;
                int v = gT[expand][i];

                if (visit[v])
                    break;
                if (!visit[v])
                {
                    visit_mark[n_visit_mark++] = v;
                    visit[v] = true;
                }
                q.push_back(v);
                    ASSERT((int)hyperGT.size() > hyperiiid);
                    hyperGT[hyperiiid].push_back(v);
                break;
            }
        }
        else
            ASSERT(false);
    }
    for (int i = 0; i < n_visit_mark; i++)
        visit[visit_mark[i]] = false;
    return n_visit_edge;
}
#endif
#ifdef CONTINUOUS

vector<double> dist;
vector<int> clean_helper;

iHeap<double> iheap;
int BuildHypergraphNode(int uStart, int hyperiiid, const Argument & arg){
    //Weibull::init();
    //Timer tm(4, "Build cont", true);
    //iheap.DeepClean();
    if(iheap.inited == false){
        iheap.initialize(n);
        INFO("init");
    }
    iheap.smart_clean(clean_helper);
    //if(hyperiiid %100 ==0)
        //INFO(hyperiiid);

    double T = arg.T;


    if((int)dist.size()!=n){
        dist=vector<double>(n);
        for(int i=0; i<n; i++)
            dist[i]=-1;
    }
    for(auto element : clean_helper)
        dist[element] = -1;
    clean_helper.clear();


    ASSERT(T>0);
    int n_visit_edge=1;


    dist[uStart]=0;
    clean_helper.push_back(uStart);

    iheap.insert(uStart, 0);


    //TRACE(hyperiiid);
    vector<int> edge_debug;
    int64 debug_time = 0;
    while(!iheap.empty()) {
        int expand=iheap.m_data[0].key;
        double cost=iheap.m_data[0].value;
        iheap.pop();
        //INFO(expand, cost);

        n_visit_edge+=gT[expand].size();
        edge_debug.push_back( gT[expand].size());

        if(cost > T)
        {
            INFO("cound not happen");
            break;
        }

            //hyperG[v].push_back(hyperiiid);
        ASSERT((int)hyperGT.size() > hyperiiid);
        hyperGT[hyperiiid].push_back(expand);

        for(int j=0; j<(int)gT[expand].size(); j++){
            //int u=expand;
            int v=gT[expand][j];
            //int64 now = rdtsc();
            double randDouble=double(rand())/double(RAND_MAX);
            double shape=probT[expand][j].F;
            double scale=probT[expand][j].S;
            //double randWeibull= scale * Weibull::f[int(randDouble*1000)][int(shape*1000)];
            double randWeibull=scale * pow( - log( randDouble), 1.0/shape);
            //TRACE(randWeibull);

            //debug_time += rdtsc() - now;
            if( ( dist[v]==-1 || dist[v] > dist[expand] + randWeibull ) && dist[expand] + randWeibull < T)
            {
                dist[v]=dist[expand]+randWeibull;
                iheap.insert( v, dist[v] );
                clean_helper.push_back(v);
            }
        }
    }
    return n_visit_edge;

    if(hyperGT[hyperiiid].size() > 1000){
        INFO(debug_time / TIMES_PER_SEC);
        INFO(hyperGT[hyperiiid].size());
        INFO(n_visit_edge);
    }
    return n_visit_edge;
}
#endif

    //return the number of edges visited
    deque<int> q;
    sfmt_t sfmtSeed;
	vector<int> seedSet;

	//This is build on Mapped Priority Queue
	double build_seedset(int k)
	{

		priority_queue<pair<int, int>, vector<pair<int, int> >, CompareBySecond>heap;
		vector<int>coverage(n, 0);

		for (int i = 0; i < n; i++)
		{
			pair<int, int>tep(make_pair(i, (int)hyperG[i].size()));
			heap.push(tep);
			coverage[i] = (int)hyperG[i].size();
		}

		int maxInd;

		long long influence = 0;
		long long numEdge = hyperGT.size();

		// check if an edge is removed
		vector<bool> edgeMark(numEdge, false);
		// check if an node is remained in the heap
		vector<bool> nodeMark(n + 1, true);
        
		seedSet.clear();
		while ((int)seedSet.size()<k)
		{
			pair<int, int>ele = heap.top();
			heap.pop();
			if (ele.second > coverage[ele.first])
			{
				ele.second = coverage[ele.first];
				heap.push(ele);
				continue;
			}

			maxInd = ele.first;
			vector<int>e = hyperG[maxInd];  
			influence += coverage[maxInd];
			seedSet.push_back(maxInd);
			nodeMark[maxInd] = false;

			for (unsigned int j = 0; j < e.size(); ++j){
				if (edgeMark[e[j]])continue;

				vector<int>nList = hyperGT[e[j]];
				for (unsigned int l = 0; l < nList.size(); ++l){
					if (nodeMark[nList[l]])coverage[nList[l]]--;
				}
				edgeMark[e[j]] = true;
			}
		}
		return 1.0*influence / hyperGT.size();
	}

};





