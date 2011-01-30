#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

#define NEW_OBJECT(target,type) { do { \
                                target = new type; \
                                } while (target == NULL); }

#define DEL_OBJECT(target) { delete(target); \
                             target = NULL; }

#define DEL_OBJECT_NO_ASSIGN_NULL(target) delete (target);

#define NEW_ARRAY_OF_OBJECTS(target,type,size) { do { \
                                target = new type[size]; \
                                } while (target == NULL); }

#define DEL_ARRAY_OF_OBJECTS(target) { delete [] (target); \
                             target = NULL; }

#define DEL_ARRAY_OF_OBJECTS_NO_ASSIGN_NULL(target) delete [] (target);

#include "tcell.h"
#include "tadaphuff.h"

#endif // COMMON_H
