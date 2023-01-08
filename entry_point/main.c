#include <stdio.h>

#include <sys/mman.h>
#include <errno.h>

const char *instructions = "\x48\x31\xFF\xB8\x3C\x00\x00\x00\x0F\x05";

int main(int argc, char const *argv[])
{
    printf("        main @ %p\n", &main);
    printf("instructions @ %p\n", instructions);

    size_t region = (size_t) instructions;
    printf("region is       %d\n", region);
    printf("instructions is %d\n", &(*instructions));
    region = region & (~0xFFF);

    int ret = mprotect(
        (void *) region,
        0x1000,
        PROT_READ | PROT_EXEC
    );
    if (ret != 0) {
        fprintf(stderr, "mprotect failed: error %d\n", errno);
        return -1;
    }

    void (*f)(void) = (void *) instructions;
    f();

    return 0;
}
