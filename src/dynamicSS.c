/*
 * Filename: dynamicSS.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Handles discrete-time
 * dynamics systems with a
 * matrix library.
 */


#include "dynamicSS.h"


void DynamicSS_init(SecondOrderDTS* system, const float fn, const float xi, const float dt)
{
    system->fn = fn;
    float wn = 2 * M_PI * fn;
    system->wn = wn;
    system->xi = xi;
    system->dt = dt;

    system->F = Mat2Dfloat_init(2, 2);
    Mat2Dfloat_insert(&system->F, 0, 0, 1.0f);
    Mat2Dfloat_insert(&system->F, 0, 1, dt);
    Mat2Dfloat_insert(&system->F, 1, 0, -(dt*wn*wn));
    Mat2Dfloat_insert(&system->F, 1, 1, -(2*dt*wn*xi)+1.0f);

    system->G = Mat2Dfloat_init(2, 1);
    Mat2Dfloat_insert(&system->G, 1, 0, dt);

    system->H = Mat2Dfloat_init(1, 2);
    Mat2Dfloat_insert(&system->H, 0, 0, system->wn);

    system->x1 = Mat2Dfloat_init(2, 1); // stores the base and first order measurements of the output. Automatically 0'd.
    system->x2 = Mat2Dfloat_init(2, 1); // stores the base and first order measurements of the output. Automatically 0'd.
    system->u1 = Mat2Dfloat_init(2, 1); // is the 2x1 vector, G, but linearly scaled to an input value.
}


// void dynamicSS_del(SecondOrderDTS* system)
// {
//     Mat2Dfloat_del(system->F);
//     Mat2Dfloat_del(system->G);
//     Mat2Dfloat_del(system->H);
//     Mat2Dfloat_del(system->x1);
//     Mat2Dfloat_del(system->x2);
//     Mat2Dfloat_del(system->u1);
//     free(system);
// }


void DynamicSS_setstate(SecondOrderDTS* system, float val1, float val2)
{
    Mat2Dfloat_insert(&system->x1, 0, 0, val1);
    Mat2Dfloat_insert(&system->x1, 1, 0, val2);
}


void DynamicSS_iterate(SecondOrderDTS* system, const float u)
{
    // Iterates the system.
    Mat2Dfloat_scale(&system->G, &system->u1, u);
    Mat2Dfloat_matmul(&system->F, &system->x1, &system->x2);
    Mat2Dfloat_arithmetic(&system->x2, &system->u1, &system->x1, OP_ADD);

    system->y = Mat2Dfloat_return(&system->H, 0, 0) * Mat2Dfloat_return(&system->x1, 0, 0);
}