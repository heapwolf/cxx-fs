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

# Filesystem

## CONSTRUCTOR
### Filesystem fs;
Creates a loop for file operations.

## INSTANCE METHODS

### fs.cwd();
Returns a string that represents the current working directory.

### fs.readFile(path, callback);
Stats, Opens, reads whole file, callback provides (`error`, `streambuf`).

### fs.readFileSync(path);
Stats, Opens, reads whole file and returns an instance of `Buffer`.

### fs.writeFile(path, buffer, callback);
Opens, writes whole buffer, callback provides (`error`).

### fs.writeFileSync(path, buffer);
Opens, writes whole value from the instance of `Buffer`.

### fs.stat(path, callback);
Callback provides (`error`, `stats`), where stats is a struct containing 
the following members...

### fs.statSync(path);
Returns a Stats object.

```cpp
uint64_t dev;
uint64_t mode;
uint64_t nlink;
uint64_t uid;
uint64_t gid;
uint64_t rdev;
uint64_t ino;
uint64_t size;
uint64_t blksize;
uint64_t blocks;
uint64_t flags;
uint64_t gen;
uv_timespec_t atime;
uv_timespec_t mtime;
uv_timespec_t ctime;
uv_timespec_t birthtime;
```

### fs.mkdirSync(path, [mode]);
Create a directory sync, optionally pass the mode as an octal.

### fs.rmdirSync(path);
Remove a direcory sync.

### fs.open(path, callback);
Callback provides (`error`, `fd`) where fd is a file descriptor.

### fs.openSync(path, [mode]);
Open a file sync, optionally pass the mode as an octal.

### fs.read(fd, bufferSize, offset, callback);
Callback provides (`error`, `uv_buf_t`).

### fs.readSync(fd, bufferSize, offset);
Callback provides (`error`, `uv_buf_t`).

### fs.write(fd, buffer, offset, callback);
Callback provides (`error`, `uv_buf_t`).

### fs.writeSync(fd, buffer, offset, length);
Write an instance of `Buffer` to a file, returns the number of bytes written.

### fs.close(fd, callback);
Close a file. Callback provides (`error`).

### fs.closeSync(fd);
Close a file sync.

# Buffers
A little sugar on top of `uv_buf_t`.

## CONSTRUCTOR
### Buffer buf(string);
### Buffer buf();
### Buffer buf(size);

## INSTANCE METHODS
### buf.toString();

## STATIC MEMBERS
### buf.data

