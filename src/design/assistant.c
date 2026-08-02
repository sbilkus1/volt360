#include "assistant.h"
#include "../core/util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// find first number-like token: optional digits with '.' 
static float pick_float(const char *s, const char *after, float deflt) {
    const char *p = after ? strstr(s, after) : s;
    if (!p) p = s;
    // scan forward from p looking for a number
    for (const char *q = p; *q; q++) {
        if ((*q >= '0' && *q <= '9') || *q == '.') {
            return (float)strtod(q, NULL);
        }
    }
    return deflt;
}

static float next_num(const char **pp) {
    const char *p = *pp;
    while (*p && !((*p >= '0' && *p <= '9') || *p == '.')) p++;
    if (!*p) return 0;
    float v = (float)strtod(p, NULL);
    *pp = p;
    return v;
}

bool assistant_parse_design(const char *msg, float cursor_x, DesignFeature *out) {
    if (!msg || !out) return false;
    const char *p = msg;
    const char *b = strstr(p, "box"), *c = strstr(p, "cyl"), *s = strstr(p, "sphere"), *ba = strstr(p, "ball"), *cu = strstr(p, "cube");
    int type = -1;
    if (b || cu) {
        type = FEAT_BOX;
        p = b ? b : cu;
        float a = next_num(&p), bb = next_num(&p), cc = next_num(&p);
        out->type = type;
        out->w = a > 0 ? a : 20;
        out->h = bb > 0 ? bb : (a > 0 ? a : 20);
        out->d = cc > 0 ? cc : (a > 0 ? a : 20);
        out->segments = 12;
    } else if (c) {
        type = FEAT_CYLINDER;
        p = c;
        float a = pick_float(p, "r", 10), hh = pick_float(p, "h", 20);
        if (a <= 0) a = next_num(&p);
        if (hh <= 0) hh = next_num(&p);
        out->type = type;
        out->w = a > 0 ? a : 10;
        out->h = hh > 0 ? hh : 20;
        out->segments = 24;
    } else if (s || ba) {
        type = FEAT_SPHERE;
        p = s ? s : ba;
        float a = pick_float(p, "r", 10);
        if (a <= 0) a = next_num(&p);
        out->type = type;
        out->w = a > 0 ? a : 10;
        out->h = a > 0 ? a : 10;
        out->d = a > 0 ? a : 10;
        out->segments = 20;
    }
    if (type < 0) return false;
    out->x = cursor_x;
    out->y = 0; out->z = 0;
    out->rx = 0; out->ry = 0; out->rz = 0;
    out->material_id = 0;
    return true;
}

char *assistant_describe_feature(DesignFeature *f) {
    if (!f) return str_dup("?");
    char buf[160];
    switch (f->type) {
        case FEAT_BOX: snprintf(buf, sizeof(buf), "Added a BOX %.1f x %.1f x %.1f mm.", f->w, f->h, f->d); break;
        case FEAT_CYLINDER: snprintf(buf, sizeof(buf), "Added a CYLINDER r=%.1f h=%.1f mm.", f->w, f->h); break;
        case FEAT_SPHERE: snprintf(buf, sizeof(buf), "Added a SPHERE r=%.1f mm.", f->w); break;
        default: return str_dup("?");
    }
    return str_dup(buf);
}

char assistant_offline_handle(const char *msg, float cursor_x, DesignFeature *feat, char **reply) {
    if (!msg) { *reply = str_dup("No message."); return '?'; }
    if (str_icmp(msg, "help") == 0 || strstr(msg, "help") || strstr(msg, "what can you do")) {
        *reply = str_dup("Offline assistant: try 'add a box 40x30x10', 'cylinder radius 5 height 20', "
                         "'sphere r 12'. Connected to Ollama, I can also chat about datasheets.");
        return 'h';
    }
    DesignFeature f;
    if (assistant_parse_design(msg, cursor_x, &f)) {
        *feat = f;
        *reply = assistant_describe_feature(&f);
        return 'f';
    }
    *reply = str_dup("I can create parametric parts (box / cylinder / sphere) from natural language. "
                     "Try: 'box 40x30x10' or 'sphere radius 12'.");
    return '?';
}
