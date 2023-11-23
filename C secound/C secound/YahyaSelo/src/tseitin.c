#include "tseitin.h"

#include <stdio.h>

#include "err.h"
#include "util.h"

/**
 * Inserts a clause with one literal into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    a literal
 */
void addUnaryClause(VarTable* vt, CNF* cnf, Literal a) {
    Clause* clause = mkTernaryClause(vt, a, 0, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with two literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 */
void addBinaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b) {
    Clause* clause = mkTernaryClause(vt, a, b, 0);
    addClauseToCNF(cnf, clause);
}

/**
 * Inserts a clause with three literals into the CNF.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param a    the first literal
 * @param b    the second literal
 * @param c    the third literal
 */
void addTernaryClause(VarTable* vt, CNF* cnf, Literal a, Literal b, Literal c) {
    Clause* clause = mkTernaryClause(vt, a, b, c);
    addClauseToCNF(cnf, clause);
}

/**
 * Adds clauses for a propositional formula to a CNF.
 *
 * For a propositional formula pf, clauses that are added that are equivalent to
 *
 *     x <=> pf
 *
 * where x is usually a fresh variable. This variable is also returned.
 *
 * @param vt   the underlying variable table
 * @param cnf  a formula
 * @param pf   a propositional formula
 * @return     the variable x, as described above
 */
VarIndex addClauses(VarTable* vt, CNF* cnf, const PropFormula* pf) {
    // fresh variable generated during the conversion process.
    Literal literal;
    VarIndex first, secound, last;
    switch (pf->kind) {
        case NOT: {
            /* 1) recursively calls addClauses
             * 2) assigns the result to variable a.
             * 3) generates a fresh variable x
             * 4) adds two binary clauses: -a -> -x and a -> x.*/
            first = addClauses(vt, cnf, pf->data.single_op);
            literal = mkFreshVariable(vt);
            addBinaryClause(vt, cnf, -first, -literal);
            addBinaryClause(vt, cnf, first, literal);
        } break;

        case AND: {
            /* 1) recursively calls addClauses on the two operands
             * 2) assigns the results to variables b and c.
             * 3) generates a fresh variable x
             * 4) adds three clauses: -x v b, -x v c, and -b v -c v x.*/
            last = addClauses(vt, cnf, pf->data.operands[0]);
            secound = addClauses(vt, cnf, pf->data.operands[1]);
            literal = mkFreshVariable(vt);
            addBinaryClause(vt, cnf, -literal, last);
            addBinaryClause(vt, cnf, -literal, secound);
            addTernaryClause(vt, cnf, -last, -secound, literal);

        } break;

        case OR: {
            /* 1) recursively calls addClauses on the two operands
             * 2) assigns the results to variables d and e.
             * 3) generates a fresh variable x
             * 4) adds three clauses: -x v d v e, -d v x, and -e v x.*/
            last = addClauses(vt, cnf, pf->data.operands[0]);
            secound = addClauses(vt, cnf, pf->data.operands[1]);
            literal = mkFreshVariable(vt);
            addTernaryClause(vt, cnf, -literal, last, secound);
            addBinaryClause(vt, cnf, -last, literal);
            addBinaryClause(vt, cnf, -secound, literal);
        } break;

        case IMPLIES: {
            /* 1) recursively calls addClauses on the two operands
             * 2) assigns the results to variables f and g.
             * 3) generates a fresh variable x
             * 4) and adds three clauses: -x v -f v g, f v x, and -g v x. */
            last = addClauses(vt, cnf, pf->data.operands[0]);
            secound = addClauses(vt, cnf, pf->data.operands[1]);
            literal = mkFreshVariable(vt);
            addTernaryClause(vt, cnf, -literal, -last, secound);
            addBinaryClause(vt, cnf, last, literal);
            addBinaryClause(vt, cnf, -secound, literal);
        } break;

        case EQUIV: {
            /* 1) recursively calls addClauses on the two operands
             * 2) assigns the results to variables h and i.
             * 3) generates a fresh variable x
             * 4) adds two ternary clauses: -x v -h v i and x v h v -i.*/
            last = addClauses(vt, cnf, pf->data.operands[0]);
            secound = addClauses(vt, cnf, pf->data.operands[1]);
            literal = mkFreshVariable(vt);
            addTernaryClause(vt, cnf, -literal, -last, secound);
            addTernaryClause(vt, cnf, -literal, -secound, last);
            addTernaryClause(vt, cnf, literal, -last, -secound);
            addTernaryClause(vt, cnf, literal, last, secound);
            // its worth mensioning that I originally had if statments and asked
            // Chatgpt to convert them into switch because I had an error that I
            // couldnt find.
            // Last thing is that Chatgpt also corrected EQUIV because for some
            // reason I had originally put (addTernaryClause(vt, cnf, -x, -h,
            // i);) two times

        } break;

        case VAR:
            literal = pf->data.var;
            break;

        default:
            err("Invalid Input");
            return 0;
            break;
    }
    return literal;
}

CNF* getCNF(VarTable* vt, const PropFormula* f) {
    CNF* res = mkCNF();

    VarIndex x = addClauses(vt, res, f);

    addUnaryClause(vt, res, x);

    return res;
}
