static std::mutex state_mutex;
static u8 state[32];
static int state_idx = 0;
static void stdlib_rand_cleanup() {}
static int stdlib_rand_add(const void *buf, int num, double add_entropy) {
  return 0;
}
static int stdlib_rand_status() {
  return 1;
}

static int stdlib_rand_seed(const void *buf, int num) {
  memcpy(state, buf, 32);
  state_idx = 0;
  return 0;
}

static int stdlib_rand_bytes(unsigned char *buf, int num) {
  for( int index = 0; index < num; ++index ) {
    buf[index] = state[state_idx++];
    if (state_idx >= 32) {
      state_idx = 0;
      SHA256_CTX sha256;
      SHA256_Init(&sha256);
      SHA256_Update(&sha256, state, 32);
      SHA256_Final(state, &sha256);
    }
  }
  return 1;
}

RAND_METHOD stdlib_rand_meth = {
  stdlib_rand_seed,
  stdlib_rand_bytes,
  stdlib_rand_cleanup,
  stdlib_rand_add,
  stdlib_rand_bytes,
  stdlib_rand_status
};

RAND_METHOD *RAND_stdlib() {
  return &stdlib_rand_meth;
}