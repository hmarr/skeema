#ifndef _PROC_H
#define _PROC_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    const char *name;
    int min_arity;
    int max_arity;
    sk_Object *(*fn)(sk_Object *, sk_Object *);
} sk_Proc;

sk_ObjectType sk_ProcType;

#define sk_proc_name_cstr(obj)  (((sk_Proc *)(obj))->name)
#define sk_proc_min_arity(obj) (((sk_Proc *)(obj))->min_arity)
#define sk_proc_max_arity(obj) (((sk_Proc *)(obj))->max_arity)
#define sk_proc_fn(obj)  (((sk_Proc *)(obj))->fn)

// Create a new sk_Int object from a long int.
sk_Object *sk_proc_new(const char *name,
                       sk_Object *(*fn)(sk_Object *, sk_Object *),
                       int min_arity, int max_arity);

#endif
