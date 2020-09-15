common_lib = static_library('common_lib',
    '../common/common.c',
    'test_exit.S',
)

tests = {
    'stack': {
        'filenames': ['stack.S'],
    },
    'stack_auxiliary': {
        'filenames': [
            '../common/stack_auxiliary.c',
            'call_auxiliary.S',
        ],
    },
    'stack_env': {
        'filenames': [
            'stack_env.S',
            'verify_env.c',
        ],
    },
}

install_dir = join_paths(pkglibdir, 'tests', 'libos', 'entrypoint')

foreach name, params : tests
    filenames = ''
    if (params.has_key('filenames'))
        filenames = params.get('filenames')
    else
        filenames = '@0@.c'.format(name)
    endif

    exe = executable(name,
        filenames,

        link_with: [
            params.get('link_with', [common_lib]),
        ],

        link_args: params.get('link_args', ['-nostdlib', '-static']),
        include_directories: params.get('include_directories', ['../common']),

        install: true,
        install_dir: install_dir,
    )
endforeach
