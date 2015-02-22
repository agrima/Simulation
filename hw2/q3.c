#include <stdio.h>
#include <stdlib.h>

#define POPULATION 30	/* POPULATION size */
#define LENGTH 12	/* geneotype length */
#define END 3000	/* number of tournaments */
#define GIFTCARD 100	/* gitf card amount */
#define MAXWEIGHT 25 /* airline weight limit */
#define REC 0.5 /* recombination rate */
#define MUT 0.1 /* mutation rate */

/* cost and weight of LENGTH items */
int cost[]={43, 11, 18, 15, 9, 13, 73, 46, 42, 32, 27, 24};
int weight[] = {22, 10, 23, 16, 8, 6, 12, 22, 34, 12, 21, 34};

int gene[POPULATION][LENGTH];
int i,j;

/* calculate the sum cost of nth member */
int sumcost(int n)
{
	int sum = 0;
	for(i=0; i<LENGTH; i++)
	{
		sum += gene[n][i] * cost[i];
	}
	return sum;
}

/* calculate the sum weight of nth member */
int sumweight(int n)
{
	int sum = 0;
	for(i=0; i<LENGTH; i++)
	{
		sum += gene[n][i] * weight[i];
	}
	return sum;
}

/* check if the sum cost exceeds the gift card amount */
int overcost(int n)
{
	if(sumcost(n) > GIFTCARD)
		return 1;
	else
		return 0;
}

/* check if the sum weight exceeds the airline weight limit */
int overweight(int n)
{
	if(sumweight(n) > MAXWEIGHT)
		return 1;
	else
		return 0;
}

/* initialize the POPULATION */
void init_pop(void)
{
	int s,t;
	for(s=0; s<POPULATION; s++)
	{
		do
		{
			for(t=0; t<LENGTH; t++)
			{
				if((rand()/(RAND_MAX+1.0)) < 0.5)
				{
					gene[s][t] = 0;
				}
				else
				{
					gene[s][t] = 1;
				}
			}
		} while(overcost(s) || overweight(s));
	}
}

/* display the nth member as the final winner */
void display(int n)
{
	int sumcost = 0;
	int sumweight = 0;
	int z;
	printf("You should buy the following items:\r\n");
	printf("-----------------------------------\r\n");
	for(z=0; z<LENGTH; z++)
	{
		if(gene[n][z])
		{
			printf("Item No.%d	cost: %d	weight: %d\r\n", z+1, cost[z], weight[z]);
			sumcost += cost[z];
			sumweight += weight[z];
		}
	}
	printf("-----------------------------------\r\n");
	printf("Your sum purchase costs %d dollars, weights %d kg.\r\n", sumcost, sumweight);
}

/* Main process */
int main(void)
{
	int a, b, winner, loser, tour, finalwinner, k;

	/* initialize the POPULATION */
	init_pop();

	/* start a tournament, with sum number of END tournaments */
	for(tour = 0; tour < END; tour ++)
	{
		/* randomly choose two members */
		a = (int)(POPULATION * (rand()/(RAND_MAX+1.0)));
		b = (int)(POPULATION * (rand()/(RAND_MAX+1.0)));
		
		/* the member with higher cost wins, the other loses */
		if (sumcost(a) < sumcost(b))
		{
			winner = b;
			loser = a;
		}
		else
		{
			winner = a;
			loser = b;
		}

		/* recombine and mutates the loser at certain probability */
		do
		{
			for(k=0; k<LENGTH;k++)
			{
				if((rand()/(RAND_MAX+1.0)) < REC)
					gene[loser][k] = gene[winner][k];
				if ((rand()/(RAND_MAX+1.0)) < MUT)
					gene[loser][k] = 1 - gene[loser][k];
			}
		} while (overcost(loser) || overweight(loser));
	}

	/* After the algorithm is done, calculate the final winner with highest cost among all members */
	finalwinner = 0;
	for (k=1; k<POPULATION; k++)
	{
		if(sumcost(k) > sumcost(finalwinner))
		{
			finalwinner = k;
		}
	}
  display(finalwinner);
  getchar();
  return 0;
}
