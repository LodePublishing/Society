#include "Person.hpp"
#include <stdio.h>

Person::Person():
	energie(0),
	id(global_id)
{
	global_id++;
	makeThisForThat();
}

void Person::makeRandom()
{
	for(int i = 0; i < SINGLE_EVALUATION_SIZE*4; i++)
		behavior[i] = rand()%4;
	for(int i = 0; i < EVALUATION_SIZE; i++)
		evaluation[i] = rand()%(SINGLE_EVALUATION_SIZE*2);
	for(int i = 0; i < EVALUATION_EXCHANGE_SIZE; i++)
		evaluationExchange[i] = rand()%SINGLE_EVALUATION_SIZE;
}

void Person::makeThisForThat()
{
	behavior[0] = 1;
	behavior[1] = 0;
	for(int i = 2; i < (4*SINGLE_EVALUATION_SIZE); i++)
		behavior[i] = 1;
		
	for(int j = 0; j < SINGLE_EVALUATION_SIZE; j++)
	{
		evaluation[j * 2] = 1 + SINGLE_EVALUATION_SIZE;
		evaluation[1 + j * 2] = SINGLE_EVALUATION_SIZE;
	}
		
	for(int i = 0; i < SINGLE_EVALUATION_SIZE; i++)
		for(int j = 0; j < SINGLE_EVALUATION_SIZE; j++)
			evaluationExchange[i + j * SINGLE_EVALUATION_SIZE] = j;
}

void Person::makeAlwaysCooperate()
{
	for(int i = 0; i < 4*SINGLE_EVALUATION_SIZE; i++)
		behavior[i] = 1;
	for(int i = 0; i < EVALUATION_SIZE; i++)
		evaluation[i] = SINGLE_EVALUATION_SIZE;
	for(int i = 0; i < EVALUATION_EXCHANGE_SIZE; i++)
		evaluationExchange[i] = 0;
}

void Person::makeAlwaysNotCooperate()
{
	for(int i = 0; i < 4*SINGLE_EVALUATION_SIZE; i++)
		behavior[i] = 0;
	for(int i = 0; i < EVALUATION_SIZE; i++)
		evaluation[i] = 0;
	for(int i = 0; i < EVALUATION_EXCHANGE_SIZE; i++)
		evaluationExchange[i] = 0;
}

int Person::getBehaviorCode(int my_evaluation, bool is_negative_score, bool is_votes)
{
	if(!is_negative_score)
	{
		if(!is_votes)
			return (my_evaluation);
		else return (my_evaluation + 2*SINGLE_EVALUATION_SIZE);
	}
	else 
	{
		if(!is_votes)
			return (my_evaluation + SINGLE_EVALUATION_SIZE);
		else return (my_evaluation + 3*SINGLE_EVALUATION_SIZE);
	}
}

bool Person::getAction(int my_behavior)
{
	return(behavior[my_behavior] == 1);
}

bool Person::getVote(int my_behavior)
{
	return(behavior[my_behavior] == 2);	
}

int Person::getNewEvaluation(int my_behavior)
{
	return evaluation[my_behavior]%SINGLE_EVALUATION_SIZE;
}

bool Person::getAllowExchange(int my_behavior)
{
	if(evaluation[my_behavior]/SINGLE_EVALUATION_SIZE == 1)
		return true;
	else false;
}

int Person::getNewEvaluationAfterExchange(int my_evaluation)
{
	return evaluationExchange[my_evaluation];
}

int Person::getEvaluationAfterExchangeCode(int other_evaluation, int my_evaluation)
{
	int code = other_evaluation;
	code += my_evaluation * SINGLE_EVALUATION_SIZE;
	return code;
}

int Person::getEvaluationCode(bool other_action, int my_evaluation)
{
	int code = 0;
	if(other_action)
		code += 1;
	code += my_evaluation * 2;
	return code;
}


Person::~Person()
{
	reset();
}

void Person::reset()
{
	for(std::list<Bewertung*>::iterator i = bewertung.begin(); i != bewertung.end(); i++)
		delete (*i);
	bewertung.clear();
	energie = 0;
}

Bewertung* Person::getEntry(Person* other_person)
{
	for(std::list<Bewertung*>::iterator i = bewertung.begin(); i != bewertung.end(); i++)
	{
		if((*i)->id == other_person->id)
			return (*i);
	}

	Bewertung* entry = new Bewertung();
	entry->id = other_person->id;
	entry->bewertung = 0;
	bewertung.push_back(entry);
	return entry;
}


void Person::calculateEnergieChange(bool my_action, bool other_action, int& my_energie, int& other_energie)
{
	if(my_action)
	{
		cooperate++;		
		if(other_action)
		{
			cooperate++;
			my_energie+=1;
			other_energie+=1;
		} else
		{
			my_energie -= 2;
			other_energie += 3;	
		}
	} else
	{
		if(other_action)
		{
			cooperate++;
			my_energie+=3;
			other_energie -= 2;
		} else
		{
			my_energie -= 1;
			other_energie -= 1;
		}
	}
}

void Person::resetVotes()
{
	for(int i = 0; i < MAX_POP_SIZE; i++)
	{
		votes[i] = votes2[i];
		votes2[i] = 0;
	}
}

void Person::exchangeInformation(Person* other_person)
{
	for(std::list<Bewertung*>::iterator j = other_person->bewertung.begin(); j != other_person->bewertung.end(); j++)
	{
		bool known = false;
		int my_evaluation = 0;
		int other_evaluation = (*j)->bewertung;
		
		Bewertung* my_entry = NULL;
		for(std::list<Bewertung*>::iterator i = bewertung.begin(); i != bewertung.end(); i++)
			if((*i)->id == (*j)->id)
			{
				my_entry = (*i);
				my_evaluation = my_entry->bewertung;
				known = true;
				break;	
			}
		int code = getEvaluationAfterExchangeCode(other_evaluation, my_evaluation);
		int new_evaluation = getNewEvaluationAfterExchange(code);
		if(!known)
		{
			my_entry = new Bewertung();
			my_entry->id = (*j)->id;
			bewertung.push_back(my_entry);
		}
		my_entry->bewertung = new_evaluation;
	}
}

void Person::meet(Person* other_person)
{
	Bewertung* entry1;
	Bewertung* entry2;
	
	entry1 = getEntry(other_person); 
	entry2 = other_person->getEntry(this); // we need two first_encounters, because one person might know the other person by gossip
	
	int my_behavior_code = getBehaviorCode(entry1->bewertung, energie < 0, Person::votes[other_person->id] > MAX_POP_SIZE/2);
	int other_behavior_code = getBehaviorCode(entry2->bewertung, other_person->energie < 0, Person::votes[other_person->id] > MAX_POP_SIZE/2);
	
	int my_action = getAction(my_behavior_code);
	int other_action = other_person->getAction(other_behavior_code);
	
	if(getVote(my_behavior_code))
		Person::votes2[other_person->id]++;
	if(other_person->getVote(other_behavior_code))
		Person::votes2[id]++;

	int my_evaluation_code = getEvaluationCode(other_action, entry1->bewertung);
	int other_evaluation_code = getEvaluationCode(my_action, entry2->bewertung);
	
	entry1->bewertung = getNewEvaluation(my_evaluation_code);
	entry2->bewertung = other_person->getNewEvaluation(other_evaluation_code);

	calculateEnergieChange(my_action, other_action, energie, other_person->energie);
	
//	TODO globales 'Verbrecherverzeichnis'?

	if(other_person->getAllowExchange(other_evaluation_code))
		exchangeInformation(other_person);
	if(getAllowExchange(my_evaluation_code))
		other_person->exchangeInformation(this);
}

void Person::generate(Person* parent, Person* child)
{
	for(int i = 0; i < 4*SINGLE_EVALUATION_SIZE; i++)
	{
		if(rand()%(4*SINGLE_EVALUATION_SIZE) == 0)
		{
			child->behavior[i] = rand()%4;
//			parent->behavior[i] = rand()%2;
		}
		else child->behavior[i] = parent->behavior[i];
	}
	
	for(int i = 0; i < EVALUATION_SIZE; i++)
	{
		if(rand()%EVALUATION_SIZE == 0)
		{
			child->evaluation[i] = rand()%(2*SINGLE_EVALUATION_SIZE);
//			parent->evaluation[i] = rand()%8;
		}
		else child->evaluation[i] = parent->evaluation[i];
	}
	
	for(int i = 0; i < EVALUATION_EXCHANGE_SIZE; i++)
	{
		if(rand()%EVALUATION_EXCHANGE_SIZE == 0)
		{
			child->evaluationExchange[i] = rand()%SINGLE_EVALUATION_SIZE;
		//	parent->evaluationExchange[i] = rand()%16;
		}
		else child->evaluationExchange[i] = parent->evaluationExchange[i];
	}
}

int Person::global_id = 0;
int Person::cooperate = 0;
int Person::votes[MAX_POP_SIZE];
int Person::votes2[MAX_POP_SIZE];
