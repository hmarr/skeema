#include "builtins.h"
#include "utils.h"
#include "sk_objects/string.h"
#include "sk_objects/int.h"
#include "sk_objects/cell.h"
#include "sk_objects/float.h"
#include "sk_objects/float.h"
#include "sk_objects/dict.h"
#include "sk_objects/proc.h"

sk_Object *sk_builtins_sum(sk_Object *scope, sk_Object *args)
{
    bool return_float = false;
    double float_sum = 0.0;
    long int_sum = 0;

    while (args != sk_nil) {
        sk_Object *item = sk_cell_car(args);
        if (sk_object_is(item, sk_IntType)) {
            int_sum += sk_int_val(item);
        } else if (sk_object_is(item, sk_FloatType)) {
            float_sum += sk_float_val(item);
            return_float = true;
        } else {
            error_obj("invalid argument to sum(): %s", item);
            return NULL;
        }
        args = sk_cell_cdr(args);
    }

    if (return_float) {
        return sk_float_new(float_sum + int_sum);
    }
    return sk_int_new(int_sum);
}

sk_Object *sk_builtins_print_scope(sk_Object *scope, sk_Object *args)
{
    sk_dict_print(scope);
    return NULL;
}

void sk_builtins_populate_scope(sk_Object *scope)
{
    sk_Object *builtin;

    builtin = sk_proc_new("+", &sk_builtins_sum, 0, -1);
    sk_dict_set(scope, "+", builtin);
    sk_dec_ref(builtin);

    builtin = sk_proc_new("print-scope", &sk_builtins_print_scope, 0, 0);
    sk_dict_set(scope, "print-scope", builtin);
    sk_dec_ref(builtin);
}

