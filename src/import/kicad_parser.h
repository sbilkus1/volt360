#pragma once
/* kicad_parser.h - Standalone recursive-descent .kicad_sch S-expression parser.
 * Direct tokenizer + recursive descent. No dependency on sexpr.c.
 * Populates a Schematic struct with symbols, wires, junctions, text labels.
 */
#include "../model/project.h"

bool kicad_parse_sch(const char *path, Schematic *out);
