{
  'includes': [ 'common.gypi' ],
  'targets': [
    {
      'target_name': 'nodeuv-fs',
      'product_name': 'nodeuv-fs',
      'type': 'static_library',
      'sources': [
        'fs.h',
        './src/fs.cc'
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
      ],
    },
    {
      'target_name': 'test',
      'type': 'executable',
      'sources': [
        './src/fs.cc',
        './test/test.cc',
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
        'nodeuv-fs'
      ],
    },
    {
      'target_name': 'sanity',
      'type': 'executable',
      'sources': [
        './src/fs.cc',
        './test/sanity.cc',
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
        'nodeuv-fs'
      ],
    },
  ],
}

