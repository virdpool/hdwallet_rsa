#pragma once
// why malloc, and not new
// https://stackoverflow.com/questions/23591196/are-calloc-malloc-faster-than-operator-new-in-c

#include <vector>
#include <string>

#define PPCAT_NX(A, B) A ## B
#define PPCAT_NX3(A, B, C) A ## B ## C
#define PPCAT_NX4(A, B, C, D) A ## B ## C ## D
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if _MSC_VER
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
//    
//    Function
//    
////////////////////////////////////////////////////////////////////////////////////////////////////

#define FN_EXPORT(NAME)                                                                     \
status = napi_create_function(env, nullptr, 0, NAME, nullptr, &__fn);                       \
if (status != napi_ok) {                                                                    \
  napi_throw_error(env, nullptr, "FN_EXPORT !napi_create_function NAME=" TOSTRING(NAME));   \
  return nullptr;                                                                           \
}                                                                                           \
                                                                                            \
status = napi_set_named_property(env, exports, TOSTRING(NAME), __fn);                       \
if (status != napi_ok) {                                                                    \
  napi_throw_error(env, nullptr, "FN_EXPORT !napi_set_named_property NAME=" TOSTRING(NAME));\
  return nullptr;                                                                           \
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//    arg
////////////////////////////////////////////////////////////////////////////////////////////////////
#define FN_ARG_HEAD_EMPTY                                                                   \
napi_status status;                                                                         \
                                                                                            \
napi_value ret_dummy;                                                                       \
status = napi_create_int32(env, 0, &ret_dummy);                                             \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_HEAD_EMPTY !napi_create_int32 ret_dummy FN_NAME=";          \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}


#define FN_ARG_HEAD(COUNT)                                                                  \
napi_status status;                                                                         \
int arg_idx = 0;                                                                            \
                                                                                            \
napi_value ret_dummy;                                                                       \
status = napi_create_int32(env, 0, &ret_dummy);                                             \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_HEAD !napi_create_int32 ret_dummy FN_NAME=";                \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return nullptr;                                                                           \
}                                                                                           \
                                                                                            \
size_t argc = COUNT;                                                                        \
napi_value argv[COUNT];                                                                     \
status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);                        \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_HEAD !napi_get_cb_info FN_NAME=";                           \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
if (argc != COUNT) {                                                                        \
  std::string err_msg = "FN_ARG_HEAD Failed to parse arguments. Wrong arg count. Expected=" TOSTRING(COUNT) " Real="; \
  err_msg += std::to_string(argc);                                                          \
  err_msg += ". FN_NAME=";                                                                  \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}

#define FN_ARG_BOOL(NAME)                                                                   \
bool NAME;                                                                                  \
status = napi_get_value_bool(env, argv[arg_idx], &NAME);                                    \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_BOOL !napi_get_value_bool. Bad bool arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
arg_idx++;

#define FN_ARG_I32(NAME)                                                                    \
i32 NAME;                                                                                   \
status = napi_get_value_int32(env, argv[arg_idx], &NAME);                                   \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_I32 !napi_get_value_int32. Bad i32 arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
arg_idx++;


#define FN_ARG_I64(NAME)                                                                    \
i64 NAME;                                                                                   \
bool PPCAT_NX(NAME,_lossless);                                                              \
status = napi_get_value_bigint_int64(env, argv[arg_idx], &NAME, &PPCAT_NX(NAME,_lossless)); \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_I64 !napi_get_value_bigint_int64. Bad i64 arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
arg_idx++;


#define FN_ARG_U32(NAME)                                                                    \
u32 NAME;                                                                                   \
status = napi_get_value_uint32(env, argv[arg_idx], &NAME);                                  \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_U32 !napi_get_value_uint32. Bad u32 arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
arg_idx++;


#define FN_ARG_U64(NAME)                                                                    \
u64 NAME;                                                                                   \
bool PPCAT_NX(NAME,_lossless);                                                              \
status = napi_get_value_bigint_uint64(env, argv[arg_idx], &NAME, &PPCAT_NX(NAME,_lossless));\
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_U64 !napi_get_value_bigint_uint64. Bad u64 arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
arg_idx++;


#define FN_ARG_F32(NAME)                                                                    \
f64 PPCAT_NX(_,NAME);                                                                       \
f32 NAME;                                                                                   \
status = napi_get_value_double(env, argv[arg_idx], &PPCAT_NX(_,NAME));                      \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "FN_ARG_F32 !napi_get_value_double. Bad f32 arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
NAME = PPCAT_NX(_,NAME)                                                                     \
arg_idx++;

#define FN_ARG_F64(NAME)                                                                    \
f64 NAME;                                                                                   \
status = napi_get_value_double(env, argv[arg_idx], &NAME);                                  \
                                                                                            \
if (status != napi_ok) {                                                                    \
  std::string err_msg = "!napi_get_value_double. Bad f64 arg " TOSTRING(NAME) " FN_NAME=";  \
  err_msg += __func__;                                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                                          \
  return ret_dummy;                                                                         \
}                                                                                           \
arg_idx++;


// This implementation contains memory leak
// free(str) should be called manually

#define FN_ARG_STR(NAME)                                                                      \
size_t PPCAT_NX(NAME,_len);                                                                   \
status = napi_get_value_string_utf8(env, argv[arg_idx], nullptr, 0, &PPCAT_NX(NAME,_len));    \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_STR !napi_get_value_string_utf8 [1]. Bad str arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
                                                                                              \
char *NAME = (char*)malloc(PPCAT_NX(NAME,_len)+1);                                            \
{                                                                                             \
  size_t tmp;                                                                                 \
  status = napi_get_value_string_utf8(env, argv[arg_idx], NAME, PPCAT_NX(NAME,_len)+1, &tmp); \
  if (status != napi_ok) {                                                                    \
    std::string err_msg = "FN_ARG_STR !napi_get_value_string_utf8 [2]. Bad str arg " TOSTRING(NAME) " FN_NAME="; \
    err_msg += __func__;                                                                      \
    napi_throw_error(env, nullptr, err_msg.c_str());                                          \
    return ret_dummy;                                                                         \
  }                                                                                           \
}                                                                                             \
arg_idx++;

#define FN_ARG_BUF(NAME)                                                                      \
u8 *NAME;                                                                                     \
size_t PPCAT_NX(NAME,_len);                                                                   \
status = napi_get_buffer_info(env, argv[arg_idx], (void**)&NAME, &PPCAT_NX(NAME,_len));       \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_BUF !napi_get_buffer_info. Bad buf arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
arg_idx++;

#define FN_ARG_BUF_VAL(NAME)                                                                  \
u8 *NAME;                                                                                     \
size_t PPCAT_NX(NAME,_len);                                                                   \
status = napi_get_buffer_info(env, argv[arg_idx], (void**)&NAME, &PPCAT_NX(NAME,_len));       \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_BUF_VAL !napi_get_buffer_info. Bad buf arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
napi_value PPCAT_NX(NAME,_val) = argv[arg_idx];                                               \
arg_idx++;

#define FN_ARG_CLASS(CLASS_NAME, NAME)                                                        \
CLASS_NAME *NAME;                                                                             \
status = napi_unwrap(env, argv[arg_idx], reinterpret_cast<void**>(&NAME));                    \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_CLASS !napi_get_buffer_info. Bad " TOSTRING(CLASS_NAME) " arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
if (NAME->_class_tag != PPCAT_NX(CLASS_NAME,_tag)) {                                          \
  std::string err_msg = "FN_ARG_CLASS Wrong class tag for arg " TOSTRING(NAME) ". expected " TOSTRING(CLASS_NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
if (NAME->_deleted) {                                                                         \
  std::string err_msg = "FN_ARG_CLASS You are calling already freed object " TOSTRING(NAME) " of class " TOSTRING(CLASS_NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
arg_idx++;

#define FN_ARG_CLASS_VAL(CLASS_NAME, NAME)                                                    \
CLASS_NAME *NAME;                                                                             \
status = napi_unwrap(env, argv[arg_idx], reinterpret_cast<void**>(&NAME));                    \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_CLASS_VAL !napi_get_buffer_info. Bad " TOSTRING(CLASS_NAME) " arg " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
if (NAME->_class_tag != PPCAT_NX(CLASS_NAME,_tag)) {                                          \
  std::string err_msg = "FN_ARG_CLASS_VAL Wrong class tag for arg " TOSTRING(NAME) ". expected " TOSTRING(CLASS_NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
if (NAME->_deleted) {                                                                         \
  std::string err_msg = "FN_ARG_CLASS_VAL You are calling already freed object " TOSTRING(NAME) " of class " TOSTRING(CLASS_NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
napi_value PPCAT_NX(NAME,_val) = argv[arg_idx];                                               \
arg_idx++;

////////////////////////////////////////////////////////////////////////////////////////////////////
//    ret
////////////////////////////////////////////////////////////////////////////////////////////////////
#define FN_RET return ret_dummy;

#define FN_THROW(ARG)             \
napi_throw_error(env, nullptr, ARG); \
return ret_dummy;


#define FN_RET_BOOL_CREATE(NAME)                                                        \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_get_boolean(env, NAME, &PPCAT_NX(_ret_,NAME));                            \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_BOOL_CREATE !napi_get_boolean " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_I32_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_int32(env, NAME, &PPCAT_NX(_ret_,NAME));                           \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_I32_CREATE !napi_create_int32 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_I64_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_bigint_int64(env, NAME, &PPCAT_NX(_ret_,NAME));                    \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_I64_CREATE !napi_create_bigint_int64 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_U32_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_uint32(env, NAME, &PPCAT_NX(_ret_,NAME));                          \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_U32_CREATE !napi_create_uint32 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_U64_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_bigint_uint64(env, NAME, &PPCAT_NX(_ret_,NAME));                   \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_U64_CREATE !napi_create_bigint_uint64 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_F32_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_double(env, NAME, &PPCAT_NX(_ret_,NAME));                          \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_F32_CREATE !napi_create_double " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_F64_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_double(env, NAME, &PPCAT_NX(_ret_,NAME));                          \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_F64_CREATE !napi_create_double " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_STR_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_string_utf8(env, NAME, PPCAT_NX(NAME,_len), &PPCAT_NX(_ret_,NAME));\
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_STR_CREATE !napi_create_string_utf8 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_STR_FREE_CREATE(NAME)                                                    \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_string_utf8(env, NAME, PPCAT_NX(NAME,_len), &PPCAT_NX(_ret_,NAME));\
free(NAME);                                                                             \
                                                                                        \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_STR_FREE_CREATE !napi_create_string_utf8 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_STR_SIMPLE_CRAFT_CREATE(NAME)                                            \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
status = napi_create_string_utf8(env, NAME->c_str(), NAME->size(), &PPCAT_NX(_ret_,NAME)); \
delete NAME;                                                                            \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_STR_SIMPLE_CRAFT_CREATE !napi_create_string_utf8 " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

// highly NOT recommended
#define FN_RET_BUF_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
{                                                                                       \
  void* _ret_tmp;                                                                       \
  status = napi_create_buffer_copy(env, PPCAT_NX(NAME,_len), NAME, &_ret_tmp, &PPCAT_NX(_ret_,NAME)); \
}                                                                                       \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_BUF_CREATE !napi_create_buffer_copy " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_DECL_RET_BUF_VAL(NAME)                                                       \
napi_value PPCAT_NX(_ret_,NAME);                                                        \

#define FN_RET_BUF_FREE_CREATE(NAME)                                                    \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
{                                                                                       \
  void* _ret_tmp;                                                                       \
  status = napi_create_buffer_copy(env, PPCAT_NX(NAME,_len), NAME, &_ret_tmp, &PPCAT_NX(_ret_,NAME)); \
}                                                                                       \
free(NAME);                                                                             \
                                                                                        \
if (status != napi_ok) {                                                                \
  std::string err_msg = "FN_RET_BUF_FREE_CREATE !napi_create_buffer_copy " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                                  \
  napi_throw_error(env, nullptr, err_msg.c_str());                                      \
  return ret_dummy;                                                                     \
}

#define FN_RET_REF_CREATE(NAME)                                                         \
napi_value PPCAT_NX(_ret_,NAME);                                                        \
if (!NAME) {                                                                            \
  status = napi_get_undefined(env, &PPCAT_NX(_ret_,NAME));                              \
  if (status != napi_ok) {                                                              \
    std::string err_msg = "FN_RET_REF_CREATE !napi_get_undefined " TOSTRING(NAME) " FN_NAME="; \
    err_msg += __func__;                                                                \
    napi_throw_error(env, nullptr, err_msg.c_str());                                    \
    return ret_dummy;                                                                   \
  }                                                                                     \
} else {                                                                                \
  status = napi_get_reference_value(env, NAME->_wrapper, &PPCAT_NX(_ret_,NAME));        \
  if (status != napi_ok) {                                                              \
    std::string err_msg = "FN_RET_BUF_FREE_CREATE !napi_create_buffer_copy " TOSTRING(NAME) " FN_NAME="; \
    err_msg += __func__;                                                                \
    napi_throw_error(env, nullptr, err_msg.c_str());                                    \
    return ret_dummy;                                                                   \
  }                                                                                     \
}


#define FN_RET_BOOL(NAME)   \
FN_RET_BOOL_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_I32(NAME)   \
FN_RET_I32_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_I64(NAME)   \
FN_RET_I64_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_U32(NAME)   \
FN_RET_U32_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_U64(NAME)   \
FN_RET_U64_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_F32(NAME)   \
FN_RET_F32_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_F64(NAME)   \
FN_RET_F64_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_STR(NAME)   \
FN_RET_STR_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_STR_FREE(NAME)   \
FN_RET_STR_FREE_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_STR_SIMPLE_CRAFT(NAME)   \
FN_RET_STR_SIMPLE_CRAFT_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_BUF(NAME)   \
FN_RET_BUF_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_BUF_FREE(NAME)   \
FN_RET_BUF_FREE_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

#define FN_RET_REF(NAME)   \
FN_RET_REF_CREATE(NAME)    \
return PPCAT_NX(_ret_,NAME);

////////////////////////////////////////////////////////////////////////////////////////////////////
//    helpers
////////////////////////////////////////////////////////////////////////////////////////////////////
void napi_helper_error_cb(napi_env env, const char* error_str, napi_value callback) {
  napi_status status;
  napi_value global;
  status = napi_get_global(env, &global);
  if (status != napi_ok) {
    std::string err_msg = "!napi_get_global. status = ";
    err_msg += std::to_string(status);
    err_msg += " FN_NAME=";
    err_msg += __func__;
    napi_throw_error(env, nullptr, err_msg.c_str());
    return;
  }
  
  napi_value call_argv[1];
  
  napi_value error;
  status = napi_create_string_utf8(env, error_str, strlen(error_str), &error);
  if (status != napi_ok) {
    std::string err_msg = "!napi_create_string_utf8. status = ";
    err_msg += std::to_string(status);
    err_msg += " FN_NAME=";
    err_msg += __func__;
    napi_throw_error(env, nullptr, err_msg.c_str());
    return;
  }
  
  status = napi_create_error(env, nullptr, error, &call_argv[0]);
  if (status != napi_ok) {
    std::string err_msg = "!napi_create_error. status = ";
    err_msg += std::to_string(status);
    err_msg += ". error = ";
    err_msg += error_str;
    err_msg += " FN_NAME=";
    err_msg += __func__;
    napi_throw_error(env, nullptr, err_msg.c_str());
    return;
  }
  
  napi_value result;
  status = napi_call_function(env, global, callback, 1, call_argv, &result);
  if (status != napi_ok) {
    // это нормальная ошибка если основной поток падает
    napi_throw_error(env, nullptr, "!napi_call_function");
    return;
  }
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//    
//    Class
//    
////////////////////////////////////////////////////////////////////////////////////////////////////
#define FN_ARG_THIS(CLASS_NAME)                                                               \
napi_value _js_this;                                                                          \
status = napi_get_cb_info(env, info, nullptr, nullptr, &_js_this, nullptr);                   \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_THIS !napi_get_cb_info " TOSTRING(NAME) " FN_NAME=";          \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
                                                                                              \
CLASS_NAME* _this;                                                                            \
status = napi_unwrap(env, _js_this, reinterpret_cast<void**>(&_this));                        \
if (status != napi_ok) {                                                                      \
  std::string err_msg = "FN_ARG_THIS !napi_unwrap " TOSTRING(NAME) " FN_NAME=";               \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \
if (_this->_deleted) {                                                                        \
  std::string err_msg = "FN_ARG_THIS You are calling already freed object of class " TOSTRING(CLASS_NAME) " FN_NAME="; \
  err_msg += __func__;                                                                        \
  napi_throw_error(env, nullptr, err_msg.c_str());                                            \
  return ret_dummy;                                                                           \
}                                                                                             \



#define CLASS_DEF(NAME)                                                     \
std::vector<napi_property_descriptor> PPCAT_NX(NAME,_prop_list);

#define CLASS_METHOD_SYNC(CLASS_NAME, METHOD_NAME)                          \
PPCAT_NX(CLASS_NAME,_prop_list).push_back({ TOSTRING(METHOD_NAME) "_sync", nullptr, PPCAT_NX4(CLASS_NAME, _, METHOD_NAME, _sync), nullptr, nullptr, nullptr, napi_enumerable, nullptr });

#define CLASS_METHOD_ASYNC(CLASS_NAME, METHOD_NAME)                         \
PPCAT_NX(CLASS_NAME,_prop_list).push_back({ TOSTRING(METHOD_NAME)        , nullptr, PPCAT_NX3(CLASS_NAME, _, METHOD_NAME       ), nullptr, nullptr, nullptr, napi_enumerable, nullptr });

#define CLASS_METHOD(CLASS_NAME, METHOD_NAME)                               \
CLASS_METHOD_SYNC(CLASS_NAME, METHOD_NAME)                                  \
CLASS_METHOD_ASYNC(CLASS_NAME, METHOD_NAME)

#define CLASS_EXPORT(NAME)                                                  \
napi_value NAME;                                                            \
status = napi_define_class(env, TOSTRING(NAME), NAPI_AUTO_LENGTH, PPCAT_NX(NAME,_constructor), nullptr, PPCAT_NX(NAME,_prop_list).size(), PPCAT_NX(NAME,_prop_list).data(), &NAME); \
if (status != napi_ok) {                                                    \
  std::string err_msg = "CLASS_EXPORT !napi_define_class " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                          \
  return nullptr;                                                           \
}                                                                           \
                                                                            \
status = napi_set_named_property(env, exports, TOSTRING(NAME), NAME);       \
if (status != napi_ok) {                                                    \
  std::string err_msg = "CLASS_EXPORT !napi_set_named_property " TOSTRING(NAME) " FN_NAME="; \
  err_msg += __func__;                                                      \
  napi_throw_error(env, nullptr, err_msg.c_str());                          \
  return nullptr;                                                           \
}
