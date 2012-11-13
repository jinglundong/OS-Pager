#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "FrameTable.h"
#include "Proc.h"
using namespace std;

enum RepAlgo {fifo, randomAlgo, lru};

class Pager
{
	FILE *pRandomFile;
	int machineSize;
	int pageSize;
	int processSize;
	int jobMix;
	int numRefPerProc;
	RepAlgo repAlgo;
	bool debugFlag;
	bool showRandom;
	int time;
	FrameTable frameTable;
	std::vector<Proc> procs;
	bool isAllFinish();
	bool isProcFinish(int procNum){return this->procs.at(procNum).isFinish();};
	FrameTableValue replacement(RepAlgo repAlgo, int* envictedFrame);				//return proc number which is enviect
public:
	Pager(int machineSize, int pageSize, int processSize, int jobMix, int numRefPerProc, RepAlgo repAlgo, bool debugFlag, bool showRandom);
	~Pager(void);
	void paging();
	//static int getRandomNum();
	void summarizeHead();
	void summarizedTail();
};
