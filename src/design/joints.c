#include "joints.h"
#include "../core/util.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int joint_add(Assembly *a, int type, int a_idx, int b_idx, V3 axis, V3 pivot,
              float speed, float amp) {
    if (!a || a_idx < 0 || a_idx >= a->nitems) return -1;
    a->joints = (Joint *)realloc(a->joints, sizeof(Joint) * (size_t)(a->njoints + 1));
    Joint *j = &a->joints[a->njoints++];
    memset(j, 0, sizeof(*j));
    j->type = type;
    j->a = a_idx;
    j->b = b_idx;
    j->axis = axis;
    j->pivot = pivot;
    j->speed = speed;
    j->amp = amp;
    j->anim = 1;
    return a->njoints - 1;
}

// advance the assembly clock; angle = amp*sin(speed*t) when amp>0 else speed*t
void assembly_tick(Assembly *a, float dt) {
    if (!a || !a->animated) return;
    a->clock += dt;
    for (int i = 0; i < a->njoints; i++) {
        Joint *j = &a->joints[i];
        if (!j->anim) continue;
        if (j->amp > 0) j->angle = j->amp * sinf(j->speed * a->clock);
        else j->angle = j->speed * a->clock;
    }
}

// rotation about arbitrary unit axis (rad)
static void m_rot_axis(float m[16], V3 k, float a) {
    float c = cosf(a), s = sinf(a), t = 1 - c;
    float x = k.x, y = k.y, z = k.z;
    float r[16] = {
        t*x*x+c,   t*x*y+s*z, t*x*z-s*y, 0,
        t*x*y-s*z, t*y*y+c,   t*y*z+s*x, 0,
        t*x*z+s*y, t*y*z-s*x, t*z*z+c,   0,
        0,         0,         0,         1
    };
    memcpy(m, r, sizeof(r));
}

static void m_mul(float out[16], const float a[16], const float b[16]) {
    float r[16];
    for (int c = 0; c < 4; c++) for (int i = 0; i < 4; i++) {
        r[c*4+i] = a[0*4+i]*b[c*4+0] + a[1*4+i]*b[c*4+1] + a[2*4+i]*b[c*4+2] + a[3*4+i]*b[c*4+3];
    }
    memcpy(out, r, sizeof(r));
}

// base pose matrix from item pos/rot (rot degrees, applied X then Y then Z)
static void item_base(PlaceItem *it, float m[16]) {
    float rx = it->rot.x * 3.14159265f / 180.0f, ry = it->rot.y * 3.14159265f / 180.0f, rz = it->rot.z * 3.14159265f / 180.0f;
    float mx[16], my[16], mz[16], tmp[16];
    m_rot_axis(mx, v3(1,0,0), rx);
    m_rot_axis(my, v3(0,1,0), ry);
    m_rot_axis(mz, v3(0,0,1), rz);
    m_mul(tmp, my, mx);
    m_mul(m, mz, tmp);
    m[3*4+0] = it->pos.x; m[3*4+1] = it->pos.y; m[3*4+2] = it->pos.z; m[3*4+3] = 1;
}

// world matrix for an item, composed of its base pose plus all joints that move it
void joint_world(Assembly *a, int item_idx, float out[16]) {
    if (!a || item_idx < 0 || item_idx >= a->nitems) { memset(out, 0, sizeof(float)*16); out[0]=out[5]=out[10]=out[15]=1; return; }
    item_base(&a->items[item_idx], out);
    for (int i = 0; i < a->njoints; i++) {
        Joint *j = &a->joints[i];
        if (!j->anim) continue;
        if (j->a != item_idx && j->b != item_idx) continue;
        float T[16], R[16], invT[16], m1[16], m2[16];
        m_rot_axis(R, j->axis, j->angle);
        // translate to pivot, rotate, translate back
        T[0]=T[5]=T[10]=T[15]=1; T[3*4+0]=j->pivot.x; T[3*4+1]=j->pivot.y; T[3*4+2]=j->pivot.z;
        for (int k = 0; k < 16; k++) invT[k] = T[k];
        invT[3*4+0] = -j->pivot.x; invT[3*4+1] = -j->pivot.y; invT[3*4+2] = -j->pivot.z;
        m_mul(m1, R, invT);
        m_mul(m2, T, m1);
        if (j->type == JOINT_PRISMATIC) {
            // sliding motion: pure translation along axis
            m2[3*4+0] = j->axis.x * j->angle; m2[3*4+1] = j->axis.y * j->angle; m2[3*4+2] = j->axis.z * j->angle;
            m2[3*4+3] = 1;
        }
        m_mul(out, m2, out);
    }
}

int assembly_demo_robot(Project *p) {
    int n = p->cad_models.len;
    if (n < 2) return -1;
    Assembly a; memset(&a, 0, sizeof(a));
    a.id = str_dup(make_id());
    a.name = str_dup("robot-demo");
    a.nitems = n;
    a.items = (PlaceItem *)calloc((size_t)n, sizeof(PlaceItem));
    float x = 0;
    for (int i = 0; i < n; i++) {
        CadModel *cm = &p->cad_models.v[i];
        PlaceItem *it = &a.items[i];
        it->kind = PART_CAD;
        it->part_id = str_dup(cm->id);
        char label[64];
        snprintf(label, sizeof(label), "%s", cm->name ? cm->name : "part");
        it->label = str_dup(label);
        it->pos = v3(x, 0, 0);
        it->rot = v3(0, 0, 0);
        float w = cm->mesh.bmax.x - cm->mesh.bmin.x;
        x += (w > 0 ? w : 20) + 5;
    }
    // revolute joint: last part swings around the world origin (axis Y)
    joint_add(&a, JOINT_REVOLUTE, n - 1, -1, v3(0, 1, 0), v3(0, 0, 0), 0.9f, 1.2f);
    a.animated = 1;
    arr_push(p->assemblies, a);
    return p->assemblies.len - 1;
}
