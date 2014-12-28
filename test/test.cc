#include "../fs.h"
#include <assert.h>
#include <unistd.h>
#include <execinfo.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

using namespace std;
using namespace fs;

#define ASSERT(message, ...) do { \
  if(!(__VA_ARGS__)) { \
    cerr << "FAIL: " << message << endl; \
  } \
  else { \
    cout << "OK: " << message << endl; \
  } \
} while(0);

void backtrace_handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}


int main() {

  signal(SIGSEGV, backtrace_handler);

  ASSERT("sanity: true is false", true == false);
  ASSERT("sanity: true is true", true == true);

  Filesystem fs;

  cout << fs.cwd() << endl;


  auto st = fs.statSync("./test.cc");
  ASSERT("this file should be found by statSync", st.mode != 0);
  st = fs.statSync("..");
  ASSERT("the parent directory should be found by statSync", st.mode != 0);
  try {
    st = fs.statSync("../bla");
  }
  catch(runtime_error &e) {
    ASSERT("a directory that does not exist should not be found by statSync", true);
  }


  Error createError = fs.mkdirSync("./foo");
  ASSERT("directory was created", createError == false);
  st = fs.statSync("./foo");
  ASSERT("directory exists", st.mode != 0);

  Error deleteError = fs.rmdirSync("./foo");
  ASSERT("directory was deleted", deleteError == false);
  try {
    st = fs.statSync("./foo"); 
  }
  catch(runtime_error &e) {
    ASSERT("directory no longer exists", true);
  }


  st = fs.statSync("./test.txt");
  uv_buf_t buf = fs.readFileSync("./test.txt");
  ASSERT("stat size should be the buf size returned by readFileSync", st.size == buf.len);


  fs.readFile("test.txt", [&](auto err, auto data) {

    ASSERT("a file should be opened without an error", err == false);
    ASSERT("the length of the data returned by the callback"
           "should match the length of the file", data.length() == 5834);

string s = "#include <functional>\n"
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

    fs.writeFile("out.txt", s, [&](auto err) {
      fs.readFile("out.txt", [&](auto err, auto data) {
        ASSERT("out.txt should be exactly the same", s.length() == data.length());
      });
    });
  });

}

