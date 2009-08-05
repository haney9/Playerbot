    /* 
    Name    : PlayerbotWarriorAI.cpp
    Complete: maybe around 37%
    Author  : Natsukawa
    Version : 0.39
    */
#include "PlayerbotWarriorAI.h"

class PlayerbotAI;
PlayerbotWarriorAI::PlayerbotWarriorAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
    HEROIC_STRIKE           = ai->getSpellId("heroic strike"); //ARMS
    REND                    = ai->getSpellId("rend"); //ARMS
    THUNDER_CLAP            = ai->getSpellId("thunder clap");  //ARMS
    HAMSTRING               = ai->getSpellId("hamstring");  //ARMS
    MOCKING_BLOW            = ai->getSpellId("mocking blow");  //ARMS
    RETALIATION             = ai->getSpellId("retaliation");  //ARMS
    SWEEPING_STRIKES        = ai->getSpellId("sweeping strikes"); //ARMS
    MORTAL_STRIKE           = ai->getSpellId("mortal strike");  //ARMS
    BLADESTORM              = ai->getSpellId("bladestorm");  //ARMS
    HEROIC_THROW            = ai->getSpellId("heroic throw");  //ARMS
    BLOODRAGE               = ai->getSpellId("bloodrage"); //PROTECTION
    DEFENSIVE_STANCE        = ai->getSpellId("defensive stance"); //PROTECTION
    DEVASTATE               = ai->getSpellId("devastate");
    SUNDER_ARMOR            = ai->getSpellId("sunder armor"); //PROTECTION  //7386;  
    TAUNT                   = ai->getSpellId("taunt"); //PROTECTION//355;
    SHIELD_BASH             = ai->getSpellId("shield bash"); //PROTECTION
    REVENGE                 = ai->getSpellId("revenge"); //PROTECTION
    SHIELD_BLOCK            = ai->getSpellId("shield block"); //PROTECTION
    DISARM                  = ai->getSpellId("disarm"); //PROTECTION
    SHIELD_WALL             = ai->getSpellId("shield wall"); //PROTECTION
    SHIELD_SLAM             = ai->getSpellId("shield slam"); //PROTECTION
    VIGILANCE               = ai->getSpellId("vigilance"); //PROTECTION
    DEVASTATE               = ai->getSpellId("devastate"); //PROTECTION
    SHOCKWAVE               = ai->getSpellId("shockwave"); //PROTECTION
    CONCUSSION_BLOW         = ai->getSpellId("concussion blow");
    SPELL_REFLECTION        = ai->getSpellId("spell reflection"); //PROTECTION
    BATTLE_SHOUT            = ai->getSpellId("battle shout"); //FURY
    DEMORALIZING_SHOUT      = ai->getSpellId("demoralizing shout"); //11556; //
    CLEAVE                  = ai->getSpellId("cleave"); //FURY
    INTIMIDATING_SHOUT      = ai->getSpellId("intimidating shout"); //FURY
    EXECUTE                 = ai->getSpellId("execute"); //FURY
    CHALLENGING_SHOUT       = ai->getSpellId("challenging shout"); //FURY
    SLAM                    = ai->getSpellId("slam"); //FURY
    BERSERKER_STANCE        = ai->getSpellId("berserker stance"); //FURY
    INTERCEPT               = ai->getSpellId("intercept"); //FURY
    DEATH_WISH              = ai->getSpellId("death wish");//FURY
    BERSERKER_RAGE          = ai->getSpellId("berserker rage");//FURY
    WHIRLWIND               = ai->getSpellId("whirlwind");//FURY
    PUMMEL                  = ai->getSpellId("pummel"); //FURY
    BLOODTHIRST             = ai->getSpellId("bloodthirst"); //FURY
    RECKLESSNESS            = ai->getSpellId("recklessness"); //FURY
    RAMPAGE                 = ai->getSpellId("rampage"); //FURY
    HEROIC_FURY             = ai->getSpellId("heroic fury"); //FURY
    COMMANDING_SHOUT        = ai->getSpellId("commanding shout"); //FURY
    ENRAGED_REGENERATION    = ai->getSpellId("enraged regeneration"); //FURY
}
PlayerbotWarriorAI::~PlayerbotWarriorAI() {}

void PlayerbotWarriorAI::DoNextCombatManeuver(Unit *pTarget)
{
    PlayerbotAI* ai = GetAI();
    if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            if (HEROIC_STRIKE > 0)
                ai->CastSpell(HEROIC_STRIKE);
            return;
    }
    // ------- Non Duel combat ----------

    

    // Damage Attacks

    Player *m_bot = GetPlayerBot();
    Unit* pVictim = pTarget->getVictim();

    if( !m_bot->HasInArc(M_PI, pTarget))
        m_bot->SetInFront(pTarget);

    if (SHIELD_WALL > 0 && ai->GetHealthPercent() < 20)
        ai->CastSpell (SHIELD_WALL, *m_bot);

    if (DEVASTATE > 0 && ai->GetRageAmount() >= 15)
        ai->CastSpell (DEVASTATE);
    else if (SUNDER_ARMOR > 0 && ai->GetRageAmount() >= 15)
        ai->CastSpell (SUNDER_ARMOR);

    if (pTarget->IsNonMeleeSpellCasted(true))
        SpellSequence = SpellPreventing;

    if (pTarget->GetHealth() < pTarget->GetMaxHealth()*0.2 && pTarget->getLevel() <= m_bot->getLevel() )
        SpellSequence = Berserker;
    else
        SpellSequence = Tanking;

    switch (SpellSequence)
    {
        case Tanking:
            ai->TellMaster("Tanking");
            if (DEFENSIVE_STANCE > 0 && !m_bot->GetAura(DEFENSIVE_STANCE, 0))//<------
                ai->CastSpell (DEFENSIVE_STANCE);

            if (DEMORALIZING_SHOUT > 0 && !pTarget->GetAura(DEMORALIZING_SHOUT, 0) && ai->GetRageAmount() >= 10)//<------
                ai->CastSpell (DEMORALIZING_SHOUT);
            else if (SHIELD_BLOCK > 0 && TankCounter < 1 && pVictim)
            {
                if (pVictim == m_bot)
                {
                    ai->CastSpell (SHIELD_BLOCK);
                    TankCounter++;
                    break;
                }
            }
            else if (SHIELD_SLAM > 0 && TankCounter < 2 && ai->GetRageAmount() >= 20)
            {
                ai->CastSpell (SHIELD_SLAM, *pTarget);
                TankCounter++;
                break;
            }
            else if (CONCUSSION_BLOW > 0 && TankCounter < 3 && ai->GetRageAmount() >= 20)
            {
                ai->CastSpell (CONCUSSION_BLOW, *pTarget);
                TankCounter++;
                break;
            }
            else if (SHOCKWAVE > 0 && TankCounter < 4 && ai->GetRageAmount() >= 15)
            {
                ai->CastSpell (SHOCKWAVE);
                TankCounter++;
                break;
            }
            else if (REVENGE > 0 && TankCounter < 5 && ai->GetRageAmount() >= 5)
            {
                ai->CastSpell (REVENGE, *pTarget);
                TankCounter++;
                break;
            }
            else if (THUNDER_CLAP > 0 && TankCounter < 6 && ai->GetRageAmount() >= 5)
            {
                ai->CastSpell (THUNDER_CLAP, *pTarget);
                TankCounter++;
                break;
            }
            else if (SHIELD_BLOCK > 0 && TankCounter < 7 && pVictim)
            {
                if (pVictim == m_bot)
                {
                    ai->CastSpell (SHIELD_BLOCK);
                    TankCounter++;
                    break;
                }
            }
            else if (HEROIC_STRIKE > 0 && TankCounter < 8 && ai->GetRageAmount() >= 15)
            {
                ai->CastSpell (HEROIC_STRIKE, *pTarget);
                TankCounter++;
                break;
            }
            else if (TAUNT > 0 && TankCounter < 9)
            {
                ai->CastSpell (TAUNT, *pTarget);
                TankCounter++;
                break;
            }
            else if (COMMANDING_SHOUT > 0 && TankCounter < 10 && !m_bot->GetAura(COMMANDING_SHOUT, 0) && ai->GetRageAmount() >= 10)//<------
            {
                ai->CastSpell (COMMANDING_SHOUT);
                TankCounter++;
                break;
            }
            else if (TankCounter < 11)
            {
                TankCounter = 0;
                ai->TellMaster("TankCounterReseter");
                break;
            }
            else
            {
                TankCounter = 0;
                ai->TellMaster("TankCounterReseter");
                break;
            }
            break;

        case Berserker:
            ai->TellMaster("Berserker");
            if (BERSERKER_STANCE > 0 && !m_bot->HasAura(BERSERKER_STANCE, 0))
                ai->CastSpell (BERSERKER_STANCE);
            else if (EXECUTE > 0 && BerserkerCounter < 1 && pTarget->GetHealth() < pTarget->GetMaxHealth()*0.2 && ai->GetRageAmount() >= 15)
            {
                ai->CastSpell (EXECUTE, *pTarget);
                BerserkerCounter++;
                break;
            }
            else if (WHIRLWIND > 0 && BerserkerCounter < 2 && ai->GetRageAmount() >= 15)
            {
                ai->CastSpell (WHIRLWIND, *pTarget);
                BerserkerCounter++;
                break;
            }
            else if (BerserkerCounter < 3)
            {
                BerserkerCounter = 0;
                ai->TellMaster("BerserkerCounterReseter");
                break;
            }
            else
            {
                BerserkerCounter = 0;
                ai->TellMaster("BerserkerCounterReseter");
                break;
            }
            break;

        case SpellPreventing:
            ai->TellMaster("Case SpellPreventing");
            if (SPELL_REFLECTION > 0 && m_bot->GetAura(DEFENSIVE_STANCE, 0) && pVictim && pTarget->IsNonMeleeSpellCasted(true) && ai->GetRageAmount() >= 15)//<------
            {
                if (pVictim == m_bot)
                {
                    ai->CastSpell (SPELL_REFLECTION, *m_bot);
                    ai->TellMaster("SpellRef");
                }
            }
            else if (PUMMEL > 0 && m_bot->GetAura(BERSERKER_STANCE, 0) && pTarget->IsNonMeleeSpellCasted(true) && ai->GetRageAmount() >= 10)//<------
            {
                ai->CastSpell (PUMMEL, *pTarget);
                ai->TellMaster("PUMMEL");
            }
            else
            {
                ai->CastSpell (SHIELD_BASH, *pTarget);
                ai->TellMaster("SHBash");
            }
            break;
    }
}

void PlayerbotWarriorAI::DoNonCombatActions()
{
    PlayerbotAI* ai = GetAI();
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

    // TODO (by Runsttren): check if shout aura bot has is casted by this bot, 
    // otherwise cast other useful shout
    // If the bot is protect talented, she/he needs stamina not attack power.
    // With stance change can the shout change to. 
    // Inserted line to battle shout m_bot->HasAura( COMMANDING_SHOUT, 0 )
    // Natsukawa
    if( ( (COMMANDING_SHOUT>0 && !m_bot->GetAura( COMMANDING_SHOUT, 0 )) ||     //<------
        (BATTLE_SHOUT>0 && !m_bot->GetAura( BATTLE_SHOUT, 0 )) ) &&             //<------
        ai->GetRageAmount()<10 && BLOODRAGE>0 && !m_bot->HasAura( BLOODRAGE, 0 ) )
    {
        // we do have a useful shout, no rage coming but can cast bloodrage... do it
        ai->CastSpell( BLOODRAGE, *m_bot );
    }
    else if( COMMANDING_SHOUT>0 && !m_bot->GetAura( COMMANDING_SHOUT, 0 ) )   //<------
    {
        // use commanding shout now
        ai->CastSpell( COMMANDING_SHOUT, *m_bot );
    }
    else if( BATTLE_SHOUT>0 && !m_bot->GetAura( BATTLE_SHOUT, 0 ) && !m_bot->GetAura( COMMANDING_SHOUT, 0 ) )//<------
    {
        // use battle shout
        ai->CastSpell( BATTLE_SHOUT, *m_bot );
    }

    // hp check
    if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);

    Item* pItem = ai->FindFood();

    if (pItem != NULL && ai->GetHealthPercent() < 30)
    {
        ai->TellMaster("I could use some food.");
        ai->UseItem(*pItem);
        ai->SetIgnoreUpdateTime(30);
        return;
    }
} // end DoNonCombatActions

void PlayerbotWarriorAI::BuffPlayer(Player* target) {
}
