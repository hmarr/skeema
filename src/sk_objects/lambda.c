#include <stdlib.h>
#include <stdio.h>

#include "sk_objects/lambda.h"
#include "sk_objects/string.h"
#include "sk_objects/cell.h"

sk_Object *sk_lambda_new(sk_Object *scope, sk_Object *args, sk_Object *expr)
{
    sk_new_object_init(sk_Lambda);
    obj->type = &sk_LambdaType;
    obj->scope = sk_inc_ref(scope);
    obj->args = sk_inc_ref(args);
    obj->expr = sk_inc_ref(expr);
    return (sk_Object *)obj;
}

sk_Object *sk_lambda_to_string(sk_Object *obj)
{
    sk_Object *arg_str = sk_object_to_string(sk_lambda_args(obj));
    char *buf = (char *)malloc(12 + sk_string_length(arg_str));
    sprintf(buf, "#<lambda>(%s)", sk_string_cstr(arg_str));
    sk_dec_ref(arg_str);
    sk_Object *str_obj = sk_string_new(buf);
    free(buf);
    return str_obj;
}

void sk_lambda_dealloc(sk_Object *obj)
{
    sk_dec_ref(sk_lambda_scope(obj));
    sk_dec_ref(sk_lambda_args(obj));
    sk_dec_ref(sk_lambda_expr(obj));
}

sk_ObjectType sk_LambdaType = {
    "lambda",             // name
    &sk_lambda_dealloc,   // custom dealloc
    &sk_lambda_to_string, // to_string
};

