#include "eval.h"
#include "builtins.h"
#include "utils.h"
#include "sk_objects/symbol.h"
#include "sk_objects/string.h"
#include "sk_objects/cell.h"
#include "sk_objects/dict.h"
#include "sk_objects/proc.h"

sk_Object *sk_eval(sk_VM *vm, sk_Object *exp)
{
    debug_obj("eval: %s", exp);

    if (sk_object_is(exp, sk_SymbolType)) {
        // direct eval() on symbol means we look it up
        debug_obj("symbol lookup: %s", exp);
        sk_Object *var = sk_dict_get(vm->scope, sk_symbol_cstr(exp));
        if (var == NULL) {
            // symbol not in present in scope
            error_obj("undefined identifier: %s", exp);
        }
        return sk_inc_ref(var);
    }

    if (!sk_object_is(exp, sk_CellType)) {
        // only symbols and cells (lists) are special, return values as is
        debug_obj("atom: %s", exp);
        return sk_inc_ref(exp);
    }

    // we've got a list, which means function invocation
    // start by resolving the function
    sk_Object *proc = sk_eval(vm, sk_cell_car(exp));
    if (proc == NULL) {
        // function doesn't exists, but they already got an error from above
        return NULL;
    } else if (sk_object_is(proc, sk_ProcType)) {
        // cool, we've got a function
        debug_obj("calling proc: %s", proc);

        // now we need to eval the args, one by one
        sk_Object *args = sk_nil, *cur_arg = sk_cell_cdr(exp);
        // loop through the list of unevaluated args
        while (cur_arg != sk_nil && sk_object_is(cur_arg, sk_CellType)) {
            // eval the arg
            sk_Object *evald_arg = sk_eval(vm, sk_cell_car(cur_arg));
            if (evald_arg == NULL) {
                // something went wrong up the call stack when evalling
                error_obj("got NULL while evalling %s", cur_arg);
                sk_dec_ref(proc);
                return NULL;
            }

            if (args == sk_nil) {
                // start of the arg list, create the first element
                args = sk_cell_new(evald_arg, sk_nil);
            } else {
                // arg list already exists, append to the back
                sk_cell_append(args, evald_arg);
            }
            // evald arg is part of arg list now, doesn't need us any more
            sk_dec_ref(evald_arg);

            // next arg, repeat the evalling!
            cur_arg = sk_cell_cdr(cur_arg);
        }

        // we've got our list of evalled args, let's invoke the proc!
        debug_obj("calling with args: %s", args);
        sk_Object *result = sk_proc_fn(proc)(vm->scope, args);
        // now that's out the way we're done with our proc and args
        sk_dec_ref(proc);
        sk_dec_ref(args);
        return result;
    }

    error_obj("expected procedure, got %s", proc);
    return NULL;
}

