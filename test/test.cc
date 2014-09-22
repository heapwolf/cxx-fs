#include "../fs.h"
#include <assert.h>

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

  fs.readFile("test.txt", [&](auto err, auto data) {

    ASSERT("a file should be opened without an error", err == false);
    ASSERT("the length of the data returned by the callback"
           "should match the length of the file", data.length() == 5834);

    fs.writeFile("out.txt", Buffer("hello"), [&](auto err) {
      cout << "OK" << endl;
    });

  });
}

