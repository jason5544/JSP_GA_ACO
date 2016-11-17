#include "StdAfx.h"
#include "Gene.h"
#include <algorithm>
#include <ctime>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string.h>


const double PI = 3.1415926;
using namespace std;

int Gene::num; 
int Gene::bitNum;

double Gene::minFitness = 9999;
vector<int> Gene::minProcdure;

Gene::Gene(void)
{
	geneSequence = new int[Gene::bitNum];
}


Gene::~Gene(void)
{
	delete[] geneSequence;
}

void Gene::init()
{
	for(int i = 0; i < Gene::bitNum; i++)
	{	
		geneSequence[i] = rand()%Gene::bitNum;
		for (int j = 0; j < i; j++)
		{
			if (geneSequence[i] == geneSequence[j])
			{
				i--;
				break;
			}
		}
	}
}

void Gene::swapGene(const Gene &gene0, const Gene &gene1, Gene &gene2, Gene &gene3)
{
	int pos = rand()%Gene::bitNum;
	for (int i = 0; i < pos; i++ )
	{
		gene2.geneSequence[i] = gene0.geneSequence[i];
		gene3.geneSequence[i] = gene1.geneSequence[i];
	}

	for (int i = pos; i < Gene::bitNum; i++ )
	{
		gene2.geneSequence[i] = gene1.geneSequence[i];
		gene3.geneSequence[i] = gene0.geneSequence[i];
	}
}

void Gene::variation()
{
	int pos = rand()%Gene::bitNum;
	geneSequence[pos] = rand()%(Gene::bitNum - pos);
}

void Gene::fitnessFunction(WorkingProcedure &workingProcedure)
{
	//cout << "-------------- Gene::bitNum ------------------" << endl;
	//for (int i = 0; i < Gene::bitNum; i++)
	//{
	//	cout << geneSequence[i] << " ";
	//}
	//cout << endl;
	//cout << "-------------- Gene::bitNum ------------------" << endl;
	fitness = 0;
	int *deviceUpbond = new int[workingProcedure.deviceNum]();
	int *workpieceLowbond = new int[workingProcedure.workpieceNum]();
	int *workpieceOrder = new int[workingProcedure.workpieceNum];
	memset(workpieceOrder, -1, workingProcedure.workpieceNum*sizeof(int));
	for (int i = 0; i < Gene::bitNum; i++)
	{
		int workpiece = geneSequence[i]/workingProcedure.deviceNum;
		//cout << "workpiece = " << workpiece << endl;
		workpieceOrder[workpiece]++;
		//cout << "workpieceOrder[workpiece] = " << workpieceOrder[workpiece] << endl;
		int usedTime = workingProcedure.table[workpiece][ workpieceOrder[workpiece] ].time;
		int usedDevice = workingProcedure.table[workpiece][ workpieceOrder[workpiece] ].device;
		int maxTime = max(deviceUpbond[usedDevice], workpieceLowbond[workpiece]);
		deviceUpbond[usedDevice] = maxTime + usedTime;
		workpieceLowbond[workpiece] = maxTime + usedTime;
		//cout << "i = " << i << endl;
	}
	for (int i = 0; i < workingProcedure.deviceNum; i++)
	{
		if(deviceUpbond[i] > fitness)
		{
			fitness = deviceUpbond[i];
		}
		//cout << deviceUpbond[i] << " ";
	}
	//cout << endl;
	delete[] deviceUpbond;
	delete[] workpieceLowbond;
	delete[] workpieceOrder;
	//cout << "fitness = " << fitness << endl;
}

void Gene::randSwap(Gene *&gene)
{

	double sumFitness = 0;
	//Gene::minFitness = 9999;
	for (int i = 0 ; i < Gene::num; i++)
	{
		sumFitness += gene[i].fitness;

	}
	
	double sumPosibility = 0;
	for (int i = 0; i < Gene::num; i++)
	{
		gene[i].posibility =1 - (double)gene[i].fitness/(double)sumFitness;
		sumPosibility += gene[i].posibility;
	}

	for (int i = 0; i < Gene::num; i++)
	{
		gene[i].posibility = (double)gene[i].posibility/(double)sumPosibility;
	}

	Gene *newGene = new Gene[Gene::num];


	for (int i = 0; i < Gene::num; i = i + 2)
	{
		int order0, order1;
		
		Gene::randSelect(gene , order0, order1);		
		Gene::swapGene(gene[order0], gene[order1], newGene[i], newGene[i + 1]);
	}


	delete[] gene;
	gene = newGene;
}

void Gene::randSelect(Gene *gene, int &order0 ,int &order1)
{
	double randNum0, randNum1;
	get_rand(randNum0, randNum1);

	//cout << "randNum0 = " << randNum0 << " " << "randNum1 = " << randNum1 << endl;
	for (int i = 0; i < Gene::num; i++)
	{
		if (randNum0 < gene[i].posibility)
		{
			order0 = i;
			break;
		}
		else
		{
			randNum0 = randNum0 - gene[i].posibility;
		}
	}

	for (int i = 0; i <Gene::num; i++)
	{
		if (randNum1 < gene[i].posibility)
		{
			order1 = i;
			break;
		}
		else
		{
			randNum1 = randNum1 - gene[i].posibility;
		}
	}

	//cout << "oder0 = " << order0 << " " << "order1 = " << order1 << endl; 
}

void Gene::get_rand(double &x, double &y)
{
	int N = 999;
	x = rand()%(N+1)/(float)(N+1);         //生成0-1间的随机数
	y = rand()%(N+1)/(float)(N+1);         //生成0-1间的随机数
}