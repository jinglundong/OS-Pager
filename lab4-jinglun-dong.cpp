#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "Proc.h"
#include "Pager.h"
using namespace std;

int main(int argc, char* argv[]) {
	int machineSize = 0;
	int pageSize = 0;
	int processSize = 0;
	int jobMix = 0;
	int numRefPerProc = 0;
	RepAlgo repAlgo;
	bool debugFlag = false;
	bool showRandom = false;
	int debugLevel = 0;
	
	string str_debugLevel, str_argv;
	if (argc >=7){
		machineSize = atoi(argv[1]);
		pageSize = atoi(argv[2]);
		processSize = atoi(argv[3]);
		jobMix = atoi(argv[4]);
		numRefPerProc = atoi(argv[5]);
		str_argv.assign(argv[6]);
		if (!str_argv.compare(string("fifo"))){
			repAlgo = fifo;
		}
		if (!str_argv.compare(string("random"))){
			repAlgo = randomAlgo;
		}
		if (!str_argv.compare(string("lru"))){
			repAlgo = lru;
		}
		if (argc >= 8){
			debugLevel = atoi(argv[7]);
			if (debugLevel == 1){
				debugFlag = true;
				showRandom = false;
			}
			else {
				if (debugLevel ==2){
					debugFlag = true;
					showRandom = true;
				}
			}
		}
	}
	Pager pager (machineSize, pageSize, processSize, jobMix, numRefPerProc, repAlgo, debugFlag, showRandom);
	pager.summarizeHead();
	pager.paging();
	pager.summarizedTail();

	//cout<<machineSize<<"\n"<<pageSize<<"\n"
	//	<<processSize<<"\n"<<jobMix<<"\n"<<numRefPerProc<<"\n"
	//	<<repAlgo<<"\n";
	//cout<<"argc:"<<argc<<"\n";
	//cout<<"debugFlag:"<<(int)debugFlag<<"showRandom"<<(int)showRandom<<"\n";
	//cout<<debugLevel;
	return 0;
}  