#ifndef _LAMBDA_H
#define _LAMBDA_H

#include "sk_objects/object.h"

typedef struct {
    sk_ObjectHeader;
    sk_Object *scope;
    sk_Object *args;
    sk_Object *expr;
} sk_Lambda;

sk_ObjectType sk_LambdaType;

#define sk_lambda_scope(obj) (((sk_Lambda *)(obj))->scope)
#define sk_lambda_args(obj)  (((sk_Lambda *)(obj))->args)
#define sk_lambda_expr(obj)  (((sk_Lambda *)(obj))->expr)

sk_Object *sk_lambda_new(sk_Object *scope, sk_Object *args, sk_Object *expr);

#endif
