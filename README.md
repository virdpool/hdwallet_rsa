# hdwallet_rsa
deterministic wallet generation for arweave RSA keys

deps
```
apt-get install -y libssl-dev
```

Only 4096 keys are supported, but it can be easily tweaked in code

install
```
npm i virdpool/hdwallet_rsa
```

Note. This mode is slower than non-deterministic version, because I use SHA2 intensively for PRNG (used for prime numbers generator)
