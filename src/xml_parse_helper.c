#include <xml_parse_helper.h>
#include <libxml/tree.h>

int xmlHasChildElementNodes(const xmlNode *node)
{
    xmlNode *child = NULL;

    if (node)
    {
        /* fail if it has any child element */
        for (child = node->children; child; child = child->next)
        {
            if (child->type == XML_ELEMENT_NODE)
            {
                return 1;
            }
        }
    }

    return 0;
}

int int_from_xml(const xmlNode *node, int *n)
{
    xmlChar *content = NULL;

    /* fail if it has any child element */
    if (!node || xmlHasChildElementNodes(node))
    {
        return 1;
    }

    /* take all text content */
    content = xmlNodeGetContent(node);

    if (content)
    {
        int c;
        /* parse as int */
        c = sscanf((const char *)content, "%d", n);
        free(content);

        /* fail if scanf had different than 1 value */
        return c != 1;
    }
    else
    {
        return 1;
    }
}
