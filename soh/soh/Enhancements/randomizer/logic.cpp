#include "logic.h"
#include "../debugger/performanceTimer.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

#include "soh/OTRGlobals.h"
#include "dungeon.h"
#include "context.h"
#include "macros.h"
#include "variables.h"
#include <spdlog/spdlog.h>

namespace Rando {
    bool Logic::IsMagicItem(RandomizerGet item) {
        return item == RG_DINS_FIRE    ||
                item == RG_FARORES_WIND ||
                item == RG_NAYRUS_LOVE  ||
                item == RG_LENS_OF_TRUTH;
    }

    bool Logic::IsMagicArrow(RandomizerGet item) {
        return item == RG_FIRE_ARROWS ||
                item == RG_ICE_ARROWS  ||
                item == RG_LIGHT_ARROWS;
    }

    bool Logic::HasItem(RandomizerGet itemName) {
        switch (itemName) {
            case RG_FAIRY_OCARINA:
                return CheckInventory(ITEM_OCARINA_FAIRY, false);
            case RG_OCARINA_OF_TIME:
                return CheckInventory(ITEM_OCARINA_TIME, true);
            case RG_DINS_FIRE:
                return CheckInventory(ITEM_DINS_FIRE, true);
            case RG_FARORES_WIND:
                return CheckInventory(ITEM_FARORES_WIND, true);
            case RG_NAYRUS_LOVE:
                return CheckInventory(ITEM_NAYRUS_LOVE, true);
            case RG_LENS_OF_TRUTH:
                return CheckInventory(ITEM_LENS, true);
            case RG_FAIRY_BOW:
                return CheckInventory(ITEM_BOW, true);
            case RG_MEGATON_HAMMER:
                return CheckInventory(ITEM_HAMMER, true);
            case RG_HOOKSHOT:
                return CheckInventory(ITEM_HOOKSHOT, false);
            case RG_LONGSHOT:
                return CheckInventory(ITEM_LONGSHOT, true);
            case RG_PROGRESSIVE_STICK_UPGRADE:
            case RG_STICKS:
                return CurrentUpgrade(UPG_STICKS);
            case RG_FIRE_ARROWS:
                return CheckInventory(ITEM_ARROW_FIRE, true);
            case RG_ICE_ARROWS:
                return CheckInventory(ITEM_ARROW_ICE, true);
            case RG_LIGHT_ARROWS:
                return CheckInventory(ITEM_ARROW_LIGHT, true);
            case RG_PROGRESSIVE_BOMBCHUS:
            case RG_BOMBCHU_5:
            case RG_BOMBCHU_10:
            case RG_BOMBCHU_20:
                return (BombchusEnabled && BuyBombchus) || CheckInventory(ITEM_BOMBCHU, true);
            case RG_FAIRY_SLINGSHOT:
                return CheckInventory(ITEM_SLINGSHOT, true);
            case RG_BOOMERANG:
                return CheckInventory(ITEM_BOOMERANG, true);
            case RG_PROGRESSIVE_NUT_UPGRADE:
            case RG_NUTS:
                return CurrentUpgrade(UPG_NUTS);
            case RG_SCARECROW:
                return Scarecrow;
            case RG_DISTANT_SCARECROW:
                return DistantScarecrow;
            case RG_KOKIRI_SWORD:
            case RG_DEKU_SHIELD:
            case RG_GORON_TUNIC:
            case RG_ZORA_TUNIC:
            case RG_HYLIAN_SHIELD:
            case RG_MIRROR_SHIELD:
            case RG_MASTER_SWORD:
            case RG_BIGGORON_SWORD:
            case RG_IRON_BOOTS:
            case RG_HOVER_BOOTS:
                return CheckEquipment(RandoGetToEquipFlag.at(itemName));
            case RG_GORONS_BRACELET:
                return CurrentUpgrade(UPG_STRENGTH);
            case RG_SILVER_GAUNTLETS:
                return CurrentUpgrade(UPG_STRENGTH) >= 2;
            case RG_GOLDEN_GAUNTLETS:
                return CurrentUpgrade(UPG_STRENGTH) >= 3;
            case RG_PROGRESSIVE_BOMB_BAG:
            case RG_BOMB_BAG:
                return CurrentUpgrade(UPG_BOMB_BAG);
            case RG_MAGIC_SINGLE:
                return GetSaveContext()->magicLevel >= 1;
                // Songs
            case RG_ZELDAS_LULLABY:
            case RG_EPONAS_SONG:
            case RG_SARIAS_SONG:
            case RG_SUNS_SONG:
            case RG_SONG_OF_TIME:
            case RG_SONG_OF_STORMS:
            case RG_MINUET_OF_FOREST:
            case RG_BOLERO_OF_FIRE:
            case RG_SERENADE_OF_WATER:
            case RG_REQUIEM_OF_SPIRIT:
            case RG_NOCTURNE_OF_SHADOW:
            case RG_PRELUDE_OF_LIGHT:
                // Dungeon Rewards
            case RG_KOKIRI_EMERALD:
            case RG_GORON_RUBY:
            case RG_ZORA_SAPPHIRE:
            case RG_FOREST_MEDALLION:
            case RG_FIRE_MEDALLION:
            case RG_WATER_MEDALLION:
            case RG_SPIRIT_MEDALLION:
            case RG_SHADOW_MEDALLION:
            case RG_LIGHT_MEDALLION:
                // Misc Quest Items
            case RG_STONE_OF_AGONY:
            case RG_GERUDO_MEMBERSHIP_CARD:
                return CheckQuestItem(RandoGetToQuestItem.at(itemName));
            case RG_RUTOS_LETTER:
                return CheckEventChkInf(EVENTCHKINF_OBTAINED_RUTOS_LETTER);
            case RG_DOUBLE_DEFENSE:
                return GetSaveContext()->isDoubleDefenseAcquired;
            case RG_FISHING_POLE:
            case RG_ZELDAS_LETTER:
            case RG_WEIRD_EGG:
            case RG_GREG_RUPEE:
                // Ocarina Buttons
            case RG_OCARINA_A_BUTTON:
            case RG_OCARINA_C_LEFT_BUTTON:
            case RG_OCARINA_C_RIGHT_BUTTON:
            case RG_OCARINA_C_DOWN_BUTTON:
            case RG_OCARINA_C_UP_BUTTON:
                // Boss Souls
            case RG_GOHMA_SOUL:
            case RG_KING_DODONGO_SOUL:
            case RG_BARINADE_SOUL:
            case RG_PHANTOM_GANON_SOUL:
            case RG_VOLVAGIA_SOUL:
            case RG_MORPHA_SOUL:
            case RG_BONGO_BONGO_SOUL:
            case RG_TWINROVA_SOUL:
            case RG_GANON_SOUL:
            case RG_SKELETON_KEY:
                return CheckRandoInf(RandoGetToRandInf.at(itemName));
                // Boss Keys
            case RG_FOREST_TEMPLE_BOSS_KEY:
            case RG_FIRE_TEMPLE_BOSS_KEY:
            case RG_WATER_TEMPLE_BOSS_KEY:
            case RG_SPIRIT_TEMPLE_BOSS_KEY:
            case RG_SHADOW_TEMPLE_BOSS_KEY:
            case RG_GANONS_CASTLE_BOSS_KEY:
                return CheckDungeonItem(DUNGEON_KEY_BOSS, RandoGetToDungeonScene.at(itemName));
                // Maps
            case RG_DEKU_TREE_MAP:
            case RG_DODONGOS_CAVERN_MAP:
            case RG_JABU_JABUS_BELLY_MAP:
            case RG_FOREST_TEMPLE_MAP:
            case RG_FIRE_TEMPLE_MAP:
            case RG_WATER_TEMPLE_MAP:
            case RG_SPIRIT_TEMPLE_MAP:
            case RG_SHADOW_TEMPLE_MAP:
            case RG_BOTTOM_OF_THE_WELL_MAP:
            case RG_ICE_CAVERN_MAP:
                return CheckDungeonItem(DUNGEON_MAP, RandoGetToDungeonScene.at(itemName));
                // Compasses
            case RG_DEKU_TREE_COMPASS:
            case RG_DODONGOS_CAVERN_COMPASS:
            case RG_JABU_JABUS_BELLY_COMPASS:
            case RG_FOREST_TEMPLE_COMPASS:
            case RG_FIRE_TEMPLE_COMPASS:
            case RG_WATER_TEMPLE_COMPASS:
            case RG_SPIRIT_TEMPLE_COMPASS:
            case RG_SHADOW_TEMPLE_COMPASS:
            case RG_BOTTOM_OF_THE_WELL_COMPASS:
            case RG_ICE_CAVERN_COMPASS:
                return CheckDungeonItem(DUNGEON_COMPASS, RandoGetToDungeonScene.at(itemName));
                // Wallets
            case RG_CHILD_WALLET:
                return CheckRandoInf(RAND_INF_HAS_WALLET);
            case RG_ADULT_WALLET:
                return CurrentUpgrade(UPG_WALLET) >= 1;
            case RG_GIANT_WALLET:
                return CurrentUpgrade(UPG_WALLET) >= 2;
            case RG_TYCOON_WALLET:
                return CurrentUpgrade(UPG_WALLET) >= 3;
                // Scales
            case RG_BRONZE_SCALE:
                return CheckRandoInf(RAND_INF_CAN_SWIM);
            case RG_SILVER_SCALE:
                return CurrentUpgrade(UPG_SCALE) >= 1;
            case RG_GOLDEN_SCALE:
                return CurrentUpgrade(UPG_SCALE) >= 2;
            case RG_POCKET_EGG:
            case RG_COJIRO:
            case RG_ODD_MUSHROOM:
            case RG_ODD_POTION:
            case RG_POACHERS_SAW:
            case RG_BROKEN_SWORD:
            case RG_PRESCRIPTION:
            case RG_EYEBALL_FROG:
            case RG_EYEDROPS:
            case RG_CLAIM_CHECK:
                return HasAdultTrade(StaticData::RetrieveItem(itemName).GetGIEntry()->itemId);
            case RG_BOTTLE_WITH_BIG_POE:
            case RG_BOTTLE_WITH_BLUE_FIRE:
            case RG_BOTTLE_WITH_BLUE_POTION:
            case RG_BOTTLE_WITH_BUGS:
            case RG_BOTTLE_WITH_FAIRY:
            case RG_BOTTLE_WITH_FISH:
            case RG_BOTTLE_WITH_GREEN_POTION:
            case RG_BOTTLE_WITH_MILK:
            case RG_BOTTLE_WITH_POE:
            case RG_BOTTLE_WITH_RED_POTION:
            case RG_EMPTY_BOTTLE:
                return HasBottle;
        }
        SPDLOG_ERROR("HasItem reached `return false;`. Missing case for RandomizerGet of {}", static_cast<uint32_t>(itemName));
        assert(false);
        return false;
    }

    //Can the passed in item be used?
    bool Logic::CanUse(RandomizerGet itemName) {
        if (!HasItem(itemName))
            return false;

        switch (itemName) {
            // Adult items
            // TODO: Uncomment those if we ever implement more item usability settings
            case RG_FAIRY_BOW:
                return IsAdult;// || BowAsChild;
            case RG_MEGATON_HAMMER:
                return IsAdult;// || HammerAsChild;
            case RG_IRON_BOOTS:
                return IsAdult;// || IronBootsAsChild;
            case RG_HOVER_BOOTS:
                return IsAdult;// || HoverBootsAsChild;
            case RG_HOOKSHOT:
                return IsAdult;// || HookshotAsChild;
            case RG_LONGSHOT:
                return IsAdult;// || HookshotAsChild;
            case RG_SILVER_GAUNTLETS:
                return IsAdult;
            case RG_GOLDEN_GAUNTLETS:
                return IsAdult;
            case RG_GORON_TUNIC:
                return IsAdult;// || GoronTunicAsChild;
            case RG_ZORA_TUNIC:
                return IsAdult;// || ZoraTunicAsChild;
            case RG_SCARECROW:
                return IsAdult;// || HookshotAsChild;
            case RG_DISTANT_SCARECROW:
                return IsAdult;// || HookshotAsChild;
            case RG_HYLIAN_SHIELD:
                return IsAdult;
            case RG_MIRROR_SHIELD:
                return IsAdult;// || MirrorShieldAsChild;
            case RG_MASTER_SWORD:
                return IsAdult;// || MasterSwordAsChild;
            case RG_BIGGORON_SWORD:
                return IsAdult;// || BiggoronSwordAsChild;

            // Child items
            case RG_FAIRY_SLINGSHOT:
                return IsChild;// || SlingshotAsAdult;
            case RG_BOOMERANG:
                return IsChild;// || BoomerangAsAdult;
            case RG_KOKIRI_SWORD:
                return IsChild;// || KokiriSwordAsAdult;
            case RG_NUTS:
                return (NutPot  || NutCrate || DekuBabaNuts) && AmmoCanDrop; //RANDOTODO BuyNuts currently mixed in with Nuts, should be seperate as BuyNuts are also a Nuts source
            case RG_STICKS:
                return IsChild /* || StickAsAdult;*/&& (StickPot || DekuBabaSticks);
            case RG_DEKU_SHIELD:
                return IsChild;// || DekuShieldAsAdult;
            case RG_WEIRD_EGG:
                return IsChild;
            case RG_PROGRESSIVE_BOMBCHUS:
            case RG_BOMBCHU_5:
            case RG_BOMBCHU_10:
            case RG_BOMBCHU_20:
                return BombchuRefill && BombchusEnabled;
            case RG_RUTOS_LETTER:
                return IsChild;

                // Adult Trade
            case RG_POCKET_EGG:
                return IsAdult;
            case RG_COJIRO:
                return IsAdult;
            case RG_ODD_MUSHROOM:
                return IsAdult;
            case RG_ODD_POTION:
                return IsAdult;
            case RG_POACHERS_SAW:
                return IsAdult;
            case RG_BROKEN_SWORD:
                return IsAdult;
            case RG_PRESCRIPTION:
                return IsAdult;
            case RG_EYEBALL_FROG:
                return IsAdult;
            case RG_EYEDROPS:
                return IsAdult;
            case RG_CLAIM_CHECK:
                return IsAdult;

            // Songs
            case RG_ZELDAS_LULLABY:
                return Ocarina && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_UP_BUTTON);
            case RG_EPONAS_SONG:
                return Ocarina && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_UP_BUTTON);
            case RG_SARIAS_SONG:
                return Ocarina && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_SUNS_SONG:
                return Ocarina && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_UP_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_SONG_OF_TIME:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_SONG_OF_STORMS:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_UP_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_MINUET_OF_FOREST:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_UP_BUTTON);
            case RG_BOLERO_OF_FIRE:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_SERENADE_OF_WATER:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_REQUIEM_OF_SPIRIT:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_NOCTURNE_OF_SHADOW:
                return Ocarina && HasItem(RG_OCARINA_A_BUTTON) && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_DOWN_BUTTON);
            case RG_PRELUDE_OF_LIGHT:
                return Ocarina && HasItem(RG_OCARINA_C_LEFT_BUTTON) && HasItem(RG_OCARINA_C_RIGHT_BUTTON) && HasItem(RG_OCARINA_C_UP_BUTTON);

            // Misc. Items
            case RG_FISHING_POLE:
                return HasItem(RG_CHILD_WALLET); // as long as you have enough rubies

            // Bottle Items
            case RG_BOTTLE_WITH_BUGS:
                return BugShrub || WanderingBugs || BugRock || GetInLogic(LOGIC_BUGS_ACCESS);
            case RG_BOTTLE_WITH_FISH:
                return LoneFish || FishGroup || GetInLogic(LOGIC_FISH_ACCESS); //is there any need to care about lone vs group?
            case RG_BOTTLE_WITH_BLUE_FIRE: //RANDOTODO should probably be better named to 
                return BlueFireAccess || GetInLogic(LOGIC_BLUE_FIRE_ACCESS);
            case RG_BOTTLE_WITH_FAIRY:
                return FairyPot || GossipStoneFairy || BeanPlantFairy || ButterflyFairy || FreeFairies || FairyPond || GetInLogic(LOGIC_FAIRY_ACCESS);

            // Magic items
            default:
                return MagicMeter && (IsMagicItem(itemName) || (IsMagicArrow(itemName) && CanUse(RG_FAIRY_BOW)));
        }
    }

    bool Logic::HasProjectile(HasProjectileAge age) {
        return HasExplosives ||
               (age == HasProjectileAge::Child  && (Slingshot || Boomerang)) ||
               (age == HasProjectileAge::Adult  && (Hookshot  || Bow      )) ||
               (age == HasProjectileAge::Both   && (Slingshot || Boomerang)  && (Hookshot || Bow)) ||
               (age == HasProjectileAge::Either && (Slingshot || Boomerang   ||  Hookshot || Bow));
    }

    bool Logic::HasBossSoul(RandomizerGet itemName) {
        if (!ctx->GetOption(RSK_SHUFFLE_BOSS_SOULS)) {
            return true;
        }
        switch(itemName) {
            case RG_GOHMA_SOUL:
            case RG_KING_DODONGO_SOUL:
            case RG_BARINADE_SOUL:
            case RG_PHANTOM_GANON_SOUL:
            case RG_VOLVAGIA_SOUL:
            case RG_MORPHA_SOUL:
            case RG_BONGO_BONGO_SOUL:
            case RG_TWINROVA_SOUL:
                return HasItem(itemName);
            case RG_GANON_SOUL:
                return ctx->GetOption(RSK_SHUFFLE_BOSS_SOULS).Is(RO_BOSS_SOULS_ON_PLUS_GANON) ? HasItem(RG_GANON_SOUL) : true;
            default:
                return false;
        }
    }

    uint8_t GetDifficultyValueFromString(Rando::Option& glitchOption) {
        return 0;
    }

    //todo rewrite glitch section

    bool Logic::CanEquipSwap(RandomizerGet itemName) {
        if (!HasItem(itemName))
            return false;
    
        if (CanDoGlitch(GlitchType::EquipSwapDins) || CanDoGlitch(GlitchType::EquipSwap))
            return true;

        return false;
    }

    bool Logic::CanDoGlitch(GlitchType glitch) {
        // TODO: Uncomment when glitches are implemented
        switch(glitch) {
            case GlitchType::EquipSwapDins:
                return ((IsAdult && HasItem(RG_DINS_FIRE)) || (IsChild && (HasItem(RG_STICKS) || HasItem(RG_DINS_FIRE)))) && false; //GlitchEquipSwapDins;
            case GlitchType::EquipSwap: // todo: add bunny hood to adult item equippable list and child trade item to child item equippable list
                return ((IsAdult && (HasItem(RG_DINS_FIRE) || HasItem(RG_FARORES_WIND) || HasItem(RG_NAYRUS_LOVE))) || (IsChild && (HasItem(RG_STICKS) || 
                    HasItem(RG_FAIRY_SLINGSHOT) || HasItem(RG_BOOMERANG) || HasBottle || CanUse(RG_NUTS) || Ocarina || HasItem(RG_LENS_OF_TRUTH) || HasExplosives ||
                    GetAmmo(ITEM_BEAN) > 0 || HasItem(RG_DINS_FIRE) || HasItem(RG_FARORES_WIND) || HasItem(RG_NAYRUS_LOVE)))) && false; //GlitchEquipSwap;
        }

        //Shouldn't be reached
        return false;
    }

    bool Logic::CanKillEnemy(std::string enemy) {
        //switch(enemy) {} RANDOTODO implement enemies enum
        if (enemy == "Big Skulltula"){
            return CanUse(RG_FAIRY_BOW) || CanUse(RG_FAIRY_SLINGSHOT) || CanJumpslash || CanUse(RG_MEGATON_HAMMER) || CanUse(RG_HOOKSHOT) || CanUse(RG_DINS_FIRE) || HasExplosives;
        }
        //Shouldn't be reached
        return false;
    }

    bool Logic::CanPassEnemy(std::string enemy) {
        //switch(enemy) {} RANDOTODO implement enemies enum
        if (CanKillEnemy(enemy)){
            return true;
        }
        if (enemy == "Big Skulltula"){
            return CanUse(RG_NUTS) || CanUse(RG_BOOMERANG);
        }
        return false;
    }

    Logic::Logic() {
        
    }

    uint8_t Logic::BottleCount() {
        uint8_t count = 0;
        if (!CanEmptyBigPoes){
            return 0;
        }
        for (int i = SLOT_BOTTLE_1; i <= SLOT_BOTTLE_4; i++) {
            uint8_t item = GetSaveContext()->inventory.items[i];
            if (item != ITEM_NONE && (item != ITEM_LETTER_RUTO || (item == ITEM_LETTER_RUTO && DeliverLetter))) {
                count++;
            }
        }
        return count;
    }

    // Updates all logic helpers. Should be called whenever a non-helper is changed
    void Logic::UpdateHelpers() {
        StartPerformanceTimer(PT_UPDATE_HELPERS);
        OcarinaButtons  =  (HasItem(RG_OCARINA_A_BUTTON) ? 1 : 0) +
                           (HasItem(RG_OCARINA_C_LEFT_BUTTON) ? 1 : 0) +
                           (HasItem(RG_OCARINA_C_RIGHT_BUTTON) ? 1 : 0) +
                           (HasItem(RG_OCARINA_C_UP_BUTTON) ? 1 : 0) +
                           (HasItem(RG_OCARINA_C_DOWN_BUTTON) ? 1 : 0);
        ZeldasLetter   = HasItem(RG_ZELDAS_LETTER);
        WeirdEgg       = CanUse(RG_WEIRD_EGG);
        BuySeed        = GetInLogic(LOGIC_BUY_SEED);
        BuyArrow       = GetInLogic(LOGIC_BUY_ARROW);
        BuyBomb        = GetInLogic(LOGIC_BUY_BOMB);
        BuyMagicPotion = GetInLogic(LOGIC_BUY_MAGIC_POTION);
        MagicBean      = GetAmmo(ITEM_BEAN) > 0;
        RutosLetter    = CanUse(RG_RUTOS_LETTER);
        Boomerang      = CanUse(RG_BOOMERANG);
        DinsFire       = CanUse(RG_DINS_FIRE);
        FaroresWind    = CanUse(RG_FARORES_WIND);
        NayrusLove     = CanUse(RG_NAYRUS_LOVE);
        LensOfTruth    = CanUse(RG_LENS_OF_TRUTH);
        ShardOfAgony   = HasItem(RG_STONE_OF_AGONY);
        //SkullMask      = false;
        //MaskOfTruth    = false;

        //Adult logic
        Hammer          = CanUse(RG_MEGATON_HAMMER);
        IronBoots       = CanUse(RG_IRON_BOOTS);
        HoverBoots      = CanUse(RG_HOVER_BOOTS);
        DekuShield      = CanUse(RG_DEKU_SHIELD);
        HylianShield    = CanUse(RG_HYLIAN_SHIELD);
        MirrorShield    = CanUse(RG_MIRROR_SHIELD);
        GoronTunic      = CanUse(RG_GORON_TUNIC);
        ZoraTunic       = CanUse(RG_ZORA_TUNIC);
        //Epona         = false;
        //BigPoe        = false;
        GerudoToken     = HasItem(RG_GERUDO_MEMBERSHIP_CARD);
        FireArrows      = CanUse(RG_FIRE_ARROWS);
        IceArrows       = CanUse(RG_ICE_ARROWS);
        LightArrows     = CanUse(RG_LIGHT_ARROWS);
        KokiriSword     = CanUse(RG_KOKIRI_SWORD);
        MasterSword     = CanUse(RG_MASTER_SWORD);
        BiggoronSword   = CanUse(RG_BIGGORON_SWORD);
        NumBottles      = BottleCount();
        HasBottle       = NumBottles >= 1;
        Slingshot       = CanUse(RG_FAIRY_SLINGSHOT) && (BuySeed || AmmoCanDrop);
        Ocarina         = HasItem(RG_FAIRY_OCARINA);
        OcarinaOfTime   = HasItem(RG_OCARINA_OF_TIME);
        MagicMeter      = HasItem(RG_MAGIC_SINGLE) && (AmmoCanDrop || (HasBottle && BuyMagicPotion));
        BombBag         = HasItem(RG_BOMB_BAG) && (BuyBomb || AmmoCanDrop);
        BombchusEnabled = ctx->GetOption(RSK_BOMBCHUS_IN_LOGIC) ? CheckInventory(ITEM_BOMBCHU, true) : BombBag;
        BuyBombchus     = (GetInLogic(LOGIC_BUY_BOMBCHUS) || CouldPlayBowling || CarpetMerchant);
        Hookshot        = CanUse(RG_HOOKSHOT);
        Longshot        = CanUse(RG_LONGSHOT);
        Bow             = CanUse(RG_FAIRY_BOW) && (BuyArrow || AmmoCanDrop);
        GoronBracelet   = HasItem(RG_GORONS_BRACELET);
        SilverGauntlets = HasItem(RG_SILVER_GAUNTLETS);
        GoldenGauntlets = HasItem(RG_GOLDEN_GAUNTLETS);
        Swim            = HasItem(RG_BRONZE_SCALE);
        SilverScale     = HasItem(RG_SILVER_SCALE);
        GoldScale       = HasItem(RG_GOLDEN_SCALE);
        ChildsWallet    = HasItem(RG_CHILD_WALLET);
        AdultsWallet    = HasItem(RG_ADULT_WALLET);
        BiggoronSword   = HasItem(RG_BIGGORON_SWORD);

        ProgressiveScale  = CurrentUpgrade(UPG_SCALE) + HasItem(RG_BRONZE_SCALE);
        ProgressiveWallet = CurrentUpgrade(UPG_WALLET) + HasItem(RG_CHILD_WALLET);

        CanSummonGohma        = HasBossSoul(RG_GOHMA_SOUL);
        CanSummonKingDodongo  = HasBossSoul(RG_KING_DODONGO_SOUL);
        CanSummonBarinade     = HasBossSoul(RG_BARINADE_SOUL);
        CanSummonPhantomGanon = HasBossSoul(RG_PHANTOM_GANON_SOUL);
        CanSummonVolvagia     = HasBossSoul(RG_VOLVAGIA_SOUL);
        CanSummonMorpha       = HasBossSoul(RG_MORPHA_SOUL);
        CanSummonBongoBongo   = HasBossSoul(RG_BONGO_BONGO_SOUL);
        CanSummonTwinrova     = HasBossSoul(RG_TWINROVA_SOUL);
        CanSummonGanon        = HasBossSoul(RG_GANON_SOUL);

        // Boss Keys
        BossKeyForestTemple = HasItem(RG_FOREST_TEMPLE_BOSS_KEY);
        BossKeyFireTemple   = HasItem(RG_FIRE_TEMPLE_BOSS_KEY);
        BossKeyWaterTemple  = HasItem(RG_WATER_TEMPLE_BOSS_KEY);
        BossKeySpiritTemple = HasItem(RG_SPIRIT_TEMPLE_BOSS_KEY);
        BossKeyShadowTemple = HasItem(RG_SHADOW_TEMPLE_BOSS_KEY);
        BossKeyGanonsCastle = HasItem(RG_GANONS_CASTLE_BOSS_KEY);

        KokiriEmerald   = HasItem(RG_KOKIRI_EMERALD);
        GoronRuby       = HasItem(RG_GORON_RUBY);
        ZoraSapphire    = HasItem(RG_ZORA_SAPPHIRE);
        ForestMedallion = HasItem(RG_FOREST_MEDALLION);
        FireMedallion   = HasItem(RG_FIRE_MEDALLION);
        WaterMedallion  = HasItem(RG_WATER_MEDALLION);
        SpiritMedallion = HasItem(RG_SPIRIT_MEDALLION);
        ShadowMedallion = HasItem(RG_SHADOW_MEDALLION);
        LightMedallion  = HasItem(RG_LIGHT_MEDALLION);

        DoubleDefense = HasItem(RG_DOUBLE_DEFENSE);
        TriforcePieces = GetSaveContext()->triforcePiecesCollected;
        GKShards = GetSaveContext()->GKShardsCollected;
        Greg = HasItem(RG_GREG_RUPEE);
        GoldSkulltulaTokens = GetGSCount();

        //you need at least 2 buttons for scarecrow song
        ScarecrowSong    = ScarecrowSong || (ctx->GetOption(RSK_SKIP_SCARECROWS_SONG) && Ocarina && OcarinaButtons >= 2) || (ChildScarecrow && AdultScarecrow);
        Scarecrow        = Hookshot && ScarecrowSong;
        DistantScarecrow = Longshot && ScarecrowSong;

        // TODO: Implement Ammo Drop Setting in place of bombchu drops
        BombchuRefill = BuyBombchus || (ctx->GetOption(RSK_ENABLE_BOMBCHU_DROPS).Is(RO_AMMO_DROPS_ON/*_PLUS_BOMBCHU*/));

        //Usage
        Bombs     = HasItem(RG_PROGRESSIVE_BOMB_BAG);
        BlueFire  = CanUse(RG_BOTTLE_WITH_BLUE_FIRE) || (ctx->GetOption(RSK_BLUE_FIRE_ARROWS) && CanUse(RG_ICE_ARROWS));

        // TODO: Implement Ammo Drop Setting in place of bombchu drops
        HasExplosives = Bombs || CanUse(RG_BOMBCHU_5);
        HasBoots = IronBoots || HoverBoots;
        //Unshuffled adult trade quest
        ClaimCheck = CanUse(RG_CLAIM_CHECK);
        Eyedrops = CanUse(RG_EYEDROPS) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && ClaimCheck);
        EyeballFrog = CanUse(RG_EYEBALL_FROG) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && Eyedrops);
        Prescription = CanUse(RG_PRESCRIPTION) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && EyeballFrog);
        BrokenSword = CanUse(RG_BROKEN_SWORD) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && Prescription);
        PoachersSaw = CanUse(RG_POACHERS_SAW) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && BrokenSword);
        OddPoultice = CanUse(RG_ODD_POTION) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && PoachersSaw);
        OddMushroom = CanUse(RG_ODD_MUSHROOM) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && OddPoultice);
        Cojiro = CanUse(RG_COJIRO) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && OddMushroom);
        PocketEgg = CanUse(RG_POCKET_EGG) || (!ctx->GetOption(RSK_SHUFFLE_ADULT_TRADE) && Cojiro);

        // IsChild = Age == AGE_CHILD;
        // IsAdult = Age == AGE_ADULT;

        CanBlastOrSmash = HasExplosives || CanUse(RG_MEGATON_HAMMER);
        CanChildAttack  = IsChild && (Slingshot || Boomerang || CanUse(RG_STICKS) || KokiriSword || HasExplosives || CanUse(RG_DINS_FIRE) || CanUse(RG_MASTER_SWORD) || CanUse(RG_MEGATON_HAMMER) || CanUse(RG_BIGGORON_SWORD));
        CanChildDamage  = IsChild && (Slingshot ||              CanUse(RG_STICKS) || KokiriSword || HasExplosives || CanUse(RG_DINS_FIRE) || CanUse(RG_MASTER_SWORD) || CanUse(RG_MEGATON_HAMMER) || CanUse(RG_BIGGORON_SWORD));
        CanAdultAttack  = IsAdult && (CanUse(RG_FAIRY_BOW) || CanUse(RG_BOOMERANG)       || CanUse(RG_STICKS) || CanUse(RG_KOKIRI_SWORD) || HasExplosives || CanUse(RG_DINS_FIRE) || MasterSword || Hammer || BiggoronSword || Hookshot);
        CanAdultDamage  = IsAdult && (CanUse(RG_FAIRY_BOW) || CanUse(RG_STICKS)          || CanUse(RG_KOKIRI_SWORD) || HasExplosives || CanUse(RG_DINS_FIRE) || MasterSword || Hammer || BiggoronSword);
        CanStunDeku     = CanAdultAttack || CanChildAttack || CanUse(RG_NUTS) || HasShield;
        CanCutShrubs    = CanUse(RG_KOKIRI_SWORD) || CanUse(RG_BOOMERANG) || HasExplosives || CanUse(RG_MASTER_SWORD) || CanUse(RG_MEGATON_HAMMER) || CanUse(RG_BIGGORON_SWORD);
        CanDive         = ProgressiveScale >= 2;
        CanLeaveForest  = ctx->GetOption(RSK_FOREST).IsNot(RO_FOREST_CLOSED) || IsAdult || DekuTreeClear || ctx->GetOption(RSK_SHUFFLE_INTERIOR_ENTRANCES) || ctx->GetOption(RSK_SHUFFLE_OVERWORLD_ENTRANCES);
        CanPlantBugs    = IsChild && CanUse(RG_BOTTLE_WITH_BUGS);
        CanRideEpona    = IsAdult && Epona && CanUse(RG_EPONAS_SONG);
        CanSummonGossipFairyWithoutSuns = CanUse(RG_ZELDAS_LULLABY) || CanUse(RG_EPONAS_SONG) || CanUse(RG_SONG_OF_TIME);
        CanSummonGossipFairy            = CanSummonGossipFairyWithoutSuns || CanUse(RG_SUNS_SONG);
        Hearts          = GetSaveContext()->healthCapacity / 16;
        EffectiveHealth = ((Hearts << (2 + DoubleDefense)) >> Multiplier) + ((Hearts << (2 + DoubleDefense)) % (1 << Multiplier) > 0); //Number of half heart hits to die, ranges from 1 to 160
        FireTimer       = CanUse(RG_GORON_TUNIC) ? 255 : (ctx->GetTrickOption(RT_FEWER_TUNIC_REQUIREMENTS)) ? (Hearts * 8) : 0;
        WaterTimer      = CanUse(RG_ZORA_TUNIC) ? 255 : (ctx->GetTrickOption(RT_FEWER_TUNIC_REQUIREMENTS)) ? (Hearts * 8) : 0;
        NeedNayrusLove      = (EffectiveHealth == 1);
        CanSurviveDamage    = !NeedNayrusLove || CanUse(RG_NAYRUS_LOVE);
        CanTakeDamage       = CanUse(RG_BOTTLE_WITH_FAIRY) || CanSurviveDamage;
        CanTakeDamageTwice  = (CanUse(RG_BOTTLE_WITH_FAIRY) && NumBottles >= 2) || ((EffectiveHealth == 2) && (CanUse(RG_NAYRUS_LOVE) || CanUse(RG_BOTTLE_WITH_FAIRY))) || (EffectiveHealth > 2);
        CanOpenBombGrotto   = CanBlastOrSmash       && (ShardOfAgony || ctx->GetTrickOption(RT_GROTTOS_WITHOUT_AGONY));
        CanOpenStormGrotto  = CanUse(RG_SONG_OF_STORMS) && (ShardOfAgony || ctx->GetTrickOption(RT_GROTTOS_WITHOUT_AGONY));
        HookshotOrBoomerang = CanUse(RG_HOOKSHOT) || CanUse(RG_BOOMERANG);
        CanGetNightTimeGS   = (CanUse(RG_SUNS_SONG) || !ctx->GetOption(RSK_SKULLS_SUNS_SONG));
        CanBreakUpperBeehives = HookshotOrBoomerang || (ctx->GetTrickOption(RT_BOMBCHU_BEEHIVES) && CanUse(RG_BOMBCHU_5));
        CanBreakLowerBeehives = CanBreakUpperBeehives || Bombs;
        CanFish = ChildsWallet && (HasItem(RG_FISHING_POLE) || !ctx->GetOption(RSK_SHUFFLE_FISHING_POLE));
        CanGetChildFish = CanFish && (IsChild || (IsAdult && !ctx->GetOption(RSK_FISHSANITY_AGE_SPLIT)));
        CanGetAdultFish = CanFish && IsAdult && ctx->GetOption(RSK_FISHSANITY_AGE_SPLIT);

        GuaranteeTradePath     = ctx->GetOption(RSK_SHUFFLE_INTERIOR_ENTRANCES) || ctx->GetOption(RSK_SHUFFLE_OVERWORLD_ENTRANCES) || ctx->GetTrickOption(RT_DMT_BOLERO_BIGGORON) || CanBlastOrSmash || StopGCRollingGoronAsAdult;
        //GuaranteeHint          = (hints == "Mask" && MaskofTruth) || (hints == "Agony") || (hints != "Mask" && hints != "Agony");
        HasFireSource          = CanUse(RG_DINS_FIRE) || CanUse(RG_FIRE_ARROWS);
        HasFireSourceWithTorch = HasFireSource || CanUse(RG_STICKS);

        //Gerudo Fortress
        CanFinishGerudoFortress = (ctx->GetOption(RSK_GERUDO_FORTRESS).Is(RO_GF_NORMAL)      && SmallKeys(RR_GERUDO_FORTRESS, 4) && (CanUse(RG_KOKIRI_SWORD) || CanUse(RG_MASTER_SWORD) || CanUse(RG_BIGGORON_SWORD)) && (GerudoToken || CanUse(RG_FAIRY_BOW) || CanUse(RG_HOOKSHOT) || CanUse(RG_HOVER_BOOTS) || ctx->GetTrickOption(RT_GF_KITCHEN))) ||
                                    (ctx->GetOption(RSK_GERUDO_FORTRESS).Is(RO_GF_FAST)      && SmallKeys(RR_GERUDO_FORTRESS, 1) && (CanUse(RG_KOKIRI_SWORD) || CanUse(RG_MASTER_SWORD) || CanUse(RG_BIGGORON_SWORD))) ||
                                    (ctx->GetOption(RSK_GERUDO_FORTRESS).IsNot(RO_GF_NORMAL) && ctx->GetOption(RSK_GERUDO_FORTRESS).IsNot(RO_GF_FAST));

        HasShield          = CanUse(RG_HYLIAN_SHIELD) || CanUse(RG_DEKU_SHIELD); //Mirror shield can't reflect attacks
        CanShield          = CanUse(RG_MIRROR_SHIELD) || HasShield;
        ChildShield        = IsChild && CanUse(RG_DEKU_SHIELD); //hylian shield is not helpful for child
        AdultReflectShield = IsAdult && CanUse(RG_HYLIAN_SHIELD); //Mirror shield can't reflect attacks
        AdultShield        = IsAdult && (CanUse(RG_HYLIAN_SHIELD) || CanUse(RG_MIRROR_SHIELD));
        CanShieldFlick     = ChildShield || AdultShield;
        CanJumpslash       = CanUse(RG_STICKS) || CanUse(RG_KOKIRI_SWORD) || CanUse(RG_MASTER_SWORD) || CanUse(RG_BIGGORON_SWORD); // Not including hammer as hammer jump attacks can be weird
        CanUseProjectile   = HasExplosives || CanUse(RG_FAIRY_BOW) || CanUse(RG_HOOKSHOT) || CanUse(RG_FAIRY_SLINGSHOT) || CanUse(RG_BOOMERANG);
        CanUseMagicArrow   = CanUse(RG_FIRE_ARROWS) || CanUse(RG_ICE_ARROWS) || CanUse(RG_LIGHT_ARROWS);

        //Bridge and LACS Requirements
        MedallionCount        = (ForestMedallion ? 1:0) + (FireMedallion ? 1:0) + (WaterMedallion ? 1:0) + (SpiritMedallion ? 1:0) + (ShadowMedallion ? 1:0) + (LightMedallion ? 1:0);
        StoneCount            = (KokiriEmerald ? 1:0) + (GoronRuby ? 1:0) + (ZoraSapphire ? 1:0);
        DungeonCount          = (DekuTreeClear ? 1:0) + (DodongosCavernClear ? 1:0) + (JabuJabusBellyClear ? 1:0) + (ForestTempleClear ? 1:0) + (FireTempleClear ? 1:0) + (WaterTempleClear ? 1:0) + (SpiritTempleClear ? 1:0) + (ShadowTempleClear ? 1:0);
        HasAllStones          = StoneCount == 3;
        HasAllMedallions      = MedallionCount == 6;
        GregInBridgeLogic     = ctx->GetOption(RSK_BRIDGE_OPTIONS).Is(RO_BRIDGE_GREG);
        GregInLacsLogic       = ctx->GetOption(RSK_LACS_OPTIONS).Is(RO_LACS_GREG_REWARD);

        CanBuildRainbowBridge = ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_ALWAYS_OPEN)                                                                         ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_VANILLA)    && ShadowMedallion && SpiritMedallion && LightArrows)             ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_STONES)     && StoneCount + (Greg && GregInBridgeLogic ? 1 : 0) >= ctx->GetOption(RSK_RAINBOW_BRIDGE_STONE_COUNT).Value<uint8_t>())                    ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_MEDALLIONS) && MedallionCount + (Greg && GregInBridgeLogic ? 1 : 0) >= ctx->GetOption(RSK_RAINBOW_BRIDGE_MEDALLION_COUNT).Value<uint8_t>())            ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_DUNGEON_REWARDS)    && StoneCount + MedallionCount + (Greg && GregInBridgeLogic ? 1 : 0) >= ctx->GetOption(RSK_RAINBOW_BRIDGE_REWARD_COUNT).Value<uint8_t>())  ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_DUNGEONS)   && DungeonCount + (Greg && GregInBridgeLogic ? 1 : 0) >= ctx->GetOption(RSK_RAINBOW_BRIDGE_DUNGEON_COUNT).Value<uint8_t>())                ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_TOKENS)     && GoldSkulltulaTokens >= ctx->GetOption(RSK_RAINBOW_BRIDGE_TOKEN_COUNT).Value<uint8_t>()) ||
                                (ctx->GetOption(RSK_RAINBOW_BRIDGE).Is(RO_BRIDGE_GREG)       && Greg);

        CanTriggerLACS = (ctx->GetSettings()->LACSCondition() == RO_LACS_VANILLA    && ShadowMedallion && SpiritMedallion)                          ||
                            (ctx->GetSettings()->LACSCondition() == RO_LACS_STONES     && StoneCount + (Greg && GregInLacsLogic ? 1 : 0) >= ctx->GetOption(RSK_LACS_STONE_COUNT).Value<uint8_t>())                    ||
                            (ctx->GetSettings()->LACSCondition() == RO_LACS_MEDALLIONS && MedallionCount + (Greg && GregInLacsLogic ? 1 : 0) >= ctx->GetOption(RSK_LACS_MEDALLION_COUNT).Value<uint8_t>())            ||
                            (ctx->GetSettings()->LACSCondition() == RO_LACS_REWARDS    && StoneCount + MedallionCount + (Greg && GregInLacsLogic ? 1 : 0) >= ctx->GetOption(RSK_LACS_REWARD_COUNT).Value<uint8_t>())  ||
                            (ctx->GetSettings()->LACSCondition() == RO_LACS_DUNGEONS   && DungeonCount + (Greg && GregInLacsLogic ? 1 : 0) >= ctx->GetOption(RSK_LACS_DUNGEON_COUNT).Value<uint8_t>())                ||
                            (ctx->GetSettings()->LACSCondition() == RO_LACS_TOKENS     && GoldSkulltulaTokens >= ctx->GetOption(RSK_LACS_TOKEN_COUNT).Value<uint8_t>());
        CanCompleteTriforce = TriforcePieces >= ctx->GetOption(RSK_TRIFORCE_HUNT_PIECES_REQUIRED).Value<uint8_t>();
        CanCompleteGanonBossKey = GKShards >= ctx->GetOption(RSK_GK_SHARDS_REQUIRED).Value<uint8_t>();
        StopPerformanceTimer(PT_UPDATE_HELPERS);
    }

    bool Logic::SmallKeys(RandomizerRegion dungeon, uint8_t requiredAmount) {
        return SmallKeys(dungeon, requiredAmount, requiredAmount);
    }

    bool Logic::SmallKeys(RandomizerRegion dungeon, uint8_t requiredAmountGlitchless, uint8_t requiredAmountGlitched) {
        if (HasItem(RG_SKELETON_KEY)) {
            return true;
        }
        switch (dungeon) {
            case RR_FOREST_TEMPLE:
                /*if (IsGlitched && (GetDifficultyValueFromString(GlitchHookshotJump_Boots) >= static_cast<uint8_t>(GlitchDifficulty::INTERMEDIATE) || GetDifficultyValueFromString(GlitchHoverBoost) >= static_cast<uint8_t>(GlitchDifficulty::NOVICE) ||
                                    (GetDifficultyValueFromString(GlitchHover) >= static_cast<uint8_t>(GlitchDifficulty::NOVICE) && GetDifficultyValueFromString(GlitchISG) >= static_cast<uint8_t>(GlitchDifficulty::INTERMEDIATE)))) {
                    return ForestTempleKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_FOREST_TEMPLE) >= requiredAmountGlitchless;

            case RR_FIRE_TEMPLE:
                /*if (IsGlitched && (GetDifficultyValueFromString(GlitchLedgeClip) >= static_cast<uint8_t>(GlitchDifficulty::INTERMEDIATE) || GetDifficultyValueFromString(GlitchHover) >= static_cast<uint8_t>(GlitchDifficulty::INTERMEDIATE))) {
                    return FireTempleKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_FIRE_TEMPLE) >= requiredAmountGlitchless;

            case RR_WATER_TEMPLE:
                /*if (IsGlitched && (false)) {
                    return WaterTempleKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_WATER_TEMPLE) >= requiredAmountGlitchless;

            case RR_SPIRIT_TEMPLE:
                /*if (IsGlitched && (false)) {
                    return SpiritTempleKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_SPIRIT_TEMPLE) >= requiredAmountGlitchless;

            case RR_SHADOW_TEMPLE:
                /*if (IsGlitched && (GetDifficultyValueFromString(GlitchHookshotClip) >= static_cast<uint8_t>(GlitchDifficulty::NOVICE))) {
                    return ShadowTempleKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_SHADOW_TEMPLE) >= requiredAmountGlitchless;

            case RR_BOTTOM_OF_THE_WELL:
                /*if (IsGlitched && (false)) {
                    return BottomOfTheWellKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_BOTTOM_OF_THE_WELL) >= requiredAmountGlitchless;

            case RR_GERUDO_TRAINING_GROUNDS:
                /*if (IsGlitched && (false)) {
                    return GerudoTrainingGroundsKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_GERUDO_TRAINING_GROUND) >= requiredAmountGlitchless;

            case RR_GANONS_CASTLE:
                /*if (IsGlitched && (false)) {
                    return GanonsCastleKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_INSIDE_GANONS_CASTLE) >= requiredAmountGlitchless;

            case RR_MARKET_TREASURE_CHEST_GAME:
                /*if (IsGlitched && (false)) {
                    return TreasureGameKeys >= requiredAmountGlitched;
                }*/
                return GetSmallKeyCount(SCENE_TREASURE_BOX_SHOP) >= requiredAmountGlitchless;

            case RR_GERUDO_FORTRESS:
                return GetSmallKeyCount(SCENE_THIEVES_HIDEOUT) >= requiredAmountGlitchless;

            default:
                return false;
        }
    }

    bool Logic::EventsUpdated() {
        if (DekuTreeClearPast        != DekuTreeClear        ||
            GoronRubyPast            != GoronRuby            ||
            ZoraSapphirePast         != ZoraSapphire         ||
            ForestTrialClearPast     != ForestTrialClear     ||
            FireTrialClearPast       != FireTrialClear       ||
            WaterTrialClearPast      != WaterTrialClear      ||
            ShadowTrialClearPast     != ShadowTrialClear     ||
            SpiritTrialClearPast     != SpiritTrialClear     ||
            LightTrialClearPast      != LightTrialClear      ||
            DrainWellPast            != DrainWell            ||
            DampesWindmillAccessPast != DampesWindmillAccess ||
            TimeTravelPast           != TimeTravel) {
            DekuTreeClearPast         = DekuTreeClear;
            GoronRubyPast             = GoronRuby;
            ZoraSapphirePast          = ZoraSapphire;
            ForestTrialClearPast      = ForestTrialClear;
            FireTrialClearPast        = FireTrialClear;
            WaterTrialClearPast       = WaterTrialClear;
            ShadowTrialClearPast      = ShadowTrialClear;
            SpiritTrialClearPast      = SpiritTrialClear;
            LightTrialClearPast       = LightTrialClear;
            DrainWellPast             = DrainWell;
            DampesWindmillAccessPast  = DampesWindmillAccess;
            return true;
        }
        return false;
    }

    std::map<RandomizerGet, uint32_t> Logic::RandoGetToEquipFlag = {
        { RG_KOKIRI_SWORD,           EQUIP_FLAG_SWORD_KOKIRI },
        { RG_MASTER_SWORD,           EQUIP_FLAG_SWORD_MASTER },
        { RG_BIGGORON_SWORD,         EQUIP_FLAG_SWORD_BGS },
        { RG_DEKU_SHIELD,            EQUIP_FLAG_SHIELD_DEKU },
        { RG_HYLIAN_SHIELD,          EQUIP_FLAG_SHIELD_HYLIAN },
        { RG_MIRROR_SHIELD,          EQUIP_FLAG_SHIELD_MIRROR },
        { RG_GORON_TUNIC,            EQUIP_FLAG_TUNIC_GORON },
        { RG_ZORA_TUNIC,             EQUIP_FLAG_TUNIC_ZORA },
        { RG_BUY_DEKU_SHIELD,        EQUIP_FLAG_SHIELD_DEKU },
        { RG_BUY_HYLIAN_SHIELD,      EQUIP_FLAG_SHIELD_HYLIAN },
        { RG_BUY_GORON_TUNIC,        EQUIP_FLAG_TUNIC_GORON },
        { RG_BUY_ZORA_TUNIC,         EQUIP_FLAG_TUNIC_ZORA },
        { RG_IRON_BOOTS,             EQUIP_FLAG_BOOTS_IRON },
        { RG_HOVER_BOOTS,            EQUIP_FLAG_BOOTS_HOVER }
    };

    std::map<RandomizerGet, uint32_t> Logic::RandoGetToRandInf = {
        { RG_ZELDAS_LETTER,          RAND_INF_ZELDAS_LETTER },
        { RG_WEIRD_EGG,              RAND_INF_WEIRD_EGG },
        { RG_GOHMA_SOUL,             RAND_INF_GOHMA_SOUL },
        { RG_KING_DODONGO_SOUL,      RAND_INF_KING_DODONGO_SOUL },
        { RG_BARINADE_SOUL,          RAND_INF_BARINADE_SOUL },
        { RG_PHANTOM_GANON_SOUL,     RAND_INF_PHANTOM_GANON_SOUL },
        { RG_VOLVAGIA_SOUL,          RAND_INF_VOLVAGIA_SOUL },
        { RG_MORPHA_SOUL,            RAND_INF_MORPHA_SOUL },
        { RG_BONGO_BONGO_SOUL,       RAND_INF_BONGO_BONGO_SOUL },
        { RG_TWINROVA_SOUL,          RAND_INF_TWINROVA_SOUL },
        { RG_GANON_SOUL,             RAND_INF_GANON_SOUL },
        { RG_OCARINA_A_BUTTON,       RAND_INF_HAS_OCARINA_A },
        { RG_OCARINA_C_UP_BUTTON,    RAND_INF_HAS_OCARINA_C_UP },
        { RG_OCARINA_C_DOWN_BUTTON,  RAND_INF_HAS_OCARINA_C_DOWN },
        { RG_OCARINA_C_LEFT_BUTTON,  RAND_INF_HAS_OCARINA_C_LEFT },
        { RG_OCARINA_C_RIGHT_BUTTON, RAND_INF_HAS_OCARINA_C_RIGHT },
        { RG_SKELETON_KEY,           RAND_INF_HAS_SKELETON_KEY },
        { RG_GREG_RUPEE,             RAND_INF_GREG_FOUND },
        { RG_FISHING_POLE,           RAND_INF_FISHING_POLE_FOUND }
    };

    std::map<uint32_t, uint32_t> Logic::RandoGetToDungeonScene = {
        { RG_FOREST_TEMPLE_SMALL_KEY,           SCENE_FOREST_TEMPLE },
        { RG_FIRE_TEMPLE_SMALL_KEY,             SCENE_FIRE_TEMPLE },
        { RG_WATER_TEMPLE_SMALL_KEY,            SCENE_WATER_TEMPLE },
        { RG_SPIRIT_TEMPLE_SMALL_KEY,           SCENE_SPIRIT_TEMPLE },
        { RG_SHADOW_TEMPLE_SMALL_KEY,           SCENE_SHADOW_TEMPLE },
        { RG_BOTTOM_OF_THE_WELL_SMALL_KEY,      SCENE_BOTTOM_OF_THE_WELL },
        { RG_GERUDO_TRAINING_GROUNDS_SMALL_KEY, SCENE_GERUDO_TRAINING_GROUND },
        { RG_GERUDO_FORTRESS_SMALL_KEY,         SCENE_THIEVES_HIDEOUT },
        { RG_GANONS_CASTLE_SMALL_KEY,           SCENE_INSIDE_GANONS_CASTLE },
        { RG_FOREST_TEMPLE_KEY_RING,            SCENE_FOREST_TEMPLE },
        { RG_FIRE_TEMPLE_KEY_RING,              SCENE_FIRE_TEMPLE },
        { RG_WATER_TEMPLE_KEY_RING,             SCENE_WATER_TEMPLE },
        { RG_SPIRIT_TEMPLE_KEY_RING,            SCENE_SPIRIT_TEMPLE },
        { RG_SHADOW_TEMPLE_KEY_RING,            SCENE_SHADOW_TEMPLE },
        { RG_BOTTOM_OF_THE_WELL_KEY_RING,       SCENE_BOTTOM_OF_THE_WELL },
        { RG_GERUDO_TRAINING_GROUNDS_KEY_RING,  SCENE_GERUDO_TRAINING_GROUND },
        { RG_GERUDO_FORTRESS_KEY_RING,          SCENE_THIEVES_HIDEOUT },
        { RG_GANONS_CASTLE_KEY_RING,            SCENE_INSIDE_GANONS_CASTLE },
        { RG_FOREST_TEMPLE_BOSS_KEY,            SCENE_FOREST_TEMPLE },
        { RG_FIRE_TEMPLE_BOSS_KEY,              SCENE_FIRE_TEMPLE },
        { RG_WATER_TEMPLE_BOSS_KEY,             SCENE_WATER_TEMPLE },
        { RG_SPIRIT_TEMPLE_BOSS_KEY,            SCENE_SPIRIT_TEMPLE },
        { RG_SHADOW_TEMPLE_BOSS_KEY,            SCENE_SHADOW_TEMPLE },
        { RG_GANONS_CASTLE_BOSS_KEY,            SCENE_INSIDE_GANONS_CASTLE },
        { RG_DEKU_TREE_MAP,                     SCENE_DEKU_TREE },
        { RG_DODONGOS_CAVERN_MAP,               SCENE_DODONGOS_CAVERN },
        { RG_JABU_JABUS_BELLY_MAP,              SCENE_JABU_JABU },
        { RG_FOREST_TEMPLE_MAP,                 SCENE_FOREST_TEMPLE },
        { RG_FIRE_TEMPLE_MAP,                   SCENE_FIRE_TEMPLE },
        { RG_WATER_TEMPLE_MAP,                  SCENE_WATER_TEMPLE },
        { RG_SPIRIT_TEMPLE_MAP,                 SCENE_SPIRIT_TEMPLE },
        { RG_SHADOW_TEMPLE_MAP,                 SCENE_SHADOW_TEMPLE },
        { RG_BOTTOM_OF_THE_WELL_MAP,            SCENE_BOTTOM_OF_THE_WELL },
        { RG_ICE_CAVERN_MAP,                    SCENE_ICE_CAVERN },
        { RG_DEKU_TREE_COMPASS,                 SCENE_DEKU_TREE },
        { RG_DODONGOS_CAVERN_COMPASS,           SCENE_DODONGOS_CAVERN },
        { RG_JABU_JABUS_BELLY_COMPASS,          SCENE_JABU_JABU },
        { RG_FOREST_TEMPLE_COMPASS,             SCENE_FOREST_TEMPLE },
        { RG_FIRE_TEMPLE_COMPASS,               SCENE_FIRE_TEMPLE },
        { RG_WATER_TEMPLE_COMPASS,              SCENE_WATER_TEMPLE },
        { RG_SPIRIT_TEMPLE_COMPASS,             SCENE_SPIRIT_TEMPLE },
        { RG_SHADOW_TEMPLE_COMPASS,             SCENE_SHADOW_TEMPLE },
        { RG_BOTTOM_OF_THE_WELL_COMPASS,        SCENE_BOTTOM_OF_THE_WELL },
        { RG_ICE_CAVERN_COMPASS,                SCENE_ICE_CAVERN },
        { RG_TREASURE_GAME_SMALL_KEY,           SCENE_TREASURE_BOX_SHOP }
    };

    std::map<uint32_t, uint32_t> Logic::RandoGetToQuestItem = {
        { RG_FOREST_MEDALLION,       QUEST_MEDALLION_FOREST },
        { RG_FIRE_MEDALLION,         QUEST_MEDALLION_FIRE },
        { RG_WATER_MEDALLION,        QUEST_MEDALLION_WATER },
        { RG_SPIRIT_MEDALLION,       QUEST_MEDALLION_SPIRIT },
        { RG_SHADOW_MEDALLION,       QUEST_MEDALLION_SHADOW },
        { RG_LIGHT_MEDALLION,        QUEST_MEDALLION_LIGHT },
        { RG_MINUET_OF_FOREST,       QUEST_SONG_MINUET },
        { RG_BOLERO_OF_FIRE,         QUEST_SONG_BOLERO },
        { RG_SERENADE_OF_WATER,      QUEST_SONG_SERENADE },
        { RG_REQUIEM_OF_SPIRIT,      QUEST_SONG_REQUIEM },
        { RG_NOCTURNE_OF_SHADOW,     QUEST_SONG_NOCTURNE },
        { RG_PRELUDE_OF_LIGHT,       QUEST_SONG_PRELUDE },
        { RG_ZELDAS_LULLABY,         QUEST_SONG_LULLABY },
        { RG_EPONAS_SONG,            QUEST_SONG_EPONA },
        { RG_SARIAS_SONG,            QUEST_SONG_SARIA },
        { RG_SUNS_SONG,              QUEST_SONG_SUN },
        { RG_SONG_OF_TIME,           QUEST_SONG_TIME },
        { RG_SONG_OF_STORMS,         QUEST_SONG_STORMS },
        { RG_KOKIRI_EMERALD,         QUEST_KOKIRI_EMERALD },
        { RG_GORON_RUBY,             QUEST_GORON_RUBY },
        { RG_ZORA_SAPPHIRE,          QUEST_ZORA_SAPPHIRE },
        { RG_STONE_OF_AGONY,         QUEST_STONE_OF_AGONY },
        { RG_GERUDO_MEMBERSHIP_CARD, QUEST_GERUDO_CARD },
    };

    uint32_t HookshotLookup[3] = { ITEM_NONE, ITEM_HOOKSHOT, ITEM_LONGSHOT };
    uint32_t OcarinaLookup[3] = { ITEM_NONE, ITEM_OCARINA_FAIRY, ITEM_OCARINA_TIME };

    void Logic::ApplyItemEffect(Item& item, bool state) {
        auto randoGet = item.GetRandomizerGet();
        if (item.GetGIEntry()->objectId == OBJECT_GI_STICK) {
            SetInventory(ITEM_STICK, (!state ? ITEM_NONE : ITEM_STICK));
        }
        if (item.GetGIEntry()->objectId == OBJECT_GI_NUTS) {
            SetInventory(ITEM_NUT, (!state ? ITEM_NONE : ITEM_NUT));
        }
        switch (item.GetItemType()) {
        case ITEMTYPE_ITEM:
        {
            switch (randoGet) {
            case RG_STONE_OF_AGONY:
            case RG_GERUDO_MEMBERSHIP_CARD:
                SetQuestItem(RandoGetToQuestItem.at(randoGet), state);
                break;
            case RG_WEIRD_EGG:
                SetRandoInf(RAND_INF_WEIRD_EGG, state);
                break;
            case RG_ZELDAS_LETTER:
                SetRandoInf(RAND_INF_ZELDAS_LETTER, state);
                break;
            case RG_DOUBLE_DEFENSE:
                mSaveContext->isDoubleDefenseAcquired = state;
                break;
            case RG_POCKET_EGG:
            case RG_COJIRO:
            case RG_ODD_MUSHROOM:
            case RG_ODD_POTION:
            case RG_POACHERS_SAW:
            case RG_BROKEN_SWORD:
            case RG_PRESCRIPTION:
            case RG_EYEBALL_FROG:
            case RG_EYEDROPS:
            case RG_CLAIM_CHECK:
                SetAdultTrade(item.GetGIEntry()->itemId, state);
                break;
            case RG_PROGRESSIVE_HOOKSHOT:
            {
                uint8_t i;
                for (i = 0; i < 3; i++) {
                    if (CurrentInventory(ITEM_HOOKSHOT) == HookshotLookup[i]) {
                        break;
                    }
                }
                auto newItem = i + (!state ? -1 : 1);
                if (newItem < 0) {
                    newItem = 0;
                }
                else if (newItem > 2) {
                    newItem = 2;
                }
                SetInventory(ITEM_HOOKSHOT, HookshotLookup[newItem]);
            }   break;
            case RG_PROGRESSIVE_STRENGTH:
            {
                auto currentLevel = CurrentUpgrade(UPG_STRENGTH);
                auto newLevel = currentLevel + (!state ? -1 : 1);
                SetUpgrade(UPG_STRENGTH, newLevel);
            }   break;
            case RG_PROGRESSIVE_BOMB_BAG:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_BOMB_BAG_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_BOMB_BAG, true);
                    break;
                }
                auto currentLevel = CurrentUpgrade(UPG_BOMB_BAG);
                auto newLevel = currentLevel + (!state ? -1 : 1);
                if (currentLevel == 0 && state || currentLevel == 1 && !state) {
                    SetInventory(ITEM_BOMB, (!state ? ITEM_NONE : ITEM_BOMB));
                }
                SetUpgrade(UPG_BOMB_BAG, newLevel);
            }   break;
            case RG_PROGRESSIVE_BOW:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_QUIVER_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_QUIVER, true);
                    break;
                }
                auto currentLevel = CurrentUpgrade(UPG_QUIVER);
                auto newLevel = currentLevel + (!state ? -1 : 1);
                if (currentLevel == 0 && state || currentLevel == 1 && !state) {
                    SetInventory(ITEM_BOW, (!state ? ITEM_NONE : ITEM_BOW));
                }
                SetUpgrade(UPG_QUIVER, newLevel);
            }   break;
            case RG_PROGRESSIVE_SLINGSHOT:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_BULLET_BAG_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_BULLET_BAG, true);
                    break;
                }
                auto currentLevel = CurrentUpgrade(UPG_BULLET_BAG);
                auto newLevel = currentLevel + (!state ? -1 : 1);
                if (currentLevel == 0 && state || currentLevel == 1 && !state) {
                    SetInventory(ITEM_SLINGSHOT, (!state ? ITEM_NONE : ITEM_SLINGSHOT));
                }
                SetUpgrade(UPG_BULLET_BAG, newLevel);
            }   break;
            case RG_PROGRESSIVE_WALLET:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_WALLET_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_MONEY, true);
                    break;
                }
                auto currentLevel = CurrentUpgrade(UPG_WALLET);
                if (!CheckRandoInf(RAND_INF_HAS_WALLET) && state) {
                    SetRandoInf(RAND_INF_HAS_WALLET, true);
                }
                else if (currentLevel == 0 && !state) {
                    SetRandoInf(RAND_INF_HAS_WALLET, false);
                }
                else {
                    auto newLevel = currentLevel + (!state ? -1 : 1);
                    SetUpgrade(UPG_WALLET, newLevel);
                }
            }   break;
            case RG_PROGRESSIVE_SCALE:
            {
                auto currentLevel = CurrentUpgrade(UPG_SCALE);
                if (!CheckRandoInf(RAND_INF_CAN_SWIM) && state) {
                    SetRandoInf(RAND_INF_CAN_SWIM, true);
                }
                else if (currentLevel == 0 && !state) {
                    SetRandoInf(RAND_INF_CAN_SWIM, false);
                }
                else {
                    auto newLevel = currentLevel + (!state ? -1 : 1);
                    SetUpgrade(UPG_SCALE, newLevel);
                }
            }   break;
            case RG_PROGRESSIVE_NUT_UPGRADE:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_NUT_UPGRADE_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_NUT_UPGRADE, true);
                    break;
                }
                auto currentLevel = CurrentUpgrade(UPG_NUTS);
                auto newLevel = currentLevel + (!state ? -1 : 1);
                if (currentLevel == 0 && state || currentLevel == 1 && !state) {
                    SetInventory(ITEM_NUT, (!state ? ITEM_NONE : ITEM_NUT));
                }
                SetUpgrade(UPG_NUTS, newLevel);
            }   break;
            case RG_PROGRESSIVE_STICK_UPGRADE:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_STICK_UPGRADE_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_STICK_UPGRADE, true);
                    break;
                }
                auto currentLevel = CurrentUpgrade(UPG_STICKS);
                auto newLevel = currentLevel + (!state ? -1 : 1);
                if (currentLevel == 0 && state || currentLevel == 1 && !state) {
                    SetInventory(ITEM_STICK, (!state ? ITEM_NONE : ITEM_STICK));
                }
                SetUpgrade(UPG_STICKS, newLevel);
            }   break;
            case RG_PROGRESSIVE_BOMBCHUS:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_BOMBCHU_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_BOMBCHUS, true);
                    break;
                }
                SetInventory(ITEM_BOMBCHU, (!state ? ITEM_NONE : ITEM_BOMBCHU));
            } break;
            case RG_PROGRESSIVE_MAGIC_METER:
            {
                auto realGI = item.GetGIEntry();
                if (realGI->itemId == RG_MAGIC_INF && realGI->modIndex == MOD_RANDOMIZER) {
                    SetRandoInf(RAND_INF_HAS_INFINITE_MAGIC_METER, true);
                    break;
                }
                mSaveContext->magicLevel += (!state ? -1 : 1);
            } break;
            case RG_PROGRESSIVE_OCARINA:
            {
                uint8_t i;
                for (i = 0; i < 3; i++) {
                    if (CurrentInventory(ITEM_OCARINA_FAIRY) == OcarinaLookup[i]) {
                        break;
                    }
                }
                i += (!state ? -1 : 1);
                if (i < 0) {
                    i = 0;
                }
                else if (i > 2) {
                    i = 2;
                }
                SetInventory(ITEM_OCARINA_FAIRY, OcarinaLookup[i]);
            }   break;
            case RG_HEART_CONTAINER:
                mSaveContext->healthCapacity += (!state ? -16 : 16);
                break;
            case RG_PIECE_OF_HEART:
                mSaveContext->healthCapacity += (!state ? -4 : 4);
                break;
            case RG_BOOMERANG:
            case RG_LENS_OF_TRUTH:
            case RG_MEGATON_HAMMER:
            case RG_DINS_FIRE:
            case RG_FARORES_WIND:
            case RG_NAYRUS_LOVE:
            case RG_FIRE_ARROWS:
            case RG_ICE_ARROWS:
            case RG_LIGHT_ARROWS:
                SetInventory(item.GetGIEntry()->itemId, (!state ? ITEM_NONE : item.GetGIEntry()->itemId));
                break;
            case RG_MAGIC_BEAN:
            case RG_MAGIC_BEAN_PACK:
            {
                auto change = (item.GetRandomizerGet() == RG_MAGIC_BEAN ? 1 : 10);
                auto current = GetAmmo(ITEM_BEAN);
                SetAmmo(ITEM_BEAN, current + (!state ? -change : change));
            }   break;
            case RG_EMPTY_BOTTLE:
            case RG_BOTTLE_WITH_MILK:
            case RG_BOTTLE_WITH_RED_POTION:
            case RG_BOTTLE_WITH_GREEN_POTION:
            case RG_BOTTLE_WITH_BLUE_POTION:
            case RG_BOTTLE_WITH_FAIRY:
            case RG_BOTTLE_WITH_FISH:
            case RG_BOTTLE_WITH_BLUE_FIRE:
            case RG_BOTTLE_WITH_BUGS:
            case RG_BOTTLE_WITH_POE:
            case RG_BOTTLE_WITH_BIG_POE:
            {
                uint8_t slot = SLOT_BOTTLE_1;
                while (slot != SLOT_BOTTLE_4) {
                    if (mSaveContext->inventory.items[slot] == ITEM_NONE) {
                        break;
                    }
                    slot++;
                }
                mSaveContext->inventory.items[slot] = item.GetGIEntry()->itemId;
            }   break;
            case RG_RUTOS_LETTER:
                SetEventChkInf(EVENTCHKINF_OBTAINED_RUTOS_LETTER, state);
                break;
            case RG_GOHMA_SOUL:
            case RG_KING_DODONGO_SOUL:
            case RG_BARINADE_SOUL:
            case RG_PHANTOM_GANON_SOUL:
            case RG_VOLVAGIA_SOUL:
            case RG_MORPHA_SOUL:
            case RG_BONGO_BONGO_SOUL:
            case RG_TWINROVA_SOUL:
            case RG_GANON_SOUL:
            case RG_OCARINA_A_BUTTON:
            case RG_OCARINA_C_UP_BUTTON:
            case RG_OCARINA_C_DOWN_BUTTON:
            case RG_OCARINA_C_LEFT_BUTTON:
            case RG_OCARINA_C_RIGHT_BUTTON:
            case RG_GREG_RUPEE:
            case RG_FISHING_POLE:
                SetRandoInf(RandoGetToRandInf.at(randoGet), state);
                break;
            case RG_TRIFORCE_PIECE:
                mSaveContext->triforcePiecesCollected += (!state ? -1 : 1);
                break;
            case RG_GK_SHARD:
                mSaveContext->GKShardsCollected += (!state ? -1 : 1);
                break;
            case RG_BOMBCHU_5:
            case RG_BOMBCHU_10:
            case RG_BOMBCHU_20:
                SetInventory(ITEM_BOMBCHU, (!state ? ITEM_NONE : ITEM_BOMBCHU));
                break;
            }
        }
        break;
        case ITEMTYPE_EQUIP:
        {
            RandomizerGet itemRG = item.GetRandomizerGet();
            if (itemRG == RG_GIANTS_KNIFE) {
                return;
            }
            uint32_t equipId = RandoGetToEquipFlag.find(itemRG)->second;
            if (!state) {
                mSaveContext->inventory.equipment &= ~equipId;
                if (equipId == EQUIP_FLAG_SWORD_BGS) {
                    mSaveContext->bgsFlag = false;
                }
            }
            else {
                mSaveContext->inventory.equipment |= equipId;
                if (equipId == EQUIP_FLAG_SWORD_BGS) {
                    mSaveContext->bgsFlag = true;
                }
            }
        }
        break;
        case ITEMTYPE_DUNGEONREWARD:
        case ITEMTYPE_SONG:
            SetQuestItem(RandoGetToQuestItem.find(item.GetRandomizerGet())->second, state);
            break;
        case ITEMTYPE_MAP:
            SetDungeonItem(DUNGEON_MAP, RandoGetToDungeonScene.find(item.GetRandomizerGet())->second, state);
            break;
        case ITEMTYPE_COMPASS:
            SetDungeonItem(DUNGEON_COMPASS, RandoGetToDungeonScene.find(item.GetRandomizerGet())->second, state);
            break;
        case ITEMTYPE_BOSSKEY:
            SetDungeonItem(DUNGEON_KEY_BOSS, RandoGetToDungeonScene.find(item.GetRandomizerGet())->second, state);
            break;
        case ITEMTYPE_FORTRESS_SMALLKEY:
        case ITEMTYPE_SMALLKEY:
        {
            auto randoGet = item.GetRandomizerGet();
            auto keyring = randoGet >= RG_FOREST_TEMPLE_KEY_RING && randoGet <= RG_GANONS_CASTLE_KEY_RING;
            auto dungeonIndex = RandoGetToDungeonScene.find(randoGet)->second;
            auto count = GetSmallKeyCount(dungeonIndex);
            if (!state) {
                if (keyring) {
                    count = 0;
                }
                else {
                    count -= 1;
                }
            }
            else {
                if (keyring) {
                    count = 10;
                }
                else {
                    count += 1;
                }
            }
            SetSmallKeyCount(dungeonIndex, count);
        } break;
        case ITEMTYPE_TOKEN:
            mSaveContext->inventory.gsTokens += (!state ? -1 : 1);
            break;
        case ITEMTYPE_EVENT:
            break;
        case ITEMTYPE_DROP:
        case ITEMTYPE_REFILL:
        case ITEMTYPE_SHOP:
        {
            RandomizerGet itemRG = item.GetRandomizerGet();
            if (itemRG == RG_BUY_HYLIAN_SHIELD || itemRG == RG_BUY_DEKU_SHIELD || itemRG == RG_BUY_GORON_TUNIC || itemRG == RG_BUY_ZORA_TUNIC) {
                uint32_t equipId = RandoGetToEquipFlag.find(itemRG)->second;
                if (!state) {
                    mSaveContext->inventory.equipment &= ~equipId;
                }
                else {
                    mSaveContext->inventory.equipment |= equipId;
                }
            }
            switch (itemRG) {
            case RG_DEKU_NUTS_5:
            case RG_DEKU_NUTS_10:
            case RG_BUY_DEKU_NUTS_5:
            case RG_BUY_DEKU_NUTS_10:
                SetInventory(ITEM_NUT, (!state ? ITEM_NONE : ITEM_NUT));
                break;
            case RG_DEKU_STICK_1:
            case RG_BUY_DEKU_STICK_1:
            case RG_STICKS:
                SetInventory(ITEM_STICK, (!state ? ITEM_NONE : ITEM_STICK));
                break;
            case RG_BOMBCHU_5:
            case RG_BOMBCHU_10:
            case RG_BOMBCHU_20:
                SetInventory(ITEM_BOMBCHU, (!state ? ITEM_NONE : ITEM_BOMBCHU));
                break;
            }
        } break;
        }
        UpdateHelpers();
    }

    SaveContext* Logic::GetSaveContext() {
        if (mSaveContext == nullptr) {
            NewSaveContext();
        }
        return mSaveContext;
    }

    void Logic::SetSaveContext(SaveContext* context) {
        mSaveContext = context;
    }

    void Logic::InitSaveContext() {
        mSaveContext->totalDays = 0;
        mSaveContext->bgsDayCount = 0;

        mSaveContext->deaths = 0;
        for (int i = 0; i < ARRAY_COUNT(mSaveContext->playerName); i++) {
            mSaveContext->playerName[i] = 0x3E;
        }
        mSaveContext->n64ddFlag = 0;
        mSaveContext->healthCapacity = 0x30;
        mSaveContext->health = 0x30;
        mSaveContext->magicLevel = 0;
        mSaveContext->magic = 0x30;
        mSaveContext->rupees = 0;
        mSaveContext->swordHealth = 0;
        mSaveContext->naviTimer = 0;
        mSaveContext->isMagicAcquired = 0;
        mSaveContext->isDoubleMagicAcquired = 0;
        mSaveContext->isDoubleDefenseAcquired = 0;
        mSaveContext->bgsFlag = 0;
        mSaveContext->ocarinaGameRoundNum = 0;
        for (int button = 0; button < ARRAY_COUNT(mSaveContext->childEquips.buttonItems); button++) {
            mSaveContext->childEquips.buttonItems[button] = ITEM_NONE;
        }
        for (int button = 0; button < ARRAY_COUNT(mSaveContext->childEquips.cButtonSlots); button++) {
            mSaveContext->childEquips.cButtonSlots[button] = SLOT_NONE;
        }
        mSaveContext->childEquips.equipment = 0;
        for (int button = 0; button < ARRAY_COUNT(mSaveContext->adultEquips.buttonItems); button++) {
            mSaveContext->adultEquips.buttonItems[button] = ITEM_NONE;
        }
        for (int button = 0; button < ARRAY_COUNT(mSaveContext->adultEquips.cButtonSlots); button++) {
            mSaveContext->adultEquips.cButtonSlots[button] = SLOT_NONE;
        }
        mSaveContext->adultEquips.equipment = 0;
        mSaveContext->unk_54 = 0;
        mSaveContext->savedSceneNum = SCENE_LINKS_HOUSE;

        // Equipment
        for (int button = 0; button < ARRAY_COUNT(mSaveContext->equips.buttonItems); button++) {
            mSaveContext->equips.buttonItems[button] = ITEM_NONE;
        }
        for (int button = 0; button < ARRAY_COUNT(mSaveContext->equips.cButtonSlots); button++) {
            mSaveContext->equips.cButtonSlots[button] = SLOT_NONE;
        }
        mSaveContext->equips.equipment = 0;

        // Inventory
        for (int item = 0; item < ARRAY_COUNT(mSaveContext->inventory.items); item++) {
            mSaveContext->inventory.items[item] = ITEM_NONE;
        }
        for (int ammo = 0; ammo < ARRAY_COUNT(mSaveContext->inventory.ammo); ammo++) {
            mSaveContext->inventory.ammo[ammo] = 0;
        }
        mSaveContext->inventory.equipment = 0;
        mSaveContext->inventory.upgrades = 0;
        mSaveContext->inventory.questItems = 0;
        for (int dungeon = 0; dungeon < ARRAY_COUNT(mSaveContext->inventory.dungeonItems); dungeon++) {
            mSaveContext->inventory.dungeonItems[dungeon] = 0;
        }
        for (int dungeon = 0; dungeon < ARRAY_COUNT(mSaveContext->inventory.dungeonKeys); dungeon++) {
            mSaveContext->inventory.dungeonKeys[dungeon] = 0x0;
        }
        mSaveContext->inventory.defenseHearts = 0;
        mSaveContext->inventory.gsTokens = 0;
        for (int scene = 0; scene < ARRAY_COUNT(mSaveContext->sceneFlags); scene++) {
            mSaveContext->sceneFlags[scene].chest = 0;
            mSaveContext->sceneFlags[scene].swch = 0;
            mSaveContext->sceneFlags[scene].clear = 0;
            mSaveContext->sceneFlags[scene].collect = 0;
            mSaveContext->sceneFlags[scene].unk = 0;
            mSaveContext->sceneFlags[scene].rooms = 0;
            mSaveContext->sceneFlags[scene].floors = 0;
        }
        mSaveContext->fw.pos.x = 0;
        mSaveContext->fw.pos.y = 0;
        mSaveContext->fw.pos.z = 0;
        mSaveContext->fw.yaw = 0;
        mSaveContext->fw.playerParams = 0;
        mSaveContext->fw.entranceIndex = 0;
        mSaveContext->fw.roomIndex = 0;
        mSaveContext->fw.set = 0;
        mSaveContext->fw.tempSwchFlags = 0;
        mSaveContext->fw.tempCollectFlags = 0;
        for (int flag = 0; flag < ARRAY_COUNT(mSaveContext->gsFlags); flag++) {
            mSaveContext->gsFlags[flag] = 0;
        }
        for (int highscore = 0; highscore < ARRAY_COUNT(mSaveContext->highScores); highscore++) {
            mSaveContext->highScores[highscore] = 0;
        }
        for (int flag = 0; flag < ARRAY_COUNT(mSaveContext->eventChkInf); flag++) {
            mSaveContext->eventChkInf[flag] = 0;
        }
        for (int flag = 0; flag < ARRAY_COUNT(mSaveContext->itemGetInf); flag++) {
            mSaveContext->itemGetInf[flag] = 0;
        }
        for (int flag = 0; flag < ARRAY_COUNT(mSaveContext->infTable); flag++) {
            mSaveContext->infTable[flag] = 0;
        }
        mSaveContext->worldMapAreaData = 0;
        mSaveContext->scarecrowLongSongSet = 0;
        for (int i = 0; i < ARRAY_COUNT(mSaveContext->scarecrowLongSong); i++) {
            mSaveContext->scarecrowLongSong[i].noteIdx = 0;
            mSaveContext->scarecrowLongSong[i].unk_01 = 0;
            mSaveContext->scarecrowLongSong[i].unk_02 = 0;
            mSaveContext->scarecrowLongSong[i].volume = 0;
            mSaveContext->scarecrowLongSong[i].vibrato = 0;
            mSaveContext->scarecrowLongSong[i].tone = 0;
            mSaveContext->scarecrowLongSong[i].semitone = 0;
        }
        mSaveContext->scarecrowSpawnSongSet = 0;
        for (int i = 0; i < ARRAY_COUNT(mSaveContext->scarecrowSpawnSong); i++) {
            mSaveContext->scarecrowSpawnSong[i].noteIdx = 0;
            mSaveContext->scarecrowSpawnSong[i].unk_01 = 0;
            mSaveContext->scarecrowSpawnSong[i].unk_02 = 0;
            mSaveContext->scarecrowSpawnSong[i].volume = 0;
            mSaveContext->scarecrowSpawnSong[i].vibrato = 0;
            mSaveContext->scarecrowSpawnSong[i].tone = 0;
            mSaveContext->scarecrowSpawnSong[i].semitone = 0;
        }

        mSaveContext->horseData.scene = SCENE_HYRULE_FIELD;
        mSaveContext->horseData.pos.x = -1840;
        mSaveContext->horseData.pos.y = 72;
        mSaveContext->horseData.pos.z = 5497;
        mSaveContext->horseData.angle = -0x6AD9;
        mSaveContext->magicLevel = 0;
        mSaveContext->infTable[29] = 1;
        mSaveContext->sceneFlags[5].swch = 0x40000000;

        // SoH specific
        mSaveContext->backupFW = mSaveContext->fw;
        mSaveContext->pendingSale = ITEM_NONE;
        mSaveContext->pendingSaleMod = MOD_NONE;
        mSaveContext->isBossRushPaused = 0;
        mSaveContext->pendingIceTrapCount = 0;

        // Init with normal quest unless only an MQ rom is provided
        mSaveContext->questId = OTRGlobals::Instance->HasOriginal() ? QUEST_NORMAL : QUEST_MASTER;

        //RANDOTODO (ADD ITEMLOCATIONS TO GSAVECONTEXT)
    }

    void Logic::NewSaveContext() {
        if (mSaveContext != nullptr && mSaveContext != &gSaveContext) {
            free(mSaveContext);
        }
        mSaveContext = new SaveContext();
        InitSaveContext();
    }

    uint8_t Logic::InventorySlot(uint32_t item) {
        return gItemSlots[item];
    }

    uint32_t Logic::CurrentUpgrade(uint32_t upgrade) {
        return (mSaveContext->inventory.upgrades & gUpgradeMasks[upgrade]) >> gUpgradeShifts[upgrade];
    }

    uint32_t Logic::CurrentInventory(uint32_t item) {
        return mSaveContext->inventory.items[InventorySlot(item)];
    }

    void Logic::SetUpgrade(uint32_t upgrade, uint8_t level) {
        mSaveContext->inventory.upgrades &= gUpgradeNegMasks[upgrade];
        mSaveContext->inventory.upgrades |= level << gUpgradeShifts[upgrade];
    }

    bool Logic::CheckInventory(uint32_t item, bool exact) {
        auto current = mSaveContext->inventory.items[InventorySlot(item)];
        return exact ? (current == item) : (current != ITEM_NONE);
    }

    void Logic::SetInventory(uint32_t itemSlot, uint32_t item) {
        mSaveContext->inventory.items[InventorySlot(itemSlot)] = item;
    }

    bool Logic::CheckEquipment(uint32_t equipFlag) {
        return (equipFlag & mSaveContext->inventory.equipment);
    }

    bool Logic::CheckQuestItem(uint32_t item) {
        return ((1 << item) & mSaveContext->inventory.questItems);
    }

    bool Logic::HasAdultTrade(uint32_t itemID) {
        int tradeIndex = itemID - ITEM_POCKET_EGG;
        return mSaveContext->adultTradeItems & (1 << tradeIndex);
    }

    void Logic::SetAdultTrade(uint32_t itemID, bool state) {
        int tradeIndex = itemID - ITEM_POCKET_EGG;
        if (!state) {
            mSaveContext->adultTradeItems &= ~(1 << tradeIndex);
        }
        else {
            mSaveContext->adultTradeItems |= (1 << tradeIndex);
        }
    }

    void Logic::SetQuestItem(uint32_t item, bool state) {
        if (!state) {
            mSaveContext->inventory.questItems &= ~(1 << item);
        }
        else {
            mSaveContext->inventory.questItems |= (1 << item);
        }
    }

    uint8_t Logic::GetSmallKeyCount(uint32_t dungeonIndex) {
        return mSaveContext->inventory.dungeonKeys[dungeonIndex];
    }

    void Logic::SetSmallKeyCount(uint32_t dungeonIndex, uint8_t count) {
        mSaveContext->inventory.dungeonKeys[dungeonIndex] = count;
    }

    bool Logic::CheckDungeonItem(uint32_t item, uint32_t dungeonIndex) {
        return mSaveContext->inventory.dungeonItems[dungeonIndex] & gBitFlags[item];
    }

    void Logic::SetDungeonItem(uint32_t item, uint32_t dungeonIndex, bool state) {
        if (!state) {
            mSaveContext->inventory.dungeonItems[dungeonIndex] &= ~gBitFlags[item];
        }
        else {
            mSaveContext->inventory.dungeonItems[dungeonIndex] |= gBitFlags[item];
        }
    }

    bool Logic::CheckRandoInf(uint32_t flag) {
        return mSaveContext->randomizerInf[flag >> 4] & (1 << (flag & 0xF));
    }

    void Logic::SetRandoInf(uint32_t flag, bool state) {
        if (!state) {
            mSaveContext->randomizerInf[flag >> 4] &= ~(1 << (flag & 0xF));
        }
        else {
            mSaveContext->randomizerInf[flag >> 4] |= (1 << (flag & 0xF));
        }
    }

    bool Logic::CheckEventChkInf(int32_t flag) {
        return mSaveContext->eventChkInf[flag >> 4] & (1 << (flag & 0xF));
    }

    void Logic::SetEventChkInf(int32_t flag, bool state) {
        if (!state) {
            mSaveContext->eventChkInf[flag >> 4] &= ~(1 << (flag & 0xF));
        }
        else {
            mSaveContext->eventChkInf[flag >> 4] |= (1 << (flag & 0xF));
        }
    }

    uint8_t Logic::GetGSCount() {
        return mSaveContext->inventory.gsTokens;
    }

    uint8_t Logic::GetAmmo(uint32_t item) {
        return mSaveContext->inventory.ammo[gItemSlots[item]];
    }

    void Logic::SetAmmo(uint32_t item, uint8_t count) {
        mSaveContext->inventory.ammo[gItemSlots[item]] = count;
    }

    void Logic::SetContext(std::shared_ptr<Context> _ctx) {
        ctx = _ctx;
    }

    bool Logic::GetInLogic(LogicVal logicVal) {
        return inLogic[logicVal];
    }

    void Logic::SetInLogic(LogicVal logicVal, bool value) {
        inLogic[logicVal] = value;
    }

    void Logic::Reset() {
        NewSaveContext();
        StartPerformanceTimer(PT_LOGIC_RESET);
        memset(inLogic, false, sizeof(inLogic));
        //Settings-dependent variables
        IsKeysanity = ctx->GetOption(RSK_KEYSANITY).Is(RO_DUNGEON_ITEM_LOC_ANYWHERE) || 
                    ctx->GetOption(RSK_KEYSANITY).Is(RO_DUNGEON_ITEM_LOC_ANYWHERE) || 
                    ctx->GetOption(RSK_KEYSANITY).Is(RO_DUNGEON_ITEM_LOC_ANYWHERE);
        AmmoCanDrop = /*AmmoDrops.IsNot(AMMODROPS_NONE) TODO: AmmoDrop setting*/ true;

        //Child item logic
        KokiriSword   = false;
        ZeldasLetter  = false;
        WeirdEgg      = false;
        HasBottle     = false;
        MagicBean     = false;
        RutosLetter   = false;
        Boomerang     = false;
        DinsFire      = false;
        FaroresWind   = false;
        NayrusLove    = false;
        LensOfTruth   = false;
        ShardOfAgony  = false;
        SkullMask     = false;
        MaskOfTruth   = false;

        //Adult logic
        Hammer        = false;
        IronBoots     = false;
        HoverBoots    = false;
        MirrorShield  = false;
        GoronTunic    = false;
        ZoraTunic     = false;
        Epona         = false;
        //BigPoe        = false;
        GerudoToken   = false;
        FireArrows    = false;
        IceArrows     = false;
        LightArrows   = false;
        MasterSword   = false;
        BiggoronSword = false;

        //Trade Quest
        PocketEgg     = false;
        Cojiro        = false;
        OddMushroom   = false;
        OddPoultice   = false;
        PoachersSaw   = false;
        BrokenSword   = false;
        Prescription  = false;
        EyeballFrog   = false;
        Eyedrops      = false;
        ClaimCheck    = false;

        //Trade Quest Events
        WakeUpAdultTalon   = false;
        CojiroAccess       = false;
        OddMushroomAccess  = false;
        OddPoulticeAccess  = false;
        PoachersSawAccess  = false;
        BrokenSwordAccess  = false;
        PrescriptionAccess = false;
        EyeballFrogAccess  = false;
        EyedropsAccess     = false;
        DisableTradeRevert = false;

        //Stones and Meddallions
        ForestMedallion = false;
        FireMedallion   = false;
        WaterMedallion  = false;
        SpiritMedallion = false;
        ShadowMedallion = false;
        LightMedallion  = false;
        KokiriEmerald   = false;
        GoronRuby       = false;
        ZoraSapphire    = false;

        //Dungeon Clears
        DekuTreeClear       = false;
        DodongosCavernClear = false;
        JabuJabusBellyClear = false;
        ForestTempleClear   = false;
        FireTempleClear     = false;
        WaterTempleClear    = false;
        SpiritTempleClear   = false;
        ShadowTempleClear   = false;

        //Trial Clears
        ForestTrialClear = false;
        FireTrialClear   = false;
        WaterTrialClear  = false;
        SpiritTrialClear = false;
        ShadowTrialClear = false;
        LightTrialClear  = false;

        //Greg
        Greg = false;
        GregInBridgeLogic = false;
        GregInLacsLogic = false;

        //Ocarina C Buttons
        bool ocBtnShuffle = ctx->GetOption(RSK_SHUFFLE_OCARINA_BUTTONS).Is(true);
        SetRandoInf(RAND_INF_HAS_OCARINA_A, !ocBtnShuffle);
        SetRandoInf(RAND_INF_HAS_OCARINA_C_UP, !ocBtnShuffle);
        SetRandoInf(RAND_INF_HAS_OCARINA_C_DOWN, !ocBtnShuffle);
        SetRandoInf(RAND_INF_HAS_OCARINA_C_LEFT, !ocBtnShuffle);
        SetRandoInf(RAND_INF_HAS_OCARINA_C_RIGHT, !ocBtnShuffle);

        //Progressive Items
        SetUpgrade(UPG_STICKS, ctx->GetOption(RSK_SHUFFLE_DEKU_STICK_BAG).Is(true) ? 0 : 1);
        SetUpgrade(UPG_NUTS, ctx->GetOption(RSK_SHUFFLE_DEKU_NUT_BAG).Is(true) ? 0 : 1);
        ProgressiveBulletBag  = 0;
        ProgressiveBombBag    = 0;
        ProgressiveMagic      = 0;
        //If we're not shuffling swim, we start with it (scale 1)
        ProgressiveScale = 0;
        if (ctx->GetOption(RSK_SHUFFLE_SWIM).Is(false)) {
            ProgressiveScale = 1;
            SetRandoInf(RAND_INF_CAN_SWIM, true);
        }
        ProgressiveHookshot   = 0;
        ProgressiveBow        = 0;
        //If we're not shuffling child's wallet, we start with it (wallet 1)
        ProgressiveWallet = 0;
        if (ctx->GetOption(RSK_SHUFFLE_CHILD_WALLET).Is(false)) {
            ProgressiveWallet = 1;
            SetRandoInf(RAND_INF_HAS_WALLET, true);
        }
        ProgressiveStrength   = 0;
        ProgressiveOcarina    = 0;
        ProgressiveGiantKnife = 0;

        //If not keysanity, start with 1 logical key to account for automatically unlocking the basement door in vanilla FiT
        if (!IsKeysanity && ctx->GetDungeon(Rando::FIRE_TEMPLE)->IsVanilla()) {
            SetSmallKeyCount(SCENE_FIRE_TEMPLE, 1);
        }

        //Boss Keys
        BossKeyForestTemple = 0;
        BossKeyFireTemple   = 0;
        BossKeyWaterTemple  = 0;
        BossKeySpiritTemple = 0;
        BossKeyShadowTemple = 0;
        BossKeyGanonsCastle = 0;

        //Gold Skulltula Count
        GoldSkulltulaTokens = 0;

        //Bottle Count
        Bottles    = 0;
        NumBottles = 0;
        CanEmptyBigPoes = true;

        //Triforce Pieces
        TriforcePieces = 0;

        // Ganon's Key Shards
        GKShards = 0;

        //Boss Souls
        CanSummonGohma        = false;
        CanSummonKingDodongo  = false;
        CanSummonBarinade     = false;
        CanSummonPhantomGanon = false;
        CanSummonVolvagia     = false;
        CanSummonMorpha       = false;
        CanSummonBongoBongo   = false;
        CanSummonTwinrova     = false;
        CanSummonGanon        = false;

        //Drops and Bottle Contents Access
        NutPot           = false;
        NutCrate         = false;
        DekuBabaNuts     = false;
        StickPot         = false;
        DekuBabaSticks   = false;
        BugShrub         = false;
        WanderingBugs    = false;
        BugRock          = false;
        BlueFireAccess   = false;
        FishGroup        = false;
        LoneFish         = false;
        GossipStoneFairy = false;
        BeanPlantFairy   = false;
        ButterflyFairy   = false;
        FairyPot         = false;
        FreeFairies      = false;
        FairyPond        = false;
        BombchuRefill    = false;
        BombchusEnabled  = false;
        BuyBombchus      = false;

        BuySeed          = false;
        BuyArrow         = false;
        BuyBomb          = false;
        BuyMagicPotion   = false;
        BuyFish          = false;
        BuyBugs          = false;
        BuyFairy         = false;

        PieceOfHeart     = 0;
        HeartContainer   = 0;
        DoubleDefense    = false;

        /* --- HELPERS, EVENTS, AND LOCATION ACCESS --- */
        /* These are used to simplify reading the logic, but need to be updated
        /  every time a base value is updated.                       */

        BulletBag        = false;
        Slingshot        = false;
        Ocarina          = false;
        OcarinaOfTime    = false;
        BombBag          = false;
        MagicMeter       = false;
        Hookshot         = false;
        Longshot         = false;
        Quiver           = false;
        Bow              = false;
        GoronBracelet    = false;
        SilverGauntlets  = false;
        GoldenGauntlets  = false;
        Swim             = false;
        SilverScale      = false;
        GoldScale        = false;
        ChildsWallet     = false;
        AdultsWallet     = false;

        ChildScarecrow   = false;
        AdultScarecrow   = false;
        ScarecrowSong    = false;
        Scarecrow        = false;
        DistantScarecrow = false;

        Bombs            = false;
        DekuShield       = false;
        HylianShield     = false;
        BlueFire         = false;
        BottleWithBigPoe = false;

        CouldPlayBowling   = false;
        HasExplosives    = false;
        HasBoots         = false;
        IsChild          = false;
        IsAdult          = false;
        IsGlitched       = ctx->GetOption(RSK_LOGIC_RULES).Is(RO_LOGIC_GLITCHED);
        CanBlastOrSmash  = false;
        CanChildAttack   = false;
        CanChildDamage   = false;
        CanCutShrubs     = false;
        CanDive          = false;
        CanLeaveForest   = false;
        CanPlantBugs     = false;
        CanRideEpona     = false;
        CanStunDeku      = false;
        CanSummonGossipFairy = false;
        CanSummonGossipFairyWithoutSuns = false;
        //CanPlantBean        = false;
        CanOpenBombGrotto     = false;
        CanOpenStormGrotto    = false;
        BigPoeKill            = false;
        HookshotOrBoomerang   = false;
        CanBreakUpperBeehives = false;
        CanBreakLowerBeehives = false;
        CanGetChildFish       = false;
        CanGetAdultFish       = false;
        FishingPole           = false;
        CanFish               = false;

        BaseHearts      = ctx->GetOption(RSK_STARTING_HEARTS).Value<uint8_t>() + 1;
        Hearts          = 0;
        Multiplier      = (ctx->GetOption(RSK_DAMAGE_MULTIPLIER).Value<uint8_t>() < 6) ? ctx->GetOption(RSK_DAMAGE_MULTIPLIER).Value<uint8_t>() : 10;
        EffectiveHealth = 0;
        FireTimer       = 0;
        WaterTimer      = 0;

        GuaranteeTradePath     = false;
        GuaranteeHint          = false;
        HasFireSource          = false;
        HasFireSourceWithTorch = false;

        CanFinishGerudoFortress = false;

        HasShield          = false;
        CanShield          = false;
        ChildShield        = false;
        AdultReflectShield = false;
        AdultShield        = false;
        CanShieldFlick     = false;
        CanJumpslash       = false;
        CanUseProjectile   = false;
        CanUseMagicArrow   = false;

        //Bridge Requirements
        HasAllStones          = false;
        HasAllMedallions      = false;
        CanBuildRainbowBridge = false;
        BuiltRainbowBridge    = false;
        CanTriggerLACS        = false;

        //Other
        AtDay         = false;
        AtNight       = false;
        Age           = ctx->GetSettings()->ResolvedStartingAge();
        GetSaveContext()->linkAge = !Age;

        //Events
        ShowedMidoSwordAndShield  = false;
        CarpenterRescue           = false;
        GF_GateOpen               = false;
        GtG_GateOpen              = false;
        DampesWindmillAccess      = false;
        DrainWell                 = false;
        GoronCityChildFire        = false;
        GCWoodsWarpOpen           = false;
        GCDaruniasDoorOpenChild   = false;
        StopGCRollingGoronAsAdult = false;
        WaterTempleLow            = false;
        WaterTempleMiddle         = false;
        WaterTempleHigh           = false;
        KakarikoVillageGateOpen   = false;
        KingZoraThawed            = false;
        ForestTempleJoelle        = false;
        ForestTempleBeth          = false;
        ForestTempleJoAndBeth     = false;
        ForestTempleAmy           = false;
        ForestTempleMeg           = false;
        ForestTempleAmyAndMeg     = false;
        FireLoopSwitch            = false;
        LinksCow                  = false;
        AtDampeTime               = false;
        DeliverLetter             = false;
        TimeTravel                = false;

        DrainWellPast            = false;
        DampesWindmillAccessPast = false;
        DekuTreeClearPast        = false;
        GoronRubyPast            = false;
        ZoraSapphirePast         = false;
        ForestTrialClearPast     = false;
        FireTrialClearPast       = false;
        WaterTrialClearPast      = false;
        SpiritTrialClearPast     = false;
        ShadowTrialClearPast     = false;
        LightTrialClearPast      = false;
        BuyDekuShieldPast        = false;
        TimeTravelPast           = false;

        StopPerformanceTimer(PT_LOGIC_RESET);
    }
}
