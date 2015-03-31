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

namespace nodeuv {

  using namespace std;

  class Readable;
  class Writable;
  class Error;

  // https://github.com/iojs/io.js/blob/master/lib/fs.js#L256
  const unsigned int MAGIC_BUFFER_SIZE = 8192;

  struct Stats {
    uint64_t dev;
    uint64_t mode;
    uint64_t nlink;
    uint64_t uid;
    uint64_t gid;
    uint64_t rdev;
    uint64_t ino;
    uint64_t size;
    uint64_t blksize;
    uint64_t blocks;
    uint64_t flags;
    uint64_t gen;
    uv_timespec_t atime;
    uv_timespec_t mtime;
    uv_timespec_t ctime;
    uv_timespec_t birthtime;
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
  // TODO
  // move out into another lib.
  //

#define MIN(a, b) ((a) < (b) ? (a) : (b))

  class Buffer {

    public:

      uv_buf_t data;

      string toString() {
        string s;
        s.assign(data.base, data.len);
        return s;
      }

      int length() {
        return data.len;
      }

      int copy(Buffer b) {
        return this->_copy(b, 0, b.length(), 0);
      }

      int copy(Buffer b, int target_start) {
        return this->_copy(b, target_start, b.length(), 0);
      }

      int copy(Buffer b, int target_start, int source_start) {
        return this->_copy(b, target_start, source_start, 0);
      }

      int _copy(Buffer b, int target_start, int source_start, int source_end) {

        size_t obj_length = b.length();
        size_t target_length = this->length();
        char* target_data = this->data.base;

        if (target_start >= target_length || source_start >= source_end) {
          return 0;
        }

        if (source_start > obj_length) {
          throw runtime_error("out of range index");
        }

        uint32_t to_copy = MIN(MIN(source_end - source_start,
          target_length - target_start),
          obj_length - source_start);

        memmove(this->data + target_start, b->data + source_start, to_copy);
        return to_copy;
      }

      Buffer(int size) {
        data.base = (char *) malloc(size);
        data.len = size;
      }

      Buffer(string str) {
        data = uv_buf_init((char*) str.c_str(), str.length());
      }

      Buffer() {
      }

      Buffer(const Buffer &buf) {
        data = buf.data;
      }

      Buffer& operator= (const Buffer &buf);
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
      uv_buf_t createBuffer(string s);

      void stat(string, Callback<Error, Stats>);
      Stats statSync(string);

      Error mkdirSync(string);
      Error mkdirSync(string, int);

      Error rmdirSync(string);

      void open(string, int, int, Callback<Error, uv_file>);
      int openSync(string, int, int);

      void read(uv_file, int64_t, int64_t, Callback<Error, uv_buf_t>);
      int readSync(uv_file fd, uv_buf_t* buffer, int64_t offset, int64_t bytes);

      void write(uv_file, uv_buf_t, int64_t, Callback<Error>);
      int writeSync(uv_file fd, Buffer buffer, int64_t offset, int64_t length);

      void close(uv_file fd, Callback<Error>);
      int closeSync(uv_file fd);

      // 
      // Higher level functions
      //
      void readFile(string, Callback<Error, Buffer>);
      void readFile(string, ReadOptions, Callback<Error, Buffer>);

      Buffer readFileSync(string, ReadOptions);
      Buffer readFileSync(string);

      void writeFile(string, string, Callback<Error>);
      void writeFile(string, string, WriteOptions, Callback<Error>);
      void writeFile(string, uv_buf_t, Callback<Error>);
      void writeFile(string, uv_buf_t, WriteOptions, Callback<Error>);
      int writeFileSync(string, Buffer);
      int writeFileSync(string, Buffer, WriteOptions);

      Filesystem() {
        UV_LOOP = uv_default_loop();
      };
  };
} // namespace fs;

#endif

