#pragma once
#include "../model/project.h"
#include "../../raylib/src/raylib.h"

typedef struct {
    char *project_json;
    int context_length;
    int mode;
} AIChatContext;

typedef struct {
    char *role;
    char *content;
    int pending;
} ChatMessage;

typedef enum {
    AI_CMD_PLACE_COMPONENT,
    AI_CMD_REPLACE_COMPONENT,
    AI_CMD_DELETE_COMPONENT,
    AI_CMD_ADD_WIRE,
    AI_CMD_ADD_TRACK,
    AI_CMD_ADD_VIA,
    AI_CMD_MOVE_COMPONENT,
    AI_CMD_SET_PROPERTY,
    AI_CMD_CREATE_BOX,
    AI_CMD_CREATE_CYLINDER,
    AI_CMD_CREATE_SPHERE,
    AI_CMD_SLICE_MESH,
    AI_CMD_EXPORT_GCODE,
    AI_CMD_BOOLEAN_UNION,
    AI_CMD_BOOLEAN_SUBTRACT,
    AI_CMD_ROUTE_NET,
    AI_CMD_ADD_TEXT,
    AI_CMD_NONE
} AICommand;

typedef struct {
    AICommand cmd;
    char ref[16];
    char mpn[64];
    char footprint[32];
    float pos_x, pos_y, pos_z;
    float rotation;
    float from_x, from_y, to_x, to_y;
    char net[32];
    int layer;
    char property[32];
    char value[128];
    float param1, param2, param3;
    int segments;
    int target_index;
    char pcb_id[32];
} AIEditCommand;

typedef struct {
    ChatMessage *messages;
    int nmessages;
    int visible;
    Rectangle panel_rect;
    char input_buf[512];
    int input_len;
    int scroll_offset;
    int edit_active;
    char last_command[256];
    int edit_result;
    char edit_error[256];
    char *suggestions[5];
    int nsuggestions;
    AIChatContext ctx;
    int connected;
    char ollama_url[128];
    char ollama_model[64];
    int input_focused;
    float anim_time;
    float pending_dots;
    Project *proj;
} AIChat;

void aichat_init(AIChat *ac);
void aichat_free(AIChat *ac);
void aichat_toggle(AIChat *ac);
void aichat_update_context(AIChat *ac, const Project *p, int mode);
void aichat_update(AIChat *ac, float dt);
void aichat_render(AIChat *ac);
void aichat_handle_input(AIChat *ac, int key);
void aichat_send_message(AIChat *ac, const char *text);
void aichat_process_response(AIChat *ac, const char *response);
void aichat_add_assistant(AIChat *ac, const char *text);
void aichat_generate_suggestions(AIChat *ac, int mode);
int aichat_parse_commands(const char *response, AIEditCommand *out, int max_commands);
int aichat_execute_command(Project *p, const AIEditCommand *cmd, char *error_out, int error_size);
