#include <stdint.h>
#include <stdio.h>

#include "libicall_dso.h"

typedef int (*int_arg_fn)(int);
typedef int (*float_arg_fn)(float);

struct foo {
    int_arg_fn int_funcs[1];
    int_arg_fn bad_int_funcs[1];
    float_arg_fn float_funcs[1];  
    int_arg_fn not_entries[1];
};

// the struct aligns the function pointer arrays
// so indexing past the end will reliably
// call working function pointers
static struct foo f = { 
    .int_funcs = {int_arg},
    .bad_int_funcs = {bad_int_arg},
    .float_funcs = {float_arg},
    .not_entries = {(int_arg_fn)((uintptr_t)(not_entry_point)+0x20)}
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
int main(int argc, const char *argv[]) {
#pragma clang diagnostic pop
    int idx;

    if(argc != 2) {
        printf("Usage: %s <option>\n", argv[0]);
        printf("Option values:\n");
        printf("\t0\tCall correct function\n");
        printf("\t1\tCall the wrong function but with the same signature\n");
        printf("\t2\tCall a float function with an int function signature\n");
        printf("\t3\tCall into the middle of a function\n");
        printf("\n");
        printf("\tAll other options are undefined, but should be caught by CFI :)\n");
        printf("\n\n");
        printf("Here are some pointers so clang doesn't optimize away members of `struct foo f`:\n");
        printf("\tint_funcs: %p\n", (void*)f.int_funcs);
        printf("\tbad_int_funcs: %p\n", (void*)f.bad_int_funcs);
        printf("\tfloat_funcs: %p\n", (void*)f.float_funcs);
        printf("\tnot_entries: %p\n", (void*)f.not_entries);
        return 1;
    }

    printf("Calling a function:\n");

    idx = argv[1][0] - '0';

    return f.int_funcs[idx](idx);
}
