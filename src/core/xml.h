#pragma once
// xml.h - minimal XML tree parser (elements, attributes, text, CDATA, comments)
#include "util.h"

typedef struct XmlAttr { char *name; char *value; } XmlAttr;
typedef struct XmlNode {
    char *name;
    char *text; // inner text content (only direct text, stripped)
    ARRAY(XmlAttr) attrs;
    ARRAY(struct XmlNode *) children;
} XmlNode;
typedef ARRAY(struct XmlNode *) XmlNodeList;

// parse entire doc; returns root node (the document element) or NULL
XmlNode *xml_parse(const char *data, size_t len);
void xml_free(XmlNode *n);

const char *xml_attr(XmlNode *n, const char *name); // NULL if absent
XmlNode *xml_child(XmlNode *n, const char *name);   // first child element with name
void xml_children(XmlNode *n, const char *name, XmlNodeList *out); // append matching children
// local name after ':' (e.g. "w:t" -> "t")
const char *xml_local_name(XmlNode *n); // returns pointer into name
bool xml_name_is(XmlNode *n, const char *local); // compares local name
// concatenate all descendant text (for docx / plain text dumps) -> malloc'd
char *xml_text_all(XmlNode *n);
