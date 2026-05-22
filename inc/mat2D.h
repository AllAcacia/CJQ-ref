/*
 * Filename: mat2D.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Indie-made matrix
 * mathematics library.
 */

#ifndef MAT2D_H
#define MAT2D_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
    size_t m; // rows
    size_t n; // cols
    float* mat_float;
} Matrix2D_Float;


typedef enum {
    OP_ADD=0,
    OP_SUB,
    OP_MUL,
    OP_DIV
} MathOp;


Matrix2D_Float Mat2Dfloat_init(const size_t m, const size_t n);

void Mat2Dfloat_fillzeros(const Matrix2D_Float* ptr);

void Mat2Dfloat_del(Matrix2D_Float* ptr);

float Mat2Dfloat_return(const Matrix2D_Float* ptr, const size_t i, const size_t j);

void Mat2Dfloat_insert(const Matrix2D_Float* ptr, const size_t i, const size_t j, float val);

void Mat2Dfloat_arithmetic(const Matrix2D_Float* a, const Matrix2D_Float* b, Matrix2D_Float* c, const MathOp mode);

void Mat2Dfloat_scale(const Matrix2D_Float* a, Matrix2D_Float* b, const float var);

void Mat2Dfloat_matmul(const Matrix2D_Float* a, const Matrix2D_Float* b, Matrix2D_Float* c);

#endif // MAT2D_H
