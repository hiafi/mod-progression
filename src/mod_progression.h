#ifndef MOD_PROGRESSION_H
#define MOD_PROGRESSION_H

#include "ScriptMgr.h"

// The hook bodies live here, in plain (non-script) members defined across the mod_progression_*.cpp files.
class Progression
{
public:
    // AllBattlegroundScript
    void OnBattlegroundEndReward(Battleground* /*bg*/, Player* /*player*/, TeamId /*winnerTeamId*/);

    // DatabaseScript
    void OnAfterDatabasesLoaded(uint32 /*updateFlags*/);

    // MailScript
    void OnBeforeMailDraftSendMailTo(MailDraft* /*mailDraft*/, MailReceiver const& /*receiver*/, MailSender const& /*sender*/, MailCheckMask& /*checked*/, uint32& /*deliver_delay*/, uint32& /*custom_expiration*/, bool& /*deleteMailItemsFromDB*/, bool& /*sendMail*/);

    // PlayerScript
    void OnPlayerUpdateArea(Player* /*player*/, uint32 /*oldArea*/, uint32 /*newArea*/);
    bool OnPlayerShouldBeRewardedWithMoneyInsteadOfExp(Player* /*player*/);
    bool OnPlayerUpdateFishingSkill(Player* /*player*/, int32 /*skill*/, int32 /*zone_skill*/, int32 /*chance*/, int32 /*roll*/);
    bool OnPlayerReputationChange(Player* /*player*/, uint32 /*factionID*/, int32& /*standing*/, bool /*incremental*/);
    void OnPlayerQuestComputeXP(Player* player, Quest const* /*quest*/, uint32& /*xpValue*/);
    void OnPlayerGiveXP(Player* /*player*/, uint32& /*amount*/, Unit* /*victim*/, uint8 /*xpSource*/);

    // UnitScript
    void ModifyPeriodicDamageAurasTick(Unit* /*target*/, Unit* /*attacker*/, uint32& /*damage*/, SpellInfo const* /*spellInfo*/);
    void ModifyMeleeDamage(Unit* /*target*/, Unit* /*attacker*/, uint32& /*damage*/);
    void ModifySpellDamageTaken(Unit* /*target*/, Unit* /*attacker*/, int32& /*damage*/, SpellInfo const* /*spellInfo*/);
    void ModifyHealReceived(Unit* /*target*/, Unit* /*healer*/, uint32& /*heal*/, SpellInfo const* /*spellInfo*/);

    // WorldScript
    void OnAfterConfigLoad(bool /*reload*/);
    void OnStartup();

private:
    inline std::vector<std::string> GetActivePhases();
};

// ScriptMgr::Unload() deletes every registry entry of every script object, so one object registered in several
// script registries (as the old multi-base Progression was) is freed once per registry. Each script type therefore
// gets its own single-base class below, each created and registered exactly once, forwarding to the shared hooks.
class ProgressionAllBattlegroundScript : public AllBattlegroundScript
{
public:
    ProgressionAllBattlegroundScript() : AllBattlegroundScript("ProgressionAllBattlegroundScript") {}

    void OnBattlegroundEndReward(Battleground* bg, Player* player, TeamId winnerTeamId) override { _progression.OnBattlegroundEndReward(bg, player, winnerTeamId); }

private:
    Progression _progression;
};

class ProgressionDatabaseScript : public DatabaseScript
{
public:
    ProgressionDatabaseScript() : DatabaseScript("ProgressionDatabaseScript") {}

    void OnAfterDatabasesLoaded(uint32 updateFlags) override { _progression.OnAfterDatabasesLoaded(updateFlags); }

private:
    Progression _progression;
};

class ProgressionMailScript : public MailScript
{
public:
    ProgressionMailScript() : MailScript("ProgressionMailScript") {}

    void OnBeforeMailDraftSendMailTo(MailDraft* mailDraft, MailReceiver const& receiver, MailSender const& sender, MailCheckMask& checked, uint32& deliver_delay, uint32& custom_expiration, bool& deleteMailItemsFromDB, bool& sendMail) override { _progression.OnBeforeMailDraftSendMailTo(mailDraft, receiver, sender, checked, deliver_delay, custom_expiration, deleteMailItemsFromDB, sendMail); }

private:
    Progression _progression;
};

class ProgressionPlayerScript : public PlayerScript
{
public:
    ProgressionPlayerScript() : PlayerScript("ProgressionPlayerScript") {}

    void OnPlayerUpdateArea(Player* player, uint32 oldArea, uint32 newArea) override { _progression.OnPlayerUpdateArea(player, oldArea, newArea); }
    bool OnPlayerShouldBeRewardedWithMoneyInsteadOfExp(Player* player) override { return _progression.OnPlayerShouldBeRewardedWithMoneyInsteadOfExp(player); }
    bool OnPlayerUpdateFishingSkill(Player* player, int32 skill, int32 zone_skill, int32 chance, int32 roll) override { return _progression.OnPlayerUpdateFishingSkill(player, skill, zone_skill, chance, roll); }
    bool OnPlayerReputationChange(Player* player, uint32 factionID, int32& standing, bool incremental) override { return _progression.OnPlayerReputationChange(player, factionID, standing, incremental); }
    void OnPlayerQuestComputeXP(Player* player, Quest const* quest, uint32& xpValue) override { _progression.OnPlayerQuestComputeXP(player, quest, xpValue); }
    void OnPlayerGiveXP(Player* player, uint32& amount, Unit* victim, uint8 xpSource) override { _progression.OnPlayerGiveXP(player, amount, victim, xpSource); }

private:
    Progression _progression;
};

class ProgressionUnitScript : public UnitScript
{
public:
    ProgressionUnitScript() : UnitScript("ProgressionUnitScript") {}

    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage, SpellInfo const* spellInfo) override { _progression.ModifyPeriodicDamageAurasTick(target, attacker, damage, spellInfo); }
    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override { _progression.ModifyMeleeDamage(target, attacker, damage); }
    void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* spellInfo) override { _progression.ModifySpellDamageTaken(target, attacker, damage, spellInfo); }
    void ModifyHealReceived(Unit* target, Unit* healer, uint32& heal, SpellInfo const* spellInfo) override { _progression.ModifyHealReceived(target, healer, heal, spellInfo); }

private:
    Progression _progression;
};

class ProgressionWorldScript : public WorldScript
{
public:
    ProgressionWorldScript() : WorldScript("ProgressionWorldScript") {}

    void OnAfterConfigLoad(bool reload) override { _progression.OnAfterConfigLoad(reload); }
    void OnStartup() override { _progression.OnStartup(); }

private:
    Progression _progression;
};

class ProgressionMgr
{
public:
    static ProgressionMgr* instance();

    void SetPhaseId(uint8 id) { phaseId = id; }
    uint8 GetPhaseId() { return phaseId; }
    void SetAuraId(uint8 id) { auraId = id; }
    uint8 GetAuraId() { return auraId; }
    float GetDamageMultiplier() { return damageMultiplier; }
    void SetDamageMultiplier(float value) { damageMultiplier = value; }
    float GetHealingMultiplier() { return healingMultiplier; }
    void SetHealingMultiplier(float value) { healingMultiplier = value; }
    void SetResetDatabase(bool value) { resetDatabase = value; }
    bool GetResetDatabase() { return resetDatabase; }

private:
    uint8 phaseId;
    uint8 auraId;
    float damageMultiplier;
    float healingMultiplier;
    bool resetDatabase;
};

#define sProgressionMgr ProgressionMgr::instance()

#endif
