#include "../fs.h"

using namespace std;
using namespace fs;

int main() {
 
  Filesystem fs;

  fs.readFile("test.txt", [&](auto err, auto data) {

    if (err) {
      cout << err.message << endl;
      return;
    }
    
    cout << data << endl;

    fs.writeFile("out.txt", Buffer("hello"), [&](auto err) {
      cout << "OK" << endl;
    });

  });
}

