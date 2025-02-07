#include "utest.h"
#include "../warlogs.h"

UTEST(match, undefined) {
  const char *str = "";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_undefined);
}

UTEST(match, version) {
  const char *str = "COMBAT_LOG_VERSION";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_version);
}

UTEST(match, zone_change) {
  const char *str = "ZONE_CHANGE";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_zone_change);
}

UTEST(match, map_change) {
  const char *str = "MAP_CHANGE";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_map_change);
}

UTEST(match, unit_died) {
  const char *str = "UNIT_DIED";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_unit_died);
}

UTEST(match, party_kill) {
  const char *str = "PARTY_KILL";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_party_kill);
}

UTEST(match, spell_cast_success) {
  const char *str = "SPELL_CAST_SUCCESS";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_cast_success);
}

UTEST(match, spell_summon) {
  const char *str = "SPELL_SUMMON";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_summon);
}

UTEST(match, spell_instakill) {
  const char *str = "SPELL_INSTAKILL";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_instakill);
}

UTEST(match, spell_energize) {
  const char *str = "SPELL_ENERGIZE";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_energize);
}

UTEST(match, spell_heal) {
  const char *str = "SPELL_HEAL";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_heal);
}

UTEST(match, spell_damage) {
  const char *str = "SPELL_DAMAGE";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_damage);
}

UTEST(match, spell_periodic_damage) {
  const char *str = "SPELL_PERIODIC_DAMAGE";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_periodic_damage);
}

UTEST(match, spell_periodic_heal) {
  const char *str = "SPELL_PERIODIC_HEAL";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_periodic_heal);
}

UTEST(match, spell_aura_applied) {
  const char *str = "SPELL_AURA_APPLIED";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_aura_applied);
}

UTEST(match, spell_aura_refresh) {
  const char *str = "SPELL_AURA_REFRESH";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_aura_refresh);
}

UTEST(match, spell_aura_removed) {
  const char *str = "SPELL_AURA_REMOVED";

  const wl_event_kind kind = wl_match(str);

  EXPECT_EQ(kind, wl_spell_aura_removed);
}
