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

int main() {

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

  //
  // sanity test.
  //
  ASSERT("sanity: true is false", true == false);
  ASSERT("sanity: true is true", true == true);
 
  Filesystem fs;

  //
  // get the currect working directory.
  //
  cout << fs.cwd() << endl;


  //
  // stat
  //
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

  //
  // mkdir
  //
  Error createError = fs.mkdirSync("./foo");
  ASSERT("directory was created", createError == false);
  st = fs.statSync("./foo");
  ASSERT("directory exists", st.mode != 0);

  //
  // rmdir
  //
  Error deleteError = fs.rmdirSync("./foo");
  ASSERT("directory was deleted", deleteError == false);
  try {
    st = fs.statSync("./foo"); 
  }
  catch(runtime_error &e) {
    ASSERT("directory no longer exists", true);
  }

  //
  // writeSync
  //
  //static auto writeBuffer = fs.createBuffer(s);

  static uv_buf_t writeBuffer;
  writeBuffer = uv_buf_init((char*) sillystring.c_str(), sillystring.length());
  int bytesWritten = fs.writeFileSync("./testwrite.txt", &writeBuffer);
  ASSERT("bytesWritten should be the size of the string written to writeFileSync", bytesWritten == sillystring.size());

  //
  // readSync
  //
  st = fs.statSync("./test.txt");
  static uv_buf_t readBuffer;
  readBuffer = fs.readFileSync("./test.txt");
  ASSERT("stat size should be the buf size returned by readFileSync", st.size == readBuffer.len);
  ASSERT("strings should be the same", bool(sillystring == readBuffer.base));

  //
  // read and write
  //
  fs.readFile("test.txt", [&](auto err, auto data) {

    ASSERT("a file should be opened without an error", err == false);

    fs.writeFile("out.txt", sillystring, [&](auto err) {
      fs.readFile("out.txt", [&](auto err, auto data) {
        ASSERT("out.txt should be exactly the same", sillystring.length() == data.length());
      });
    });
  });

}

