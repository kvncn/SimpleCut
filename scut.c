/*
 * Kevin Nisterenko CSC 352 Fall 2022
 * PA3 - scut.c
 *
 * This program defines the necessary functions
 * to implement the simple cut command structure.
 * It takes two command line arguments, one for 
 * the cut mode and another for the cut sequence.
 * Then, it uses those while continuously getting
 * input to correctly cut and output the lines.
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
 * This function gets a character as a 
 * parameter and checks by ascii value 
 * if it represents a numerical digit. 
 *
 * @param character, char representing 
 * the character to be checked
 * @return bool, true if the character 
 * represents a digit, false otherwise
 */
bool isDigit(char character) {
 	if (character > 57 || character < 48) {
	 	return false;
	}
	return true;
}

/*
 * This function uses the letter cutting
 * rule to print out the characters at the
 * correct index.
 *
 * @param line, character array representing
 * a line
 * @param idx, integer representing which 
 * column to print from the line after 
 * separation
 */
void cutLetter(char line[], int idx) {
	int size = strlen(line);

	for (int i = 0; i < size; i++) {
		char curr = line[i];
		// since we have no special characters to 
		// check, the column matches the
		// string index (+1)
		if (i+1 > idx) {
			printf(" ");
			return;
		} else if (i + 1 == idx) {
			printf("%c", curr);
		}
	}
}

/*
 * This function uses the comma cutting
 * rule to print out the characters at the
 * correct index.
 *
 * @param line, character array representing
 * a line
 * @param idx, integer representing which
 * column to print from the line after
 * separation
 */
void cutComma(char line[], int idx) {
	int size = strlen(line);
	int count = 1;

	for (int i = 0; i < size; i++) {
		char curr = line[i];
		// if character is a comma, we increase
		// the column count
		if (curr == ',') {
			count++;
			continue;
		}
		if (count > idx) {
			printf(" ");
			return;
		} 
		// if column and index match, 
		// output column
		if (count == idx) {
			printf("%c", curr);
		}
	}
}

/*
 * This function uses the space cutting
 * rule to print out the characters at the
 * correct index.
 *
 * @param line, character array representing
 * a line
 * @param idx, integer representing which
 * column to print from the line after
 * separation
 */
void cutSpace(char line[], int idx) {
	int size = strlen(line);
	int count = 1;
	int i = 0;

	while (i < size) {
		char curr = line[i];

		// if char is a space/blank character (from ascii)
		// we keep going until we exit this blank block
		if (curr >=  9 && curr <= 32) {
			while (line[i+1] >= 9 && line[i+1] <= 32) {
				i++;
			}
			// increment count, because we are at the next block
			// (column)
			count++;
			if (count > idx) {
				printf(" ");
				return;
			}
		} else {
			// column and index match, so we can output character
				if (count == idx) {
					printf("%c", curr);
				}
		}
		i++;
	}
}

/*
 * This function checks if the selection
 * argument (ranges of cut) is valid,
 * it checks for first and last chars
 * as numbers, and if the string only 
 * contains digits, commas and dashes.
 *
 * @param selection, character array 
 * representing the columns that the cut
 * line will have
 * @return bool, true if the selection
 * is valid, false otherwise
 */
bool checkSelection(char selection[]) {
	int size = strlen(selection);
	
	if (!isDigit(selection[0])) {
		return false;
	} 
	if (!isDigit(selection[size - 1])) {
		return false;
	}

	// no need to check first and last again
	for (int i = 1; i < size - 1; i++) {
		char curr = selection[i]; 
		// if it's not a digit, it needs to 
		// be a comma or dash, otherwise return 
		// invalid
		if (!isDigit(curr)) {
			if (curr != ',' && curr != '-') {
				return false;
			}
		}
	}
	return true;
}

/*
 * This function uses a switch block to
 * call the appropriate line cutting 
 * function. 
 *
 * @param mode, character array representing
 * the delimiter of the cut tool
 * @param line, character array representing
 * a line
 * @param idx, integer representing which 
 * column to get from the cut line
 */
void selectCutter(char mode[], char line[], int idx) {
	switch (mode[1]) {
		case 'l':
			cutLetter(line, idx);
			break;
		case 'c':
			cutComma(line, idx);
			break;
		case 'w':
			cutSpace(line, idx);
			break;
		default:
			return;
	}
}

/*
 * This function iterates over the selection argument
 * to correctly call the cut function and get the
 * right columns to print out to the output. It uses
 * a simple while loop to iterate over the selection
 * argument, there is an inner while loop for the
 * dash/range part.
 *
 * @param line, character array representing
 * a line
 * @param mode, character array representing
 * the delimiter of the cut tool
 * @param selection, character array representing
 * the range and columns to cut
 */
void cutLine(char line[], char mode[], char selection[]) {
	int size = strlen(selection);
	int i = 0;
	int idx = 0;

	while (i < size) {
		char curr = selection[i];

		// if the current char is a digit, we check if
		// it's a 2 digit number, regardless we call the cut function
		if (isDigit(curr)) {
			char next = selection[i+1];			
			if (isDigit(next)) {
				idx = ((curr - '0') * 10) + (next - '0');
				i++;
			} else {
				idx = curr - '0';
			}
			selectCutter(mode, line, idx);
		}
		// just skip over if the character is a comma
		if (curr == ',') {
			i++;
			continue;
		}	
		// if the character is a dash, we need to check the
		// two next ones, so we know which range to loop
		// over and cut the line
		if (curr == '-') {
			int stop = idx+1;
			// we know next is a digit, no checks needed
			char next = selection[i+1];
			// check for double digit number
			char secondNext = selection[i+2];
			if (isDigit(secondNext)) {
				stop = ((next - '0') * 10) + secondNext - '0';
			} else {
				stop = next - '0';
			}
			// iterate over dash range
			idx++;
			while (idx < stop) {
				selectCutter(mode, line, idx);
				idx++;
			}
			// reset back the index because the actual last number 
			// in range will be dealt with in the next iteration
			idx--;
		}
		i++;
	}
}

int main(int argc, char *argv[]) {
	char buffer[102];

	if (argc != 3) {
		fprintf(stderr, "expected 2 command line arguments.\n");
		return 1;
	}

	char *mode = argv[1];

	if (strcmp(mode, "-l") != 0 &&
			strcmp(mode, "-w") != 0 &&
			strcmp(mode, "-c") != 0) {
		fprintf(stderr, "Invalid delimiter type.\n");
		return 2;
	}

	if (!checkSelection(argv[2])) {
		fprintf(stderr, "Invalid selection.\n");
		return 3;
	}

	// while the user input is valid, keep getting it
	while (fgets(buffer, 100, stdin) != NULL) {
		// make sure to correctly end strings
		if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = '\0';
		}
		// call the cutline function on this line
		cutLine(buffer, argv[1], argv[2]);
		printf("\n");
	}
	return 0;
}
