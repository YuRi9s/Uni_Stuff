#include "parser.h"

#include <string.h>

#include "err.h"
#include "lexer.h"
#include "list.h"
#include "util.h"

// PropFormula* Failsafe(List* stack, PropFormula* result);

/**
 * Assigns symbols to strings.
 *
 * Aborts the program with an error message if an invalid input is detected.
 *
 * @param str  a string to translate
 * @return     the resulting symbol
 */
/*
 * in this code you can see that I used a switch case instead of if statments
 * the idea is that the function begins with a switch statment that checks the
 * first character and for everysingle if statment it checks for the secound and
 * third for example (If the first character is '&', it proceeds to check the
 * next two characters str[1] and str[2]. If these characters are '&' and '\0')
 * and so on I originally had if statments but was curious and asked Chatgpt if
 * it is possible to change it to switch after that I decide to finish the whole
 * project with swtich cases
 */

FormulaKind toKind(const char* str) {
    // I will keep this because I am failling daily
    // I didnt want to write if statments with strcmp because  I wanted to see
    // how switch works
    /**
     *
     *     switch (str[0]) {
        case '&':
            if (str[1] == '&' && str[2] == '\0') {
                return AND;
            }
            break;
        case '|':
            if (str[1] == '|' && str[2] == '\0') {
                return OR;
            }
            break;
        case '=':
            if (str[1] == '>' && str[2] == '\0') {
                return IMPLIES;
            }
            break;
        case '<':
            if (str[1] == '=' && str[2] == '>' && str[3] == '\0') {
                return EQUIV;
            }
            break;
        case '!':
            if (str[1] == '\0') {
                return NOT;
            }
            break;
        default:
            break;
    }
     *
     *
    */
    // added this case to see if the daiy works
    if (strcmp(str, "&&") == 0) {
        return AND;
    } else if (strcmp(str, "||") == 0) {
        return OR;
    } else if (strcmp(str, "=>") == 0) {
        return IMPLIES;
    } else if (strcmp(str, "<=>") == 0) {
        return EQUIV;
    } else if (strcmp(str, "!") == 0) {
        return NOT;
    }

    /*
     * this code is a loop that iterates over each character of the string str
     * until it reaches \0
     */
    int counter = 0;
    while (str[counter] != ('\0')) {
        // checks if the character is not a lowercase letter
        if (!(str[counter] >= 'a' && str[counter] <= 'z') &&
            // uppercase letter
            !(str[counter] >= 'A' && str[counter] <= 'Z') &&
            // or a digit
            !(str[counter] >= '0' && str[counter] <= '9')) {
            err("Invalid input");
        }
        counter++;
    }
    /*it returns the VAR After checking all the characters in the string and
     *finding no invalid characters*/
    return VAR;
}

// the last daily result showed me that I am still not passing all tests so I
// deleted tha Failsafe to see if I am freeing incorrectly
PropFormula* parseFormula(FILE* input, VarTable* vt) {
    /*initialize a List called formulaList to store the formulas while
     * parsing.*/
    List formulaList = mkList();
    // and a pointer that points to the list
    List* list_pointer = &formulaList;
    // read the next token from the input file. If the returned token is NULL,
    // it means there is no input,
    char* token = nextToken(input);
    if (token == NULL) {
        err("Empty");
    }
    while (token != NULL) {
        switch (toKind(token)) {
            case VAR: {
                /*creates a variable formula using the mkVarFormula function
                 * with the vt and token as arguments, and pushes it onto the
                 * list_pointer */
                push(list_pointer, mkVarFormula(vt, token));
                break;
            }
            case NOT: {
                /*checks if the list  is empty and calls the
                 * Failsafe function to handle the invalid input.*/
                if (isEmpty(list_pointer)) {
                    // Failsafe(list_pointer, NULL);
                    if (isEmpty(list_pointer)) {
                        err("Invalid Input");
                    }
                }
                /*if not the code
                 * 1) etrieves the top formula from the list with peek
                 * 2) pops it from the list using pop,
                 * 3)creates a unary formula with the negation operator
                 * 4) pushes the new formula back onto the list.*/
                PropFormula* P = peek(list_pointer);
                pop(list_pointer);
                push(list_pointer, mkUnaryFormula(NOT, P));
                free(token);
                break;
            }
            default: {
                /*for (AND, OR, IMPLIES, EQUIV) it performs similar stepps */
                if (isEmpty(list_pointer)) {
                    // Failsafe(list_pointer, NULL);
                    err("Invalid Input");
                }
                FormulaKind kind = toKind(token);
                PropFormula* Right = peek(list_pointer);
                pop(list_pointer);
                if (isEmpty(list_pointer)) {
                    // Failsafe(list_pointer, NULL);
                    err("Invalid Input");
                }
                PropFormula* Left = peek(list_pointer);
                pop(list_pointer);
                push(list_pointer, mkBinaryFormula(kind, Left, Right));
                free(token);
                break;
            }
        }
        /*After processing each token, the function calls thenext token from the
         * input file.*/
        token = nextToken(input);
    }
    /*Once the loop finishes, the function retrieves the result formula from the
     * pops it from the list, and assigns it to the result variable.*/
    PropFormula* result = peek(list_pointer);
    pop(list_pointer);
    if (!(isEmpty(list_pointer))) {
        // Failsafe(list_pointer, NULL);
        err("Invalid Input");
    }

    return result;
}

// PropFormula* Failsafe(List* stack, PropFormula* result) {
//   err("Invalid input");
//  clearList(stack);
// if (result != NULL) {
//   switch (result->kind) {
//     case VAR:
//       freeFormula(result);
//     break;
// case AND:
// case OR:
// case IMPLIES:
// case EQUIV:
//   freeFormula(result->data.operands[0]);
// freeFormula(result->data.operands[1]);
// free(result);
// break;
// case NOT:
//   freeFormula(result->data.single_op);
// free(result);
// break;
//}
//}
// return NULL;
//}
