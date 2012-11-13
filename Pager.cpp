#include "Pager.h"
#include <cmath>
#include <fstream>


using namespace std;

Pager::Pager(int machineSize, int pageSize, int processSize, int jobMix, int numRefPerProc, RepAlgo repAlgo, bool debugFlag, bool showRandom)
{
	this-> machineSize = machineSize;
	this-> pageSize = pageSize;
	this-> processSize = processSize;
	this-> jobMix = jobMix;
	this-> numRefPerProc = numRefPerProc;
	this-> repAlgo = repAlgo; 
	this-> debugFlag = debugFlag; 
	this-> showRandom = showRandom;
	
	pRandomFile = fopen("random.txt", "r");
	this-> frameTable = FrameTable(machineSize/pageSize);
	if (jobMix == 1){
		procs.push_back(Proc(processSize, jobMix, numRefPerProc, 1, pageSize));
	}
	else{
		for (int i = 0 ; i < 4; i++){
			procs.push_back(Proc(processSize, jobMix, numRefPerProc, i+1, pageSize));
		}
	}
	this-> time = 1;

}

void Pager::paging(){
	while(!isAllFinish()){
		for (int i = 0 ; i< (int)procs.size(); i++){
			for (int ref = 0; ref<3; ref++){
				if (procs.at(i).isFinish()){
					break;
				}
				if (debugFlag){
					cout<<i+1<<" references word "<<procs.at(i).getCurWord()<<" (page "<<procs.at(i).getCurPage()
						<<") at time "<<time<<": ";
				}
				if (frameTable.isHit(i, procs.at(i).getCurPage()) != -1){		//hit
					if (debugFlag){
						cout<<"Hit in frame "<<frameTable.isHit(i, procs.at(i).getCurPage())<<"\n";
					}
					if (repAlgo == lru){
						if (!frameTable.updateTime(i, procs.at(i).getCurPage(), time)){
							throw "update a proc that is not exsit in frametable";
						}
					}
				}
				else{															//page fault
					if (debugFlag){
						cout<<"Fault, ";
					}
					procs.at(i).addFaultCount();
					if (frameTable.isFull()){									//need to evict
						int tmpEnvictedFrom = -1;
						FrameTableValue tmpEnvicted = replacement(repAlgo, &tmpEnvictedFrom);
						//compute residency time
						this->procs.at(tmpEnvicted.procNum).envictedUpdate(time - tmpEnvicted.addedTime);
						//set new value
						FrameTableValue tmp;
						tmp.procNum = i;
						tmp.pageNum = procs.at(i).getCurPage();
						tmp.isUsed = true;
						tmp.time = time;
						tmp.addedTime = time;
						this->frameTable.setFrameTableValue(tmpEnvictedFrom, tmp);
						if (debugFlag){
							cout<<"evicting page "<<tmpEnvicted.pageNum<<" of "<<tmpEnvicted.procNum+1<<" from frame "
								<<tmpEnvictedFrom<<".\n";
						}
					}
					else{														//use free frame
						int frame = frameTable.addToTable(i, procs.at(i).getCurPage(), time);
						if (frame == -1){
							throw "should not be full but it is";
						}
						else{
							if (debugFlag){
								cout<<"using free frame "<<frame<<".\n";
							}
						}
					}
				}
				procs.at(i).goToNextRef(i, pRandomFile, showRandom);
				time++;
			}
		}
	}
}

bool Pager::isAllFinish(){
	for (int i = 0 ; i < (int)procs.size(); i++){
		if (!procs.at(i).isFinish()){
			return false;
		}
	}
	return true;
}

//int Pager::getRandomNum(){
//	char tempStr [15];
//	
//	if (fscanf(pRandomFile, "%s", tempStr) != EOF) {
//		cout<<"?? uses random number: "<<atoi(tempStr)<<"\n";
//		return (atoi(tempStr));
//	}
//	else{
//		rewind(pRandomFile);
//		return getRandomNum();
//	}
//}

Pager::~Pager(void)
{
}

FrameTableValue Pager::replacement(RepAlgo repAlgo, int* envictedFrame){
	FrameTableValue result;
	switch (repAlgo){
	case fifo:	
		result = frameTable.envictFirst( envictedFrame);
		break;
	case randomAlgo:
		result = frameTable.envictRandom(machineSize/pageSize, envictedFrame, pRandomFile, showRandom);
		break;
	case lru:
		result = frameTable.envictFirst(envictedFrame);
		break;
	}
	return result;
}

void Pager::summarizeHead(){
	cout<<"The machine size is "<<machineSize<<".\n";
	cout<<"The page size is "<<pageSize<<".\n";
	cout<<"The process size is "<<processSize<<".\n";
	cout<<"The job mix number is "<<jobMix<<".\n";
	cout<<"The number of references per process is "<<numRefPerProc<<".\n";
	switch (repAlgo){
	case fifo:
		cout<<"The replacement algorithm is fifo.\n";
		break;
	case randomAlgo:
		cout<<"The replacement algorithm is random.\n";
		break;
	case lru:
		cout<<"The replacement algorithm is lru.\n";
		break;
	}
	cout<<"The level of debugging output is debugFlag:"<<debugFlag<<"\nshowRandom:"<<showRandom<<".\n\n";
}

void Pager::summarizedTail(){
	int numFaultSum = 0;
	int residencySum = 0;
	int EnvictedSum = 0;
	cout<<"\n";
	for (int i = 0; i< (int)procs.size(); i++){
		procs.at(i).summarizeTail(i);
		numFaultSum += procs.at(i).getFaltCount();
		residencySum += procs.at(i).getResidencySum();
		EnvictedSum += procs.at(i).getNumEnvict();
	}
	if (EnvictedSum!= 0){
		cout<<"\nThe total number of faults is "<<numFaultSum<<" and the overall average residency is "
			<<(double)residencySum/EnvictedSum<<".";
	}
	else{
		cout<<"\nThe total number of faults is "<<numFaultSum<<".\n";
		cout<<"     With no evictions, the overall average residence is undefined.";
	}
}