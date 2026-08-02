#include "bus.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Bus *bus_create(const char *name, const char **signals, int n) {
    Bus *b = (Bus *)calloc(1, sizeof(Bus));
    snprintf(b->name, sizeof(b->name), "%s", name ? name : "bus");
    b->n_signals = n;
    b->signals = (char **)malloc(sizeof(char *) * (size_t)n);
    for (int i = 0; i < n; i++)
        b->signals[i] = str_dup(signals ? signals[i] : "?");
    return b;
}

void bus_free(Bus *b) {
    if (!b) return;
    for (int i = 0; i < b->n_signals; i++) free(b->signals[i]);
    free(b->signals); free(b);
}

void bus_tap(Schematic *s, const char *bus_name, const char *signal_name, V2 pos) {
    if (!s || !bus_name || !signal_name) return;
    NetLabel nl; nl.name = str_dup(signal_name); nl.pos = pos;
    s->labels = (NetLabel *)realloc(s->labels, sizeof(NetLabel) * (size_t)(s->nlabels + 1));
    s->labels[s->nlabels++] = nl;
}

void label_add_global(Schematic *s, const char *name, V2 pos) {
    if (!s || !name) return;
    NetLabel nl; nl.name = str_dup(name); nl.pos = pos;
    s->labels = (NetLabel *)realloc(s->labels, sizeof(NetLabel) * (size_t)(s->nlabels + 1));
    s->labels[s->nlabels++] = nl;
}

void label_add_local(Schematic *s, const char *name, V2 pos) {
    label_add_global(s, name, pos); // same data model, semantics differ
}

char *bus_auto_detect(Schematic *s) {
    if (!s) return str_dup("no schematic");
    char buf[1024]; int off = 0;
    // group net labels by prefix
    for (int i = 0; i < s->nlabels; i++) {
        // find matching labels with numeric suffix
        const char *nm = s->labels[i].name;
        int len = (int)strlen(nm);
        if (len < 1) continue;
        // check if ends with digit
        if (nm[len-1] >= '0' && nm[len-1] <= '9') {
            int idx = len;
            while (idx > 0 && nm[idx-1] >= '0' && nm[idx-1] <= '9') idx--;
            // prefix = nm[0..idx-1]
            int pfx_len = idx;
            off += snprintf(buf+off, sizeof(buf)-off, "Bus candidate: %.*s[*]\n", pfx_len, nm);
        }
    }
    if (off == 0) off += snprintf(buf+off, sizeof(buf)-off, "No bus candidates found\n");
    return str_dup(buf);
}
