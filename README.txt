/*****************************************************************************
README
Lab4-DONG-Jinglun
*****************************************************************************/

UNZIP:
unzip lab4-dong-jinglun.zip

CHANGE DIRECTORY:
cd ..

COMPILE:
g++ lab4-jinglun-dong.cpp Pager.cpp Proc.cpp FrameTable.cpp -o lab4-dong-jinglun.exe

RUN:
THE FOLLOWING COMMANDS ARE VALID:
./lab4-dong-jinglun.exe 20 10 10 3 10 lru 0		//Normal
./lab4-dong-jinglun.exe 20 10 10 3 10 lru 1		//Debugging
./lab4-dong-jinglun.exe 20 10 10 3 10 lru 0		//ShowRandom

//Please put random file into the same directory. (Did not test other cases)