#include "dpll.h"

#include "err.h"
#include "list.h"
#include "util.h"
#include "variables.h"

typedef enum Reason { CHOSEN, IMPLIED } Reason;
/*
 *Note this Question was really hard I had to ask Chatgpt to explain every
 *single step of the code and after multiple attempts it gave me a structure
 *that was almost correct after that I change the parts that didnt make any
 *sense and asked him again to make it efficient and or fix any possible errors
 *which he did (I hope because I still am not passing all tests)
 *
 */

/**
 * Struct to represent an entry in the assignment stack. Should only be created
 * and freed by pushAssignment and popAssignment.
 */
typedef struct Assignment {
    VarIndex var;
    Reason reason;
} Assignment;

/**
 * Adds a new assignment to the assignment stack.
 *
 * @param stack  an assignment stack
 * @param var    the variable to assign
 * @param r      the reason for the assignment
 */
void pushAssignment(List* stack, VarIndex var, Reason r) {
    Assignment* a = (Assignment*)malloc(sizeof(Assignment));
    a->var = var;
    a->reason = r;
    push(stack, a);
}

/**
 * Removes the head element of an assignment stack and frees it.
 *
 * @param stack  an assignment stack
 */
void popAssignment(List* stack) {
    Assignment* a = (Assignment*)peek(stack);
    free(a);
    pop(stack);
}

/**
 * Führt eine Iteration des DPLL Algorithmus aus.
 *
 * @param vt       die zugrunde liegende Variablentabelle
 * @param stack    der Zuweisungsstack
 * @param cnf      die zu prüfende Formel
 * @return         1 falls der Algorithmus mit SAT terminieren sollte,
 *                 0 falls der Algorithmus weiterlaufen sollte,
 *                 -1 falls der Algorithmus mit UNSAT terminieren sollte
 */

void reset_stack(VarTable* vt, List* stack) {
    // retrieving the current top element of the assignment stack
    Assignment* current_value = peek(stack);
    /*what the loop does
     * 1) remove all assignments from the stack that were not chosen
     * 2) updates the variable value in the variable table (vt)
     * 3) pops the assignment from the stack
     */
    // fixed something here (current !=NULL)
    while (!isEmpty(stack) && current_value->reason != CHOSEN) {
        updateVariableValue(vt, current_value->var, UNDEFINED);
        popAssignment(stack);
        current_value = peek(stack);
    }
    // check if there is still an element remaining in the stack
    // 1) sets the reason of the current assignment to IMPLIED
    // 2) and update variable table to FALSE.
    isEmpty(stack) ? err("reset_stack")
                   : (current_value->reason = IMPLIED,
                      updateVariableValue(vt, current_value->var, FALSE));
    // the code that I hade in mind first
    // if (isEmpty(stack)) {
    //  if not we indicate an error
    //      err("reset_stack");

    //} else {
    /*
     * 1) sets the reason of the current assignment to IMPLIED
     * 2) and update variable table to FALSE.
     *
     */

    // current_value->reason = IMPLIED;
    // updateVariableValue(vt, current_value->var, FALSE);
    // }
}

/**
 * Checks if the assignment stack contains any chosen assignments.
 */
int hasChosenAssignment(List* stack) {
    // creates a list iterator
    ListIterator it = mkIterator(stack);
    if (isEmpty(stack)) {
        err("empty");
    } else {
        while (isValid(&it)) {
            /*
             * 1) retrieves the current element from the assignment stack
             * 2)checks if the reason for the current assignment is CHOSEN.
             * */
            Assignment* current_value = (Assignment*)getCurr(&it);
            // the chosen assignment is present in the stack
            return (current_value->reason == CHOSEN) ? 1 : 0;

            // if it is not CHOSEN, it moves the iterator to the next element in
            // the stack
            next(&it);
        }
    }
    return 0;
}

/**
 * Finds the first unit clause in the CNF.
 */
Clause* findUnitClause(VarTable* vt, CNF* cnf) {
    // creates a list iterator
    List* pointer_list = &cnf->clauses;
    ListIterator iter_list = mkIterator(pointer_list);
    while (isValid(&iter_list)) {
        /*
         * 1) the loop iterates over the clauses in the CNF formula
         * 2) the loop retrieves the current clause from the list
         * 3) afterthat casts it to a pointer.
         * 4) then we pass the variable table and the current clause to
         * determine if the clause is a unit clause
         * 5) get the unit literal.
         * */
        Clause* current_value = (Clause*)getCurr(&iter_list);
        Literal unitLiteral = getUnitLiteral(vt, current_value);
        // I cant change this to ternary operator so I left an if here
        if (unitLiteral != 0) {
            // the current clause is a unit clause.
            return current_value;
        } else {
            // the current clause is not a unit clause.
            next(&iter_list);
        }
    }
    return NULL;
}

/**
 * Performs one iteration of the DPLL algorithm.
 */
int iterate(VarTable* vt, List* stack, CNF* cnf) {
    // check the truth value of the CNF formula using the evalCNF function.
    if (evalCNF(cnf) == TRUE) {
        return 1;
    }
    // check if the formula is unsatisfiable.
    if (evalCNF(cnf) == FALSE) {
        // check if there is any chosen assignment in the assignment stack
        if (!(hasChosenAssignment(stack))) {
            // the algorithm should terminate with UNSAT
            return -1;

        } else {
            // reset the assignment stack and return 0
            reset_stack(vt, stack);
            return 0;
        }
    }
    Clause* unitClause = findUnitClause(vt, cnf);
    // If a unit clause is found, retrieve the unit literal
    // please note that this code was modified after I asked chatGpt how to code
    // the iterate function and because of that I now understand a new way to
    // code if statments (condition ? value_if_true : value_if_false) which is
    // known as  ternary operator
    if (unitClause != NULL) {
        Literal unitLiteral = getUnitLiteral(vt, unitClause);
        // If the unit literal is not 0

        // copied from Chatgpt (originally I had if statments but I wanted my
        // code to look like a space ship)
        return (unitLiteral != 0)
                   ? (updateVariableValue(
                          vt, (unitLiteral > 0) ? unitLiteral : -unitLiteral,
                          (unitLiteral > 0) ? TRUE : FALSE),
                      pushAssignment(
                          stack, (unitLiteral > 0) ? unitLiteral : -unitLiteral,
                          IMPLIED),
                      0)
                   : (err("Invalid unit literal"), 0);
    }

    VarIndex nextVar = getNextUndefinedVariable(vt);
    // if there are no more undefined variables in the variable table
    if (nextVar == 0) {
        // throw an error
        err("Invalid variable index");
    }
    // retrieve the index of the next undefined variable and push the
    // assignment onto the assignment stack
    updateVariableValue(vt, nextVar, TRUE);
    pushAssignment(stack, nextVar, CHOSEN);
    return 0;
}

char isSatisfiable(VarTable* vt, CNF* cnf) {
    List stack = mkList();

    int res;
    do {
        res = iterate(vt, &stack, cnf);
    } while (res == 0);

    while (!isEmpty(&stack)) {
        popAssignment(&stack);
    }

    return (res < 0) ? 0 : 1;
}