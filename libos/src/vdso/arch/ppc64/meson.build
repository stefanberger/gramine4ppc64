vdso_so_dbg = shared_library('vdso',
    files('vdso.c', 'vdso.S'),
    name_prefix: '',
    name_suffix: 'so.dbg',

    # TODO this should only include gramine_entry_api.h and files defining necessary types
    include_directories: includes_libos,

    c_args: [
        '-fno-stack-protector',
        '-fPIC',
        '-fno-builtin',
    ],
    link_args: [
        '-nostdlib',
        '-Wl,-T@0@'.format(join_paths(meson.current_source_dir(), 'vdso.lds')),
        '-Wl,--hash-style=both',
        '-Wl,-Bsymbolic',
        '-Wl,--no-undefined',
        '-Wl,-zmax-page-size=65536',
        '-Wl,-zcommon-page-size=65536',
        '-Wl,--soname=linux-vdso64.so.1',
    ],
    link_depends: ['vdso.lds'],
)

vdso_not_checked_so = custom_target('vdso_not_checked.so',
    command: [objcopy, '-S', '@INPUT@', '@OUTPUT@'],
    input: vdso_so_dbg,
    output: 'vdso_not_checked.so',
)

# vdso.so is required to have no relocations. This rule checks it, and if good, copies to vdso.so.
vdso_so = custom_target('vdso.so',
    command: [check_no_reloc_prog, '@INPUT@', '@OUTPUT@'],
    input: vdso_not_checked_so,
    output: 'vdso.so',
)
