#pragma once
// ollama.h - minimal Ollama HTTP client (no external deps, WinHTTP)
#include "../model/project.h"

// Checks server reachability by calling GET /api/tags. Returns true if any model is listed.
bool ollama_ping(const char *url);

// Returns a malloc'd list of model names (newline separated) available on the server, or NULL.
char *ollama_models(const char *url);

// Extracts a Component from raw datasheet text using the given model.
// Fills *out (caller must component_free_contents) and links doc_id.
// Returns true on success (even if server returned partial data).
bool ollama_extract_component(const char *url, const char *model, const char *text, const char *doc_id, Component *out);

// Single-shot chat. Returns malloc'd reply text, or NULL on failure.
char *ollama_chat(const char *url, const char *model, const char *user_msg);
