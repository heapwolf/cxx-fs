{
  'includes': [ 'common.gypi' ],
  'targets': [
    {
      'target_name': 'nodeuv-fs',
      'product_name': 'nodeuv-fs',
      'type': 'static_library',
      'sources': [
        'fs.h', 'fs.cc'
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
      ],
    },
    {
      'target_name': 'test',
      'type': 'executable',
      'sources': [
        'http.cc', 'test.cc',
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
        'nodeuv-fs'
      ],
    },
  ],
}

