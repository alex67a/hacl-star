module Hacl.Impl.Frodo.Pack

open FStar.HyperStack.All
open FStar.HyperStack
open FStar.HyperStack.ST
open FStar.Mul

open LowStar.Buffer

open Lib.IntTypes
open Lib.PQ.Buffer
open Lib.Endianness

open Hacl.Impl.Matrix
open Hacl.Impl.Frodo.Params

module ST = FStar.HyperStack.ST
module Lemmas = Spec.Frodo.Lemmas
module S = Spec.Frodo.Pack

#reset-options "--z3rlimit 50 --max_fuel 0 --max_ifuel 0 --using_facts_from '* -Spec +Spec.Frodo.Pack +Spec.Matrix'"

module Seq = Lib.Sequence

inline_for_extraction noextract
val frodo_pack8:
    a:lbuffer uint16 8
  -> d:size_t{v d <= 16}
  -> res:lbytes d
  -> Stack unit
    (requires fun h0 -> live h0 a /\ live h0 res /\ disjoint a res)
    (ensures  fun h0 _ h1 -> modifies (loc_buffer res) h0 h1 /\
      as_seq h1 res == S.frodo_pack8 (as_seq h0 a) (v d))
let frodo_pack8 a d res =
  let h0 = ST.get() in
  push_frame();
  let maskd = to_u16 (u32 1 <<. size_to_uint32 d) -. u16 1 in
  let v16 = create #_ #16 (size 16) (u8 0) in
  let a0 = index a (size 0) &. maskd in
  let a1 = index a (size 1) &. maskd in
  let a2 = index a (size 2) &. maskd in
  let a3 = index a (size 3) &. maskd in
  let a4 = index a (size 4) &. maskd in
  let a5 = index a (size 5) &. maskd in
  let a6 = index a (size 6) &. maskd in
  let a7 = index a (size 7) &. maskd in
  let templong =
       to_u128 a0 <<. size_to_uint32 (size 7 *! d)
    |. to_u128 a1 <<. size_to_uint32 (size 6 *! d)
    |. to_u128 a2 <<. size_to_uint32 (size 5 *! d)
    |. to_u128 a3 <<. size_to_uint32 (size 4 *! d)
    |. to_u128 a4 <<. size_to_uint32 (size 3 *! d)
    |. to_u128 a5 <<. size_to_uint32 (size 2 *! d)
    |. to_u128 a6 <<. size_to_uint32 (size 1 *! d)
    |. to_u128 a7 <<. size_to_uint32 (size 0 *! d)
  in
  uint_to_bytes_be v16 templong;
  let src = sub v16 (size 16 -! d) d in // Skips the 1st byte when d = 15
  copy res d src;
  pop_frame()

inline_for_extraction noextract
val frodo_pack8':
    #n1:size_t
  -> #n2:size_t{v n1 * v n2 <= max_size_t /\ v n2 % 8 = 0}
  -> a8:lbuffer uint16 8
  -> d:size_t{v d * (v n1 * v n2 / 8) <= max_size_t /\ v d <= 16}
  -> res:lbytes (d *! (n1 *! n2 /. size 8))
  -> i:size_t{v i < v n1 * v n2 / 8}
  -> Stack unit
    (requires fun h0 -> live h0 a8 /\ live h0 res /\ disjoint a8 res)
    (ensures  fun h0 _ h1 ->
      modifies (loc_buffer res) h0 h1 /\
      Seq.equal
        (as_seq h1 (gsub #_ #_ #(v d) res (d *! i) d))
        (S.frodo_pack8 (as_seq h0 a8) (v d)) /\
      (forall (j:nat{j < v i}).
        {:pattern as_seq h1 (gsub #_ #_ #(v d) res (d *! size j) d)}
        v d * j + v d <= v d * (v n1 * v n2 / 8) /\
        Seq.equal #_ #(v d)
          (as_seq h1 (gsub #_ #_ #(v d) res (d *! size j) d))
          (as_seq h0 (gsub #_ #_ #(v d) res (d *! size j) d))))
let frodo_pack8' #n1 #n2 a8 d res i =
  let r = sub #_ #_ #(v d) res (d *! i) d in
  frodo_pack8 a8 d r

val frodo_pack:
    #n1:size_t
  -> #n2:size_t{v n1 * v n2 <= max_size_t /\ v n2 % 8 = 0}
  -> a:matrix_t n1 n2
  -> d:size_t{v d * (v n1 * v n2 / 8) <= max_size_t /\ v d <= 16}
  -> res:lbytes (d *! (n1 *! n2 /. size 8))
  -> Stack unit
    (requires fun h -> live h a /\ live h res /\ disjoint a res)
    (ensures  fun h0 _ h1 ->
      modifies (loc_buffer res) h0 h1 /\
      as_seq h1 res == S.frodo_pack (as_matrix h0 a) (v d))
[@"c_inline"]
let frodo_pack #n1 #n2 a d res =
  let h0 = ST.get () in
  assert (forall (j:nat{j < v n1 * v n2 / 8}). v d * j + v d <= v d * (v n1 * v n2 / 8));
  let inv h (i:nat{i <= v n1 * v n2 / 8}) =
    modifies (loc_buffer res) h0 h /\
    (forall (j:nat{j < i}).//{:pattern as_seq h1 (gsub #_ #_ #(v d) res (d *! size j) d)}
      let a8 = Seq.sub #uint16 (as_matrix h0 a) (8 * j) 8 in
      Seq.equal
        (as_seq h (gsub #_ #_ #(v d) res (d *! size j) d))
        (S.frodo_pack8 a8 (v d)))
  in
  let f (i:size_t{v i < v n1 * v n2 / 8}) : Stack unit
    (requires fun h1 -> inv h1 (v i))
    (ensures  fun _ _ h2 -> inv h2 (v i + 1)) =
      let a8 = sub a (size 8 *! i) (size 8) in
      frodo_pack8' a8 d res i
  in
  Lib.Loops.for (size 0) (n1 *! n2 /. size 8) inv f;
  let h1 = ST.get() in
  let a0 = as_matrix h0 a in
  let res0 = S.frodo_pack a0 (v d) in
  let f (j:nat{j < v n1 * v n2 / 8}) : Lemma
    (let a8 = Seq.sub #uint16 a0 (8 * j) 8 in
     Seq.equal
       (as_seq h1 (gsub #_ #_ #(v d) res (d *! size j) d))
       (Seq.sub res0 (v d * j) (v d)))
  = 
    let a8 = Seq.sub #uint16 a0 (8 * j) 8 in
    Seq.eq_elim (Seq.sub res0 (v d * j) (v d)) (S.frodo_pack8 a8 (v d))
  in
  S.equal_slices (v n1 * v n2 / 8) (v d) (as_seq h1 res) (S.frodo_pack a0 (v d)) f


/// Unpack

inline_for_extraction noextract
val frodo_unpack8:
    d:size_t{v d <= 16}
  -> b:lbytes d
  -> res:lbuffer uint16 8
  -> Stack unit
    (requires fun h0 -> live h0 b /\ live h0 res /\ disjoint b res)
    (ensures  fun h0 _ h1 -> modifies (loc_buffer res) h0 h1 /\
      Seq.equal (as_seq h1 res) (S.frodo_unpack8 (v d) (as_seq h0 b)))
let frodo_unpack8 d b res =
  let h0 = ST.get() in
  push_frame();
  let maskd = to_u16 (u32 1 <<. size_to_uint32 d) -. u16 1 in
  let src = create #_ #16 (size 16) (u8 0) in
  update_sub src (size 16 -! d) d b;
  let templong = uint_from_bytes_be src in
  res.(size 0) <- to_u16 (templong >>. size_to_uint32 (size 7 *! d)) &. maskd;
  res.(size 1) <- to_u16 (templong >>. size_to_uint32 (size 6 *! d)) &. maskd;
  res.(size 2) <- to_u16 (templong >>. size_to_uint32 (size 5 *! d)) &. maskd;
  res.(size 3) <- to_u16 (templong >>. size_to_uint32 (size 4 *! d)) &. maskd;
  res.(size 4) <- to_u16 (templong >>. size_to_uint32 (size 3 *! d)) &. maskd;
  res.(size 5) <- to_u16 (templong >>. size_to_uint32 (size 2 *! d)) &. maskd;
  res.(size 6) <- to_u16 (templong >>. size_to_uint32 (size 1 *! d)) &. maskd;
  res.(size 7) <- to_u16 (templong >>. size_to_uint32 (size 0 *! d)) &. maskd;
  pop_frame()

inline_for_extraction noextract
val frodo_unpack8':
    #n1:size_t
  -> #n2:size_t{v n1 * v n2 <= max_size_t /\ v n2 % 8 = 0}
  -> d:size_t{v d * (v n1 * v n2 / 8) <= max_size_t /\ v d <= 16}
  -> b:lbytes d
  -> res:matrix_t n1 n2
  -> i:size_t{v i < v n1 * v n2 / 8}
  -> Stack unit
    (requires fun h0 -> live h0 b /\ live h0 res /\ disjoint b res)
    (ensures  fun h0 _ h1 ->
      modifies (loc_buffer res) h0 h1 /\
      Seq.equal
        (as_seq h1 (gsub #_ #_ #8 res (size 8 *! i) (size 8)))
        (S.frodo_unpack8 (v d) (as_seq h0 b)) /\
      (forall (j:nat{j < v i}).
        {:pattern as_seq h1 (gsub #_ #_ #8 res (size 8 *! size j) (size 8))}
        8 * j + 8 <= v n1 * v n2 /\
        Seq.equal #_ #8
          (as_seq h1 (gsub #_ #_ #8 res (size 8 *! size j) (size 8)))
          (as_seq h0 (gsub #_ #_ #8 res (size 8 *! size j) (size 8)))))
let frodo_unpack8' #n1 #n2 d b res i =
  let res = sub res (size 8 *! i) (size 8) in
  frodo_unpack8 d b res

val frodo_unpack:
    n1:size_t
  -> n2:size_t{v n1 * v n2 <= max_size_t /\ v n2 % 8 = 0}
  -> d:size_t{v d * (v n1 * v n2 / 8) <= max_size_t /\ v d <= 16}
  -> b:lbytes (d *! (n1 *! n2 /. size 8))
  -> res:matrix_t n1 n2
  -> Stack unit
    (requires fun h -> live h b /\ live h res /\ disjoint b res)
    (ensures  fun h0 _ h1 -> modifies (loc_buffer res) h0 h1 /\
      as_seq h1 res == S.frodo_unpack #(v n1) #(v n2) (v d) (as_seq h0 b))
[@"c_inline"]
let frodo_unpack n1 n2 d b res =
  assert (8 * (v n1 * v n2 / 8) == 8 * (v n1 * (v n2 / 8)));
  assert (8 * (v n1 * v n2 / 8) == (v n1 * v n2));
  assert (forall (j:nat{j < v n1 * v n2 / 8}). v d * j + v d <= v d * (v n1 * v n2 / 8));
  let h0 = ST.get () in
  let inv h (i:nat{i <= v n1 * v n2 / 8}) =
    modifies (loc_buffer res) h0 h /\
    (forall (j:nat{j < i}).
      let b = Seq.sub #_ #(v d * (v n1 * v n2 / 8)) (as_seq h0 b) (v d * j) (v d) in
      Seq.equal #uint16 #8
        (as_seq h (gsub #uint16 #(v n1 * v n2) #8 res (size 8 *! size j) (size 8)))
        (S.frodo_unpack8 (v d) b))
  in
  let f (i:size_t{v i < v n1 * v n2 / 8}) : Stack unit
    (requires fun h1 -> inv h1 (v i))
    (ensures  fun _ _ h2 -> inv h2 (v i + 1)) =
      let b = sub b (d *! i) d in
      frodo_unpack8' d b res i
  in
  assert (forall (j:nat{j < v n1 * v n2 / 8}). 0 <= v d * j /\ v d * j <= max_size_t);
  Lib.Loops.for (size 0) (n1 *! n2 /. size 8) inv f;
  let h1 = ST.get() in
  let b0 = as_seq h0 b in
  let res0 = S.frodo_unpack #(v n1) #(v n2) (v d) b0 in
  let f (j:nat{j < v n1 * v n2 / 8}) : Lemma
    (Seq.equal
       (Seq.sub #_ #(v n1 * v n2) (as_seq h1 res) (8 * j) 8)
       (Seq.sub res0 (8 * j) 8))
  =
    let b = Seq.sub #_ #(v d * (v n1 * v n2 / 8)) b0 (v d * j) (v d) in
    Seq.eq_elim (Seq.sub res0 (8 * j) 8) (S.frodo_unpack8 (v d) b)
  in
  S.equal_slices (v n1 * v n2 / 8) 8 (as_seq h1 res) res0 f
