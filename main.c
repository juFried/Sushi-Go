#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Constants for creating the deck
#define TOTAL_CARDS		108
#define TEMPURA 		14
#define SASHIMI 		14
#define DUMPLING 		14
#define MAKI_ROLLS_THREE 	8
#define MAKI_ROLLS_TWO		12
#define MAKI_ROLLS_ONE		6
#define NIGIRI_SALMON		10
#define NIGIRI_SQUID		5
#define NIGIRI_EGG		5
#define PUDDING			10
#define WASABI			6
#define CHOPSTICKS		4
#define MAX_CARD_NAME		17 // maki rolls three\0
#define MAX_CATEGORY_NAME	11 // maki rolls\0


// The linked list of cards within the hash table
typedef struct CardNode {
	char * name;
	struct CardNode * next;
} CardNode;

// Each value of the binary tree in the board
typedef struct CategoryNode {
	char name[MAX_CARD_NAME];
	CardNode * cardList;
	struct CategoryNode * left, * right;
} CategoryNode;

// Player. Each player has cards in hand and on the board
typedef struct Player {
	int points;
	int numMakis;
	int numPuddings;
	char ** hand; // List of an unknown number of strings of length 17
	CategoryNode * board;
} Player;


// Create category on board's binary tree
CategoryNode * createCategoryNode(char * cardName) {
	CategoryNode * key = malloc(sizeof(CategoryNode));
	if (key == NULL) {
		return NULL;
	}
	// key -> name = malloc(sizeof(cardName) + 1);
	strcpy(key -> name, cardName);
	
	key -> cardList = NULL;
	key -> left = NULL;
	key -> right = NULL;
	return key;
}

// Get pointer to key in binary tree
CategoryNode * getNode(CategoryNode * key, char * cardName) {
	if (key == NULL) return NULL;
	int compare = strcmp(key -> name, cardName);
	if (*(key -> name) == *cardName) return key; // Only need to check the first letter
	else if (compare < 0) return getNode(key -> right, cardName);
	else if (compare > 0) return getNode(key -> left, cardName);
}

// The board is a hash table represented in a binary search tree of cards
CategoryNode * initializeBoard() {
	// Create binary tree in alphabetical order
	CategoryNode * rootNode = createCategoryNode("nigiri");
	rootNode -> left = createCategoryNode("dumplings");
	rootNode -> left -> left = createCategoryNode("chopsticks");
	rootNode -> left -> right = createCategoryNode("maki rolls");
	rootNode -> right = createCategoryNode("sashimi");
	rootNode -> right -> left = createCategoryNode("puddings");
	rootNode -> right -> right = createCategoryNode("tempura");
	rootNode -> right -> right -> right = createCategoryNode("wasabi");
	
	return rootNode;
	
}

// Create linked list nodes for the board hash table
void createCardNode(CategoryNode * board, char * card) {

	// Make a node of the linked list with name = card
	CardNode * node = malloc(sizeof(CardNode));
	node -> name = malloc(sizeof(card) + 1);
	strcpy(node -> name, card);

	// Category points to the key in the board hash table
	CategoryNode * category = getNode(board, card);
	
	// Next points to the first element in the linked list
	node -> next = category -> cardList;
	
	// Node becomes the new first element of the linked list
	category -> cardList = node;
}

// Number of cards on the board in a hash key
int getNumCards(CategoryNode * board, char * card) {
	int count = 0;
	CardNode * current = getNode(board, card) -> cardList;
	while(current) {
		if (strcmp(card, current -> name) == 0) count++;
		current = current -> next;
	}
	return count;
}

// Initialize deck of cards. Returns a list of pointers to strings
char ** generateCards() {
	char ** cards = malloc(TOTAL_CARDS * sizeof(char *));
	int j=0;
	for (int i=0; i<TEMPURA; i++) {
		cards[j] = malloc(sizeof("tempura") + 1);
		strcpy(cards[j], "tempura");
		j++;
	}
	for (int i=0; i<SASHIMI; i++) {
                cards[j] = malloc(sizeof("sashimi") + 1);
		strcpy(cards[j], "sashimi");
                j++;
        }
        for (int i=0; i<DUMPLING; i++) {
                cards[j] = malloc(sizeof("dumpling") + 1);
		strcpy(cards[j], "dumpling");
                j++;
        }
        for (int i=0; i<MAKI_ROLLS_THREE; i++) {
                cards[j] = malloc(sizeof("maki rolls three") + 1);
		strcpy(cards[j], "maki rolls three");
                j++;
        }
        for (int i=0; i<MAKI_ROLLS_TWO; i++) {
                cards[j] = malloc(sizeof("maki rolls two") + 1);
		strcpy(cards[j], "maki rolls two");
                j++;
        }
        for (int i=0; i<MAKI_ROLLS_ONE; i++) {
                cards[j] = malloc(sizeof("maki rolls one") + 1);
		strcpy(cards[j], "maki rolls one");
                j++;
        }
        for (int i=0; i<NIGIRI_SALMON; i++) {
                cards[j] = malloc(sizeof("nigiri salmon") + 1);
		strcpy(cards[j], "nigiri salmon");
                j++;
        }
        for (int i=0; i<NIGIRI_SQUID; i++) {
                cards[j] = malloc(sizeof("nigiri squid") + 1);
		strcpy(cards[j], "nigiri squid");
                j++;
        }
        for (int i=0; i<NIGIRI_EGG; i++) {
                cards[j] = malloc(sizeof("nigiri egg") + 1);
		strcpy(cards[j], "nigiri egg");
                j++;
        }
        for (int i=0; i<PUDDING; i++) {
                cards[j] = malloc(sizeof("pudding") + 1);
		strcpy(cards[j], "pudding");
                j++;
        }
        for (int i=0; i<WASABI; i++) {
                cards[j] = malloc(sizeof("wasabi") + 1);
		strcpy(cards[j], "wasabi");
                j++;
        }
        for (int i=0; i<CHOPSTICKS; i++) {
                cards[j] = malloc(sizeof("chopsticks") + 1);
		strcpy(cards[j], "chopsticks");
                j++;
        }
	return cards;
}

// Pick a random number out of range numbers
int randomNum(int range) {
	return (int)(rand() % range);
}


// Play of one round
void playRound(Player * players, int numPlayers, char ** cards) {
	int sizeHand = 12 - numPlayers;
	int index = 0;

	// Pick starting hands
	for (int i=0; i<numPlayers; i++) {
		for (int j=0; j<sizeHand; j++) {
				do index = randomNum(TOTAL_CARDS); while (strcmp(cards[index], "used") == 0); // Keep getting new random cards until it is not "used"
				strcpy(players[i].hand[j], cards[index]);
				strcpy(cards[index], "used");
		}
	}

	// Loop through the turns
	for (int i=0; i<sizeHand; i++) {
		// Place down a random card from deck
		for (int j=0; j<numPlayers; j++) {
			do index = randomNum(sizeHand); while (strcmp(players[j].hand[index], "used") == 0);
			createCardNode(players[j].board, players[j].hand[index]);
			strcpy(players[j].hand[index], "used");
		}

		// Swap hands using Player 0 as a buffer
		char ** tmpHand;
		for (int j=1; j<numPlayers; j++) {
			tmpHand = players[0].hand;
			players[0].hand = players[j].hand;
			players[j].hand = tmpHand;
		}
				
	}
	

}

/* 
	Update the player with their points for the round (except Maki and Pudding points)
	Note: I am not considering Chopsticks in the point-scoring or game playing
*/
void calculatePoints(Player * player) {
	int total = 0;
	CategoryNode * board = player -> board;

	total += 5 * (int)(getNumCards(board, "tempura")/2);
	
	total += 10 * (int)(getNumCards(board, "sashimi")/3);
	
	// 0.5numDumplings^2 + 0.5numDumplings represents the distribution of dumpling scores. This avoids if - else chain
	int numDumplings = getNumCards(board, "dumplings");
	total += 0.5 * (numDumplings * numDumplings + numDumplings);
	
	// Increase the points of however many of the highest scoring nigiri there are. Overflowing wasabi move to the next highest scoring nigiri
	int numWasabi = getNumCards(board, "wasabi");
	int numNigiriSquid = getNumCards(board, "nigiri squid");
        int numNigiriSalmon = getNumCards(board, "nigiri salmon");
        int numNigiriEgg = getNumCards(board, "nigiri egg");

	total += 3 * numNigiriSquid + (numNigiriSquid * (numWasabi >= numNigiriSquid) + numWasabi * (numWasabi < numNigiriSquid)) * 6;
	numWasabi = (numNigiriSquid - numWasabi) * (numWasabi > numNigiriSquid);

        total += 2 * numNigiriSalmon + (numNigiriSalmon * (numWasabi >= numNigiriSalmon) + numWasabi * (numWasabi < numNigiriSalmon)) * 4;
        numWasabi = (numNigiriSalmon - numWasabi) * (numWasabi > numNigiriSalmon);

        total += numNigiriEgg + (numNigiriEgg * (numWasabi >= numNigiriEgg) + numWasabi * (numWasabi < numNigiriEgg)) * 2;

	// Update player's member variables
	player -> points += total;
	player -> numMakis = getNumCards(board, "maki rolls three") * 3 + getNumCards(board, "maki rolls two") * 2 + getNumCards(board, "maki rolls one");
	player -> numPuddings += getNumCards(board, "pudding");
}

// Merge lists (part of merge sort algorithm)
int (* merge(int (* left)[2], int leftSize, int (* right)[2], int rightSize))[2]{
	int l = 0, r = 0;
	int (* sortedList)[2] = malloc((leftSize + rightSize) * sizeof(* sortedList));

	// Merge left and right from biggest to smallest
	while (l < leftSize && r < rightSize) {
		if (left[l][1] > right[r][1]) {
			sortedList[l+r][0] = left[l][0];
			sortedList[l+r][1] = left[l][1];
			l++;
		} else {
			sortedList[l+r][0] = right[r][0];
			sortedList[l+r][1] = right[r][1];
			r++;
		}
	}
	
	// When one list runs out of numbers, fill the rest of the sorted list with the other list's numbers
	for (; l<leftSize; l++) {
		sortedList[l+r][0] = left[l][0];
		sortedList[l+r][1] = left[l][1];
	}
	for (; r<rightSize; r++) {
		sortedList[l+r][0] = right[r][0];
		sortedList[l+r][1] = right[r][1];
	}
	
	// Free the sorted lists from the prior recursive run
	free(left);
	free(right);
	
	return sortedList;

}

// Merge sort main function
int (* sort(int (* list)[2], int left, int right))[2] {

	// Base case
	if (left >= right - 1) {
		int (* sizeOneList)[2] = malloc(1 * sizeof(sizeOneList));
		sizeOneList[0][0] = list[left][0];
		sizeOneList[0][1] = list[left][1];

		return sizeOneList;
	}

	int mid = (left + right) / 2;
	int (* leftSort)[2] = sort(list, left, mid);
	int (* rightSort)[2] = sort(list, mid, right);

	return merge(leftSort, mid - left, rightSort, right - mid);
}

// Free cards
void freeCards (char ** cards) {
	
	for (int i=0; i<TOTAL_CARDS; i++) {
		free(cards[i]);
	}
	
	free(cards);
}

// Free numMakis and numPuddings
void freeInts(int (*list)[2], int length){
	for (int i=0; i<length; i++) {
		free(list[i]);
	}
	free(list);
}

// Free linked lists within board
void freeNodes(CategoryNode * key) {
	CardNode * past = key -> cardList;
	CardNode * future;
        while (past != NULL) {
		future = past -> next;
		free(past -> name);
                free(past);
		past = future;
	}
}

// Free player's board
void freeBoard(CategoryNode * board) {
	freeNodes(board);
	if (board -> left) {
		freeBoard(board -> left);
	}
	if (board -> right) {
		freeBoard(board -> right);
	}
	free(board);
}

// Free players
void freePlayers(Player * players, int numPlayers) {
	for (int i=0; i<numPlayers; i++) {
		for (int j=0; j<(12-numPlayers); j++) {
			free(players[i].hand[j]);
		}

		free(players[i].hand);
	}
}

void main() {
	// For randomness function
	srand(time(0));

	// Get number of players
	int numPlayers;
	printf("How many players are there? (2-5)\n");
	scanf("%d", &numPlayers);

	int handSize = 12 - numPlayers;
	
	// Initialize 2d arrays for numMakis and numPuddings
	int (* numMakis)[2] = malloc(numPlayers * sizeof(* numMakis));
	int (* numPuddings)[2] = malloc(numPlayers * sizeof(* numPuddings));
	
	// Initialize variables for players
	Player players[numPlayers];
	for (int i=0; i<numPlayers; i++) {
		players[i].points = 0;
		players[i].hand = malloc(handSize * sizeof(players[i].hand));
		for (int j=0; j<handSize; j++) {
			players[i].hand[j] = malloc(MAX_CARD_NAME * sizeof(char));
		}
		players[i].board = initializeBoard();
		players[i].numPuddings = 0;
	}

	// Make list of cards
	char ** cards = generateCards();

	// Play rounds
	for (int i=0; i<3; i++) {
		// Play the round. Note no return value because the function directly updates the data at the memory address
		playRound(players, numPlayers, cards);
		
		// Calculate points for the round
		for (int j=0; j<numPlayers; j++) {
			calculatePoints(& players[j]);
			
			// Update Makis
			numMakis[j][0] = j;
			numMakis[j][1] = players[j].numMakis;

			// Update Puddings
			numPuddings[j][0] = j;
			numPuddings[j][1] = players[j].numPuddings;
		}

		// Maki point distributing
		int (* numMakisSorted)[2] = sort(numMakis, 0, numPlayers-1);
		free(numMakis);
		numMakis = numMakisSorted;
		int makiTieSize = 1;
		int makiSecondTieSize = 0;

		// Calculate Maki ties
		for (int j=1; j<numPlayers; j++) {
			if (numMakis[j][1] == numMakis[0][1]) makiTieSize++;
			if (numMakis[j][1] != numMakis[0][1] && numMakis[j][1] == numMakis[makiTieSize+1][1]) makiSecondTieSize++;
		}

		// Adding Maki points
		for (int j=0; j<makiTieSize; j++) {
			players[numMakis[j][0]].points += (int) (6 / makiTieSize);
		}
		for (int j=makiTieSize; j<(makiTieSize + makiSecondTieSize); j++) {
			players[numMakis[j][0]].points += (int) (3 / makiSecondTieSize);
		}
		
		
		// Pudding point distributing
		if (!(3-i)) {
			int (* numPuddingsSorted)[2] = sort(numPuddings, 1, numPlayers-1);
			free(numPuddings);
			numPuddings = numPuddingsSorted;

			int mostPuddingsTieSize = 0;
			int leastPuddingsTieSize = 0;
			
			// Calculating ties
			for (int j=0; j<numPlayers; j++) {
				if (numPuddings[j][1] == numPuddings[0][1]) mostPuddingsTieSize++;
				if (numPuddings[j][1] == numPuddings[numPlayers][1]) leastPuddingsTieSize++;
			}
			
			// Adding Pudding points
			for (int j=0; j<mostPuddingsTieSize; j++) {
				players[numPuddings[j][0]].points += (int) (6 / mostPuddingsTieSize);
			}
			if (numPlayers > 2) {
				for (int j=0; j<leastPuddingsTieSize; j++) {
					players[numPuddings[numPlayers-j-1][0]].points -= (int) (6 / leastPuddingsTieSize);
				}
			}

			// Free board on last run of rounds loop to avoid munmap_chunk error
			for (int j=0; j<numPlayers; j++) {
				freeBoard(players[j].board);
			}
			
		} else {
			// Round updates on all but the final (3rd) round
			printf("\nRound %d\n", i+1);
			for (int j=1; j<numPlayers+1; j++) {
				printf("Player %d: %d points\t%d Puddings\n", j, players[j-1].points, players[j-1].numPuddings);
			}
		}
	}

	// Determine winner
	int maxPoints = 0;
	int winnerIndex = 0;
	for (int i=0; i<numPlayers; i++) {
		int playerPoints = players[i].points;
		if (playerPoints > maxPoints) {
			maxPoints = playerPoints;
			winnerIndex = i;
		}
	}
	
	printf("\nWinner is Player %d! Congratulations!\n\n", winnerIndex+1);
	
	// Free memory
	free(numMakis);
	free(numPuddings);
	freeCards(cards);
	freePlayers(players, numPlayers);
}



