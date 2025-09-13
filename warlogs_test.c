#include <criterion/assert.h>
#include <criterion/parameterized.h>
#include <criterion/internal/assert.h>

#include "warlogs.h"

Test(warlogs, match) {
	const char* strs[] = {
		"",
		"COMBAT_LOG_VERSION",
		"ZONE_CHANGE",
		"MAP_CHANGE",
		"UNIT_DIED",
		"PARTY_KILL",
		"SPELL_CAST_SUCCESS",
		"SPELL_SUMMON",
		"SPELL_INSTAKILL",
		"SPELL_ENERGIZE",
		"SPELL_HEAL",
		"SPELL_DAMAGE",
		"SPELL_PERIODIC_DAMAGE",
		"SPELL_PERIODIC_HEAL",
	};
	const wl_event_kind events[] = {
		wl_undefined,
		wl_version,
		wl_zone_change,
		wl_map_change,
		wl_unit_died,
		wl_party_kill,
		wl_spell_cast_success,
		wl_spell_summon,
		wl_spell_instakill,
		wl_spell_energize,
		wl_spell_heal,
		wl_spell_damage,
		wl_spell_periodic_damage,
		wl_spell_periodic_heal
	};
	for (int i = 0; i < 14; i++) {
		const wl_event_kind expected = events[i];
		const wl_event_kind actual   = wl_match(strs[i]);
		
		cr_assert_eq(
			expected,
			actual,
			"%s\n"
			"expected: %s | actual: %s",
			strs[i],
			WL_EVENT_NAMES[expected],
			WL_EVENT_NAMES[actual]
		);
	}
}

Test(warlogs, timestamp) {
	const char* strs[] = {
		"01/01/1970 00:00:00.0000",
		"01/01/1971 00:00:00.0000",
		"02/01/1970 00:00:00.0000",
		"01/02/1970 00:00:00.0000",
		"01/01/1970 01:00:00.0000",
		"01/01/1970 00:01:00.0000",
	};
	const time_t ts[] = {
		0,
		31622400,
		2678400,
		86400,
		3600,
		60,
	};
	for (int i = 0; i < 6; i++) {
		const time_t expected = ts[i];

		time_t actual;
		const wl_return_code rc = wl_parse_timestamp(&actual, strs[i]);
		
		cr_assert_eq(wl_ok, rc);
		cr_assert_eq(
			expected,
			actual,
			"%s\n"
			"expected: %s | actual: %s",
			strs[i],
			expected,
			actual
		);
	}
}
