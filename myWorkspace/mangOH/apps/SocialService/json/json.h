
//--------------------------------------------------------------------------------------------------
/**
 * @file json.h
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef LEGATO_AVTOOLS_JSON_H_INCLUDE_GUARD
#define LEGATO_AVTOOLS_JSON_H_INCLUDE_GUARD




#define JSON_EXPORT __attribute__ ((visibility ("default")))



// Grab the stl we need for our data structures.
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

// Now the library itself.
#include "value.h"
#include "jsonOut.h"
#include "jsonIn.h"




#endif // LEGATO_AVTOOLS_JSON_H_INCLUDE_GUARD
