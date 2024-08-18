#include <stdio.h>
#include "quadraticEquationLib.h"



// ----------------------------- MAIN ----------------------------------------

int main() {
    struct QuadraticEquation equation;

    readEquation(&equation);
    printEquation(&equation);
    // these two functions works only if it's quadratic equation (a != 0), otherwise assertion will be raised
    printf("Coordinate X of top of parabola: %.10Lg\n", getVertX(&equation));
    printf("Coordinate Y of top of parabola: %.10Lg\n", getVertY(&equation));
    printf("Value at point 5: %.10Lg\n", getPointValue(&equation, 5));
    solveAndPrintEquation(&equation);

    return 0;
}
