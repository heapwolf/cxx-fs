# SYNOPSIS
A friendly, nodejs-like file i/o experience for C++

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

### `string` fs.cwd();
Returns a string that represents the current working directory.

### `string` fs.readFile(string path, Callback cb);
Stats, Opens, reads whole file, callback provides (`error`, `streambuf`).

### `Buffer` fs.readFileSync(string path);
Stats, Opens, reads whole file and returns an instance of `Buffer`.

### `void` fs.writeFile(string path, Buffer buf, Callback cb);
Opens, writes whole buffer, callback provides (`error`).

### `Buffer` fs.writeFileSync(string path, Buffer buf);
Opens, writes whole value from the instance of `Buffer`.

### `void` fs.stat(string path, Callback cb);
Callback provides (`error`, `stats`), where stats is a struct containing 
the following members...

### `Stats` fs.statSync(string path);
Returns a Stats struct.

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

### `Error` fs.mkdirSync(string path, [int mode]);
Create a directory sync, optionally pass the mode as an octal. Returns
an error instance that will have the value of null if the operation was
successful.

### `Error` fs.rmdirSync(string path);
Remove a direcory sync. Returns an error object that has the value of
null if the operation was a success.

### `void` fs.open(string path, Callback cb);
The callback provides two arguments `Error` which will be null if the
operation was a success and a file descriptor.

### `int` fs.openSync(string path, [int mode]);
Open a file sync, optionally pass the mode as an octal. Returns the
file descriptor.

### `void` fs.read(int fd, int bufferSize, int offset, Callback cb);
Callback provides (`error`, `uv_buf_t`).

### `Buffer` fs.readSync(int fd, int bufferSize, int offset);
Returns an instance of `Buffer`.

### `void` fs.write(int fd, Buffer buf, int offset, Callback cb);
Callback provides (`error`, `uv_buf_t`).

### `int` fs.writeSync(int fd, Buffer buf, int offset, int length);
Write an instance of `Buffer` to a file, returns the number of bytes
written.

### `void` fs.close(fd, callback);
Close a file. Callback provides an instance of `Error` which is null
if the operation was a success.

### `Error` fs.closeSync(fd);
Close a file sync.

# Buffers
A little sugar on top of `uv_buf_t`. This should get moved out to
another module called `nodeuv-buffer`.

## CONSTRUCTOR
### Buffer buf(string);
### Buffer buf();
### Buffer buf(size);

## INSTANCE METHODS
### buf.toString();

## STATIC MEMBERS
### buf.data

