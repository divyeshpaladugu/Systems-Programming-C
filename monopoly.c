#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//There will be m players in an array
typedef struct Player 
{
	int id;
	int loc;
	long balance;
} TPlayer;

//There will be n properties in an array
typedef struct Property
{
	int id;
	int owner_id;
	int rent;
} TProperty;


//TODO
//Implement the following function
//The player p1 needs to pay the player p2 'amount' of dollars
//If p1 has enough balance, the transaction will be successful
//Otherwise, p1 will pay all of their balance to p2, and the transaction
//is not successful 
//return 1 if the transaction is successful
//Otherwise, return 0
int transaction(TPlayer *p1, TPlayer *p2, int amount)
{
	//check if p1 balance is greater than or equal to the amount
	if (p1->balance >= amount)
	{
		// if it is, then take it from p1s bal and add it to p2s, bascically making p1 pay p2
		p1->balance -= amount;
		p2->balance += amount;
		return 1;
	}
	else //if p1 doesnt have suffient bal
	{
		// makes p1 pay p2 everything they have
		int amount_to_pay = p1->balance;
		p1->balance = 0;
		p2->balance += amount_to_pay;
		return 0;
	}
}

//TODO
//Finish the following function
//If one player cannot pay rent to another player, this function should return 0. 
//The rest of the players will not get chances to play when this happens 

int one_round(int m, int n, TPlayer p[], TProperty prop[])
{
	for(int i = 0; i < m; i++)
	{
		int steps = rand() % 6 + 1 + rand() % 6 + 1;
		//fill in the code below
		
		// old location is the location the player is currently on
		// p[i] can let us go thru every player, p[1], p[2], etc...
		int old_loc = p[i].loc;
		//updates players location and can wrap around using % n
		int new_loc = (p[i].loc + steps) % n;
		// location of player is new location
		p[i].loc = new_loc;

		if (new_loc <= old_loc)
		{
			p[i].balance += n;
		}
		// pointer to the property a player landed on
		TProperty *property_player_is_on = prop + p[i].loc;
		// if no one owns it then they buy it
		if (property_player_is_on->owner_id == -1) {
		// owner id of property is player id
			property_player_is_on->owner_id = p[i].id;
		}
		// if its owned by someone else they have to pay them
		else if (property_player_is_on->owner_id != p[i].id) {
			// pointer to owner
			TPlayer *owner = &p[property_player_is_on->owner_id];
			int works = transaction(&p[i], owner, property_player_is_on->rent);
			
			if (works == 0) 
			{
				return 0;
			}
		}
	}
	return 1;
}

//used for printing out results
void print_result(int m, TPlayer p[])
{
	printf("      id    balance\n");
	for(int i = 0; i < m; i++)
	{
		printf("%8d %8ld\n", p[i].id, p[i].balance);
	}
	long sum = 0;
	long max = 0;
	for(int i = 0; i < m; i++)
	{
		sum += p[i].balance;
		if(p[i].balance > max) max = p[i].balance;
	}
	printf("average: %f max: %ld, max/average = %lf\n", (double)sum/m, max, (double)max*m/sum); 
}

//max_rounds is needed because the game may never finish
void monopoly(int m, int n, TPlayer p[], TProperty prop[], int max_rounds)
{
	srand(12345);
	int rounds = 1;
	while(one_round(m, n, p, prop) && rounds < max_rounds)
	{
        rounds ++;
	}

	print_result(m, p);
	printf("after %d rounds\n", rounds);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Usage: %s m n rounds\n", argv[0]);
		return -1;
	}
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int rounds = atoi(argv[3]);
	assert(n >= 13);
	assert(m >= 1);
	assert(rounds >= 1);

	
	TPlayer p[m];
	TProperty prop[n];

	for(int i = 0; i < n; i++)
	{
		prop[i].id = i;
		prop[i].owner_id = -1;
		prop[i].rent = i + 1;
	}

	for(int j = 0; j < m; j++)
	{
		p[j].id = j;
		p[j].loc = 0;
		p[j].balance = n;
	}
	monopoly(m, n, p, prop, rounds);
	return 0;	
}