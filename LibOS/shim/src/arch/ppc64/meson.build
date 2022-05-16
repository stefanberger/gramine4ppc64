libos_sources_arch = files(
    'emulate_syscall_wrapper.S',
    'rt_sigreturn_wrapper.S',
    'shim_context.c',
    'shim_elf_entry.S',
    'shim_table.c',
    'start.S',
    'syscallas.S',
)

shim_lds = join_paths(meson.current_source_dir(), 'shim.lds')
