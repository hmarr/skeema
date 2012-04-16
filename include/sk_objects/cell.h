#ifndef _CELL_H
#define _CELL_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    sk_Object *car;
    sk_Object *cdr;
} sk_Cell;

sk_ObjectType sk_CellType;

// Retreive the car and cdr values of a cell (returned without references).
#define sk_cell_car(obj) (((sk_Cell *)(obj))->car)
#define sk_cell_cdr(obj) (((sk_Cell *)(obj))->cdr)

// Create a new cell object. Both car and cdr are inc_ref'd.
sk_Object *sk_cell_new(sk_Object *car, sk_Object *cdr);

// Overwrite the current car and cdr values of a cell. The old value is
// dec_ref'd, and the new value is inc_ref'd.
void sk_cell_set_car(sk_Object *obj, sk_Object *car);
void sk_cell_set_cdr(sk_Object *obj, sk_Object *cdr);

#endif
