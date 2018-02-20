#ifndef MACROS_H
#define MACROS_H

// Macros to support option testing
#define _CAT(a, ...) a ## __VA_ARGS__
#define SWITCH_ENABLED_false 0
#define SWITCH_ENABLED_true  1
#define SWITCH_ENABLED_0     0
#define SWITCH_ENABLED_1     1
#define SWITCH_ENABLED_      1
#define ENABLED(b) _CAT(SWITCH_ENABLED_, b)
#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember)) 

#endif //__MACROS_H
