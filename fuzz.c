#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "warlogs.h"

int LLVMFuzzerTestOneInput(
	const uint8_t *data,
	size_t        size
) {
	int64_t ts;
	wl_event e;
	wl_parse(&ts, &e, (const char*)data, size);
	return 0;
}
