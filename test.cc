#include "fs.h"

using namespace fs;

int main() {

  auto text = "Hello, World";

  fs.writeFile("./foo.txt", text, [](auto &err, &bytes) {

    fs.readFile("./foo.txt", [](auto &err, &data) {
      cout << data << endl;
    });
  });

  cout << fs.readStream("bar");
}

