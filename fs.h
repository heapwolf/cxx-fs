#ifndef NODEUV_FS_H
#define NODEUV_FS_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

#ifndef _WIN32
#include <unistd.h>
#endif

extern "C" {
#include "uv.h"
}

#ifndef HTTP_UV_LOOP
#define HTTP_UV_LOOP
static uv_loop_t *UV_LOOP;
#endif

namespace fs {

    

}
