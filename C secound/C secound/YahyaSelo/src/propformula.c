#include "propformula.h"

#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "util.h"

PropFormula* mkVarFormula(VarTable* vt, char* name) {
    PropFormula* res = (PropFormula*)malloc(sizeof(PropFormula));

    res->kind = VAR;
    res->data.var = mkVariable(vt, name);

    return res;
}

PropFormula* mkBinaryFormula(FormulaKind kind, PropFormula* left_op,
                             PropFormula* right_op) {
    PropFormula* res = (PropFormula*)malloc(sizeof(PropFormula));

    res->kind = kind;
    res->data.operands[0] = left_op;
    res->data.operands[1] = right_op;

    return res;
}

PropFormula* mkUnaryFormula(FormulaKind kind, PropFormula* operand) {
    PropFormula* res = (PropFormula*)malloc(sizeof(PropFormula));

    res->kind = kind;
    res->data.single_op = operand;

    return res;
}

void freeFormula(PropFormula* pf) {
    // for some reason If I freed every single Case I pass less tests this is
    // why I have in lines 55 and 60 comments because my original if statment I
    // freed every single if
    if (pf == NULL) {
        return;
    }
    // last thin to notice I wanted to learn how the switch works this is why I
    // cpoied my if statment to ChatGpt and asked him how the switch works and
    // asked if he could change my if to switch
    //  switch (pf->kind) {
    //    case VAR:
    //   we don't need to do anything here
    // free(pf);
    //      break;

    // case AND:
    // case OR:
    // case IMPLIES:
    // case EQUIV:
    //   freeFormula(pf->data.operands[0]);
    // freeFormula(pf->data.operands[1]);
    //  free(pf);
    // break;

    // case NOT:
    //   freeFormula(pf->data.single_op);
    // free(pf);
    // break;
    //}
    // the idea is that I go in the switch part and after going throuh all cases
    // I free
    // free(pf);

    /*
     * So I tried the switch and I still didnt pass the daily test so I returned
     * to if statments
     */

    if (pf->kind == VAR) {
        free(pf);
    } else if (pf->kind == NOT) {
        freeFormula(pf->data.single_op);
        free(pf);
    } else {
        freeFormula(pf->data.operands[0]);
        freeFormula(pf->data.operands[1]);
        free(pf);
    }
}

void prettyPrintFormula_impl(FILE* f, VarTable* vt, PropFormula* pf) {
    switch (pf->kind) {
        case VAR: {
            VarIndex v = pf->data.var;
            fprintf(f, "%s", getVariableName(vt, v));
            break;
        }

        case AND:
        case OR:
        case IMPLIES:
        case EQUIV: {
            fprintf(f, "(");

            prettyPrintFormula_impl(f, vt, pf->data.operands[0]);

            switch (pf->kind) {
                case AND:
                    fprintf(f, " && ");
                    break;
                case OR:
                    fprintf(f, " || ");
                    break;
                case IMPLIES:
                    fprintf(f, " => ");
                    break;
                case EQUIV:
                    fprintf(f, " <=> ");
                    break;
                default:;
            }

            prettyPrintFormula_impl(f, vt, pf->data.operands[1]);

            fprintf(f, ")");
            break;
        }

        case NOT: {
            fprintf(f, "(! ");
            prettyPrintFormula_impl(f, vt, pf->data.single_op);
            fprintf(f, ")");
            break;
        }
    }
}

void prettyPrintFormulaEval(VarTable* vt, PropFormula* pf) {
    prettyPrintFormula_impl(stderr, vt, pf);
}

void prettyPrintFormula(VarTable* vt, PropFormula* pf) {
    prettyPrintFormula_impl(stdout, vt, pf);
}
