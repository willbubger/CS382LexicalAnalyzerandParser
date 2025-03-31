Lexical Analyzer

Will Reed
Jared Sandfoss

1. Overview
This project is a lexical analyzer implemented in C that processes simple arithmetic expressions. It reads an input
file (`front.in`), tokenizes identifiers, integer literals, operators, and parentheses, and outputs the corresponding
tokens.

2. General Description
Reads an arithmetic expression from `front.in`.
Processes each character to determine if it's a letter, digit, operator, or parenthesis.
Outputs a tokenized representation of the expression.

3. EBNF grammar
main() EBNF grammar:
    lexical_analysis = { lexeme }
lex() EBNF grammar:
    lexeme = identifier | integer_literal | operator | punctuation
    identifier = letter, { letter | digit }
    integer_literal = digit, { digit }
lookup() EBNF grammar:
    letter = a | b | ... |z | A | B | ... |Z
    digit = 0 | 1 | ... | 9
    operator = "+" | "-" | "*" | "/" | "="
    punctuation = "(" | ")"
getChar() EBNF grammar:
    letter = a | b | ... |z | A | B | ... |Z

4. Association with Functions
Function	    Associated Data Structures	            Explanation
getChar()	    nextChar, charClass	                    Reads the next character from the input file and determines its
                                                        classification (LETTER, DIGIT, or UNKNOWN).
addChar()	    lexeme, lexLen	                        Adds a character to the lexeme array while ensuring it doesn't
                                                        exceed the maximum length.
getNonBlank()	nextChar	                            Skips whitespace characters by repeatedly calling getChar().
lookup()	    nextChar, nextToken	                    Matches nextChar with an operator or punctuation and assigns a
                                                        corresponding nextToken value.
lex()	        lexeme, lexLen, charClass, nextToken	The main lexical analysis function. It identifies lexemes,
                                                        assigns token codes, and prints results.
main()	        in_fp, nextToken	                    Controls program execution, opens the file, and repeatedly calls
                                                        lex() to process input.

5. Token Definitions
Token Type		        Code
Integer Literal		    10
Identifier			    11
Assignment `=`		    20
Addition `+`		    21
Subtraction `-`		    22
Multiplication `*`		23   
Division `/`		    24
Left Parenthesis `(`	25
Right Parenthesis `)`	26 

6. Example Output
(sum + 47) / total
For the above input, the program will produce:
Next token is: 25, Next lexeme is (
Next token is: 11, Next lexeme is sum
Next token is: 21, Next lexeme is +
Next token is: 10, Next lexeme is 47
Next token is: 26, Next lexeme is )
Next token is: 24, Next lexeme is /
Next token is: 11, Next lexeme is total
Next token is: -1, Next lexeme is EOF

Parser

Will Reed
Jared Sandfross

1. Overview
This parser follows a recursive descent approach, parsing expressions according to defined grammar rules. The functions are structured to ensure correct hierarchical parsing while providing meaningful output for debugging and understanding the parse process.

2. General Description
expr(): Parses arithmetic expressions involving addition (+) and subtraction (-).
term(): Parses arithmetic expressions involving multiplication (*) and division (/).
factor(): Parses integer literals and identifiers.

3. EBNF grammar
expr() EBNF grammar:
    <expr> -> <term> { (+|-) <term>}
term() EBNF grammar:
    <term> -> <factor> {(*|/) <factor>}
factor() EBNF grammar:
    <factor>-> <id> | <int> | (<expr>)

4. Association with Functions
Function	    Associated Data Structures	            Explanation
expr()	        nextToken, lexeme	                    Parses arithmetic expressions with addition and subtraction.
                                                        Calls term() and processes + or - operators.
term()	        nextToken, lexeme	                    Parses terms with multiplication and division. Calls factor()
                                                        and processes * or / operators.
factor()	    nextToken, lexeme	                    Parses identifiers, integer constants, or parenthesized
                                                        expressions. Calls expr() when encountering (.

6. Example Output
B + C
For the above input, the program will produce:
[expr
 [term
  [factor
   [id [B]]
  ]
 ]
 [+]
 [term
  [factor
   [id [C]]
  ]
 ]
]