#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants for creating the deck
const int TOTAL_CARDS		= 108;
const int TEMPURA 		= 14;
const int SASHIMI 		= 14;
const int DUMPLING 		= 14;
const int MAKI_ROLLS_THREE 	= 8;
const int MAKI_ROLLS_TWO	= 12;
const int MAKI_ROLLS_ONE	= 6;
const int NIGIRI_SALMON		= 10;
const int NIGIRI_SQUID		= 5;
const int NIGIRI_EGG		= 5;
const int PUDDING		= 10;
const int WASABI		= 6;
const int CHOPSTICKS		= 4;
const int MAX_CARD_NAME		= 17; // maki rolls three \0
const int MAX_CATEGORY_NAME	= 11; // maki rolls\0


// The linked list of cards within the hash table
typedef struct CardNode {
	char name[MAX_CARD_NAME];
	struct CardNode * next;
} CardNode;

// Each value of the binary tree in the board
typedef struct CategoryNode {
	char name[MAX_CATEGORY_NAME];
	CardNode * cardList;
	struct CategoryNode * left, * right;
} CategoryNode;

// Player. Each player has cards in hand and on the board
typedef struct Player {
	int points;
	int numMakis;
	int numPuddings;
	char (* hand)[MAX_CARD_NAME]; // List of an unknown number of strings of length 17
	CategoryNode * board;
} Player;


// Create category on board's binary tree
CategoryNode * createCategoryNode(char * cardName) {
	CategoryNode * node = malloc(sizeof(CategoryNode));
	node -> name = strdup(cardName);
	node -> cardList = malloc(sizeof(CardNode));
	node -> left = NULL;
	node -> right = NULL;
	return node;
}

// Get pointer to key in binary tree
CategoryNode * getNode(CategoryNode * node, char * cardName) {
	if (node == NULL) return NULL;
	
	int compare = strcmp(node -> name, cardName);
	
	if ((node -> name)[0] == cardName[0]) return node; // Only need to check the first letter
	else if (compare > 0) return getNode(node -> right, cardName);
	else if (compare < 0) return getNode(node -> left, cardName);
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
	node -> name = card;

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
	}
	return count;
}

// Initialize deck of cards. Returns a list of strings
char (* generateCards())[] {
	char cards[TOTAL_CARDS][MAX_CARD_NAME] = malloc(TOTAL_CARDS * MAX_CARD_NAME * sizeof(char));
	int j=0;
	for (int i=0; i<TEMPURA; i++) {
		cards[j] == "tempura";
		j++;
	}
	for (int i=0; i<SASHIMI; i++) {
                cards[j] == "sashimi";
                j++;
        }
        for (int i=0; i<DUMPLING; i++) {
                cards[j] == "dumpling";
                j++;
        }
        for (int i=0; i<MAKI_ROLLS_THREE; i++) {
                cards[j] == "maki rolls three";
                j++;
        }
        for (int i=0; i<MAKI_ROLLS_TWO; i++) {
                cards[j] == "maki rolls two";
                j++;
        }
        for (int i=0; i<MAKI_ROLLS_ONE; i++) {
                cards[j] == "maki rolls one";
                j++;
        }
        for (int i=0; i<NIGIRI_SALMON; i++) {
                cards[j] == "nigiri salmon";
                j++;
        }
        for (int i=0; i<NIGIRI_SQUID; i++) {
                cards[j] == "nigiri squid";
                j++;
        }
        for (int i=0; i<NIGIRI_EGG; i++) {
                cards[j] == "nigiri egg";
                j++;
        }
        for (int i=0; i<PUDDING; i++) {
                cards[j] == "pudding";
                j++;
        }
        for (int i=0; i<WASABI; i++) {
                cards[j] == "wasabi";
                j++;
        }
        for (int i=0; i<CHOPSTICKS; i++) {
                cards[j] == "chopsticks";
                j++;
        }

	return cards;
}

// Pick a random number out of range numbers
int random(int range) {
	return rand() % (range);
}


// Play of one round
Player * round(Player * players[], int numPlayers, char (* cards)[]) {
	int sizeHand = 12 - numPlayers;
	int index = 0;
	
	// Pick starting hands
	for (int i=0; i<numPlayers; i++) {
		players[i] -> hand = malloc(sizeHand * MAX_CARD_NAME * sizeof(char));
		for (int j=0; j<sizeHand; j++) {
				do index = random(TOTAL_CARDS); while (cards[index] == NULL); // Keep getting new random cards until it is not NULL
				(players[i] -> hand)[j] = cards[index];
				cards[index] = NULL;
		}
	}

	// Loop through the turns
	for (int i=0; i<sizeHand; i++) {
		// Place down a random card from deck
		for (int j=0; j<numPlayers; j++) {
			do index = random(sizeHand); while ((players[j] -> hand)[index] == NULL);
			createCardNode(players[j] -> board, (players[j] -> hand)[index]);
			(players[j] -> hand)[index] = NULL;
		}

		// Player 1 gets 0's hand, 2 gets 1's hand, etc. Use Player 0 as a buffer
		char (* tmpHand)[MAX_CARD_NAME];
		for (int j=1; j<numPlayers; j++) {
			tmpHand = players[0] -> hand;
			players[0] -> hand = players[j % numPlayers] -> hand;
			players[j % numPlayers] -> hand = tmpHand;
		}
		
	}

	return players;
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
	total += 0.5(numDumplings * numDumplings + numDumplings);

	// Increase the points of however many of the highest scoring nigiri there are. Overflowing wasabi move to the next highest scoring nigiri
	int numWasabi = getNode(board, "wasabi");
	int numNigiriSquid = getNumCards(board, "nigiri squid");
        int numNigiriSalmon = getNumCards(board, "nigiri salmon");
        int numNigiriEgg = getNumCards(board, "nigiri egg");

	total += 3 * numNigiriSquid + (numNigiriSquid * (numWasabi >= numNigiriSquid) + numWasabi * (numWasabi < numNigiriSquid)) * 6;
	numWasabi = (numNigiriSquid - numWasabi) * (numWasabi > numNigiriSquid);

        total += 2 * numNigiriSalmon + (numNigiriSalmon * (numWasabi >= numNigiriSalmon) + numWasabi * (numWasabi < numNigiriSalmon)) * 4;
        numWasabi = (numNigiriSalmon - numWasabi) * (numWasabi > numNigiriSalmon);

        total += numNigiriEgg + (numNigiriEgg * (numWasabi >= numNigiriEgg) + numWasabi * (numWasabi < numNigiriEgg)) * 2;

	player -> points += total;
	player -> numMakis = getNumCards(board, "maki rolls three") * 3 + getNumCards(board, "maki rolls two") * 2 + getNumCards(board, "maki rolls one");
}

// Merge lists (part of merge sort algorithm)
int [][] merge(int [][] left, int leftSize, int [][] right, int rightSize) {
	int l = 0, r = 0;
	int sortedList[leftSize + rightSize][2];
	
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
int[][] sort(int [][] list, int left, int right) {
	if (left >= right - 1) {
		return list;
	}

	int mid = (left + right) / 2;
	int leftSort[][] = sort(list, left, mid);
	int rightSort[][] = sort(list, mid, right);

	return merge(leftSort, mid - left, rightSort, right - mid);
}

// Free cards
void freeCards (char (* cards)[]) {
	free(cards);
}

// Free player's board
void freeBoard(CategoryNode * board) {
	if (board -> left) freeBoard(board -> left);
	if (board -> right) freeBoard(board -> right);
	
	free(board -> name);
	free(board -> cardList);
	free(board);
}

// Free players
void freePlayers(Player * players[], int numPlayers) {
	for (int i=0; i<numPlayers; i++) {
		free(players[i] -> hand);
		freeBoard(players[i]); // Note: this requires a separate function for recursion
	}

	free(players);
}

void main() {
	// Get number of players
	int numPlayers;
	printf("How many players are there? (2-5)\n");
	scanf("%d", &numPlayers);

	// Initialize variables for players
	Player players[numPlayers]; 
	for (int i=0; i<numPlayers; i++) {
		players[i].points = 0;
		players[i].hand = NULL; // Defined in round function
		players[i].board = initializeBoard();
	}

	// Make list of cards
	char (* cards)[MAX_CARD_NAME] = generateCards();

	// Play rounds
	for (int i=0; i<3; i++) {
		// Play the round
		players = round(players, numPlayers, cards);

		int numMakis[numPlayers][2];
		int numPuddings[numPlayers][2];

		// Calculate points for the round
		for (int j=0; j<numPlayers; j++) {
			players[j] -> points = calculatePoints(players[j]);
			
			// Maki calculations
			numMakis[j][0] = j;
			numMakis[j][1] = players[j] -> numMakis;

			// Update Puddings
			numPuddings[j][0] = j;
			numPuddings[j][1] = players[j] -> numPuddings;
		}

		// Maki point distributing
		numMakis = sort(numMakis);
		int makiTieSize = 1;
		int makiSecondTieSize = 0;

		// Calculate ties
		for (int j=1; j<numPlayers; j++) {
			if (numMakis[j][1] == numMakis[0][1]) makiTieSize++;
			if (numMakis[j][1] != numMakis[0][1] && numMakis[j][1] == numMakis[makiTieSize+1][1]) makiSecondTieSize++;
		}

		// Adding Maki points
		for (int j=0; j<makiTieSize; j++) {
			players[numMakis[j][0]] -> points += (int) (6 / makiTieSize);
		}
		for (int j=makiTiesSize; j<(makiTieSize + makiSecondTieSize); j++) {
			players[numMakis[j][0]] -> points += (int) (3 / makiTieSize);
		}
		
		
		// Pudding point distributing
		if (!(3-i)) {
			numPuddings = sort(numPuddings);
			int mostPuddingsTieSize = 0;
			int leastPuddingsTieSize = 0;
			// Calculating ties
			for (int j=0; j<numPlayers; j++) {
				if (numPuddings[j][1] == numPuddings[0][1]) mostPuddingsTieSize++;
				if (numPuddings[j][1] == numPuddings[numPlayers][1]) leastPuddingsTieSize++;
			}
			
			// Adding Pudding points
			for (int j=0; j<mostPuddingsTieSize; j++) {
				players[numPuddings[j][0]] -> points += (int) (6 / mostPuddingsTieSize);
			}
			if (numPlayers > 2) {
				for (int j=0; j<leastPuddingsTieSize; j++) {
					players[numPuddings[numPlayers-j][0]] -> points -= (int) (6 / mostPuddingsTieSize);
				}
			}
		} else {
			// Round updates on all but the final (3rd) round
			for (int j=1; j<numPlayers+1; j++) {
				printf("Player %d: %d points\t%d Puddings\n", j, player[j-1] -> points, player[j-1] -> numPuddings);
			}
		}

	}

	// Determine winner
	int maxPoints = 0;
	int winnerIndex = 0;
	for (int i=0; i<numPlayers; i++) {
		int playerPoints = players[i] -> points;
		if (playerPoints > maxPoints) {
			maxPoints = playerPoints;
			winnerIndex = i;
		}
	}

	printf("\nWinner is Player %d\n", winnerIndex+1);
	
	// Free memory
	freeCards(cards);
	freePlayers(players, numPlayers);
}



