#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_LEFT_CURLY_BRACKET,
	TOKEN_RIGHT_CURLY_BRACKET,
	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_COMMA,
	TOKEN_COLON,
	TOKEN_TRUE,
	TOKEN_FALSE,
	TOKEN_NULL,
	TOKEN_EOF,
	TOKEN_ERROR
} token_t;

const char* token_names[] = {
	"TOKEN_LEFT_BRACKET",
	"TOKEN_RIGHT_BRACKET",
	"TOKEN_LEFT_CURLY_BRACKET",
	"TOKEN_RIGHT_CURLY_BRACKET",
	"TOKEN_STRING",
	"TOKEN_NUMBER",
	"TOKEN_COMMA",
	"TOKEN_COLON",
	"TOKEN_TRUE",
	"TOKEN_FALSE",
	"TOKEN_NULL",
	"TOKEN_EOF",
	"TOKEN_ERROR"
};

token_t scan_token(FILE *fp){
	int current_char = fgetc(fp);

	while (isspace(current_char)) {
		current_char = fgetc(fp);
	}
	// early exit if end of file
	if (current_char == -1) return TOKEN_EOF;

	if (current_char == '"'){ // Here is where check for strings
		// need to ingest next char so not stuck on next loop
		//printf("\nleft quote captured\n");
		current_char = fgetc(fp);
		while (current_char != '"'){
			// if end of file before end of string, than is an error
			if (current_char == -1) return TOKEN_ERROR;
			//printf("\ncurrent captured char is %c", current_char);
			current_char = fgetc(fp);
		}
		//printf("\nright quote captured\n");
		return TOKEN_STRING;

	} else if (isdigit(current_char)) { // Here is where check for numbers
		bool hasPeriod = false;
		//printf("\nfirst digit in digit call is %c\n\n", current_char);

		current_char = fgetc(fp);
		
		while (isdigit(current_char) || !hasPeriod && current_char == '.'){
			//printf("\ncurrent char in digit call is %c\n\n", current_char);
			if (current_char = '.'){
				hasPeriod = true;
			}
			current_char = fgetc(fp);
		}

		ungetc(current_char, fp);
		return TOKEN_NUMBER;
	} else if (current_char == 't' || current_char == 'f' || current_char == 'n'){
		char buffer[256];
		int index = 0;
		
		do {
			buffer[index] = current_char;
			index += 1;
			current_char = fgetc(fp);
		} while (isalpha(current_char));
		
		buffer[index] = '\0';

		if (strcmp(buffer, "true") == 0) return TOKEN_TRUE;
		else if (strcmp(buffer, "false") == 0) return TOKEN_FALSE;
		else if (strcmp(buffer, "null") == 0) return TOKEN_NULL;

		return TOKEN_ERROR;

	}

	switch(current_char) {
		case '{': return TOKEN_LEFT_CURLY_BRACKET;
		case '}': return TOKEN_RIGHT_CURLY_BRACKET;
		case '[': return TOKEN_LEFT_BRACKET;
		case ']': return TOKEN_RIGHT_BRACKET;
		case ',': return TOKEN_COMMA;
		case ':': return TOKEN_COLON;
	}

	// default return of error token
	return TOKEN_ERROR;	

}

int main(int argc, char *argv[]){
	if (argc < 2) {
		printf("Too few args provided\n");
		return EXIT_FAILURE;
	}

	const char* filename = argv[1];
	FILE *file;

	file = fopen(filename, "r");
	if (file == NULL) {
		perror("error opening file");
		return EXIT_FAILURE;
	}
	
	token_t current_token = scan_token(file);
	while(current_token != TOKEN_EOF){
		printf("%s \n",token_names[current_token]);
		current_token = scan_token(file);
		if(current_token == TOKEN_ERROR){
			return EXIT_FAILURE;
		}
}
	
	

	return EXIT_SUCCESS;

}

