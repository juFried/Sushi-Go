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
	
	key -> cardList = malloc(sizeof(CardNode));
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
	node -> name = strdup(card);
	// category points to the key in the board hash table
	CategoryNode * category = getNode(board, card);
	// next points to the first element in the linked list
	node -> next = category -> cardList;
	// node becomes the new first element of the linked list
	category -> cardList = node;
}

// Number of cards on the board in a hash key
int getNumCards(CategoryNode * board, char * card) {
	int count = 0;
	CardNode * current = getNode(board, card) -> cardList;
	while(current) {
		count++;
		current = current -> next;
		if (card[0] == 'm') printf("COUNT: %d\nName: %s\nPointer: %p\n", count, card, current);
	}
	return count;
}

// Initialize deck of cards. Returns a list of strings
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
	return (int)(rand() % (range));
}


// Play of one round
void playRound(Player players[], int numPlayers, char ** cards) {



	int sizeHand = 12 - numPlayers;
	int index = 0;
	// Pick starting hands
	for (int i=0; i<numPlayers; i++) {
		for (int j=0; j<sizeHand; j++) {
				do index = randomNum(TOTAL_CARDS); while (strcmp(cards[index], "used") == 0); // Keep getting new random cards until it is not "used"
				players[i].hand[j] = malloc(sizeof(cards[index]) +  1);
				strcpy(players[i].hand[j], cards[index]);
				cards[index] = "used"; // Nullify the pointer to the string
		}
	}

	for (int i=0; i<numPlayers; i++){
	for (int j=0; j<sizeHand; j++){
	printf("%s\n", players[i].hand[j]);
	}printf("\n\n");
	}
	// Loop through the turns
	for (int i=0; i<sizeHand; i++) {
		// Place down a random card from deck
		for (int j=0; j<numPlayers; j++) {
			do index = randomNum(sizeHand); while (strcmp(players[j].hand[index], "used") == 0);
			createCardNode(players[j].board, players[j].hand[index]);
			players[j].hand[index] = "used";
		}

		// Player 1 gets 0's hand, 2 gets 1's hand, etc. Use Player 0 as a buffer
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
void calculatePoints(Player player) {
	int total = 0;
	CategoryNode * board = player.board;
	
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

	player.points += total;
	player.numMakis = getNumCards(board, "maki rolls three") * 3 + getNumCards(board, "maki rolls two") * 2 + getNumCards(board, "maki rolls one");
	printf("NUMBER OF MAKIS: %d\n", player.numMakis);
}

// Merge lists (part of merge sort algorithm)
int (* merge(int (* left)[2], int leftSize, int (* right)[2], int rightSize))[2]{
	int l = 0, r = 0;
	int (* sortedList)[2] = malloc(leftSize + rightSize * 2 * sizeof(* sortedList));
	
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
	
	// When one list runs out of numbers, ensure the other list's numbers are not lost
	for (; l<leftSize; l++) {
		sortedList[l+r][0] = left[l][0];
		sortedList[l+r][1] = left[l][1];
	}
	for (; r<rightSize; r++) {
		sortedList[l+r][0] = right[r][0];
		sortedList[l+r][1] = right[r][1];
	}

	return sortedList;

}

// Merge sort main function
int (* sort(int (* list)[2], int left, int right))[2] {
	if (left >= right - 1) {
		return list;
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
	printf("D\n");
	CardNode * past = key -> cardList;
	CardNode * future;
	printf("E\n");
        while (past != NULL) {
		future = past -> next;
		printf("F\n");
		printf("NAME: %p\n", *(past -> name));
		free(past -> name);
		printf("I\n");
                free(past);
		printf("G\n");
		past = future;
		printf("H\n");
	}
	printf("F\n");
}

// Free player's board
void freeBoard(CategoryNode * board) {
	printf("A\n");
	freeNodes(board);
	printf("B\n");
	if (board -> left) {
		freeBoard(board -> left);
	}
	if (board -> right) {
		freeBoard(board -> right);
	}
	printf("C\n");
	free(board -> name);
	free(board -> cardList);
	free(board);
}

// Free players
void freePlayers(Player * players, int numPlayers) {
	for (int i=0; i<numPlayers; i++) {
		for (int j=0; j<(12-numPlayers); j++) {
			free(players[i].hand[j]);
		}
		free(players[i].hand);

		freeBoard(players[i].board); // Note: this requires a separate function for recursion
		
	}

	free(players);
}

void main() {
	// Get number of players
	int numPlayers;
	printf("How many players are there? (2-5)\n");
	scanf("%d", &numPlayers);

	int handSize = 12 - numPlayers;

	int (* numMakis)[2] = malloc(numPlayers * sizeof(* numMakis));
	int (* numPuddings)[2] = malloc(numPlayers * sizeof(* numPuddings));

	// Initialize variables for players
	Player players[numPlayers];
	for (int i=0; i<numPlayers; i++) {
		players[i].points = 0;
		players[i].hand = malloc(handSize * sizeof(char *));
		players[i].board = initializeBoard();
	}
	
	// Make list of cards
	char ** cards = generateCards();
       	
	// Play rounds
	for (int i=0; i<3; i++) {
		// Play the round. Note no return value because the function directly updates the data at the memory address
		printf("Still on\n");
		playRound(players, numPlayers, cards);


		// Calculate points for the round
		for (int j=0; j<numPlayers; j++) {
			calculatePoints(players[j]);
			
			// Maki calculations
			numMakis[j][0] = j;
			numMakis[j][1] = players[j].numMakis;

			// Update Puddings
			numPuddings[j][0] = j;
			numPuddings[j][1] = players[j].numPuddings;
		}

		// Maki point distributing
		for (int j=0; j<numPlayers; j++){
			printf("Player: %d\nMakis: %d\n\n", numMakis[j][0],numMakis[j][1]);}
		numMakis = sort(numMakis, 0, numPlayers-1);
		int makiTieSize = 1;
		int makiSecondTieSize = 0;

		// Calculate ties
		for (int j=1; j<numPlayers; j++) {
			if (numMakis[j][1] == numMakis[0][1]) makiTieSize++;
			if (numMakis[j][1] != numMakis[0][1] && numMakis[j][1] == numMakis[makiTieSize+1][1]) makiSecondTieSize++;
		}

		// Adding Maki points
		for (int j=0; j<makiTieSize; j++) {
			players[numMakis[j][0]].points += (int) (6 / makiTieSize);
		}
		for (int j=makiTieSize; j<(makiTieSize + makiSecondTieSize); j++) {
			players[numMakis[j][0]].points += (int) (3 / makiTieSize);
		}
		
		
		// Pudding point distributing
		if (!(3-i)) {
			printf("Over\n");
			numPuddings = sort(numPuddings, 0, numPlayers-1);
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
					players[numPuddings[numPlayers-j][0]].points -= (int) (6 / mostPuddingsTieSize);
				}
			}
		} else {
			// Round updates on all but the final (3rd) round
			for (int j=1; j<numPlayers+1; j++) {
				printf("Round %d\n", i+1);
				printf("Player %d: %d points\t%d Puddings\n", j, players[j-1].points, players[j-1].numPuddings);
			}
		}
		printf("RUNNING\n");
		for (int j=0; j<numPlayers; j++) {
			printf("a\n");
			freeBoard(players[j].board);
			printf("b\n");
			players[j].board = initializeBoard();
			printf("c\n");
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

	printf("\nWinner is Player %d\n", winnerIndex+1);
	
	// Free memory
	//freeInts(numMakis, numPlayers);
	//freeInts(numPuddings, numPlayers);
	free(numMakis);
	free(numPuddings);
	freeCards(cards);
	freePlayers(players, numPlayers);
}



