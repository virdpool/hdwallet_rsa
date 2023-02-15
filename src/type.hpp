#pragma once
#include <stdbool.h>

// typedef signed   char   i8;
// typedef unsigned char   u8;
// typedef signed   short  i16;
// typedef unsigned short  u16;
// 
// typedef signed   int    i32;
// typedef unsigned int    u32;
// 
// typedef signed   long long i64;
// typedef unsigned long long u64;

typedef  int8_t   i8;
typedef uint8_t   u8;
typedef  int16_t  i16;
typedef uint16_t  u16;

typedef  int32_t  i32;
typedef uint32_t  u32;

typedef  int64_t  i64;
typedef uint64_t  u64;

typedef float   f32;
typedef double  f64;


// typedef u32 size_t;

#define u32_MAX_VALUE 4294967295
#define u32_MIN_VALUE 0

// #define and &&
// #define or ||

// Я НЕ доволен этим кодом
inline u64 f64_to_u64(f64 arg) {
  u64 buf;
  memcpy(&buf, &arg, 8);
  return buf;
}

inline f64 u64_to_f64(u64 arg) {
  f64 buf;
  memcpy(&buf, &arg, 8);
  return buf;
}
