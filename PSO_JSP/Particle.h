#pragma once
#include <stdlib.h>

class Particle
{
public:
	static int particleCount;
	static int bitCount;
	int *particleSequence;
	int *gbest;					//ȫ�����Ž�
	int *pbest;					//�ֲ����Ž�
	
	static void init(Particle &particle)
	{
		for(int i = 0; i < Particle::bitCount; i++)
		{	
			particle.particleSequence[i] = rand()%Particle::bitCount;
			for (int j = 0; j < i; j++)
			{
				if (particle.particleSequence[i] == particle.particleSequence[j])
				{
					i--;
					break;
				}
			}
		}
	}


};

