#pragma once
// assistant.h - offline design assistant (keyword -> parametric feature)
#include "design.h"

// Parse a natural-language request into a DesignFeature.
// Understands: "box 40x30x10", "cube 20", "cylinder r 5 h 20", "cyl radius 5 height 20",
// "sphere r 12", "ball 10". Returns true and fills *out (positioned at cursor_x).
bool assistant_parse_design(const char *msg, float cursor_x, DesignFeature *out);

// Produce an offline reply message for *out (malloc'd).
char *assistant_describe_feature(DesignFeature *f);

// Decide what to do with a user message without Ollama.
// Returns one of:
//   'f' feature created (*feat filled, reply describes it)
//   'h' help requested (reply lists capabilities)
//   '?' no design intent (reply is generic)
char assistant_offline_handle(const char *msg, float cursor_x, DesignFeature *feat, char **reply);
