void _rsa_gen_impl(
  std::string*& err,
  i32          size,
  u8*          seed_buf,
  size_t       seed_buf_len,
  u8*          dst_n_buf,
  size_t       dst_n_buf_len,
  u8*          dst_e_buf,
  size_t       dst_e_buf_len,
  u8*          dst_d_buf,
  size_t       dst_d_buf_len,
  u8*          dst_p_buf,
  size_t       dst_p_buf_len,
  u8*          dst_q_buf,
  size_t       dst_q_buf_len,
  u8*          dst_dp_buf,
  size_t       dst_dp_buf_len,
  u8*          dst_dq_buf,
  size_t       dst_dq_buf_len,
  u8*          dst_qi_buf,
  size_t       dst_qi_buf_len,
  i32&         ret_n_size,
  i32&         ret_e_size,
  i32&         ret_d_size,
  i32&         ret_p_size,
  i32&         ret_q_size,
  i32&         ret_dp_size,
  i32&         ret_dq_size,
  i32&         ret_qi_size
) {
  /*
    i32          size,
    u8*          seed_buf,
    size_t       seed_buf_len,
    u8*          dst_n_buf,
    size_t       dst_n_buf_len,
    u8*          dst_e_buf,
    size_t       dst_e_buf_len,
    u8*          dst_d_buf,
    size_t       dst_d_buf_len,
    u8*          dst_p_buf,
    size_t       dst_p_buf_len,
    u8*          dst_q_buf,
    size_t       dst_q_buf_len,
    u8*          dst_dp_buf,
    size_t       dst_dp_buf_len,
    u8*          dst_dq_buf,
    size_t       dst_dq_buf_len,
    u8*          dst_qi_buf,
    size_t       dst_qi_buf_len,
    i32&         ret_n_size,
    i32&         ret_e_size,
    i32&         ret_d_size,
    i32&         ret_p_size,
    i32&         ret_q_size,
    i32&         ret_dp_size,
    i32&         ret_dq_size,
    i32&         ret_qi_size
  */
  
  if (seed_buf_len != 32) {
    err = new std::string("seed_buf_len != 32");
    return;
  }
  
  const std::lock_guard<std::mutex> lock(state_mutex);
  
  RAND_set_rand_method(RAND_stdlib());
  RAND_seed(seed_buf, seed_buf_len);
  
  RSA *rsa = RSA_new();
  {
    BIGNUM* e = BN_new();
    BN_set_word(e, 65537);
  
    RSA_generate_key_ex(rsa, size, e, nullptr);
    if (!rsa) {
      err = new std::string("!RSA_generate_key_ex");
      return;
    }
  }
  
  EVP_PKEY *pkey = EVP_PKEY_new();
  if (!pkey) {
    err = new std::string("!EVP_PKEY_new");
    RSA_free(rsa);
    return;
  }
  
  if (!EVP_PKEY_assign_RSA(pkey, rsa)) {
    err = new std::string("!EVP_PKEY_assign_RSA");
    EVP_PKEY_free(pkey);
    RSA_free(rsa);
    return;
  }
  
  const BIGNUM* rsa_n = RSA_get0_n(rsa);
  const BIGNUM* rsa_e = RSA_get0_e(rsa);
  const BIGNUM* rsa_d = RSA_get0_d(rsa);
  const BIGNUM* rsa_p = RSA_get0_p(rsa);
  const BIGNUM* rsa_q = RSA_get0_q(rsa);
  const BIGNUM* rsa_dp = RSA_get0_dmp1(rsa);
  const BIGNUM* rsa_dq = RSA_get0_dmq1(rsa);
  const BIGNUM* rsa_qi = RSA_get0_iqmp(rsa);
  
  if (dst_n_buf_len < BN_num_bytes(rsa_n)) {
    err = new std::string("dst_n_buf_len < BN_num_bytes(rsa_n)");
    *err += std::string("; dst_n_buf_len = ");
    *err += std::to_string(dst_n_buf_len);
    *err += std::string("; BN_num_bytes(rsa_n) = ");
    *err += std::to_string(BN_num_bytes(rsa_n));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_e_buf_len < BN_num_bytes(rsa_e)) {
    err = new std::string("dst_e_buf_len < BN_num_bytes(rsa_e)");
    *err += std::string("; dst_e_buf_len = ");
    *err += std::to_string(dst_e_buf_len);
    *err += std::string("; BN_num_bytes(rsa_e) = ");
    *err += std::to_string(BN_num_bytes(rsa_e));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_d_buf_len < BN_num_bytes(rsa_d)) {
    err = new std::string("dst_d_buf_len < BN_num_bytes(rsa_d)");
    *err += std::string("; dst_d_buf_len = ");
    *err += std::to_string(dst_d_buf_len);
    *err += std::string("; BN_num_bytes(rsa_d) = ");
    *err += std::to_string(BN_num_bytes(rsa_d));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_p_buf_len < BN_num_bytes(rsa_p)) {
    err = new std::string("dst_p_buf_len < BN_num_bytes(rsa_p)");
    *err += std::string("; dst_p_buf_len = ");
    *err += std::to_string(dst_p_buf_len);
    *err += std::string("; BN_num_bytes(rsa_p) = ");
    *err += std::to_string(BN_num_bytes(rsa_p));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_q_buf_len < BN_num_bytes(rsa_q)) {
    err = new std::string("dst_q_buf_len < BN_num_bytes(rsa_q)");
    *err += std::string("; dst_q_buf_len = ");
    *err += std::to_string(dst_q_buf_len);
    *err += std::string("; BN_num_bytes(rsa_q) = ");
    *err += std::to_string(BN_num_bytes(rsa_q));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_dp_buf_len < BN_num_bytes(rsa_dp)) {
    err = new std::string("dst_dp_buf_len < BN_num_bytes(rsa_dp)");
    *err += std::string("; dst_dp_buf_len = ");
    *err += std::to_string(dst_dp_buf_len);
    *err += std::string("; BN_num_bytes(rsa_dp) = ");
    *err += std::to_string(BN_num_bytes(rsa_dp));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_dq_buf_len < BN_num_bytes(rsa_dq)) {
    err = new std::string("dst_dq_buf_len < BN_num_bytes(rsa_dq)");
    *err += std::string("; dst_dq_buf_len = ");
    *err += std::to_string(dst_dq_buf_len);
    *err += std::string("; BN_num_bytes(rsa_dq) = ");
    *err += std::to_string(BN_num_bytes(rsa_dq));
    EVP_PKEY_free(pkey);
    return;
  }
  if (dst_qi_buf_len < BN_num_bytes(rsa_qi)) {
    err = new std::string("dst_qi_buf_len < BN_num_bytes(rsa_qi)");
    *err += std::string("; dst_qi_buf_len = ");
    *err += std::to_string(dst_qi_buf_len);
    *err += std::string("; BN_num_bytes(rsa_qi) = ");
    *err += std::to_string(BN_num_bytes(rsa_qi));
    EVP_PKEY_free(pkey);
    return;
  }
  
  BN_bn2bin(rsa_n,  dst_n_buf);
  BN_bn2bin(rsa_e,  dst_e_buf);
  BN_bn2bin(rsa_d,  dst_d_buf);
  BN_bn2bin(rsa_p,  dst_p_buf);
  BN_bn2bin(rsa_q,  dst_q_buf);
  BN_bn2bin(rsa_dp, dst_dp_buf);
  BN_bn2bin(rsa_dq, dst_dq_buf);
  BN_bn2bin(rsa_qi, dst_qi_buf);
  
  ret_n_size = BN_num_bytes(rsa_n);
  ret_e_size = BN_num_bytes(rsa_e);
  ret_d_size = BN_num_bytes(rsa_d);
  ret_p_size = BN_num_bytes(rsa_p);
  ret_q_size = BN_num_bytes(rsa_q);
  ret_dp_size= BN_num_bytes(rsa_dp);
  ret_dq_size= BN_num_bytes(rsa_dq);
  ret_qi_size= BN_num_bytes(rsa_qi);
  
  EVP_PKEY_free(pkey); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//   sync
////////////////////////////////////////////////////////////////////////////////////////////////////
napi_value rsa_gen_sync(napi_env env, napi_callback_info info) {
  FN_ARG_HEAD(10)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_ARG_I32(size)
  FN_ARG_BUF(seed_buf)
  FN_ARG_BUF(dst_n_buf)
  FN_ARG_BUF(dst_e_buf)
  FN_ARG_BUF(dst_d_buf)
  FN_ARG_BUF(dst_p_buf)
  FN_ARG_BUF(dst_q_buf)
  FN_ARG_BUF(dst_dp_buf)
  FN_ARG_BUF(dst_dq_buf)
  FN_ARG_BUF(dst_qi_buf)
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  std::string *err = nullptr;
  i32 ret_n_size;
  i32 ret_e_size;
  i32 ret_d_size;
  i32 ret_p_size;
  i32 ret_q_size;
  i32 ret_dp_size;
  i32 ret_dq_size;
  i32 ret_qi_size;
  _rsa_gen_impl(err, size, seed_buf, seed_buf_len, dst_n_buf, dst_n_buf_len, dst_e_buf, dst_e_buf_len, dst_d_buf, dst_d_buf_len, dst_p_buf, dst_p_buf_len, dst_q_buf, dst_q_buf_len, dst_dp_buf, dst_dp_buf_len, dst_dq_buf, dst_dq_buf_len, dst_qi_buf, dst_qi_buf_len, ret_n_size, ret_e_size, ret_d_size, ret_p_size, ret_q_size, ret_dp_size, ret_dq_size, ret_qi_size);
  if (err) {
    napi_throw_error(env, nullptr, err->c_str());
    delete err;
    return ret_dummy;
  }
  
  
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value ret_array_wrap;
  status = napi_create_array_with_length(env, 8, &ret_array_wrap);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_array_with_length FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_n_size)
  status = napi_set_element(env, ret_array_wrap, 0, _ret_ret_n_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=0 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_e_size)
  status = napi_set_element(env, ret_array_wrap, 1, _ret_ret_e_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=1 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_d_size)
  status = napi_set_element(env, ret_array_wrap, 2, _ret_ret_d_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=2 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_p_size)
  status = napi_set_element(env, ret_array_wrap, 3, _ret_ret_p_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=3 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_q_size)
  status = napi_set_element(env, ret_array_wrap, 4, _ret_ret_q_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=4 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_dp_size)
  status = napi_set_element(env, ret_array_wrap, 5, _ret_ret_dp_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=5 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_dq_size)
  status = napi_set_element(env, ret_array_wrap, 6, _ret_ret_dq_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=6 FAIL");
    return ret_dummy;
  }
  FN_RET_I32_CREATE(ret_qi_size)
  status = napi_set_element(env, ret_array_wrap, 7, _ret_ret_qi_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=7 FAIL");
    return ret_dummy;
  }
  return ret_array_wrap;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//   async
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Worker_ctx_rsa_gen {
  i32 size;
  u8* seed_buf;
  size_t seed_buf_len;
  napi_ref seed_buf_ref;
  u8* dst_n_buf;
  size_t dst_n_buf_len;
  napi_ref dst_n_buf_ref;
  u8* dst_e_buf;
  size_t dst_e_buf_len;
  napi_ref dst_e_buf_ref;
  u8* dst_d_buf;
  size_t dst_d_buf_len;
  napi_ref dst_d_buf_ref;
  u8* dst_p_buf;
  size_t dst_p_buf_len;
  napi_ref dst_p_buf_ref;
  u8* dst_q_buf;
  size_t dst_q_buf_len;
  napi_ref dst_q_buf_ref;
  u8* dst_dp_buf;
  size_t dst_dp_buf_len;
  napi_ref dst_dp_buf_ref;
  u8* dst_dq_buf;
  size_t dst_dq_buf_len;
  napi_ref dst_dq_buf_ref;
  u8* dst_qi_buf;
  size_t dst_qi_buf_len;
  napi_ref dst_qi_buf_ref;
  i32 ret_n_size;
  i32 ret_e_size;
  i32 ret_d_size;
  i32 ret_p_size;
  i32 ret_q_size;
  i32 ret_dp_size;
  i32 ret_dq_size;
  i32 ret_qi_size;
  
  std::string* err;
  napi_ref callback_reference;
  napi_async_work work;
};

void _worker_ctx_rsa_gen_clear(napi_env env, struct Worker_ctx_rsa_gen* worker_ctx) {
  if (worker_ctx->err) {
    delete worker_ctx->err;
    worker_ctx->err = nullptr;
  }
  napi_status status;
  if (worker_ctx->seed_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->seed_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for seed_buf");
      return;
    }
  }
  if (worker_ctx->dst_n_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_n_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_n_buf");
      return;
    }
  }
  if (worker_ctx->dst_e_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_e_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_e_buf");
      return;
    }
  }
  if (worker_ctx->dst_d_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_d_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_d_buf");
      return;
    }
  }
  if (worker_ctx->dst_p_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_p_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_p_buf");
      return;
    }
  }
  if (worker_ctx->dst_q_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_q_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_q_buf");
      return;
    }
  }
  if (worker_ctx->dst_dp_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_dp_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_dp_buf");
      return;
    }
  }
  if (worker_ctx->dst_dq_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_dq_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_dq_buf");
      return;
    }
  }
  if (worker_ctx->dst_qi_buf_ref) {
    status = napi_delete_reference(env, worker_ctx->dst_qi_buf_ref);
    if (status != napi_ok) {
      printf("status = %d\n", status);
      napi_throw_error(env, nullptr, "napi_delete_reference fail for dst_qi_buf");
      return;
    }
  }
  
  status = napi_delete_async_work(env, worker_ctx->work);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_delete_async_work fail");
  }
  delete worker_ctx;
}

void _execute_rsa_gen(napi_env env, void* _data) {
  struct Worker_ctx_rsa_gen* worker_ctx = (struct Worker_ctx_rsa_gen*)_data;
  _rsa_gen_impl(worker_ctx->err, worker_ctx->size, worker_ctx->seed_buf, worker_ctx->seed_buf_len, worker_ctx->dst_n_buf, worker_ctx->dst_n_buf_len, worker_ctx->dst_e_buf, worker_ctx->dst_e_buf_len, worker_ctx->dst_d_buf, worker_ctx->dst_d_buf_len, worker_ctx->dst_p_buf, worker_ctx->dst_p_buf_len, worker_ctx->dst_q_buf, worker_ctx->dst_q_buf_len, worker_ctx->dst_dp_buf, worker_ctx->dst_dp_buf_len, worker_ctx->dst_dq_buf, worker_ctx->dst_dq_buf_len, worker_ctx->dst_qi_buf, worker_ctx->dst_qi_buf_len, worker_ctx->ret_n_size, worker_ctx->ret_e_size, worker_ctx->ret_d_size, worker_ctx->ret_p_size, worker_ctx->ret_q_size, worker_ctx->ret_dp_size, worker_ctx->ret_dq_size, worker_ctx->ret_qi_size);
}

void _complete_rsa_gen(napi_env env, napi_status execute_status, void* _data) {
  napi_status status;
  struct Worker_ctx_rsa_gen* worker_ctx = (struct Worker_ctx_rsa_gen*)_data;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //    prepare for callback (common parts)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value callback;
  status = napi_get_reference_value(env, worker_ctx->callback_reference, &callback);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to get referenced callback (napi_get_reference_value)");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_delete_reference(env, worker_ctx->callback_reference);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to delete reference callback_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  
  napi_value global;
  status = napi_get_global(env, &global);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create return value global (napi_get_global)");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  if (execute_status != napi_ok) {
    // avoid code duplication
    if (!worker_ctx->err) {
      worker_ctx->err = new std::string("execute_status != napi_ok");
    }
  }
  
  if (worker_ctx->err) {
    napi_helper_error_cb(env, worker_ctx->err->c_str(), callback);
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //    callback OK
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  napi_value result;
  napi_value call_argv[2];
  
  status = napi_get_undefined(env, &call_argv[0]);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_get_undefined FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_create_array_with_length(env, 8, &call_argv[1]);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_array_with_length FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  
  napi_value _ret_ret_n_size;
  status = napi_create_int32(env, worker_ctx->ret_n_size, &_ret_ret_n_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 0, _ret_ret_n_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=0 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_e_size;
  status = napi_create_int32(env, worker_ctx->ret_e_size, &_ret_ret_e_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 1, _ret_ret_e_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=1 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_d_size;
  status = napi_create_int32(env, worker_ctx->ret_d_size, &_ret_ret_d_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 2, _ret_ret_d_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=2 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_p_size;
  status = napi_create_int32(env, worker_ctx->ret_p_size, &_ret_ret_p_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 3, _ret_ret_p_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=3 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_q_size;
  status = napi_create_int32(env, worker_ctx->ret_q_size, &_ret_ret_q_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 4, _ret_ret_q_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=4 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_dp_size;
  status = napi_create_int32(env, worker_ctx->ret_dp_size, &_ret_ret_dp_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 5, _ret_ret_dp_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=5 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_dq_size;
  status = napi_create_int32(env, worker_ctx->ret_dq_size, &_ret_ret_dq_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 6, _ret_ret_dq_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=6 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  napi_value _ret_ret_qi_size;
  status = napi_create_int32(env, worker_ctx->ret_qi_size, &_ret_ret_qi_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_int32 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  status = napi_set_element(env, call_argv[1], 7, _ret_ret_qi_size);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_set_element ret_idx=7 FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  
  status = napi_call_function(env, global, callback, 2, call_argv, &result);
  if (status != napi_ok) {
    fprintf(stderr, "status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_call_function FAIL");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return;
  }
  _worker_ctx_rsa_gen_clear(env, worker_ctx);
}

napi_value rsa_gen(napi_env env, napi_callback_info info) {
  FN_ARG_HEAD(11)
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  FN_ARG_I32(size)
  FN_ARG_BUF_VAL(seed_buf)
  FN_ARG_BUF_VAL(dst_n_buf)
  FN_ARG_BUF_VAL(dst_e_buf)
  FN_ARG_BUF_VAL(dst_d_buf)
  FN_ARG_BUF_VAL(dst_p_buf)
  FN_ARG_BUF_VAL(dst_q_buf)
  FN_ARG_BUF_VAL(dst_dp_buf)
  FN_ARG_BUF_VAL(dst_dq_buf)
  FN_ARG_BUF_VAL(dst_qi_buf)
  napi_value callback = argv[arg_idx];
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  Worker_ctx_rsa_gen* worker_ctx = new Worker_ctx_rsa_gen;
  worker_ctx->err = nullptr;
  worker_ctx->seed_buf_ref = nullptr;
  worker_ctx->dst_n_buf_ref = nullptr;
  worker_ctx->dst_e_buf_ref = nullptr;
  worker_ctx->dst_d_buf_ref = nullptr;
  worker_ctx->dst_p_buf_ref = nullptr;
  worker_ctx->dst_q_buf_ref = nullptr;
  worker_ctx->dst_dp_buf_ref = nullptr;
  worker_ctx->dst_dq_buf_ref = nullptr;
  worker_ctx->dst_qi_buf_ref = nullptr;
  status = napi_create_reference(env, callback, 1, &worker_ctx->callback_reference);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_helper_error_cb(env, "Unable to create reference for callback. napi_create_reference", callback);
    delete worker_ctx;
    return ret_dummy;
  }
  
  // NOTE no free utf8 string
  napi_value async_resource_name;
  status = napi_create_string_utf8(env, "dummy", 5, &async_resource_name);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create value async_resource_name set to 'dummy'");
    delete worker_ctx;
    return ret_dummy;
  }
  
  worker_ctx->size = size;
  worker_ctx->seed_buf = seed_buf;
  worker_ctx->seed_buf_len = seed_buf_len;
  status = napi_create_reference(env, seed_buf_val, 1, &worker_ctx->seed_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for seed_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_n_buf = dst_n_buf;
  worker_ctx->dst_n_buf_len = dst_n_buf_len;
  status = napi_create_reference(env, dst_n_buf_val, 1, &worker_ctx->dst_n_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_n_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_e_buf = dst_e_buf;
  worker_ctx->dst_e_buf_len = dst_e_buf_len;
  status = napi_create_reference(env, dst_e_buf_val, 1, &worker_ctx->dst_e_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_e_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_d_buf = dst_d_buf;
  worker_ctx->dst_d_buf_len = dst_d_buf_len;
  status = napi_create_reference(env, dst_d_buf_val, 1, &worker_ctx->dst_d_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_d_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_p_buf = dst_p_buf;
  worker_ctx->dst_p_buf_len = dst_p_buf_len;
  status = napi_create_reference(env, dst_p_buf_val, 1, &worker_ctx->dst_p_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_p_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_q_buf = dst_q_buf;
  worker_ctx->dst_q_buf_len = dst_q_buf_len;
  status = napi_create_reference(env, dst_q_buf_val, 1, &worker_ctx->dst_q_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_q_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_dp_buf = dst_dp_buf;
  worker_ctx->dst_dp_buf_len = dst_dp_buf_len;
  status = napi_create_reference(env, dst_dp_buf_val, 1, &worker_ctx->dst_dp_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_dp_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_dq_buf = dst_dq_buf;
  worker_ctx->dst_dq_buf_len = dst_dq_buf_len;
  status = napi_create_reference(env, dst_dq_buf_val, 1, &worker_ctx->dst_dq_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_dq_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  worker_ctx->dst_qi_buf = dst_qi_buf;
  worker_ctx->dst_qi_buf_len = dst_qi_buf_len;
  status = napi_create_reference(env, dst_qi_buf_val, 1, &worker_ctx->dst_qi_buf_ref);
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "Unable to create reference for dst_qi_buf. napi_create_reference");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  status = napi_create_async_work(
    env,
    nullptr,
    async_resource_name,
    _execute_rsa_gen,
    _complete_rsa_gen,
    (void*)worker_ctx,
    &worker_ctx->work
  );
  if (status != napi_ok) {
    printf("status = %d\n", status);
    napi_throw_error(env, nullptr, "napi_create_async_work fail");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  status = napi_queue_async_work(env, worker_ctx->work);
  if (status != napi_ok) {
    napi_throw_error(env, nullptr, "napi_queue_async_work fail");
    _worker_ctx_rsa_gen_clear(env, worker_ctx);
    return ret_dummy;
  }
  
  
  return ret_dummy;
}
