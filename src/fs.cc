#include "../fs.h"

namespace fs {

  // @TODO
  // explore this windows bug...
  //
  // #if 0
  //   //It will crash on Windows7, but work on Linux
  //   uv_write_t write_req;
  // #else
  //   uv_write_t *write_req = new uv_write_t();
  // #endif

  void Filesystem::read(uv_file fd, int64_t bytes, int64_t offset, Callback<Error, uv_buf_t> cb) {

    uv_fs_t read_req;
    uv_buf_t buffer;
    buffer.base = (char *) malloc(bytes);
    buffer.len = bytes;
    Error err;

    static function<void(uv_fs_t* req)> on_read;

    on_read = [&](uv_fs_t* req) {
      auto result = req->result;
      uv_fs_req_cleanup(req);

      if (result > 0) {
        cb(err, buffer);
      }
      else if (result < 0) {
        err.message = uv_err_name(result);
        err.code = result;
        cb(err, buffer);
      }
    };

    uv_fs_read(UV_LOOP, &read_req, fd, &buffer, 1, offset, [](uv_fs_t* req) {
      on_read(req);
    });

    if (!running) {
      uv_run(UV_LOOP, UV_RUN_DEFAULT);
    }
  }


  void Filesystem::write(uv_file fd, uv_buf_t buffer, int64_t offset, Callback<Error> cb) {

    uv_fs_t write_req;
    Error err;

    static function<void(uv_fs_t* req)> on_write;

    on_write = [&](uv_fs_t* req) {
      auto result = req->result;
      uv_fs_req_cleanup(req);

      if (result < 0) {
        err.message = uv_err_name(result);
        err.code = result;
      }
      cb(err);
    };

    uv_fs_write(UV_LOOP, &write_req, fd, &buffer, 1, offset, [](uv_fs_t* req) {
      on_write(req);
    });

    if (!running) {
      uv_run(UV_LOOP, UV_RUN_DEFAULT);
    }
  }


  void Filesystem::open(const char* path, int flags, int mode, Callback<Error, uv_file> cb) {

    uv_fs_t open_req;
    static function<void(uv_fs_t* req)> on_open;
    Error err;

    on_open = [&](uv_fs_t* req) {
      auto result = req->result;
      uv_fs_req_cleanup(req);

      if (result == -1) {
        err.message = uv_err_name(result);
        err.code = result;
      }
      cb(err, result);
    };

    uv_fs_open(UV_LOOP, &open_req, path, flags, mode, [](uv_fs_t* req) {
      on_open(req);
    });

    if (!running) {
      uv_run(UV_LOOP, UV_RUN_DEFAULT);
    }
  }


  void Filesystem::close(uv_file fd, Callback<Error> cb) {
    uv_fs_t close_req;
    static function<void(uv_fs_t* req)> on_close;
    Error err;

    on_close = [&](uv_fs_t* req) {
      int result = req->result;
      if (result == -1) {
        err.message = uv_err_name(result);
        err.code = result;
      }
      cb(err);
    };

    uv_fs_close(UV_LOOP, &close_req, fd, [](uv_fs_t* req) {
      on_close(req);
    });

    if (!running) {
      uv_run(UV_LOOP, UV_RUN_DEFAULT);
    }
  }


  void Filesystem::stat(const char* path, Callback<Error, Stats> cb) {

    uv_fs_t stat_req;
    static function<void(uv_fs_t* req)> on_stat;
    Error err;

    on_stat = [&](uv_fs_t* req) {
      int result = req->result;
      Stats stats;

      if (result >= 0) {

        auto s = static_cast<const uv_stat_t*>(req->ptr);

        stats.dev = s->st_dev;
        stats.mode = s->st_mode;
        stats.nlink = s->st_nlink;
        stats.uid = s->st_uid;
        stats.gid = s->st_gid;
        stats.rdev = s->st_rdev;
        stats.size = STAT_GET_DOUBLE(size);
        stats.ino = STAT_GET_DOUBLE(ino);
        stats.atime = STAT_GET_DATE(atim);
        stats.mtime = STAT_GET_DATE(mtim);
        stats.ctime = STAT_GET_DATE(ctim);
      }

      uv_fs_req_cleanup(req);
  
      if (result >= 0) {
       cb(err, stats);
      }
      else {
        err.message = uv_err_name(result);
        err.code = result;
        cb(err, stats);
      }
    };

    uv_fs_stat(UV_LOOP, &stat_req, path, [](uv_fs_t* req) {
      on_stat(req);
    });

    if (!running) {
      uv_run(UV_LOOP, UV_RUN_DEFAULT);
    }
  }


  void Filesystem::readFile(const char* path, ReadOptions opts, Callback<Error, string> cb) {

    stat(path, [&](auto err, auto stats) {

      if (err) {
        cb(err, "");
        return;
      }
 
      auto size = stats.size;

      open(path, opts.flags, opts.mode, [&](auto err, auto fd) {

        if (err) {
          cb(err, "");
          return;
        }

        int64_t offset = 0;
        stringstream ss;
        static function<void()> reader;

        reader = [&]() {

          if (offset < size) {
            auto readsize = MAGIC_BUFFER_SIZE;
            auto diff = size - offset;

            if (readsize > diff) {
              readsize = diff;
            }

            read(fd, size, offset, [&](auto err, auto buf) {
              offset = offset + buf.len;
              ss << string(buf.base);
              free(buf.base);
              reader();
            });
            return;
          }
          else {
            close(fd, [&](auto err) {
              cb(err, ss.str().substr(0, size));
            });
          }
        }; 

        reader();
      });

    });

  }


  void Filesystem::readFile(const char* path, Callback<Error, string> cb) {
    ReadOptions opts;
    readFile(path, opts, cb);
  }


  void Filesystem::writeFile(const char* path, uv_buf_t buffer, WriteOptions opts, Callback<Error> cb) {

    open(path, opts.flags, opts.mode, [&](auto err, auto fd) {

      if (err) {
        cb(err);
        return;
      }

      write(fd, buffer, 0, [&](auto err) {

        if (err) {
          cb(err);
          return;
        }

        close(fd, cb);

      });
    });

  }


  void Filesystem::writeFile(const char* path, uv_buf_t buffer, Callback<Error> cb) {
    WriteOptions opts;
    writeFile(path, buffer, opts, cb);
  }


  void Filesystem::writeFile(const char* path, string s, Callback<Error> cb) {
    WriteOptions opts;
    writeFile(path, s, opts, cb);
  }


  void Filesystem::writeFile(const char* path, string s, WriteOptions opts, Callback<Error> cb) {

    char b[s.size()];
    strcpy(b, s.c_str());
    uv_buf_t buffer;
    buffer = uv_buf_init(b, sizeof(b));
  
    writeFile(path, buffer, opts, cb);  
  }

} // namespace fs

