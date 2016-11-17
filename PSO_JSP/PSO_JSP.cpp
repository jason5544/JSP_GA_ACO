// PSO_JSP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

using namespace std;

//exchanger
struct Exchanger{
	int i,j;
};

//calculate length of a path
double calc_path_len(const vector<int>& path,const vector<vector<double> >& distance)
{
	double dis=0;
	for (int i=0;i<path.size()-1;i++){
		dis+=distance[path[i]][path[i+1]];
	}
	dis+=distance[path[path.size()-1]][path[0]];
	return dis;
}
//generate a integer sequence from 0 to n-1 at random
void rand_int_order(vector<int>& randorder,const int n)   
{   
	assert(n>1);  

	randorder.clear();
	randorder.resize(n);
	vector<int> recoder;
	for (int i=0;i<n;i++){
		recoder.push_back(i);		
	}

	for(int i=0;i<n;i++){
		int sel=rand()%recoder.size();
		randorder[i]=recoder[sel];
		recoder.erase(recoder.begin()+sel);
	}

	assert(randorder.size()==n);
}

//operation: path add exchanger
void exch_add(vector<int>& path,Exchanger speed)
{
	int t=path[speed.i];
	path[speed.i]=path[speed.j];
	path[speed.j]=t;
}

//operation: path add exchange_sequences
void exch_add(vector<int>& path,const vector<Exchanger>& speed)
{
	for (int i=0;i<speed.size();i++){
		exch_add(path,speed[i]);
	}
}
//operation: exchange_sequences add exchange_sequences
void exch_add(vector<Exchanger>&speed_augend,const vector<Exchanger>& speed_addend,const int len)
{
	for (int i=0;i<speed_addend.size();i++){
		speed_augend.push_back(speed_addend[i]);
	}	
	//if the length of speed is too long,truncate it 
	while(speed_augend.size()>len){
		//delete element at random
		speed_augend.erase(speed_augend.begin()+rand()%len);
	}
}
//multiplication for exchange sequences
vector<Exchanger> exch_multiply(const vector<Exchanger>&speed,const double accelerator)
{
	vector<Exchanger> exch_v;
	//if accelerator is bigger than 1,copy (int)accelerator times of initial speed to new speed
	int dup=(int)accelerator;
	exch_v.resize(dup*speed.size());
	for (int i=0;i<dup;i++){
		copy(speed.begin(),speed.end(),exch_v.begin()+i*speed.size());
	}

	//	
	int copyLen=(int)((accelerator-dup)*speed.size());
	for (int i=0;i<copyLen;i++){
		exch_v.push_back(speed[i]);
	}

	//if speed length is too long ,truncate it
	//////////////////////////////////////////////////////////////////////////

	return exch_v;
}
//subtraction for exchange sequences
vector<Exchanger> exch_sub(vector<int>&path_minuend,vector<int>&path_subtrahend)
{
	vector<int> path_subtra=path_subtrahend;

	vector<Exchanger> exch_sub;	
	for (int s=0;s<path_minuend.size();s++){
		if (path_minuend[s]!=path_subtrahend[s]){
			Exchanger ex;
			ex.i=find(path_subtra.begin()+s,path_subtra.end(),path_minuend[s])-path_subtra.begin();
			ex.j=s;
			exch_sub.push_back(ex);
			exch_add(path_subtra,ex);
		}
	}
	return exch_sub;
}
//compress speed(exchange sequence),i.e. get the equivalent exchange_sequence
//this function is not neccessory,but it might be more high-efficiency
void exch_compress(vector<Exchanger>& exch_sequence)
{

}

int main()
{
	srand((unsigned)time(NULL));

	//read data from file
	fstream filein("data/berlin52.tsp.processed",ios::in);
	if (filein==NULL){
		cout<<"cannot open data file"<<endl;
		return 1;
	}
	fstream fileout("data/result.txt",ios::out);
	if (fileout==NULL){
		cout<<"cannot open a file to write data"<<endl;
		return 1;
	}
	//append the best result found in this test to a file named "best.txt"
	fstream bestout("data/best.txt",ios::app);
	if (bestout==NULL){
		cout<<"cannot open a file to write data"<<endl;
		return 1;
	}	

	//number of cities
	unsigned int cityNum;
	filein>>cityNum;
	cout<<cityNum<<endl;	

	//standard path
	int* standardPath;
	standardPath=new int[cityNum];
	for (int i=0;i<cityNum;i++){
		filein>>standardPath[i];
	}

	//distance between cities
	vector<vector<double> > distance;
	for (int i=0;i<cityNum;i++){		
		vector<double> v;
		for (int j=0;j<cityNum;j++){
			double d;
			filein>>d;
			v.push_back(d);
		}
		distance.push_back(v);
	}

	//////////////////////////////////////////////////////////////////////////
	//parameters
	//partical number
	int partical_num=100;
	//partical swarm
	vector<vector<int> > swarm(partical_num);
	//accelerator
	double alpha=2;//if it equals to 1,accelerators will make no sense 
	double beta=2;
	//inertia
	double inertia=1;
	//speed length
	int speed_len=cityNum/4;

	//global best partical
	vector<int> best_global(partical_num);
	//length of global best partical
	double best_global_len;
	//partial best particals
	vector<vector<int> > best_partial;
	//length of partial best particals
	vector<double> best_partial_len(partical_num);
	//speed of every partical
	vector<vector<Exchanger> >speed(partical_num);

	//initilization
	//particals
	for (int i=0;i<partical_num;i++){
		rand_int_order(swarm[i],cityNum);
	}
	//initialize best partical length
	int l=1<<(8*sizeof(int)-2);
	best_global_len=l;
	for (int i=0;i<partical_num;i++){
		best_partial_len[i]=l;
	}
	//best partial particals
	best_partial=swarm;
	//initialize speed
	for (int i=0;i<partical_num;i++){
		int init_speed_len=rand()%speed_len;
		for (int j=0;j<init_speed_len;j++){
			Exchanger ex;
			ex.i=rand()%cityNum;
			ex.j=ex.i;
			while(ex.i=ex.j){
				ex.j=rand()%cityNum;
			}
			speed[i].push_back(ex);
		}
	}

	//generation 
	int generation=3000;
	for (int g=0;g<generation;g++){
		//update speed and best particals and length of these particals
		double length;
		//find best of last generation
		int best_index_this=0;
		double best_len_this=1<<(8*sizeof(int)-2);
		for (int i=0;i<partical_num;i++){
			length=calc_path_len(swarm[i],distance);
			if (length<best_global_len){
				//update global best partical
				best_global=swarm[i];
				best_global_len=length;

				best_partial[i]=swarm[i];
				best_partial_len[i]=length;
			}else if (length<best_partial_len[i]){
				//update partical best particals
				best_partial[i]=swarm[i];
				best_partial_len[i]=length;
			}
			if (length<best_len_this){
				best_len_this=length;
				best_index_this=i;
			}
		}		

		fileout<<"generation "<<g<<" "<<endl
			<<"best path found in this generation:"<<endl;
		for (int i=0;i<cityNum;i++){
			fileout<<swarm[best_index_this][i]<<" ";
		}
		fileout<<endl;
		fileout<<"best path length in this generation  : "<<best_len_this<<endl;
		fileout<<"best path length found for the moment: "<<best_global_len<<endl;
		fileout<<endl;

		cout<<"generation :"<<g<<endl
			<<"length in this generation  :"<<best_len_this<<endl
			<<"length found for the moment:"<<best_global_len<<endl;
		//shorten exchange sequences,i.e. speed
		for (int i=0;i<partical_num;i++){
			exch_compress(speed[i]);
		}

		for (int i=0;i<partical_num;i++){
			//update speed
			exch_add(exch_multiply(speed[i],inertia),exch_multiply(exch_sub(best_partial[i],swarm[i]),alpha*rand()/RAND_MAX),speed_len);
			exch_add(speed[i],exch_multiply(exch_sub(best_global,swarm[i]),beta*rand()/RAND_MAX),speed_len);
			//update particals
			exch_add(swarm[i],exch_multiply(speed[i],inertia));
		}


	}
	//write best result found into a file 
	bestout<<best_global_len<<endl;
	for (int i=0;i<cityNum;i++){
		bestout<<best_global[i]<<" ";
	}
	bestout<<endl;
	//record time
	time_t currenttime;
	time(&currenttime);
	bestout<<ctime(&currenttime);		


	//show standard path and distance
	fileout<<"standard path:"<<endl;
	double standardbestlen=0;
	for (int i=0;i<cityNum;i++){
		fileout<<standardPath[i]<<" ";		
	}
	for (int i=0;i<cityNum-1;i++){
		standardbestlen+=distance[standardPath[i]][standardPath[i+1]];
	}
	standardbestlen+=distance[standardPath[cityNum-1]][standardPath[0]];
	fileout<<endl<<"length of standard path: "<<endl<<"	"<<standardbestlen<<endl;

	//free memory/////////////////////////////////////////////////////////////	
	filein.close();
	fileout.close();
	bestout.close();

	delete[] standardPath;	

	return 0;
}
