#include "erc.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static void erc_add(ErcReport *r, int level, const char *msg) {
    r->issues = (ErcIssue *)realloc(r->issues, sizeof(ErcIssue) * (size_t)(r->nissues + 1));
    ErcIssue *e = &r->issues[r->nissues++];
    e->level = level; snprintf(e->message, sizeof(e->message), "%s", msg);
}

void erc_report_free(ErcReport *r) { free(r->issues); r->issues = NULL; r->nissues = 0; }

ErcReport erc_check(Schematic *s) {
    ErcReport r = {0};
    if (!s) { erc_add(&r, 2, "No schematic loaded"); return r; }

    // 1. unconnected pins: wires that don't connect to anything at one end
    int unconnected = 0;
    for (int i = 0; i < s->nwires; i++) {
        bool a_ok = false, b_ok = false;
        for (int j = 0; j < s->nwires; j++) {
            if (i == j) continue;
            float d1 = (s->wires[i].a.x - s->wires[j].a.x) * (s->wires[i].a.x - s->wires[j].a.x) +
                       (s->wires[i].a.y - s->wires[j].a.y) * (s->wires[i].a.y - s->wires[j].a.y);
            float d2 = (s->wires[i].a.x - s->wires[j].b.x) * (s->wires[i].a.x - s->wires[j].b.x) +
                       (s->wires[i].a.y - s->wires[j].b.y) * (s->wires[i].a.y - s->wires[j].b.y);
            if (d1 < 0.01f || d2 < 0.01f) a_ok = true;
            d1 = (s->wires[i].b.x - s->wires[j].a.x) * (s->wires[i].b.x - s->wires[j].a.x) +
                 (s->wires[i].b.y - s->wires[j].a.y) * (s->wires[i].b.y - s->wires[j].a.y);
            d2 = (s->wires[i].b.x - s->wires[j].b.x) * (s->wires[i].b.x - s->wires[j].b.x) +
                 (s->wires[i].b.y - s->wires[j].b.y) * (s->wires[i].b.y - s->wires[j].b.y);
            if (d1 < 0.01f || d2 < 0.01f) b_ok = true;
        }
        // also check against component pins
        for (int j = 0; j < s->ninsts; j++) {
            float d = (s->wires[i].a.x - s->insts[j].pos.x) * (s->wires[i].a.x - s->insts[j].pos.x) +
                     (s->wires[i].a.y - s->insts[j].pos.y) * (s->wires[i].a.y - s->insts[j].pos.y);
            if (d < 100.0f) a_ok = true;
            d = (s->wires[i].b.x - s->insts[j].pos.x) * (s->wires[i].b.x - s->insts[j].pos.x) +
                (s->wires[i].b.y - s->insts[j].pos.y) * (s->wires[i].b.y - s->insts[j].pos.y);
            if (d < 100.0f) b_ok = true;
        }
        if (!a_ok || !b_ok) unconnected++;
    }
    if (unconnected > 0) {
        char msg[128]; snprintf(msg, sizeof(msg), "WARN: %d wire(s) may be unconnected", unconnected);
        erc_add(&r, 1, msg);
    }

    // 2. missing values
    int no_val = 0;
    for (int i = 0; i < s->ninsts; i++)
        if (!s->insts[i].value || !s->insts[i].value[0]) no_val++;
    if (no_val > 0) {
        char msg[128]; snprintf(msg, sizeof(msg), "WARN: %d component(s) missing value", no_val);
        erc_add(&r, 1, msg);
    }

    // 3. components without wires (isolated)
    int isolated = 0;
    for (int i = 0; i < s->ninsts; i++) {
        bool connected = false;
        for (int j = 0; j < s->nwires && !connected; j++) {
            float d = (s->insts[i].pos.x - s->wires[j].a.x) * (s->insts[i].pos.x - s->wires[j].a.x) +
                     (s->insts[i].pos.y - s->wires[j].a.y) * (s->insts[i].pos.y - s->wires[j].a.y);
            if (d < 100.0f) connected = true;
            d = (s->insts[i].pos.x - s->wires[j].b.x) * (s->insts[i].pos.x - s->wires[j].b.x) +
                (s->insts[i].pos.y - s->wires[j].b.y) * (s->insts[i].pos.y - s->wires[j].b.y);
            if (d < 100.0f) connected = true;
        }
        if (!connected && s->nwires > 0) isolated++;
    }
    if (isolated > 0) {
        char msg[128]; snprintf(msg, sizeof(msg), "ERR: %d component(s) not connected to any wire", isolated);
        erc_add(&r, 2, msg);
    }

    if (r.nissues == 0) erc_add(&r, 0, "ERC passed: no issues found");
    return r;
}
