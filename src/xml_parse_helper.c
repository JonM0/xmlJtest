#include <xml_parse_helper.h>
#include <libxml/tree.h>

static const xmlChar *ARRAY_ELEMENT_NODE = BAD_CAST "e";

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

int intarr_from_xml(const xmlNode *node, int **arr, int *len)
{
    xmlNode *child_xml = NULL;
    int *buff = NULL, size = 4, i = 0;

    /* init buffer */
    buff = (int *)malloc(sizeof(int) * size);
    if (!buff)
        return 1;

    /* fill */
    for (child_xml = node->children; child_xml; child_xml = child_xml->next)
    {
        if (child_xml->type == XML_ELEMENT_NODE)
        {
            /* insert element */
            if (xmlStrEqual(child_xml->name, ARRAY_ELEMENT_NODE))
            {
                int n;
                if (!int_from_xml(child_xml, &n))
                {
                    /* element parsed successfully */
                    if (i >= size)
                    {
                        size *= 2;
                        buff = (int *)realloc(buff, sizeof(int) * size);
                        if (!buff)
                            return 1;
                    }

                    buff[i] = n;
                    i++;
                }
                else
                {
                    goto parse_fail;
                }
            }
            /* extra elemets mean parse fails */
            else
            {
                goto parse_fail;
            }
        }
        /* extra text means parse fails */
        else if (xmlNodeIsText(child_xml) && !xmlIsBlankNode(child_xml))
        {
            goto parse_fail;
        }
    }

    /* return */
    *arr = buff;
    *len = i;

    return 0;

parse_fail:
    free(buff);
    return 1;
}