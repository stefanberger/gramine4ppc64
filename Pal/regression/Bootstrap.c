#include "pal.h"
#include "pal_regression.h"

int main(int argc, char** argv, char** envp) {
    /* check if the program is loaded */
    pal_printf("User Program Started\n");

    /* check control block */
    /* check arguments */
    pal_printf("# of Arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        pal_printf("argv[%d] = %s\n", i, argv[i]);
    }

    /* parent process */
    const struct pal_public_state* pal_public_state = DkGetPalPublicState();
    pal_printf("Parent Process: %p\n", pal_public_state->parent_process);

    /* test debug stream */
    const char* msg = "Written to Debug Stream\n";
    int ret = DkDebugLog((char*)msg, strlen(msg));
    if (ret < 0) {
        pal_printf("Failed to write the debug message.\n");
        return 1;
    }

    /* Allocation Alignment */
    pal_printf("Allocation Alignment: %ld\n", pal_public_state->alloc_align);

    /* user address range */
    pal_printf("User Address Range: %p - %p\n", pal_public_state->user_address.start,
               pal_public_state->user_address.end);

    if (pal_public_state->user_address.start && pal_public_state->user_address.end &&
        pal_public_state->user_address.start < pal_public_state->user_address.end)
        pal_printf("User Address Range OK\n");

    const struct pal_cpu_info* ci = &pal_public_state->cpu_info;
    pal_printf("CPU num: %zu\n",      pal_public_state->topo_info.online_logical_cores_cnt);
#if defined(__i386__) || defined(__x86_64__)
    pal_printf("CPU vendor: %s\n",    ci->cpu_vendor);
    pal_printf("CPU brand: %s\n",     ci->cpu_brand);
    pal_printf("CPU family: %ld\n",   ci->cpu_family);
    pal_printf("CPU model: %ld\n",    ci->cpu_model);
    pal_printf("CPU stepping: %ld\n", ci->cpu_stepping);
    pal_printf("CPU flags: %s\n",     ci->cpu_flags);
#elif defined(__powerpc64__)
    pal_printf("CPU cpu: %s\n", ci->cpu);
    pal_printf("CPU clock: %s\n", ci->clock);
    pal_printf("CPU revision: %s\n", ci->revision);
    pal_printf("CPU timebase: %s\n", ci->timebase);
    pal_printf("CPU platform: %s\n", ci->platform);
    pal_printf("CPU model: %s\n", ci->model);
    pal_printf("CPU machine: %s\n", ci->machine);
    pal_printf("CPU firmware: %s\n", ci->firmware);
    pal_printf("CPU MMU: %s\n", ci->mmu);
#else
#error Unsupported architecture
#endif

    return 0;
}
