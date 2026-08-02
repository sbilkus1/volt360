#pragma once
// joints.h - assembly motion joints (revolute / prismatic)
#include "../model/project.h"

// Add a joint to assembly `a`. type: JOINT_REVOLUTE / JOINT_PRISMATIC.
// a/b are indices into a->items; b may be -1 (ground). axis is the motion axis,
// pivot is the world point revolute joints rotate around. speed in rad/s or mm/s,
// amp is oscillation amplitude (0 = continuous rotation / constant slide).
// Returns the index of the new joint, or -1 on failure.
int joint_add(Assembly *a, int type, int a_idx, int b_idx, V3 axis, V3 pivot,
              float speed, float amp);

// Build a demo assembly from the project's first N cad models:
// parts placed in a row along X, with a revolute joint on the last part.
// Returns the index of the created assembly in p->assemblies, or -1.
int assembly_demo_robot(Project *p);

// Advance all animated joints by dt (seconds): updates each joint's angle and
// writes the resulting world transform into the placed item. Call per frame.
void assembly_tick(Assembly *a, float dt);

// Compute world matrix (column-major float[16], raylib-compatible) for an item
// given the current joint state.
void joint_world(Assembly *a, int item_idx, float out[16]);
