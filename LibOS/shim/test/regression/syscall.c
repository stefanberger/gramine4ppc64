#include <sys/syscall.h>
#if defined(__i386__) || defined(__x86_64__)
#include <sysdep-x86_64.h>
#elif defined(__powerpc64__)
#include <sysdep-ppc64.h>
#endif

int main(int argc, char** argv) {
    const char buf[] = "Hello world\n";
    INLINE_SYSCALL(write, 3, 1, buf, sizeof(buf) - 1);
    return 0;
}
