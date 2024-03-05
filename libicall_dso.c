#include <stdio.h>
#include <stdlib.h>

#include "libicall_dso.h"

int __attribute__((visibility("default"))) int_arg(int arg) {
    printf("In %s: (%d)\n", __FUNCTION__, arg);
    return 0;
}

int __attribute__((visibility("default"))) float_arg(float arg) {
    printf("CFI should protect transfer to here\n");
    printf("In %s: (%f)\n", __FUNCTION__, (double)arg);
    return 0;
}

int __attribute__((visibility("default"))) bad_int_arg(int arg) {
    printf("CFI will not protect transfer to here\n");
    printf("In %s: (%d)\n", __FUNCTION__, arg);
    return 0;
}

int __attribute__((visibility("default"))) not_entry_point(int arg) {
    // nop sled for x86 / x86-64
    // these instructions act as a buffer
    // for an indirect control flow transfer to skip
    // a valid function entry point, but continue
    // to execute normal code
    __asm__ volatile (
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
            );
    printf("CFI ensures control flow only transfers to potentially valid destinations\n");
    printf("In %s: (%d)\n", __FUNCTION__, arg);
    // need to exit or the program will segfault anyway,
    // since the indirect call skipped the function preamble
    exit(arg);
}
