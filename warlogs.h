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

// assumes str >= 2
static int wl_parse_int2(
	const char* str
) {
	unsigned v0 = (unsigned)(str[0] - '0');
	unsigned v1 = (unsigned)(str[1] - '0');
	if (v0 > 9 || v1 > 9)
		return INT_MAX;
	return 10 * v0 + v1;
}

// assumes str >= 4
static int wl_parse_int4(
	const char* str
) {
	unsigned v0 = (unsigned)(str[0] - '0');
	unsigned v1 = (unsigned)(str[1] - '0');
	unsigned v2 = (unsigned)(str[2] - '0');
	unsigned v3 = (unsigned)(str[3] - '0');
	if (v0 > 9 || v1 > 9 || v2 > 9 || v3 > 9)
		return INT_MAX;
	return 1000 * v0 + 100 * v1 + 10 * v2 + v3;
}

static int wl_parse_uint(
	const char** str,
	size_t*      len
) {
	const size_t ilen = *len;
	int val = 0;
	while (*len > 0 && **str >= '0' && **str <= '9') {
		const int n = **str - '0';
		if (val > (INT_MAX - n) / 10)
			return INT_MAX;
		val = 10 * val + n;
		(*str)++;
		(*len)--;
	}
	if (ilen == *len)
		return INT_MAX;
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
	const char** str,
	size_t*      len
) {
	*month = wl_parse_uint(str, len);
	if (*month == INT_MAX)
		return wl_invalid_date;
	if (*len == 0 || **str != '/')
		return wl_invalid_date;
	if (*month < 1 || *month > 12)
		return wl_invalid_date;

	(*str)++;
	(*len)--;

	*day = wl_parse_uint(str, len);
	if (*day == INT_MAX)
		return wl_invalid_date;
	if (*len == 0 || **str != '/')
		return wl_invalid_date;
	if (*day < 1 || *day > 31)
		return wl_invalid_date;

	(*str)++;
	(*len)--;

	if (*len < 4)
		return wl_invalid_date;

	// year is technically non-fixed width
	// however realistically it will always be width 4
	*year = wl_parse_int4(str[0]);
	if (*year == INT_MAX)
		return wl_invalid_date;
	if (*year < 1970 || *year > 2050)
		return wl_invalid_date;

	*str += 4;
	*len -= 4;

	return wl_ok;
}

// Parses time format of xx:yy:zz:qqqq
// where x is hour, y is min, z is sec, and q is ns
// fixed width format
static wl_error wl_parse_time(
	int*        hour,
	int*        min,
	int*        sec,
	int*        f,
	const char* str
) {
	if (str[2] != ':' || str[5] != ':' || str[8] != '.')
		return wl_invalid_date;

	*hour = wl_parse_int2(&str[0]);
	*min  = wl_parse_int2(&str[3]);
	*sec  = wl_parse_int2(&str[6]);
	*f    = wl_parse_int4(&str[9]);

	if (*hour == INT_MAX || *min == INT_MAX || *sec == INT_MAX || *f == INT_MAX)
		return wl_invalid_date;
	if (*hour < 0 || *hour > 23 || *min < 0 || *min > 59 || *sec < 0 || *sec > 59 || *f < 0 || *f > 9999)
		return wl_invalid_date;

	return wl_ok;
}

static wl_error wl_parse_timestamp(
	int64_t*      ts,
	const char** str,
	size_t*      len
) {
	int y, m, d, H, M, S, F;
	wl_error err;

	err = wl_parse_date(&y, &m, &d, str, len);
	if (err != wl_ok)
		return err;

	if (*len < 14)
		return wl_invalid_date;

	(*str)++;
	(*len)--;

	err = wl_parse_time(&H, &M, &S, &F, *str);
	if (err != wl_ok)
		return err;

	*len -= 13;
	*str += 13;

        *ts = wl_days_from_civil(y,m,d)*86400LL + H*3600 + M*60 + S;
	*ts = (*ts * 1000000000LL) + ((int64_t)F * 100000LL);

	return wl_ok;
}

static wl_error wl_parse(
	int64_t*     ts,
	wl_event*   e,
	const char* str,
	size_t      len
) {
	wl_error err;

	err = wl_parse_timestamp(ts, &str, &len);
	if (err != wl_ok)
		return err;

	return wl_ok;
}

#endif
