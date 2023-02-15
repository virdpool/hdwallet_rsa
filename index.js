var module = this;
var crypto = require("crypto");
var mod = require("./build/Release/module");

var base64url_encode = function(t) {
  var res = t.toString("base64")
  
  // https://github.com/brianloveswords/base64url/blob/master/src/base64url.ts
  res = res.replace(/=/g, "")
  res = res.replace(/\+/g, "-")
  res = res.replace(/\//g, "_")
  return res;
}

this.getKeySync = function(secret_buf, idx) {
  var idx_buf = Buffer.from(idx.toString());
  var seed_buf = crypto.createHash("sha256").update(secret_buf).update(idx_buf).digest();
  
  dst_n_buf = Buffer.alloc(512);
  dst_e_buf = Buffer.alloc(3);
  dst_d_buf = Buffer.alloc(512);
  dst_p_buf = Buffer.alloc(256);
  dst_q_buf = Buffer.alloc(256);
  dst_dp_buf= Buffer.alloc(256);
  dst_dq_buf= Buffer.alloc(256);
  dst_qi_buf= Buffer.alloc(256);
  
  var ret = mod.rsa_gen_sync(4096, seed_buf,
    dst_n_buf,
    dst_e_buf,
    dst_d_buf,
    dst_p_buf,
    dst_q_buf,
    dst_dp_buf,
    dst_dq_buf,
    dst_qi_buf
  )
  let [
    ret_n_size,
    ret_e_size,
    ret_d_size,
    ret_p_size,
    ret_q_size,
    ret_dp_size,
    ret_dq_size,
    ret_qi_size,
  ] = ret;
  
  return {
    kty: "RSA",
    n : base64url_encode(dst_n_buf.slice(0, ret_n_size)),
    e : base64url_encode(dst_e_buf.slice(0, ret_e_size)),
    d : base64url_encode(dst_d_buf.slice(0, ret_d_size)),
    p : base64url_encode(dst_p_buf.slice(0, ret_p_size)),
    q : base64url_encode(dst_q_buf.slice(0, ret_q_size)),
    dp: base64url_encode(dst_dp_buf.slice(0, ret_dp_size)),
    dq: base64url_encode(dst_dq_buf.slice(0, ret_dq_size)),
    qi: base64url_encode(dst_qi_buf.slice(0, ret_qi_size)),
  };
}

this.getKey = function(secret_buf, idx, cb) {
  var idx_buf = Buffer.from(idx.toString());
  var seed_buf = crypto.createHash("sha256").update(secret_buf).update(idx_buf).digest();
  
  dst_n_buf = Buffer.alloc(512);
  dst_e_buf = Buffer.alloc(3);
  dst_d_buf = Buffer.alloc(512);
  dst_p_buf = Buffer.alloc(256);
  dst_q_buf = Buffer.alloc(256);
  dst_dp_buf= Buffer.alloc(256);
  dst_dq_buf= Buffer.alloc(256);
  dst_qi_buf= Buffer.alloc(256);
  
  mod.rsa_gen(4096, seed_buf,
    dst_n_buf,
    dst_e_buf,
    dst_d_buf,
    dst_p_buf,
    dst_q_buf,
    dst_dp_buf,
    dst_dq_buf,
    dst_qi_buf,
    function(err, ret) {
      if (err) return cb(err);
      let [
        ret_n_size,
        ret_e_size,
        ret_d_size,
        ret_p_size,
        ret_q_size,
        ret_dp_size,
        ret_dq_size,
        ret_qi_size,
      ] = ret;
      
      cb(null, {
        kty: "RSA",
        n : base64url_encode(dst_n_buf.slice(0, ret_n_size)),
        e : base64url_encode(dst_e_buf.slice(0, ret_e_size)),
        d : base64url_encode(dst_d_buf.slice(0, ret_d_size)),
        p : base64url_encode(dst_p_buf.slice(0, ret_p_size)),
        q : base64url_encode(dst_q_buf.slice(0, ret_q_size)),
        dp: base64url_encode(dst_dp_buf.slice(0, ret_dp_size)),
        dq: base64url_encode(dst_dq_buf.slice(0, ret_dq_size)),
        qi: base64url_encode(dst_qi_buf.slice(0, ret_qi_size)),
      });
    }
  )
}

this.getKeyPromise = function(secret_buf, idx) {
  return new Promise(function(resolve, reject) {
    module.getKey(secret_buf, idx, function(err, res){
      if (err) return reject(err);
      resolve(res);
    })
  })
}
