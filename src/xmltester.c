#include "xmltester.h"

#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include <string.h>


typedef struct _test_definition test_definition;
struct _test_definition
{
    test_function test;
    const xmlChar *name;
};

#define MAX_TEST_COUNT 32

struct _xmltester
{
    test_definition tests[MAX_TEST_COUNT];
    unsigned int tests_count;
};


static const xmlChar *ROOT_TEST_NODE = (const xmlChar *)"Tests";
static const xmlChar *TEST_ATTR_NAME = (const xmlChar *)"test";


static const test_definition *find_test_by_name(const xmltester *tester, const xmlChar *name)
{
    unsigned int i;
    const test_definition *test = NULL;

    for (i = 0; !test && i < tester->tests_count; i++)
    {
        if (xmlStrEqual(tester->tests[i].name, name))
        {
            test = &tester->tests[i];
        }
    }

    return test;
}

static void run_test(const xmltester *tester, const xmlNode *test_node)
{
    const test_definition *test;
    test_result result;
    xmlChar *test_f_name = NULL;

    /* figure out what test to run */

    test_f_name = xmlGetProp(test_node, TEST_ATTR_NAME);

    printf("Test \033[1m%s\033[0m: ", test_node->name);

    if (test_f_name)
    {
        test = find_test_by_name(tester, test_f_name);

        if (test)
        {
            /* run the test */

            result = test->test(test_node);

            /* print result */

            switch (result)
            {
            case passed:
                printf("\033[1;32mSuccess!\033[0m");
                break;
            case failed:
                printf("\033[1;31mFailed!\033[0m");
                break;
            case broken:
                printf("\033[1;33mBroken!\033[0m");
                break;
            }
        }
        else
        {
            /* no test with given name is registered */

            printf("\033[1;33mTest function not found \033[0m \033[33m\"%s\"\033[0m", test_f_name);
        }

        free(test_f_name);
    }
    else
    {
        /* there is no attribute to choose test function */

        printf("\033[1;33mMissing test function attribute \033[0m \033[33m<%s %s=\"name\">\033[0m", test_node->name, TEST_ATTR_NAME);
    }

    printf("\033[0m\n");
}

static int run_xml_tests(const xmltester *tester, const xmlNode *root)
{
    if (root->type == XML_ELEMENT_NODE && xmlStrEqual(root->name, ROOT_TEST_NODE))
    {
        xmlNode *current = NULL;
        for (current = root->children; current; current = current->next)
        {
            if (current->type == XML_ELEMENT_NODE)
            {
                run_test(tester, current);
            }
        }
        return 0;
    }
    else
    {
        return 1;
    }
}

int xmltester_run_file(const xmltester *tester, const char *const filename)
{

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* parse the file and get the DOM */
    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL)
    {
        printf("error: could not parse file %s\n", filename);
        return 1;
    }

    root_element = xmlDocGetRootElement(doc);

    run_xml_tests(tester, root_element);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return 0;
}

xmltester *xmltester_create()
{
    xmltester *tester;

    tester = (xmltester *)malloc(sizeof(xmltester));

    if (tester)
    {
        tester->tests_count = 0;
    }

    return tester;
}

void xmltester_free(xmltester *tester)
{
    if (tester)
    {
        unsigned int i;
        for (i = 0; i < tester->tests_count; i++)
        {
            free((void *)tester->tests[i].name);
        }

        free(tester);
    }
}

int xmltester_registertest(xmltester *tester, test_function test, const char *name)
{
    if (tester->tests_count >= MAX_TEST_COUNT)
    {
        return 1;
    }
    else
    {
        xmlChar *test_f_name = xmlCharStrdup(name);

        if (!test_f_name)
        {
            return 2;
        }
        else if (find_test_by_name(tester, test_f_name))
        {
            free(test_f_name);
            return 3;
        }
        else
        {
            tester->tests[tester->tests_count].name = test_f_name;
            tester->tests[tester->tests_count].test = test;

            tester->tests_count++;

            return 0;
        }
    }
}