#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "FrameTable.h"
#include <stdlib.h>
using namespace std;

struct FrameTableValue {
	int procNum;
	int pageNum;
	bool isUsed;
	int time;
	int addedTime;
};

class FrameTable
{
	int usedNum;
	std::vector<FrameTableValue> frameTable;
public:
	FrameTable(int size);
	FrameTable(void);
	~FrameTable(void);
	int isHit(int procNum, int pageNum);
	int addToTable(int procNum, int pageNum, int time);
	int isFull(){return usedNum>=(int)frameTable.size();};
	FrameTableValue envictFirst(int* envictedFrame);				//return frameTableValue which is envicted
	FrameTableValue envictRandom(int numOfPages, int* envictedFrame, FILE *pRandomFile, bool showRandom);   //return frameTableValue which is envicted
	bool updateTime(int procNum, int pageNum, int time);
	int getRandomNum(FILE *pRandomFile, bool showRandom);
	void setFrameTableValue(int frameNum, FrameTableValue newValue);
};

