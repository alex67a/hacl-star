/* This file was auto-generated by KreMLin! */

#include "Hacl_Impl_Bignum.h"
#include <immintrin.h>
#include <x86intrin.h>

static inline char addcarry_u64(char c,uint64_c a, uint64_t b, uint64_t* o) {
  uint64_t r = b;
  uint64_t x = a;
  char c;
 __asm__ __volatile__ ( \
 "adcxq %[x], %[y]\n\t" "setc %[z]" \
 : [y] "+r" (r), [z] "=r" (c)   \
 : [x]  "r" (x) \
 : "cc");
 *o = r
 return c;
}

static inline char addcarryx_u64(char c,uint64_c a, uint64_t b, uint64_t* o) {
  uint64_t r = b;
  uint64_t x = a;
  char c;
 __asm__ __volatile__ ( \
 "adcxq %[x], %[y]\n\t" "setc %[z]" \
 : [y] "+r" (r), [z] "=r" (c)   \
 : [x]  "r" (x) \
 : "cc");
 *o = r
 return c;
}


static inline uint64_t addcarryx_u64(uint64_t a, uint64_t b) {
 uint64_t r = b;
 uint64_t x = a;
 __asm__ __volatile__ ( \
 "adoxq %[x], %[y]" \
 : [y] "+r" (r)	\
 : [x]  "r" (x) \
 : "cc");
 return r;
}

static inline void clearcarry_u64() {
 uint64_t r = 0;
 __asm__ __volatile__ ( \
 "test %[x], %[x]" \
 : \
 : [x] "r" (r) \
 : "cc");
}


static inline uint64_t getcarry_u64() {
char r = 0;
 __asm__ __volatile__ ( \
 "setc %[x]" \
 : [x] "=r" (r)	\
 : \
 : "cc");
 return r;
}

static inline uint64_t getcarryx_u64() {
char r = 0;
 __asm__ __volatile__ ( \
 "seto %[x]" \
 : [x] "=r" (r)	\
 : \
 : "cc");
 return r;
}



Prims_nat Hacl_Impl_Bignum_v(uint32_t x)
{
  return FStar_UInt32_v(x);
}

bool Hacl_Impl_Bignum_lt_u64(uint64_t a, uint64_t b)
{
  return a < b;
}

FStar_UInt128_t Hacl_Impl_Bignum_mul_wide(uint64_t a, uint64_t b)
{
  return FStar_UInt128_mul_wide(a, b);
}

void Hacl_Impl_Bignum_fill(Prims_nat len, uint32_t clen, uint64_t *b, uint64_t z){
  for (uint32_t i = (uint32_t)0U; i < clen; i = i + (uint32_t)1U)
    b[i] = z;
}

/*
uint64_t
Hacl_Impl_Bignum_bn_add_(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint32_t i,
  uint64_t carry,
  uint64_t *res
)
{
  if (i < caLen)
  {
    uint64_t t1 = a[i];
    uint64_t t2;
    if (i < cbLen)
      t2 = b[i];
    else
      t2 = (uint64_t)0U;
    uint64_t t11 = t1 + carry;
    uint64_t carry1;
    if (t11 < carry)
      carry1 = (uint64_t)1U;
    else
      carry1 = (uint64_t)0U;
    uint64_t res_i = t11 + t2;
    res[i] = res_i;
    uint64_t carry2;
    if (res_i < t11)
      carry2 = carry1 + (uint64_t)1U;
    else
      carry2 = carry1;
    return Hacl_Impl_Bignum_bn_add_(aLen, bLen, caLen, a, cbLen, b, i + (uint32_t)1U, carry2, res);
  }
  else 
    return carry;
}

void
Hacl_Impl_Bignum_bn_add(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint64_t *res
)
{
  (void)Hacl_Impl_Bignum_bn_add_(aLen, bLen, caLen, a, cbLen, b, (uint32_t)0U, (uint64_t)0U, res);
}
*/

static inline uint64_t bval(uint32_t cbLen, uint64_t* b, uint32_t i) {
  return (i < cbLen? b[i]: 0);
}

inline static uint64_t
Hacl_Impl_Bignum_bn_add_(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint32_t i,
  uint8_t carry,
  uint64_t *res
)
{
  clearcarry_u64();
  for(int i = 0; i < caLen; i=i+1) {
    uint64_t bv = bval(cbLen,b,i);
    res[i] = addcarryx_u64(a[i],bv);
  }
  char c = getcarryx_u64();
  return c;
}

inline static void
Hacl_Impl_Bignum_bn_add(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint64_t *res
)
{
  (void)Hacl_Impl_Bignum_bn_add_(aLen,
    bLen,
    caLen,
    a,
    cbLen,
    b,
    (uint32_t)0U,
    (uint8_t)0U,
    res);
}

void
Hacl_Impl_Bignum_bn_add_carry(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint64_t *res
)
{
  uint64_t *res_ = res;
  uint64_t
  carry =
    Hacl_Impl_Bignum_bn_add_(aLen,
      bLen,
      caLen,
      a,
      cbLen,
      b,
      (uint32_t)0U,
      (uint8_t)0U,
      res_);
  res[caLen] = carry;
}

inline static void
Hacl_Impl_Bignum_bn_mult_by_limb_addj(
  Prims_nat aLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t l,
  uint32_t i,
  uint32_t j,
  uint32_t resLen,
  uint64_t carry,
  uint64_t *res
)
{
  uint32_t ij = i + j;
  if (i < aaLen)
  {
    uint64_t res_ij = res[ij];
    uint64_t uu____258 = a[i];
    FStar_UInt128_t u =
      FStar_UInt128_add(FStar_UInt128_add(FStar_UInt128_mul_wide(uu____258,
                  l),
                FStar_UInt128_uint64_to_uint128(carry)),
			FStar_UInt128_uint64_to_uint128(res_ij));
    uint64_t carry_ = FStar_UInt128_uint128_to_uint64(FStar_UInt128_shift_right(u,(uint32_t)64U));
    uint64_t res_ij1 = FStar_UInt128_uint128_to_uint64(u);
    res[ij] = res_ij1;
    Hacl_Impl_Bignum_bn_mult_by_limb_addj(aLen,
      aaLen,
      a,
      l,
      i + (uint32_t)1U,
      j,
      resLen,
      carry_,
      res);
  }
  else
    res[ij] = carry;
}

inline static void
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
)
{
  if (j < bbLen)
  {
    uint64_t uu____2231 = b[j];
    Hacl_Impl_Bignum_bn_mult_by_limb_addj(aLen,
      aaLen,
      a,
      uu____2231,
      (uint32_t)0U,
      j,
      resLen,
      (uint64_t)0U,
      res);
    Hacl_Impl_Bignum_bn_mult_(aLen, bLen, aaLen, a, bbLen, b, j + (uint32_t)1U, resLen, res);
  }
}

inline static void
Hacl_Impl_Bignum_bn_mul(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t aaLen,
  uint64_t *a,
  uint32_t bbLen,
  uint64_t *b,
  uint64_t *res
)
{
  uint32_t resLen = aaLen + bbLen;
  //Hacl_Impl_Bignum_fill(Prims_op_Addition(aLen, bLen), resLen, res, (uint64_t)0U);
  memset(res,0,8*resLen);
  Hacl_Impl_Bignum_bn_mult_(aLen, bLen, aaLen, a, bbLen, b, (uint32_t)0U, resLen, res);
}


inline static void mult_fast_loop(
  uint32_t aLen,
  uint64_t *x,
  uint64_t *y,
  uint64_t *z
)
{
  aLen = 4;
  char c = 0;
  uint64_t zero = 0;
  uint64_t u0, u1;
  clearcarry_u64();
  u0 = _mulx_u64(x[0], y[0],&u1);
  z[0] = u0;
  uint64_t hj = u1;
  uint64_t l,h0;
  for (int j = 1; j < aLen; j++){
    clearcarry_u64();
    u0 = _mulx_u64(x[0], y[j],&u1);
    l = u0;
    z[j] = addcarry_u64(hj,l);
    hj = u1;
  }
  z[aLen] = addcarry_u64(hj,zero); 

  uint64_t l0;
  char d = 0;
  clearcarry_u64(); 
  for (int i = 1; i < aLen; i++){
    char c = 0;
    u0 = _mulx_u64(x[i], y[0],&u1);
    h0 = u1; 
    l0 = u0;
    z[i] = addcarryx_u64(z[i],l0);
    hj = h0;

    for (int j = 1; j < aLen; j++){
      u0 = _mulx_u64(x[i], y[j],&u1);
      l = u0;
      hj = addcarry_u64(hj,l);
      z[i+j] = addcarryx_u64(hj,z[i + j]);
      hj = u1;
    }
    hj = addcarry_u64(hj,zero);
    z[i + aLen] = addcarryx_u64(hj,zero);
  }
}  


void mult_fast_inlined(
  uint32_t aLen,
  uint64_t *x,
  uint64_t *y,
  uint64_t *z
)
{
  char c = 0;
  char d = 0;
  uint64_t l0,h0,l1,h1,l2,h2,l3,h3;
  uint64_t z0,z1,z2,z3,z4;
  uint64_t x0 = x[0];
  clearcarry_u64();
  l0 = _mulx_u64(y[0],x0,&h0);
  z0 = l0;
  l1 = _mulx_u64(y[1],x0,&h1);
  z1 = addcarryx_u64(h0,l1);
  l2 = _mulx_u64(y[2],x0,&h2);
  z2 = addcarryx_u64(h1,l2);
  l3 = _mulx_u64(y[3],x0,&h3);
  z3 = addcarryx_u64(h2,l3);
  c = getcarryx_u64();
  z4 = h3 + c;
  z[0] = z0;
  z[1] = z1;
  z[2] = z2;
  z[3] = z3;
  z[4] = z4;
  
  for (int i = 1; i < 4; i++)
  {
    uint64_t xi = x[i];
    clearcarry_u64();
    l0 = _mulx_u64(y[0],xi,&h0);
    z0 = addcarryx_u64(z[i+0],l0);
    l1 = _mulx_u64(y[1],xi,&h1);
    h0 = addcarry_u64(h0,l1);
    z1 = addcarryx_u64(h0,z[i+1]);
    l2 = _mulx_u64(y[2],xi,&h2);
    h1 = addcarry_u64(h1,l2);
    z2 = addcarryx_u64(h1,z[i+2]);
    l3 = _mulx_u64(y[3],xi,&h3);
    h2 = addcarry_u64(h2,l3);
    z3 = addcarryx_u64(h2,z[i+3]);
    c = getcarry_u64();
    d = getcarryx_u64();
    z[i+0] = z0;
    z[i+1] = z1;
    z[i+2] = z2;
    z[i+3] = z3;
    z[i+4] = h3 + c + d;
  }
}  



void Hacl_Impl_Bignum_bn_sqr(Prims_nat aLen, uint32_t aaLen, uint64_t *a, uint64_t *res)
{
  uint32_t resLen = aaLen + aaLen;
  Hacl_Impl_Bignum_fill(Prims_op_Addition(aLen, aLen), resLen, res, (uint64_t)0U);
  Hacl_Impl_Bignum_bn_mult_(aLen, aLen, aaLen, a, aaLen, a, (uint32_t)0U, resLen, res);
}

void
Hacl_Impl_Bignum_bn_mul_u64(
  Prims_nat aLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t b,
  uint64_t *res
)
{
  uint32_t resLen = aaLen + (uint32_t)1U;
  Hacl_Impl_Bignum_fill(Prims_op_Addition(aLen, (krml_checked_int_t)1),
    resLen,
    res,
    (uint64_t)0U);
  Hacl_Impl_Bignum_bn_mult_by_limb_addj(aLen,
    aaLen,
    a,
    b,
    (uint32_t)0U,
    (uint32_t)0U,
    resLen,
    (uint64_t)0U,
    res);
}

void Hacl_Impl_Bignum_bn_reduce(uint64_t *a, uint64_t *res)
{
  uint64_t *a0 = a;
  uint64_t *a1 = a + (uint32_t)4U;
  uint64_t buf[5U] = { 0U };
  Hacl_Impl_Bignum_bn_mul_u64((krml_checked_int_t)4, (uint32_t)4U, a1, (uint64_t)38U, buf);
  Hacl_Impl_Bignum_bn_add((krml_checked_int_t)5,
    (krml_checked_int_t)4,
    (uint32_t)5U,
    buf,
    (uint32_t)4U,
    a0,
    buf);
  uint64_t *c0 = buf;
  uint64_t c1 = buf[4U];
  res[0U] = (uint64_t)38U * c1;
  Hacl_Impl_Bignum_bn_add((krml_checked_int_t)4,
    (krml_checked_int_t)4,
    (uint32_t)4U,
    res,
    (uint32_t)4U,
    c0,
    res);
  /* uint64_t *c01 = buf;
  uint64_t c11 = buf[4U];
  res[0U] = (uint64_t)38U * c11;
  Hacl_Impl_Bignum_bn_add((krml_checked_int_t)4,
    (krml_checked_int_t)4,
    (uint32_t)4U,
    res,
    (uint32_t)4U,
    c01,
    res); */
}

void Hacl_Impl_Bignum_bn_mul_mod(uint64_t *a, uint64_t *b, uint64_t *res)
{
  Hacl_Impl_Bignum_fill((krml_checked_int_t)4, (uint32_t)4U, res, (uint64_t)0U);
  uint64_t buf[8U] = { 0U };
  Hacl_Impl_Bignum_bn_mul((krml_checked_int_t)4,
    (krml_checked_int_t)4,
    (uint32_t)4U,
    a,
    (uint32_t)4U,
    b,
    buf);
  /*  printf("\nusual:");
  for (int i = 0; i < 8; i++) 
    printf("%llx, ",buf[i]);
    printf("\n"); */
    Hacl_Impl_Bignum_bn_reduce(buf, res);
}

void Hacl_Impl_Bignum_bn_mul_mod_fast(uint64_t *a, uint64_t *b, uint64_t *res)
{
  Hacl_Impl_Bignum_fill((krml_checked_int_t)4, (uint32_t)4U, res, (uint64_t)0U);
  uint64_t buf[8U] = { 0U };
  mult_fast_inlined((uint32_t)4U,
    a,
    b,
    buf);
  /*  printf("\nfast:");
  for (int i = 0; i < 8; i++) 
    printf("%llx, ",buf[i]);
    printf("\n"); */
    Hacl_Impl_Bignum_bn_reduce(buf, res);
}

void Hacl_Impl_Bignum_text_to_nat(uint8_t *input, uint64_t *res)
{
  uint64_t uu____4430 = load64_be(input + (uint32_t)24U);
  res[0U] = uu____4430;
  uint64_t uu____4504 = load64_be(input + (uint32_t)16U);
  res[1U] = uu____4504;
  uint64_t uu____4578 = load64_be(input + (uint32_t)8U);
  res[2U] = uu____4578;
  uint64_t uu____4651 = load64_be(input);
  res[3U] = uu____4651;
}

void Hacl_Impl_Bignum_nat_to_text(uint64_t *input, uint8_t *res)
{
  uint64_t uu____4864 = input[3U];
  store64_be(res, uu____4864);
  uint64_t uu____4938 = input[2U];
  store64_be(res + (uint32_t)8U, uu____4938);
  uint64_t uu____5012 = input[1U];
  store64_be(res + (uint32_t)16U, uu____5012);
  uint64_t uu____5085 = input[0U];
  store64_be(res + (uint32_t)24U, uu____5085);
}

