#include "symedit.h"
#include "../core/util.h"
#include <string.h>

Symbol *symedit_create(Project *p, const char *name, float w, float h,
                       V2 *pin_pos, const char **pin_names, int n_pins) {
    if (!p || !name) return NULL;
    Symbol s; memset(&s, 0, sizeof(s));
    s.name = str_dup(name);
    s.reference = str_dup(name);
    s.pins = NULL; s.npins = 0;
    for (int i = 0; i < n_pins; i++) {
        Pin pn; memset(&pn, 0, sizeof(pn));
        char num[8]; snprintf(num, sizeof(num), "%d", i + 1);
        pn.name = str_dup(pin_names ? pin_names[i] : "?");
        pn.number = str_dup(num);
        pn.pos = pin_pos ? pin_pos[i] : v2(0,0);
        pn.angle = 0; pn.ptype = 0;
        s.pins = (Pin *)realloc(s.pins, sizeof(Pin) * (size_t)(s.npins + 1));
        s.pins[s.npins++] = pn;
    }
    s.bmin = v2(-w*0.5f, -h*0.5f);
    s.bmax = v2(w*0.5f, h*0.5f);
    s.has_bbox = 1;
    arr_push(p->symbols, s);
    return &p->symbols.v[p->symbols.len - 1];
}

bool symedit_delete(Project *p, const char *symbol_id) {
    if (!p || !symbol_id) return false;
    for (int i = 0; i < p->symbols.len; i++)
        if (p->symbols.v[i].name && strcmp(p->symbols.v[i].name, symbol_id) == 0) {
            symbol_free_contents(&p->symbols.v[i]);
            for (int j = i; j < p->symbols.len - 1; j++) p->symbols.v[j] = p->symbols.v[j + 1];
            p->symbols.len--;
            return true;
        }
    return false;
}
