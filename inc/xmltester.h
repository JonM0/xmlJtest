#ifndef XMLTESTER
#define XMLTESTER

#include <libxml/tree.h>

typedef enum
{
    passed,
    failed,
    broken
} test_result;

typedef test_result (*test_function)(const xmlNode *);

typedef struct _xmltester xmltester;


xmltester* xmltester_create();
void xmltester_free(xmltester* tester);

int xmltester_run_file(const xmltester *tester, const char * const filename);
int xmltester_registertest(xmltester* tester, test_function test, const char *name);

#endif