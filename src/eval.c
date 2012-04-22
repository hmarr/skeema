#include "eval.h"
#include "builtins.h"
#include "utils.h"
#include "sk_objects/symbol.h"
#include "sk_objects/string.h"
#include "sk_objects/cell.h"
#include "sk_objects/dict.h"
#include "sk_objects/proc.h"
#include "sk_objects/lambda.h"

sk_Object *sk_proc_apply(sk_VM *vm, sk_Object *proc, sk_Object *arg_exp)
{
    // cool, we've got a function
    debug_obj("calling proc: %s", proc);

    // now we need to eval the args, one by one
    sk_Object *args = sk_nil;
    // loop through the list of unevaluated args
    while (arg_exp != sk_nil && sk_object_is(arg_exp, sk_CellType)) {
        // eval the arg
        sk_Object *evald_arg = sk_eval(vm, sk_cell_car(arg_exp));
        if (evald_arg == NULL) {
            // something went wrong up the call stack when evalling
            error_obj("got NULL while evalling %s", arg_exp);
            sk_dec_ref(proc);  // TODO: why is this here?
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
        arg_exp = sk_cell_cdr(arg_exp);
    }

    // we've got our list of evalled args, let's invoke the proc!
    debug_obj("calling with args: %s", args);
    sk_Object *result = sk_proc_fn(proc)(vm, vm->scope, args);
    // now that's out the way we're done with our args
    sk_dec_ref(args);
    return result;
}

sk_Object *sk_lambda_apply(sk_VM *vm, sk_Object *lambda, sk_Object *arg_exp)
{
    // cool, we've got a function
    debug_obj("calling lambda: %s", lambda);

    sk_Object *scope = sk_lambda_scope(lambda);

    sk_Object *arg = arg_exp, *arg_name = sk_lambda_args(lambda);

    while (arg != sk_nil && arg_name != sk_nil) {
        debug_obj("arg: %s", arg);
        debug_obj("arg name: %s", arg_name);
        // eval the arg
        sk_Object *evald_arg = sk_eval(vm, sk_cell_car(arg));
        if (evald_arg == NULL) {
            // something went wrong up the call stack when evalling
            error_obj("got NULL while evalling %s", arg_exp);
            sk_dec_ref(lambda);  // TODO: should this be here?? as above.
            return NULL;
        }

        sk_dict_set(scope, sk_string_cstr(sk_cell_car(arg_name)), evald_arg);
        sk_dec_ref(evald_arg);

        // next arg, repeat the evalling!
        arg = sk_cell_cdr(arg);
        arg_name = sk_cell_cdr(arg_name);
    }

    if (arg != sk_nil) {
        error_obj("too many args provided to %s", lambda);
        return NULL;
    }

    if (arg_name != sk_nil) {
        error_obj("too few args provided to %s", lambda);
        return NULL;
    }

    // we've got our list of evalled args, let's invoke the proc!
    //debug_obj("calling with args: %s", args);
    sk_Object *result = sk_eval(vm, sk_lambda_expr(lambda));

    return result;
}

sk_Object *sk_eval_def(sk_VM *vm, sk_Object *exp)
{
    exp = sk_cell_cdr(exp);  // advance to the name cell
    sk_Object *name = sk_cell_car(exp);
    if (!sk_object_is(name, sk_SymbolType)) {
        error("def arg 1 must be a symbol, not a %s", name->type->name);
        return NULL;
    }

    exp = sk_cell_cdr(exp);  // advance to the value cell
    if (sk_cell_cdr(exp) != sk_nil) {
        error("def takes exactly 2 args");
        return NULL;
    }

    sk_Object *value = sk_eval(vm, sk_cell_car(exp));
    sk_dict_set(vm->scope, sk_symbol_cstr(name), value);
    sk_dec_ref(value);

    return NULL;  // is this the right thing to do?
}

sk_Object *sk_eval_lambda(sk_Object *scope, sk_Object *exp)
{
    // format: (lambda (arg1 arg2) (expr))
    exp = sk_cell_cdr(exp);  // advance to the name cell
    sk_Object *args = sk_cell_car(exp);
    if (!sk_object_is(args, sk_CellType)) {
        error("invalid syntax: lambda expects args, not %s", args->type->name);
        return NULL;
    }

    exp = sk_cell_cdr(exp);  // advance to the value cell
    if (sk_cell_cdr(exp) != sk_nil) {
        error_obj("lambda received unexpected value %s", sk_cell_cdr(exp));
        return NULL;
    }

    sk_Object *func_body = sk_cell_car(exp);
    if (!sk_object_is(func_body, sk_CellType)) {
        error("invalid syntax: lambda expects expression, not %s",
              args->type->name);
        return NULL;
    }

    return sk_lambda_new(scope, args, func_body);
}

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

    // special form: def
    if (sk_symbol_is(vm, sk_cell_car(exp), "def")) {
        return sk_eval_def(vm, exp);
    }

    // special form: quote
    if (sk_symbol_is(vm, sk_cell_car(exp), "quote")) {
        return sk_inc_ref(sk_cell_car(sk_cell_cdr(exp)));
    }

    // special form: lambda
    if (sk_symbol_is(vm, sk_cell_car(exp), "lambda") ||
        sk_symbol_is(vm, sk_cell_car(exp), "->")) {
        return sk_eval_lambda(vm->scope, exp);
    }

    // we've got a list, which means function invocation
    // start by resolving the function
    sk_Object *result, *proc = sk_eval(vm, sk_cell_car(exp));
    if (proc == NULL) {
        // function doesn't exists, but they already got an error from above
        result = NULL;
    } else if (sk_object_is(proc, sk_ProcType)) {
        result = sk_proc_apply(vm, proc, sk_cell_cdr(exp));
    } else if (sk_object_is(proc, sk_LambdaType)) {
        result = sk_lambda_apply(vm, proc, sk_cell_cdr(exp));
    } else {
        error_obj("expected procedure, got %s", proc);
        result = NULL;
    }
    sk_dec_ref(proc);

    return result;
}

