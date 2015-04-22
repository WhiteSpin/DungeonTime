#include "Core.h"

const std::string greetings[] = {"Greetings traveler, I once was an adventurer like you, but then I took an arrow to the knee",
							"Make thy leave, sirrah.",
							"Goodman, how farest thou?",
							"How now!",
							"Prithee, begone! Thy stench offendeth me."
							};

std::string get_random_greeting() {
	srand(time(NULL));
	int l = sizeof(greetings)/sizeof(greetings[0]);
	int randIndex = srand() % l;
	return greetings[randIndex];		
}
