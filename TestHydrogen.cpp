#include <deepstate/DeepState.hpp>
using namespace deepstate;

#include "hydrogen.h"

static const char *ctx = "libtests";

TEST(Hydrogen, Basic) {
    if (hydro_init() != 0) {
        ASSERT (0) << "hydrogen failed to initialize!";
    }

    uint8_t key[hydro_secretbox_KEYBYTES];
    uint8_t m[25];
    uint8_t m2[25];
    uint8_t c[hydro_secretbox_HEADERBYTES + 25];
    uint8_t dk[hydro_random_SEEDBYTES];
    uint8_t probe[hydro_secretbox_PROBEBYTES];
    
    memset(dk, 0, sizeof dk);
    //hydro_random_buf_deterministic(m, sizeof m, dk);
    DeepState_SymbolizeData(m, m + sizeof m);
    bool anyNonZero = false;
    for (size_t i = 0; i < sizeof m; i++) {
      LOG(TRACE) << "m[" << i << "] = " << (unsigned char) m[i];
      if ((unsigned char) m[i] != 0) {
	anyNonZero = true;
      }
    }
    hydro_increment(dk, sizeof dk);
    //hydro_random_buf_deterministic(key, sizeof key, dk);
    DeepState_SymbolizeData(key, key + sizeof key);
    for (size_t i = 0; i < sizeof key; i++) {
      LOG(TRACE) << "key[" << i << "] = " << (unsigned char) key[i];
    }
    hydro_increment(dk, sizeof dk);
    hydro_secretbox_encrypt(c, m, sizeof m, 0, ctx, key);
    ASSERT (hydro_secretbox_decrypt(m2, c, sizeof c, 0, ctx, key) == 0) << "DECRYPT M FAILED";;
    ASSERT (hydro_equal(m, m2, sizeof m)) << "DECRYPT(ENCRYPT(M)) != M";

    hydro_secretbox_probe_create(probe, c, sizeof c, ctx, key);
    ASSERT (hydro_secretbox_probe_verify(probe, c, sizeof c, ctx, key) == 0) << "PROBE 0 FAILED";
    probe[0]++;
    ASSERT (hydro_secretbox_probe_verify(probe, c, sizeof c, ctx, key) == -1) << "PROBE 1 FAILED";
    probe[0]--;
    key[0]++;
    ASSERT (hydro_secretbox_probe_verify(probe, c, sizeof c, ctx, key) == -1) << "PROBE 2 FAILED";
    key[0]--;

    ASSERT (hydro_secretbox_decrypt(m2, c, 0, 0, ctx, key) == -1) << "DECRYPT M2 0 0 NOT -1";
    ASSERT (hydro_secretbox_decrypt(m2, c, 1, 0, ctx, key) == -1) << "DECRYPT M2 1 0 NOT -1";
    ASSERT (hydro_secretbox_decrypt(m2, c, hydro_secretbox_HEADERBYTES, 0, ctx, key) == -1) <<
      "DECRYPT HEADERBYTES 0 NOT -1";
    ASSERT (hydro_secretbox_decrypt(m2, c, sizeof c, 1, ctx, key) == -1) << "DECRYPT M2 SIZEEOF 1 NOT -1";
    ASSERT (!anyNonZero || !hydro_equal(m, m2, sizeof m)) << "M EQUALS M2";
    key[0]++;
    ASSERT (hydro_secretbox_decrypt(m2, c, sizeof c, 0, ctx, key) == -1) << "DECRYPT M2 SIZEOF 0 NOT -1";
    key[0]--;
    c[DeepState_UIntInRange(0, sizeof c - 1)]++;
    ASSERT (hydro_secretbox_decrypt(m2, c, sizeof c, 0, ctx, key) == -1) << "DECRYPT M2 RANDOMIZED NOT -1";
}
