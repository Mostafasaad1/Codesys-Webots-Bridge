#pragma once

#include <cstdint>
#include <string>

// ============================================================================
// COMPLETE CODESYS TYPE ALIASES FOR C++
// ============================================================================
// All IEC 61131-3 types + CODESYS 3.x extensions
//
// Note: When used with Windows.h, conflicting types use Windows definitions.
// Non-conflicting CODESYS-specific types are always safe to use.

// ============================================================================
// Check if Windows.h was already included
// ============================================================================

#ifdef _WINDOWS_
  // Windows.h defines: BOOL, BYTE, WORD, DWORD, INT, UINT, DATE
  // We skip redefining these and use Windows definitions
  #define CODESYS_WINDOWS_COMPAT
#endif

// ============================================================================
// 1. BOOLEAN TYPES
// ============================================================================

#ifndef CODESYS_WINDOWS_COMPAT
using BOOL = bool;  // 1 bit (stored in 1 byte)
#endif

// ============================================================================
// 2. BIT STRING TYPES (Unsigned)
// ============================================================================

#ifndef CODESYS_WINDOWS_COMPAT
using BYTE  = uint8_t;   // 8 bits  (0...255)
using WORD  = uint16_t;  // 16 bits (0...65535)
using DWORD = uint32_t;  // 32 bits (0...4294967295)
#endif
using LWORD = uint64_t;  // 64 bits - Not in Windows.h

// ============================================================================
// 3. SIGNED INTEGER TYPES
// ============================================================================

using SINT = int8_t;    // Short Int:  -128...127
#ifndef CODESYS_WINDOWS_COMPAT
using INT  = int16_t;   // Integer:    -32768...32767
#endif
using DINT = int32_t;   // Double Int: -2147483648...2147483647
using LINT = int64_t;   // Long Int:   64-bit signed

// ============================================================================
// 4. UNSIGNED INTEGER TYPES
// ============================================================================

using USINT = uint8_t;   // Unsigned Short Int:  0...255
#ifndef CODESYS_WINDOWS_COMPAT
using UINT  = uint16_t;  // Unsigned Integer:    0...65535
#endif
using UDINT = uint32_t;  // Unsigned Double Int: 0...4294967295
using ULINT = uint64_t;  // Unsigned Long Int:   64-bit unsigned

// ============================================================================
// 5. FLOATING POINT TYPES
// ============================================================================

using REAL  = float;   // 32-bit IEEE 754 Single Precision
using LREAL = double;  // 64-bit IEEE 754 Double Precision

// ============================================================================
// 6. TIME/DURATION TYPES
// ============================================================================

using TIME  = int32_t;  // Duration in milliseconds (or implementation-specific)
using LTIME = int64_t;  // Long Time - Duration in nanoseconds

// ============================================================================
// 7. DATE AND TIME TYPES
// ============================================================================

// Standard (32-bit)
#ifndef CODESYS_WINDOWS_COMPAT
using DATE          = uint32_t;  // Date (days since epoch or seconds)
#endif
using TIME_OF_DAY   = uint32_t;  // Time since midnight (milliseconds)
using TOD           = TIME_OF_DAY;  // Alias for TIME_OF_DAY
using DATE_AND_TIME = uint32_t;  // Date + Time (Unix timestamp)
using DT            = DATE_AND_TIME;  // Alias for DATE_AND_TIME

// Extended (64-bit) - CODESYS 3.5+
using LDATE           = uint64_t;  // Long Date
using LTIME_OF_DAY    = uint64_t;  // Long Time of Day (nanoseconds since midnight)
using LTOD            = LTIME_OF_DAY;  // Alias
using LDATE_AND_TIME  = uint64_t;  // Long Date and Time
using LDT             = LDATE_AND_TIME;  // Alias

// ============================================================================
// 8. STRING TYPES
// ============================================================================

using STRING  = std::string;   // ASCII string (variable length in C++)
using WSTRING = std::wstring;  // Wide string / Unicode (UTF-16)

// ============================================================================
// 9. PLATFORM-DEPENDENT TYPES (CODESYS 3.x)
// ============================================================================
// These types adapt to the target platform (32-bit or 64-bit)

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    // 64-bit platform
    using __XINT  = int64_t;
    using __UXINT = uint64_t;
    using __XWORD = uint64_t;
#else
    // 32-bit platform
    using __XINT  = int32_t;
    using __UXINT = uint32_t;
    using __XWORD = uint32_t;
#endif
