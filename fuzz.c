#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "warlogs.h"

int LLVMFuzzerTestOneInput(
	const uint8_t *data,
	size_t        size
) {
	int64_t ts;
	wl_event e;
	char* str = malloc(size + 1);
	memcpy(str, data, size);
	str[size] = '\0';
	wl_parse(&ts, &e, str);
	free(str);
	return 0;
}
