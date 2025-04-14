/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org>
 *
 * Author: Donald Robson, VyperCore, 2024
 */

#ifndef FFF_CLASS_WRAPPER_H
#define FFF_CLASS_WRAPPER_H

#include "fff.h"

#define NONE

// Unroll the user-defined macros into the actual FFF definitions
#define VALUE_FAKES(...) FAKE_VALUE_FUNC(__VA_ARGS__);
#define VOID_FAKES(...) FAKE_VOID_FUNC(__VA_ARGS__);

// Unroll the user-defined macros into wrapper methods
#define VALUE_WRAPPER_METHODS(ret_type, name, ...)                             \
    ret_type name(ARGS_TYPES(PP_NARG(__VA_ARGS__), ##__VA_ARGS__))             \
        const noexcept {                                                       \
        return fff_mocks::FAKE_CLASS_NAME::name(ARGS(PP_NARG(__VA_ARGS__)));   \
    }
#define VOID_WRAPPER_METHODS(name, ...)                                        \
    void name(ARGS_TYPES(PP_NARG(__VA_ARGS__), ##__VA_ARGS__))                 \
        const noexcept {                                                       \
        return fff_mocks::FAKE_CLASS_NAME::name(ARGS(PP_NARG(__VA_ARGS__)));   \
    }
#define STATIC_VALUE_WRAPPER_METHODS(ret_type, name, ...)                      \
    static ret_type name(                                                      \
        ARGS_TYPES(PP_NARG(__VA_ARGS__), ##__VA_ARGS__)) noexcept {            \
        return fff_mocks::FAKE_CLASS_NAME::name(ARGS(PP_NARG(__VA_ARGS__)));   \
    }
#define STATIC_VOID_WRAPPER_METHODS(name, ...)                                 \
    static void name(ARGS_TYPES(PP_NARG(__VA_ARGS__), ##__VA_ARGS__))          \
        noexcept {                                                             \
        return fff_mocks::FAKE_CLASS_NAME::name(ARGS(PP_NARG(__VA_ARGS__)));   \
    }

// Unroll reset function calls
#define RESET_VALUE_FAKES(ret_type, name, ...) RESET_NAMED_FAKE(name);
#define RESET_VOID_FAKES(name, ...) RESET_NAMED_FAKE(name)
#define RESET_NAMED_FAKE(name) RESET_FAKE(fff_mocks::FAKE_CLASS_NAME::name)

#define MAKE_FAKE_CLASS                                                        \
    namespace fff_mocks::FAKE_CLASS_NAME {                                     \
    FAKE_VALUE_FUNCS(VALUE_FAKES)                                              \
    FAKE_VOID_FUNCS(VOID_FAKES)                                                \
    FAKE_STATIC_VOID_FUNCS(VOID_FAKES)                                         \
    FAKE_STATIC_VALUE_FUNCS(VALUE_FAKES)                                       \
    }                                                                          \
    class FAKE_CLASS_NAME {                                                    \
      public:                                                                  \
        FAKE_VALUE_FUNCS(VALUE_WRAPPER_METHODS)                                \
        FAKE_VOID_FUNCS(VOID_WRAPPER_METHODS)                                  \
        FAKE_STATIC_VALUE_FUNCS(STATIC_VALUE_WRAPPER_METHODS)                  \
        FAKE_STATIC_VOID_FUNCS(STATIC_VOID_WRAPPER_METHODS)                    \
        static void reset_fakes() noexcept {                                   \
            FAKE_VALUE_FUNCS(RESET_VALUE_FAKES)                                \
            FAKE_VOID_FUNCS(RESET_VOID_FAKES)                                  \
            FAKE_STATIC_VALUE_FUNCS(RESET_VALUE_FAKES)                         \
            FAKE_STATIC_VOID_FUNCS(RESET_VOID_FAKES)                           \
        }                                                                      \
    };

// Count args (up to 20, as this is the limit for FFF)
#define PP_NARG(...) PP_NARG_(0, ##__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    N, ...) N
#define PP_RSEQ_N() \
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// For declaring the arguments of the wrapper methods
#define ARGS_TYPES(N, ...) ARGS_TYPES_(N, __VA_ARGS__)
#define ARGS_TYPES_(N, ...) ARGS_TYPES_##N(__VA_ARGS__)
#define ARGS_TYPES_0(...)
#define ARGS_TYPES_1(arg1_type) \
    arg1_type arg1
#define ARGS_TYPES_2(arg1_type, arg2_type) \
    arg1_type arg1, arg2_type arg2
#define ARGS_TYPES_3(arg1_type, arg2_type, arg3_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3
#define ARGS_TYPES_4(arg1_type, arg2_type, arg3_type, arg4_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4
#define ARGS_TYPES_5(arg1_type, arg2_type, arg3_type, arg4_type, \
                     arg5_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5
#define ARGS_TYPES_6(arg1_type, arg2_type, arg3_type, arg4_type, \
                     arg5_type, arg6_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6
#define ARGS_TYPES_7(arg1_type, arg2_type, arg3_type, arg4_type, \
                     arg5_type, arg6_type, arg7_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7
#define ARGS_TYPES_8(arg1_type, arg2_type, arg3_type, arg4_type, \
                     arg5_type, arg6_type, arg7_type, arg8_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8
#define ARGS_TYPES_9(arg1_type, arg2_type, arg3_type, arg4_type, \
                     arg5_type, arg6_type, arg7_type, arg8_type, arg9_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9
#define ARGS_TYPES_10(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10
#define ARGS_TYPES_11(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11
#define ARGS_TYPES_12(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12
#define ARGS_TYPES_13(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13
#define ARGS_TYPES_14(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14
#define ARGS_TYPES_15(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type, arg15_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14, arg15_type arg15
#define ARGS_TYPES_16(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type, arg15_type, arg16_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14, arg15_type arg15, arg16_type arg16
#define ARGS_TYPES_17(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type, arg15_type, arg16_type, \
                      arg17_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14, arg15_type arg15, arg16_type arg16, \
    arg17_type arg17
#define ARGS_TYPES_18(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type, arg15_type, arg16_type, \
                      arg17_type, arg18_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14, arg15_type arg15, arg16_type arg16, \
    arg17_type arg17, arg18_type arg18
#define ARGS_TYPES_19(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type, arg15_type, arg16_type, \
                      arg17_type, arg18_type, arg19_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14, arg15_type arg15, arg16_type arg16, \
    arg17_type arg17, arg18_type arg18, arg19_type arg19
#define ARGS_TYPES_20(arg1_type, arg2_type, arg3_type, arg4_type, \
                      arg5_type, arg6_type, arg7_type, arg8_type, \
                      arg9_type, arg10_type, arg11_type, arg12_type, \
                      arg13_type, arg14_type, arg15_type, arg16_type, \
                      arg17_type, arg18_type, arg19_type, arg20_type) \
    arg1_type arg1, arg2_type arg2, arg3_type arg3, arg4_type arg4, \
    arg5_type arg5, arg6_type arg6, arg7_type arg7, arg8_type arg8, \
    arg9_type arg9, arg10_type arg10, arg11_type arg11, arg12_type arg12, \
    arg13_type arg13, arg14_type arg14, arg15_type arg15, arg16_type arg16, \
    arg17_type arg17, arg18_type arg18, arg19_type arg19, arg20_type arg20

// For forwarding arguments to the fake free function from the method
#define ARGS(N) ARGS_(N)
#define ARGS_(N) ARGS_##N
#define ARGS_0
#define ARGS_1 arg1
#define ARGS_2 ARGS_1, arg2
#define ARGS_3 ARGS_2, arg3
#define ARGS_4 ARGS_3, arg4
#define ARGS_5 ARGS_4, arg5
#define ARGS_6 ARGS_5, arg6
#define ARGS_7 ARGS_6, arg7
#define ARGS_8 ARGS_7, arg8
#define ARGS_9 ARGS_8, arg9
#define ARGS_10 ARGS_9, arg10
#define ARGS_11 ARGS_10, arg11
#define ARGS_12 ARGS_11, arg12
#define ARGS_13 ARGS_12, arg13
#define ARGS_14 ARGS_13, arg14
#define ARGS_15 ARGS_14, arg15
#define ARGS_16 ARGS_15, arg16
#define ARGS_17 ARGS_16, arg17
#define ARGS_18 ARGS_17, arg18
#define ARGS_19 ARGS_18, arg19
#define ARGS_20 ARGS_19, arg20

#endif // FFF_CLASS_WRAPPER_H
