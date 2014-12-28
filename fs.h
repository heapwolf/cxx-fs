#ifndef NODEUV_FS_H
#define NODEUV_FS_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

extern "C" {
#include "uv.h"
}

namespace fs {

  using namespace std;

  class Readable;
  class Writable;
  class Error;

  // https://github.com/joyent/node/blob/master/lib/fs.js#L256
  const unsigned int MAGIC_BUFFER_SIZE = 8192;

  #define STAT_GET_DOUBLE(name) \
    static_cast<double>(s->st_##name)

  #define STAT_GET_DATE(name) \
    (static_cast<double>(s->st_##name.tv_sec) * 1000) + \
    (static_cast<double>(s->st_##name.tv_nsec / 1000000))

  struct Stats {
    int dev;
    int mode;
    int nlink;
    int uid;
    int gid;
    int rdev;
    double size;
    double ino;
    double atime;
    double mtime;
    double ctime;
  };

  template <typename... Args>
  using Callback = function<void(Args... params)>;

  class Error {
    public:
      string message = "";
      int code = 0;

      inline operator bool() const {
        return (message.length() > 0 || code != 0);
      }
  };

  //
  // Some reasonable defaults for Read and Write options...
  //
  struct ReadOptions {
    int flags = O_CREAT | O_RDWR;
    int mode = S_IRUSR | S_IWUSR;
  };

  struct WriteOptions {
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  };

  class Filesystem {
    public:
      uv_loop_t* UV_LOOP;
      bool running = false;

      string cwd();

      void stat(const char*, Callback<Error, Stats>);
      Stats statSync(const char* path);

      Error mkdirSync(const char* path);
      Error mkdirSync(const char* path, int mode);

      Error rmdirSync(const char* path);

      void open(const char*, int, int, Callback<Error, uv_file>);
      int openSync(const char*, int, int);

      void read(uv_file, int64_t, int64_t, Callback<Error, uv_buf_t>);
      int readSync(uv_file fd, uv_buf_t* buffer, int64_t offset, int64_t bytes);

      void write(uv_file, uv_buf_t, int64_t, Callback<Error>);
      
      void close(uv_file fd, Callback<Error>);
      int closeSync(uv_file fd);

      // 
      // Higher level functions
      //
      void readFile(const char*, Callback<Error, string>);
      void readFile(const char*, ReadOptions, Callback<Error, string>);

      uv_buf_t readFileSync(const char*, ReadOptions);
      uv_buf_t readFileSync(const char*);

      void writeFile(const char*, string, Callback<Error>);
      void writeFile(const char*, string, WriteOptions, Callback<Error>);
      void writeFile(const char*, uv_buf_t, Callback<Error>);
      void writeFile(const char*, uv_buf_t, WriteOptions, Callback<Error>);

      Filesystem() {
        UV_LOOP = uv_default_loop();
      };
  };
} // namespace fs;

#endif

