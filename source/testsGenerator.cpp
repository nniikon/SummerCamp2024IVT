/**

    \file
    \brief class containing realization of all methods from testsGenerator lib

    This is main file, with all functionality of class. File contains realization of all methods of class Tester

*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/colourfullPrint.hpp"
#include "../include/quadraticEquation.hpp"
#include "../include/testsGenerator.hpp"

/// @brief error occurs if there are too few tests and testIndex is bigger than number of tests
const char* TOO_FEW_TESTS_ERROR = "Error: there are too few tests\n";

/// @brief error occurs if some tests are in invalid format
const char* VALIDATION_FAIL_ERROR = "Error: tests validation failed\n";

/// @brief error occurs if argument to function is set to NULL or it's reaches invalid case in switch
const char* ILLEGAL_ARG_ERROR = "Error: invalid argument (possible set to NULL)\n";

/**
    \brief returns number of tests
    \param[in] tester tester that contains tests
    \result number of tests
*/
static size_t getCntOfTests(const Tester* tester) {
    ///\throw \param[in] tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);
    return sizeof(tester->tests) / sizeof(*tester->tests);
}

/**
    \brief return if answers are equal
    \param[in] one, two answers from getSolutions func and tests
    \result are two answer equal
*/
static bool checkIfAnswerEqual(const QuadraticEquationAnswer* mine, const QuadraticEquationAnswer* corr) {
    ///\throw mine should not be NULL
    ///\throw corr should not be NULL
    assert(mine != NULL);
    assert(corr != NULL);
    if (mine->numOfSols != corr->numOfSols)
        return false;

    switch (mine->numOfSols) {
        case NO_ROOTS:
            return true;
        case INFINITE_ROOTS:
            return true;
        case TWO_ROOTS:
            if (sign(corr->root_1 - corr->root_1) != 0)
                return false;
        case ONE_ROOT:
            return (sign(mine->root_2 - corr->root_2) == 0);
        default:
            assert(false);
            printError("%s", ILLEGAL_ARG_ERROR);
            break;
    }
    return false;
}

/**
    \brief runs given getSolutions func on all tests from tester
    \param[in] tester tester that contains tests and pointer to solver func
    \result returns testing state and test index
*/
CheckOnTestsOutput checkOnTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    CheckOnTestsOutput result = {}; //FIXME:
    size_t arrLen = getCntOfTests(tester);
    for (size_t i = 0; i < arrLen; ++i) {
        QuadraticEquationAnswer answer = {}; //FIXME:
        Test test = tester->tests[i];
        // we want to match answer type of quad eq lib (if ONE_ROOT root_2 == root_1
        if (test.answer.numOfSols == ONE_ROOT)
            test.answer.root_2 = test.answer.root_1;

        (*tester->GetSolutionsFunc)(&test.equation, &answer);
        if (!checkIfAnswerEqual(&answer, &test.answer)) {
            printf("Failed on test: #%zu\n", i);
            printf("Test (expected):\n");
            printTest(tester, &test);
            printf("Yours (wrong):\n");
            printSolutions(&answer, 10, NULL);

            printf("f : %Lg, e : %Lg\n", answer.root_1, answer.root_2);

            result.testIndex = (int)i;
            result.state = FAILED_ON_SOME_TEST;
            return result;
        }
    }

    changeTextColor(GREEN_COLOR);
    colourfullPrint("All tests passed\n");
    result.state = ALL_TESTS_PASSED;
    return result;
}

/**
    \brief prints given test
    \param[in] tester that contains tests
*/
void printTest(const Tester* tester, const Test* test) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    printf("-------------------------\n");
    printEquation(&test->equation);
    printSolutions(&test->answer, test->equation.outputPrecision, NULL);
}

/**
    \brief prints all tests
    \param[in] tester that contains tests
*/
void printAllTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    size_t arrLen = getCntOfTests(tester);
    printf("All tests:\n"); // FIXME;
    for (size_t i = 0; i < arrLen; ++i)
        printTest(tester, &tester->tests[i]);
}

/**
    \brief prints test with given index
    \param[in] tester that contains tests
    \param[in] testIndex index of test that will be outputed
*/
void printTestWithInd(const Tester* tester, int testIndex) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    if (testIndex >= (int)getCntOfTests(tester)) {
        printError("%s", TOO_FEW_TESTS_ERROR);
        return;
    }

    printTest(tester, &tester->tests[testIndex]);
}

/**
    \brief checks if test is valid
    \param[in] test test to check
    \result is test valid
*/
static bool isValidTest(const Test* test) {
    ///\throw test should not be NULL
    assert(test != NULL);

    int numOfSols = test->answer.numOfSols;
    if (numOfSols == TWO_ROOTS &&
        sign(test->answer.root_1 - test->answer.root_2) >= 0)
            return false;

    QuadEqErrors error = {};
    if (numOfSols == ONE_ROOT ||
        numOfSols == TWO_ROOTS) {
        long double val = NAN;
        error = getPointValue(&test->equation, test->answer.root_1, &val);
        if (error)
            printError("%s", getErrorMessage(error));
        if (sign(val)) return false;
    }

    if (numOfSols == TWO_ROOTS) {
        long double val = NAN;
        error = getPointValue(&test->equation, test->answer.root_2, &val);
        if (error)
            printError("%s", getErrorMessage(error));
        if (sign(val)) return false;
    }

    // QuadEqErrors error = {};
    // switch (test->answer.numOfSols) {
    //     case NO_ROOTS:
    //         return true;
    //     case INFINITE_ROOTS:
    //         return true;
    //     case TWO_ROOTS:
    //         long double val = NAN;
    //         error = getPointValue(&test->equation, test->answer.root_1, &val);
    //         if (error)
    //             printError("%s", getErrorMessage(error));
    //         if (sign(val)) return false;
    //     case ONE_ROOT:
    //         val = NAN;
    //         error = getPointValue(&test->equation, test->answer.root_2, &val);
    //         if (error)
    //             printError("%s", getErrorMessage(error));
    //         if (sign(val)) return false;
    //         break;
    //     default:
    //         assert(false);
    //         printError("%s", ILLEGAL_ARG_ERROR);
    //         break;
    // }

    return true;
}

void validateAllTests(const Tester* tester) {
    ///\throw tester should not be NULL
    ///\throw tester->tests should not be NULL
    assert(tester != NULL && tester->tests != NULL);

    size_t arrLen = getCntOfTests(tester);
    for (size_t i = 0; i < arrLen; ++i) {
        Test test = tester->tests[i];
        // we want this to match quad eq lib
        if (!isValidTest(&test)) {
            printError("Test: %zu\n", i);
            printError("%s", VALIDATION_FAIL_ERROR);
            return;
        }
    }

    changeTextColor(GREEN_COLOR);
    colourfullPrint("All tests are valid\n");
}




