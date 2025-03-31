/*
* front.c - Lexical Analyzer for Simple Arithmetic Expressions
 * Author: [Will Reed, Jared Sandfoss]
 * Date: [03/18/2025]
 * Description:
 *    This program reads arithmetic expressions from an input file (`front.in`),
 *    processes each character to identify tokens, and prints the recognized
 *    tokens and lexemes.
 */

#include <stdio.h>
#include <ctype.h>
#include <set>

int indentCount = 0;    // used to keep track of indents for output purposes
void setIndent();       // sets indentation levels as necessary for output

/*Global declarations*/
/*Variables*/
int charClass;          // Character class of the current character
char lexeme [100];      // Array to store the current lexeme
char nextChar;          // The next character read from the input file
int lexLen;             // Length of the current lexeme
int token;              // Token code of the current lexeme
int nextToken;          // Token code of the next lexeme
FILE *in_fp, *out_fp, *fopen();  // File pointer for the input file

/*Function declarations*/
void addChar();         // Adds the next character to the lexeme
void getChar();         // Gets the next character from the input file
void getNonBlank();     // Skips whitespace characters
int lex();              // Lexical analyzer function
int lookup(char ch);    // Looks up operators and parentheses and returns the token code

void expr();            // Parses strings in the language generated by the rule: <expr> -> <term> {(+ | -) <term>}
void term();            // Parses strings in the language generated by the rule: <term> -> <factor> {(* | /) <factor>}
void factor();          // Parses strings in the language generated by the rule: <factor> -> id | int_constant | ( <expr )

/*Character classes*/
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/*Token codes*/
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/**
 * Main driver function.
 * Opens the input data file and processes its contents.
 * Calls getChar() to read the first character and then repeatedly calls lex() to analyze the input.
 * lexical_analysis = { lexeme }
 */
int main() {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
        return 1;
    }
    /* Open the output file */
    if ((out_fp = fopen("output.txt", "w")) == NULL) {
        printf("ERROR - cannot open output.txt \n");
        return 1;
    }
    /* Redirect stdout to the output file */
    stdout = out_fp;
    getChar();
    do {
        lex();
        expr();
    } while (nextToken != EOF);
    /* Close the files */
    fclose(in_fp);
    fclose(out_fp);
    return 0;
}

/**
 * Looks up operators and parentheses and returns the token code.
 * @param ch The character to look up.
 * @return The token code corresponding to the character.
 * operator = "+" | "-" | "*" | "/" | "="
 * punctuation = "(" | ")"
 * letter = a | b | ... | z | A | B | ... | Z
 * digit = 0 | 1 | ... | 9
 */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/**
 * Adds the next character to the lexeme.
 * If the lexeme is too long, prints an error message.
 */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

/**
 * Gets the next character from the input file and determines its character class.
 * <letter> -> a | b | ... | z | A | ... | Z
 */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

/**
 * Skips whitespace characters by repeatedly calling getChar() until a non-whitespace character is found.
 */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/**
 * Lexical analyzer function.
 * Analyzes the input and returns the token code of the next lexeme.
 * @return The token code of the next lexeme.
 * lexeme = identifier | integer_literal | operator | punctuation
 */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /*Parse identifiers*/
        case LETTER:
            addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) { //identifier = letter, { letter | digit } || integer_literal = digit, { digit }
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;
        /*Parse integer literals*/
        case DIGIT:
            addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;
        /*Parentheses and operators*/
        case UNKNOWN:
            lookup(nextChar);
        getChar();
        break;
        /*EOF*/
        case EOF:
            nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    //printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}

/* expr
  Parses strings in the language generated by the rule:
  <expr> -> <term> {(+ | -) <term>}
  */
void expr() {
    setIndent();
    printf("[expr\n");
    /* Parse the first term */
    indentCount++;
    term();
    indentCount--;
    /* As long as the next token is + or -, get
      the next token and parse the next term */
    while  (nextToken == ADD_OP || nextToken == SUB_OP) {
        indentCount++;
        setIndent();
        printf("[%s]\n", lexeme);  // Print operator
        indentCount--;

        indentCount++;
        lex();
        term();
        indentCount--;
    }
    setIndent();
    printf("]\n"); // Close expr bracket
}  /* End of function expr */

/* term
  Parses strings in the language generated by the rule:
  <term> -> <factor> {(* | /) <factor>)
  */
void term() {
    setIndent();
    printf("[term\n");
    /* Parse the first factor */
    indentCount++;
    factor();
    indentCount--;
    /* As long as the next token is * or /, get the
      next token and parse the next factor */
    while  (nextToken == MULT_OP || nextToken == DIV_OP) {
        indentCount++;
        setIndent();
        printf("[%s]\n", lexeme);  // Print operator
        indentCount--;

        indentCount++;
        lex();
        factor();
        indentCount--;
    }
    setIndent();
    printf("]\n"); // Close term bracket
}  /* End of function term*/

/* factor
  Parses strings in the language generated by the rule:
  <factor> -> id | int_constant | ( <expr> )
  */
void factor() {
    /* Determine which RHS */

    if (nextToken == IDENT) {
        setIndent();
        printf("[factor\n");

        indentCount++;
        setIndent();
        printf("[id [%s]]\n", lexeme);  // Print identifier
        indentCount--;
        lex();
    } else if (nextToken == INT_LIT) {
        setIndent();
        printf("[factor\n");

        indentCount++;
        setIndent();
        printf("[int_constant [%s]]\n", lexeme);  // Print integer literal
        indentCount--;
        lex();
    } else if (nextToken == LEFT_PAREN) {
        setIndent();
        printf("[(]\n");

        indentCount++;
        lex();
        expr();
        indentCount--;

        if (nextToken == RIGHT_PAREN) {
            setIndent();
            printf("[)]\n");
            lex();
        } else {
            printf("ERROR: Expected )\n");
        }
    } else {
        printf("ERROR: Unexpected token in factor\n");
    }
    setIndent();
    printf("]\n"); // Close factor bracket
}

// set indents as necessary for output
void setIndent() {
    for (int i = 0; i < indentCount; i++) {
        printf("   ");
    }
}