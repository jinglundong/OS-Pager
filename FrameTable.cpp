#include "FrameTable.h"
#include "Pager.h"

FrameTable::FrameTable(int size)
{
	FrameTableValue tmp;
	tmp.procNum = -1;
	tmp.pageNum = -1;
	tmp.isUsed = false;
	tmp.time = -1;
	tmp.addedTime = -1;
	frameTable.resize(size, tmp);
	usedNum = 0;
}


FrameTable::FrameTable(void)
{
}

FrameTable::~FrameTable(void)
{
}

int FrameTable::isHit(int procNum, int pageNum){
	for (int i = 0 ; i < (int)frameTable.size(); i++){
		if (frameTable.at(i).procNum == procNum && frameTable.at(i).pageNum == pageNum){
			return i;
		}
	}
	return -1;
}

int FrameTable::addToTable(int procNum, int pageNum, int time){
	if (isFull())
		return -1;
	else{
		FrameTableValue tmp;
		tmp.procNum = procNum;
		tmp.pageNum = pageNum;
		tmp.isUsed = true;
		tmp.time = time;
		tmp.addedTime = time;
		for (int i = (int)frameTable.size()-1; i >= 0; i--){
			if (!frameTable.at(i).isUsed){
				frameTable.at(i) = tmp;
				usedNum++;
				return i;
			}
		}
		return -1;
	}
}

FrameTableValue FrameTable::envictFirst(int* envictedFrame){
	if (!isFull()){
		throw "envict a table which is not full";
	}
	int tempMinVal = -1;
	FrameTableValue resultValue;
	int result;
	for (int i =0; i< (int)frameTable.size(); i++){
		if (frameTable.at(i).time < tempMinVal || tempMinVal == -1){
			tempMinVal = frameTable.at(i).time;
			result = i;
		}
	}
	resultValue = frameTable.at(result);
	frameTable.at(result).isUsed = false;
	frameTable.at(result).pageNum = -1;
	frameTable.at(result).procNum = -1;
	frameTable.at(result).time = -1;
	frameTable.at(result).addedTime = -1;
	*envictedFrame = result;
	return resultValue;
}

FrameTableValue FrameTable::envictRandom(int numOfPages, int* envictedFrame, FILE *pRandomFile, bool showRandom){
	FrameTableValue resultValue;
	if (!isFull()){
		throw "envict a table which is not full";
	}
	int tmprandom = this->getRandomNum(pRandomFile, showRandom);
	if (showRandom){
		cout<<"?? uses random number: "<<tmprandom<<"\n";
	}

	int result = tmprandom % numOfPages ;
	resultValue = frameTable.at(result);
	frameTable.at(result).isUsed = false;
	frameTable.at(result).pageNum = -1;
	frameTable.at(result).procNum = -1;
	frameTable.at(result).time = -1;
	frameTable.at(result).addedTime = -1;
	*envictedFrame = result;
	return resultValue;
}

bool FrameTable::updateTime(int procNum, int pageNum, int time){
	for (int i = 0 ; i < (int)frameTable.size(); i++){
		if (frameTable.at(i).procNum == procNum && frameTable.at(i).pageNum == pageNum){
			frameTable.at(i).time = time;
			return true;
		}
	}
	return false;
}

int FrameTable::getRandomNum(FILE *pRandomFile, bool showRandom){
	char tempStr [15];
	if (fscanf(pRandomFile, "%s", tempStr) != EOF) {
		if (showRandom){
			cout<<"?? uses random number: "<<atoi(tempStr)<<"\n";
		}
		return (atoi(tempStr));
	}
	else{
		rewind(pRandomFile);
		return getRandomNum(pRandomFile, showRandom);
	}
}

void FrameTable::setFrameTableValue(int frameNum, FrameTableValue newValue){
	frameTable.at(frameNum) = newValue;
}