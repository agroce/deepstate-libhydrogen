#include <deepstate/DeepState.hpp>
using namespace deepstate;

#include "hydrogen.h"

#define TRIES 64

TEST(Hydrogen, Basic) {
    if (hydro_init() != 0) {
        ASSERT (0) << "hydrogen failed to initialize!";
    }
    
    for (unsigned i = 0; i < TRIES; i++) {
        uint32_t upper_bound = DeepState_UInt();
        uint32_t rand_uniform = hydro_random_uniform(upper_bound);
	if (upper_bound > 0) {
	  ASSERT (rand_uniform < upper_bound) << "With upper bound " <<
            upper_bound << " hydro_random_uniform returned " <<
            rand_uniform;
	}
    }
}
