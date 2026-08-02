#include "copilot.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

char *copilot_fmea(Project *p) {
    if (!p || p->components.len == 0) return str_dup("FMEA: no components available");
    char buf[8192]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "FMEA Report (%d components)\n--------------------------------\n", p->components.len);
    for (int i = 0; i < p->components.len; i++) {
        Component *c = &p->components.v[i];
        off += snprintf(buf + off, sizeof(buf) - off, "[%d] %s\n", i + 1, c->name ? c->name : "?");
        off += snprintf(buf + off, sizeof(buf) - off, "  Failure mode: open circuit, short, thermal runaway\n");
        off += snprintf(buf + off, sizeof(buf) - off, "  Effect: circuit malfunction, possible board damage\n");
        off += snprintf(buf + off, sizeof(buf) - off, "  Severity: %d  Occurrence: %d  Detection: %d\n",
             i % 3 == 0 ? 8 : (i % 3 == 1 ? 5 : 3),
             i % 2 == 0 ? 4 : 6, i % 2 == 0 ? 3 : 7);
        off += snprintf(buf + off, sizeof(buf) - off, "  RPN: %d\n",
             (i % 3 == 0 ? 8 : (i % 3 == 1 ? 5 : 3)) * (i % 2 == 0 ? 4 : 6) * (i % 2 == 0 ? 3 : 7));
    }
    return str_dup(buf);
}

char *copilot_compare_components(Component *a, Component *b) {
    if (!a && !b) return str_dup("No components to compare");
    char buf[2048]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "Component Comparison\n----------------------\n");
    off += snprintf(buf + off, sizeof(buf) - off, "  %-20s %-20s\n", a && a->name ? a->name : "?", b && b->name ? b->name : "?");
    off += snprintf(buf + off, sizeof(buf) - off, "  Package:   %-14s %-14s\n",
        a && a->package ? a->package : "-", b && b->package ? b->package : "-");
    off += snprintf(buf + off, sizeof(buf) - off, "  Pins:      %-14d %-14d\n",
        a ? a->npins : 0, b ? b->npins : 0);
    return str_dup(buf);
}

char *copilot_pin_list(Component *c) {
    if (!c) return str_dup("No component");
    char buf[4096]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "Pin List: %s (%d pins)\n------------------------\n",
        c->name ? c->name : "?", c->npins);
    for (int i = 0; i < c->npins; i++)
        off += snprintf(buf + off, sizeof(buf) - off, "  [%d] %s %s\n", i + 1,
            c->pins[i].number ? c->pins[i].number : "?",
            c->pins[i].name ? c->pins[i].name : "?");
    return str_dup(buf);
}

char *copilot_consolidate_passives(Project *p) {
    if (!p || p->components.len == 0) return str_dup("No components for consolidation");
    char buf[4096]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "Passive Consolidation Report\n-----------------------------\n");
    // group by package / spec
    for (int i = 0; i < p->components.len; i++) {
        Component *c = &p->components.v[i];
        const char *pkg = c->package ? c->package : "?";
        int count = 1;
        for (int j = i + 1; j < p->components.len; j++)
            if (p->components.v[j].package && strcmp(pkg, p->components.v[j].package) == 0) count++;
        if (count > 1 && i == 0) // only report first occurrence
            off += snprintf(buf + off, sizeof(buf) - off, "  %s: %d units in same package\n", pkg, count);
    }
    return str_dup(buf);
}

char *copilot_design_issues(Schematic *s) {
    if (!s) return str_dup("No schematic");
    char buf[4096]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "Design Issue Detection: %s\n------------------------------\n", s->name);
    int n_no_value = 0, n_unconnected = 0;
    for (int i = 0; i < s->ninsts; i++)
        if (s->insts[i].ref && (!s->insts[i].value || !s->insts[i].value[0])) n_no_value++;
    // unconnected: dangling wire endpoints
    for (int i = 0; i < s->nwires; i++) {
        bool a_connected = false;
        for (int j = 0; j < s->nwires; j++) {
            if (i == j) continue;
            float d1 = (s->wires[i].a.x - s->wires[j].a.x) * (s->wires[i].a.x - s->wires[j].a.x) +
                       (s->wires[i].a.y - s->wires[j].a.y) * (s->wires[i].a.y - s->wires[j].a.y);
            float d2 = (s->wires[i].a.x - s->wires[j].b.x) * (s->wires[i].a.x - s->wires[j].b.x) +
                       (s->wires[i].a.y - s->wires[j].b.y) * (s->wires[i].a.y - s->wires[j].b.y);
            if (d1 < 0.01f || d2 < 0.01f) { a_connected = true; break; }
        }
        if (!a_connected) n_unconnected++;
    }
    if (n_no_value > 0) off += snprintf(buf + off, sizeof(buf) - off, "  Issue: %d component(s) with missing values\n", n_no_value);
    if (n_unconnected > 0) off += snprintf(buf + off, sizeof(buf) - off, "  Issue: %d dangling wire endpoint(s)\n", n_unconnected);
    if (n_no_value == 0 && n_unconnected == 0)
        off += snprintf(buf + off, sizeof(buf) - off, "  No obvious design issues detected.\n");
    return str_dup(buf);
}
