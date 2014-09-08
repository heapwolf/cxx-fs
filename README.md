# SYNOPSIS

# USAGE
```cpp
#include "fs.h"

using namespace fs;

int main() {

  auto text = "Hello, World";

  //
  // Async write and read...
  //
  fs.writeFile("./foo.txt", text, [](auto &err, &bytes) {

    fs.readFile("./foo.txt", [](auto &err, &data) {
      cout << data << endl;
    });
  });

  //
  // A readable stream...
  //
  cout << fs.readStream("bar");
}
```

