#include "Proc.h"
#include "Pager.h"

Proc::Proc(void)
{
}

Proc::Proc(int processSize, int jobMix, int numRef, int procNum, int pageSize){
	this-> processSize = processSize;
	this-> jobMix = jobMix;
	this-> numRef = numRef;
	this-> pageSize = pageSize;
	this-> curNumRef = 1;
	this->numEnvict = 0;
	this->residencySum = 0;
	this->faultCount = 0;
	curWord = 111 * procNum % processSize;
	finishFlag = false;
}

Proc::~Proc(void)
{
}

int Proc::getRandomNum(FILE *pRandomFile, bool showRandom){
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

void Proc::goToNextRef(int procNum, FILE *pRandomFile, bool showRandom){
	int randomNum = getRandomNum(pRandomFile, showRandom);
	int randomNum2;
	double y = randomNum / (2147483648 + 1.0);
	switch (jobMix){
	case 1:
		curWord += 1;
		break;
	case 2:
		curWord += 1;
		break;
	case 3:
		randomNum2 = getRandomNum(pRandomFile, showRandom);
		curWord = randomNum2;
		break;
	case 4:
		switch (procNum){
		case 0:
			if (y<0.75){
				curWord += 1;
			}
			else{
				curWord -= 5;
			}
			break;
		case 1:
			if ( y <0.75){
				curWord += 1;
			}
			else{
				curWord += 4;
			}
			break;
		case 2:
			if (y < 0.75){
				curWord +=1;
			}
			else{
				if (y <0.875){
					curWord -=5;
				}
				else{
					curWord +=4;
				}
			}
			break;
		case 3:
			if (y < 0.5){
				curWord += 1;
			}
			else{
				if (y < 0.625){
					curWord -= 5;
				}
				else{
					if (y < 0.75){
						curWord += 4;
					}
					else{
						randomNum2 = getRandomNum(pRandomFile, showRandom);
						curWord = randomNum2;
					}
				}
			}
			break;
		}
	}
	curWord = (curWord + processSize)%processSize;
	curNumRef ++;
	if (curNumRef>numRef){
		finishFlag = true;
	}
}

void Proc::summarizeTail(int procNum){
	if (numEnvict != 0){
		cout<<"Process "<<procNum+1<<" had "<<this->faultCount<<" faults and "<<(double)residencySum/numEnvict<<" average residency.\n";
	}
	else{
		cout<<"Process "<<procNum+1<<" had "<<this->faultCount<<" faults\n";
		cout<<"     With no evictions, the average residence is undefined.\n";
	}
}