/* 
 * File:   Vector.h
 * Author: pablo
 *
 * Created on August 1, 2018, 5:24 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define VECTOR_INITIAL_CAPACITY 100000

// Define a vector type
typedef struct {
  int size;      // slots used so far
  int capacity;  // total available slots
  int *data;     // array of integers we're storing
} Vector;

void vector_init(Vector *vector);

void vector_append(Vector *vector, int value);

int vector_get(Vector *vector, int index);

void vector_set(Vector *vector, int index, int value);

void vector_double_capacity_if_full(Vector *vector);

void vector_free(Vector *vector);

#ifdef	__cplusplus
}
#endif

#endif	/* VECTOR_H */

