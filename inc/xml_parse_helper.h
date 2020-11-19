#ifndef XML_PARSE_HELPER
#define XML_PARSE_HELPER

#include <libxml/tree.h>

/* macros for a parse loop, taking only elements, a max of once per pointer */

#define PARSENODES(n)                                         \
    {                                                         \
        xmlNode *child = NULL;                                \
        for (child = n->children; child; child = child->next) \
        {                                                     \
            if (child->type == XML_ELEMENT_NODE)              \
            {

#define ENDPARSENODES                \
    {                                \
        return broken;               \
    }                                \
    }                                \
    else if (!xmlIsBlankNode(child)) \
    {                                \
        return broken;               \
    }                                \
    }                                \
    }

#define NODE(ptr, n)                         \
    if (!ptr && xmlStrEqual(child->name, n)) \
    {                                        \
        ptr = child;                         \
    }                                        \
    else

int xmlHasChildElementNodes(const xmlNode *node);

int int_from_xml(const xmlNode *node, int *n);

int intarr_from_xml(const xmlNode *node, int **arr, int *len);

#endif