#ifndef _TEMP_LOOP_H_
#define _TEMP_LOOP_H_

#include "basic_includes.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

extern void read_temp_loop_data();
extern void send_temp_loop_data();
extern void temp_loop();
extern void init_bang_bang();

#endif
