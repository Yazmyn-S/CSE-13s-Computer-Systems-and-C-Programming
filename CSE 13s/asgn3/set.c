#include "set.h"

Set set_empty() {
    // All bits are set to 0.
    return 0;
}

Set set_universal(void) {
    // All bits are set to 1 by inverting 0.
    // This should be equal to 2^8 - 1 (255).
    return ~0;
}

bool set_member(Set s, int x) {
    // Return true if both are set.
    return (s & (1 << x));
}

Set set_insert(Set s, int x) {
    // Return a int with all of s and the new bit set.
    return (s | (1 << x));
}

Set set_remove(Set s, int x) {
    // Get the bit that we want to set to 0 and set it to 1.
    // Invert this number so that we have all 1s and exactly one zero.
    // Return and with s, so that we set all fields that S has set except
    // for x.
    return (s & ~(1 << x));
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_difference(Set s, Set t) {
    // XOR will return a number that has exactly on bit set between s and t
    // If we AND this with original, it should have only the bits that are in S
    // but not in t.
    return (s & (s ^ t));
}

Set set_complement(Set s) {
    // Negate will flip all the bits.
    return ~s;
}
