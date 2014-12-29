#include <string>
#include <iostream>
 
extern "C" {
#include "uv.h"
}
 
using namespace std;
 
int main() {
 
  uv_loop_t* UV_LOOP;
  UV_LOOP = uv_default_loop();
 
  int n = 2000;
  int flags = O_CREAT | O_RDWR;
  int mode = S_IRUSR | S_IWUSR;
  bool fail = false;
 
  string sillystring = "#include <functional>\n"
  "#include <iostream>\n"
  "    class {\n"
  "    class {\n"
  "#define place string(\"world\")\n"
  "\n"
  "#define bar()\\\n"
  "      cout << 10 << endl;\n"
  "\n"
  "    public:\n"
  "      string message = \"hello \" + place;\n"
  "\n"
  "#undef place\n"
  "#undef bar\n"
  "    } b;\n"
  "\n"
  "\n"
  "      public:\n"
  "      void print() {\n"
  "            cout << b.message << endl;\n"
  "              }\n"
  "\n"
  "    } bla;\n"
  "\n"
  "\n"
  "  int main() {\n"
  "\n"
  "      bla.print();\n"
  "  }\n";
 
  const char* path = "./out.txt";

  while(n--) {
 
    uv_fs_t stat_req;
    uv_fs_t open_req;
    uv_fs_t read_req;
    uv_fs_t close_req;
 
    int statR = uv_fs_stat(UV_LOOP, &stat_req, path, NULL);
    const uv_stat_t* stat = static_cast<const uv_stat_t*>(stat_req.ptr);
    int fd = uv_fs_open(UV_LOOP, &open_req, path, flags, mode, NULL);

    int size = (int) stat->st_size;
    int offset = 0;
 
    uv_buf_t buf;
    buf.base = (char *) malloc(size);
    buf.len = size;
 
    int readR = uv_fs_read(UV_LOOP, &read_req, fd, &buf, 1, offset, NULL);
    int closeR = uv_fs_close(UV_LOOP, &close_req, fd, NULL);

    if (stat_req.result < 0) {
      cout << uv_err_name(stat_req.result) << endl;
      fail = true;
      break;
    }

    if (open_req.result < 0) {
      cout << uv_err_name(open_req.result) << endl;
      fail = true;
      break;
    }
    
    if (read_req.result < 0) {
      cout << uv_err_name(read_req.result) << endl;
      fail = true;
      break;
    } 

    if (close_req.result < 0) {
      cout << uv_err_name(close_req.result) << endl;
      fail = true;
      break;
    } 

    uv_fs_req_cleanup(&stat_req);
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);
    uv_fs_req_cleanup(&close_req);
 
    if (sillystring != buf.base) {
      fail = true;
      cout << read_req.result << " " << size << endl;
      break;
    }
  }

  cout << (fail ? "fail" : "success") << endl;
 
  uv_run(UV_LOOP, UV_RUN_DEFAULT);
}

