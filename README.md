# SYNOPSIS
A friendly c++1y wrapper for libuv's filesystem operations

# USAGE

```cpp
#include "../fs.h"

using namespace std;
using namespace fs;

int main() {
 
  Filesystem fs;

  fs.readFile("test.txt", [](auto err, auto data) {

    if (err) {
      cout << err.message << endl;
      return;
    }

    cout << data << endl;
  });
}
```

# API

## CONSTRUCTOR
### Filestream fs;
Creates a loop for file operations.

## METHODS

### fs.readFile(path, callback);
Stats, Opens, reads whole file, callback provides (`error`, `streambuf`).

### fs.stat(path, callback);
Callback provides (`error`, `stats`), where stats is a struct containing 
the following members...

```cpp
int stats.dev
int stats.mode
int stats.nlink
int stats.uid
int stats.gid
int stats.rdev
double stats.size
double stats.ino
double stats.atime
double stats.mtime
double stats.ctime
```

### fs.open(path, callback);
Callback provides (`error`, `fd`) where fd is a file descriptor.

### fs.read(fd, bufferSize, offset, callback);
Callback provides (`error`, `uv_buf_t`).

### fs.close(fd, callback);
Callback provides (`error`).

