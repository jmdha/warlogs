#ifndef WARLOGS_H
#define WARLOGS_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WL_MAX_INT 32
#define WL_MAX_GUID 32
#define WL_MAX_NAME 64

typedef enum wl_error {
	wl_ok,
	wl_invalid_num,
	wl_invalid_year,
	wl_invalid_month,
	wl_invalid_day,
	wl_invalid_hour,
	wl_invalid_min,
	wl_invalid_sec,
	wl_invalid_ns,
	wl_invalid_ts_delim,
	wl_invalid_date,
} wl_error;

typedef enum wl_event_kind {
	wl_event_unknown
} wl_event_kind;

typedef struct wl_event {
	wl_event_kind kind;
} wl_event;

// Each digit is validated before the next byte is read,
// so a NUL terminator is never read past
static int wl_parse_int2(
	const char* str
) {
	unsigned v0 = (unsigned)(str[0] - '0');
	if (v0 > 9) return INT_MAX;
	unsigned v1 = (unsigned)(str[1] - '0');
	if (v1 > 9) return INT_MAX;
	return 10 * v0 + v1;
}

static int wl_parse_int4(
	const char* str
) {
	unsigned v0 = (unsigned)(str[0] - '0');
	if (v0 > 9) return INT_MAX;
	unsigned v1 = (unsigned)(str[1] - '0');
	if (v1 > 9) return INT_MAX;
	unsigned v2 = (unsigned)(str[2] - '0');
	if (v2 > 9) return INT_MAX;
	unsigned v3 = (unsigned)(str[3] - '0');
	if (v3 > 9) return INT_MAX;
	return 1000 * v0 + 100 * v1 + 10 * v2 + v3;
}

// NUL is not a digit, so the loop halts at the terminator
static int wl_parse_uint(
	const char** str
) {
	const char* s = *str;
	int val = 0;
	for (; (unsigned)(*s - '0') <= 9; s++) {
		const int n = *s - '0';
		if (val > (INT_MAX - n) / 10)
			return INT_MAX;
		val = 10 * val + n;
	}
	if (s == *str)
		return INT_MAX;
	*str = s;
	return val;
}

static int64_t wl_days_from_civil(int y, int m, int d) {
    y -= m <= 2;
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = (unsigned)(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return (int64_t)era * 146097 + (int64_t)doe - 719468;
}

// Parses date format of x/y/z
// where x is month, y is day, z is year
// non-fixed width format
static wl_error wl_parse_date(
	int*         year,
	int*         month,
	int*         day,
	const char** str
) {
	*month = wl_parse_uint(str);
	if (*month == INT_MAX || **str != '/' || *month < 1 || *month > 12)
		return wl_invalid_date;
	(*str)++;

	*day = wl_parse_uint(str);
	if (*day == INT_MAX || **str != '/' || *day < 1 || *day > 31)
		return wl_invalid_date;
	(*str)++;

	// year is technically non-fixed width
	// however realistically it will always be width 4
	*year = wl_parse_int4(*str);
	if (*year == INT_MAX || *year < 2000 || *year > 2040)
		return wl_invalid_date;
	*str += 4;

	return wl_ok;
}

// Parses time format of xx:yy:zz.qqqq
// where x is hour, y is min, z is sec, and q is ns
// fixed width format, each field validated before the delimiter after it is read
static wl_error wl_parse_time(
	int*        hour,
	int*        min,
	int*        sec,
	int*        f,
	const char* str
) {
	*hour = wl_parse_int2(str);
	if (*hour == INT_MAX || str[2] != ':')
		return wl_invalid_date;
	*min = wl_parse_int2(str + 3);
	if (*min == INT_MAX || str[5] != ':')
		return wl_invalid_date;
	*sec = wl_parse_int2(str + 6);
	if (*sec == INT_MAX || str[8] != '.')
		return wl_invalid_date;
	*f = wl_parse_int4(str + 9);
	if (*f == INT_MAX)
		return wl_invalid_date;
	if (*hour > 23 || *min > 59 || *sec > 59)
		return wl_invalid_date;

	return wl_ok;
}

static wl_error wl_parse_timestamp(
	int64_t*     ts,
	const char** str
) {
	int y, m, d, H, M, S, F;
	wl_error err;

	err = wl_parse_date(&y, &m, &d, str);
	if (err != wl_ok)
		return err;

	if (**str != ' ')
		return wl_invalid_date;
	(*str)++;

	err = wl_parse_time(&H, &M, &S, &F, *str);
	if (err != wl_ok)
		return err;
	*str += 13;

	*ts = wl_days_from_civil(y,m,d)*86400LL + H*3600 + M*60 + S;
	*ts = (*ts * 1000000000LL) + ((int64_t)F * 100000LL);

	return wl_ok;
}

// str must be NUL terminated
static wl_error wl_parse(
	int64_t*    ts,
	wl_event*   e,
	const char* str
) {
	return wl_parse_timestamp(ts, &str);
}

#endif
