/* This file was auto-generated by KreMLin! */
#include "kremlib.h"
#ifndef __Hacl_Impl_Bignum_H
#define __Hacl_Impl_Bignum_H

Prims_nat Hacl_Impl_Bignum_v(uint32_t x);

bool Hacl_Impl_Bignum_lt_u64(uint64_t a, uint64_t b);

FStar_UInt128_t Hacl_Impl_Bignum_mul_wide(uint64_t a, uint64_t b);

void Hacl_Impl_Bignum_fill(Prims_nat len, uint32_t clen, uint64_t *b, uint64_t z);

typedef struct 
{
  uint64_t fst;
  uint64_t snd;
}
K___uint64_t_uint64_t;



void
Hacl_Impl_Bignum_bn_mult_(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t aaLen,
  uint64_t *a,
  uint32_t bbLen,
  uint64_t *b,
  uint32_t j,
  uint32_t resLen,
  uint64_t *res
);

void
Hacl_Impl_Bignum_bn_mul(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t aaLen,
  uint64_t *a,
  uint32_t bbLen,
  uint64_t *b,
  uint64_t *res
);

void Hacl_Impl_Bignum_bn_mul_mod_fast(uint64_t *a, uint64_t *b, uint64_t *res);

void Hacl_Impl_Bignum_bn_sqr(Prims_nat aLen, uint32_t aaLen, uint64_t *a, uint64_t *res);

void
Hacl_Impl_Bignum_bn_mul_u64(
  Prims_nat aLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t b,
  uint64_t *res
);

void Hacl_Impl_Bignum_bn_reduce(uint64_t *a, uint64_t *res);

void Hacl_Impl_Bignum_bn_mul_mod(uint64_t *a, uint64_t *b, uint64_t *res);

void Hacl_Impl_Bignum_text_to_nat(uint8_t *input, uint64_t *res);

void Hacl_Impl_Bignum_nat_to_text(uint64_t *input, uint8_t *res);
#endif
