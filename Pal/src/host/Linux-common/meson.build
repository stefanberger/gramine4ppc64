pal_linux_common_sources_enclave = files(
    'bogomips.c',
    'gramine_unix_socket_addr.c',
)
pal_linux_common_sources_urts = files(
    'command.c',
    'debug_map.c',
    'file_utils.c',
    'main_exec_path.c',
    'proc_maps.c',
    'timespec_utils.c',
    'topo_info.c',
)

pal_linux_common_sources_urts_arch = [
    'syscall.S',
]
if host_machine.cpu_family() == 'ppc64le' or host_machine.cpu_family() == 'ppc64'
    pal_linux_common_sources_urts_arch += 'clone.S'
    pal_linux_common_sources_urts_arch += 'vfork.c'
endif
foreach src : pal_linux_common_sources_urts_arch
    pal_linux_common_sources_urts += files(join_paths('arch', host_machine.cpu_family(), src))
endforeach

pal_linux_common_sources = [
    pal_linux_common_sources_enclave,
    pal_linux_common_sources_urts,
]
