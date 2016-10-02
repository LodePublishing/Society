#include <stdlib.h>
#include <stdio.h>

#include "Person.hpp"

void swap(int& a, int& b)
{
	int t = a;
	a = b;
	b = t;
}

#define RUNS 200

int main(int argc, char *argv[])
{
	int POP_SIZE = 30;
	srand(time(NULL));
	Person* p[MAX_POP_SIZE];
	for(int i = 0; i < MAX_POP_SIZE; i++)
		p[i] = new Person();
		
	bool hs[POP_SIZE];
	int run = 0;
	Person::resetVotes();
	while(true)
	{
		run++;
//		printf("Resetting.\n");		
		for(int i = 0; i < POP_SIZE; i++)
			p[i]->reset();
		Person::cooperate = 0;
//		printf("Run");
		for(int k = 0; k < RUNS; k++)
		{
			for(int i = 0; i < POP_SIZE; i++)
				hs[i] = false;
				
			for(int i = 0; i < POP_SIZE/2; i++)
			{
				hs[i] = true;
				int j = i;
				while(hs[j])
				{
					j = rand()%POP_SIZE;
				}
				p[i]->meet(p[j]);
			}
		}
		Person::resetVotes();
		
//		printf("\nSorting.\n");
		for(int l = 0; l < POP_SIZE-1; l++)
			for(int m = 0; m < POP_SIZE-1-l; m++)
				// sort
				if(p[m]->energie < p[m+1]->energie)
				{
					Person* t = p[m];
					p[m] = p[m+1];
					p[m+1] = t;
				}

		int total_energie = 0;
		
		for(int i = 0; i < POP_SIZE; i++)
			total_energie += p[i]->energie;
		
		printf("(Run %4i) {POP_SIZE: %i} [c: %4i], best Energie: %4i [", run, POP_SIZE, Person::cooperate, p[0]->energie);
		
		for(int i = 0; i < (2*SINGLE_EVALUATION_SIZE); i++)
			printf("%i", p[0]->behavior[i]);
		printf("]\n");

//		printf("Creating new generation\n");
/*			while((POP_SIZE >= 10) && (p[POP_SIZE-1]->energie < 0))
			{
				POP_SIZE--;
			}
			int tPOP_SIZE = POP_SIZE;
			for(int i = 0; i < POP_SIZE; i++)
			{
				if((tPOP_SIZE < MAX_POP_SIZE) && (rand()%(i+1) == 0))
				{
					Person::generate(p[i], p[tPOP_SIZE]);
					Person::generate(p[i], p[i]);
					tPOP_SIZE++;
				}
			}
			POP_SIZE = tPOP_SIZE;
		}			*/
		for(int i = 0; i < POP_SIZE / 2; i++)
			Person::generate(p[i], p[POP_SIZE - i]);
		
//		p[POP_SIZE/2]->makeThisForThat();*/
	
//		char a = getchar();
/*
		if(run%10 == 0)
		{
			FILE* f;
			if ((f = fopen("output.txt", "a")) == NULL)
				fprintf(stderr, "Cannot open output.txt\n");
			else
			{
				fprintf(f, "\n-------------------\nEnergie: %i [Avg: %f, Cooperate: %i]\n\nBehavior:\n", p[0]->energie, (double)total_energie / (double)POP_SIZE, Person::cooperate);
				for(int i = 0; i < SINGLE_EVALUATION_SIZE; i++)
					fprintf(f, "  [%i] -> %i\n", i, p[0]->behavior[i]);
				fprintf(f, "\n");
				for(int j = 0; j < 2; j++)
				{
					fprintf(f, "Cooperation %i:\n", j);
					for(int k = 0; k < SINGLE_EVALUATION_SIZE; k++)
						fprintf(f, "  [%i] -> [%i]\n", k, p[0]->evaluation[j+2*k]);
				}
				fclose(f);
			}
		}*/		
	}
	
    system("PAUSE");
    return EXIT_SUCCESS;
}





