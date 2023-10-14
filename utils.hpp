#pragma once

#define unpackRGBA(c) (c>>24)/255.0, ((c>>16)&0xFF)/255.0, ((c>>8)&0xFF)/255.0, (c&0xFF)/255.0
#define unpackRGB(c) ((c>>16)&0xFF)/255.0, ((c>>8)&0xFF)/255.0, (c&0xFF)/255.0

#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t
#define i32 int32_t
#define i16 int16_t
#define i8  int8_t

const int Height=600, Width=600;
