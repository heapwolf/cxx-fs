#include "../fs.h"
#include <assert.h>
#include <unistd.h>

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

  ASSERT("sanity: true is false", true == false);
  ASSERT("sanity: true is true", true == true);

  Filesystem fs;

  cout << fs.cwd() << endl;


  auto st = fs.statSync("./test.cc");
  ASSERT("this file should be found by statSync", st.mode != 0);
  st = fs.statSync("..");
  ASSERT("the parent directory should be found by statSync", st.mode != 0);
  st = fs.statSync("../bla");
  ASSERT("a directory that does not exist should not be found by statSync", st.mode == 0);


  Error createError = fs.mkdirSync("./foo");
  ASSERT("directory was created", createError == false);
  st = fs.statSync("./foo");
  ASSERT("directory exists", st.mode != 0);

  Error deleteError = fs.rmdirSync("./foo");
  ASSERT("directory was deleted", deleteError == false);
  st = fs.statSync("./foo");
  ASSERT("directory no longer exists", st.mode == 0);


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

