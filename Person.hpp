#ifndef _PERSON_HPP
#define _PERSON_HPP

#include <list>

#define ACTION_SIZE 2
#define SINGLE_EVALUATION_SIZE 4

#define EVALUATION_SIZE (ACTION_SIZE * SINGLE_EVALUATION_SIZE)
#define EVALUATION_EXCHANGE_SIZE (SINGLE_EVALUATION_SIZE * SINGLE_EVALUATION_SIZE)

#define MAX_POP_SIZE 100

struct Bewertung
{
	int bewertung;
	int id;
};

class Person
{
	public:
		Person();
		~Person();
		
		static int votes[MAX_POP_SIZE];
		static int votes2[MAX_POP_SIZE];
		static void resetVotes();
		
		void reset();

		std::list<Bewertung*> bewertung;
		
		int behavior[SINGLE_EVALUATION_SIZE*4];
		int evaluation[EVALUATION_SIZE];
		int evaluationExchange[EVALUATION_EXCHANGE_SIZE];
		
		int energie;
		int id;
		static int global_id;
		static int cooperate;
		Bewertung* getEntry(Person* other_person);
		void meet(Person* other_person);
		
		bool getAction(int my_behavior);
		bool getVote(int my_behavior);
		
		int getNewEvaluation(int my_behavior);
		bool getAllowExchange(int my_behavior);
		
		int getNewEvaluationAfterExchange(int my_evaluation);

		static int getBehaviorCode(int my_evaluation, bool is_negative_score, bool votes);
		static int getEvaluationAfterExchangeCode(int other_evaluation, int my_evaluation);
		static int getEvaluationCode(bool other_action, int my_evaluation);
		static void calculateEnergieChange(bool my_action, bool other_action, int& my_energie, int& other_energie);
		static void calculateEnergieChange2(bool my_action, bool other_action, int& my_energie, int& other_energie);
		
		void exchangeInformation(Person* other_person);
		
		static void generate(Person* parent, Person* child);
		
		void makeThisForThat();
		void makeAlwaysCooperate();
		void makeAlwaysNotCooperate();
		void makeRandom();
};

#endif
