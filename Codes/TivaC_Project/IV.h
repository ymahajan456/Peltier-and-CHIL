#ifndef _IV_H_
#define _IV_H_

#include "basic_includes.h"

#ifdef DEFINE_VARIABLES
#define EXTERN
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)
#endif

void get_IV();
void start_IV();

#endif

