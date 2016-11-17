// GA_JSP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Gene.h"
#include "Grefenstette.h"
#include "workingProcdure.h"
#include <ctime>
#include <cstdlib>

using namespace std;

const int iterTimes = 1000;
int _tmain(int argc, _TCHAR* argv[])
{
	srand(static_cast<unsigned short>(time(0)));
	WorkingProcedure workingProcedure;
	WorkingProcedure::readProcedure("./working_procedure.txt", workingProcedure);
	WorkingProcedure::readManHour("./man_hour.txt", workingProcedure);
	Gene::num = 8;
	Gene::bitNum = workingProcedure.workpieceNum*workingProcedure.deviceNum;

	Gene *geneArray = new Gene[Gene::num];
	for (int i = 0; i < Gene::num; i++)
	{
		geneArray[i].init();
	}

	for (int i = 0; i < iterTimes; i++)
	{	
		for (int j = 0; j < Gene::num; j++)
		{
			geneArray[j].fitnessFunction(workingProcedure);
			if (geneArray[j].fitness < Gene::minFitness)
			{
				Gene::minFitness = geneArray[j].fitness;
				Gene::minProcdure.clear();
				for (int k = 0; k < Gene::bitNum; k++)
				{
					Gene::minProcdure.push_back(geneArray[j].geneSequence[k]);
				}
			}

			Grefenstette::coding(geneArray[j].geneSequence, Gene::bitNum);
		}

		Gene::randSwap(geneArray);

		for (int j = 0; j < Gene::num; j++)
		{
			geneArray[j].variation();
		}

		for (int j = 0; j < Gene::num; j++)
		{
			Grefenstette::Decoding(geneArray[j].geneSequence, Gene::bitNum);
		}
		cout <<  "Gene::minFitness = " << Gene::minFitness  << endl;
		//for (int j = 0; j < Gene::num; j++)
		//{
		//	for (int k = 0; k < Gene::bitNum; k++)
		//	{
		//		cout << geneArray[j].geneSequence[k] << " ";
		//	}
		//	cout << endl;
		//}
	}

	// 输出最优解
	cout << endl;
	cout << "迭代 " << iterTimes << "次的最小fitness = " << Gene::minFitness << endl;
	cout << "最优工序方案为：" << endl;
	for (int i = 0; i < Gene::bitNum; i++)
	{
		int workpiece = Gene::minProcdure[i]/workingProcedure.deviceNum;
		cout << workpiece << " " ;
	}
	cout <<endl;



	return 0;
}


