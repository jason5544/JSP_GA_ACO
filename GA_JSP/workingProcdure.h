#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#pragma once
class ProcedureNode
{
public:
	int device;
	int time;
};

class WorkingProcedure
{
public:
	int deviceNum;
	int workpieceNum;
	vector< vector<ProcedureNode> > table;

	static void readProcedure(char *path, WorkingProcedure &workingProcedure)
	{
		ifstream fileInput(path);
		if (!fileInput)
		{
			cerr << "Error in read " << path << endl;
			exit(0);
		}

		fileInput >> workingProcedure.workpieceNum >> workingProcedure.deviceNum;
		//cout << "workingProcedure.workpieceNum = " << workingProcedure.workpieceNum << endl;
		//cout << "workingProcedure.deviceNum = " << workingProcedure.deviceNum << endl;
		workingProcedure.table.resize(workingProcedure.workpieceNum);
		for (int i = 0; i < workingProcedure.workpieceNum; i++)
		{
			workingProcedure.table[i].resize(workingProcedure.deviceNum);
		}

		for (int i = 0; i < workingProcedure.workpieceNum; i++)
		{
			for (int j = 0; j < workingProcedure.deviceNum; j++)
			{
				fileInput >> workingProcedure.table[i][j].device;
				workingProcedure.table[i][j].device--;
			}
		}
	}

	static void readManHour(char *path, WorkingProcedure &workingProcedure)
	{
		ifstream fileInput(path);
		if (!fileInput)
		{
			cerr << "Error in read " << path << endl;
			exit(0);
		}
		for (int i = 0; i < workingProcedure.workpieceNum; i++)
		{
			for (int j = 0; j < workingProcedure.deviceNum; j ++)
			{
				fileInput >> workingProcedure.table[i][j].time;
			}
		}
	}
};

