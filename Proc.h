#pragma once
#include <fstream>
#include <stdlib.h>

class Proc
{
	int processSize;
	int jobMix;
	int numRef;
	int curWord;
	bool finishFlag;
	int pageSize;
	int curNumRef;
	int numEnvict;		//being envicted
	int residencySum;
	int faultCount;
public:
	Proc(void);
	Proc(int processSize, int jobMix, int numRefPerProc, int procNum, int pageSize);
	~Proc(void);
	bool isFinish(){return finishFlag;};
	void goToNextRef(int procNum, FILE *pRandomFile, bool showRandom);
	int getRandomNum(FILE *pRandomFile, bool showRandom);
	void envictedUpdate(int residencyTime){numEnvict++; residencySum+=residencyTime;};	//update residency time and envicted count when this proc is envicted
	void summarizeTail(int procNum);
	void addFaultCount(){this->faultCount++;};
	/*getters*/
	int getCurWord(){return curWord;};
	int getCurPage(){return curWord/pageSize;};
	int getResidencySum(){return this->residencySum;};
	int getFaltCount(){return this->faultCount;};
	int getNumEnvict(){return this->numEnvict;};
};

