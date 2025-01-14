#include "../location_access.hpp"
#include "../../entrance.h"
#include "../../dungeon.h"

using namespace Rando;

void RegionTable_Init_DekuTree() {
  /*--------------------------
  |    VANILLA/MQ DECIDER    |
  ---------------------------*/
  areaTable[RR_DEKU_TREE_ENTRYWAY] = Region("Deku Tree Entryway", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_LOBBY,      {[]{return randoCtx->GetDungeon(DEKU_TREE)->IsVanilla();}}),
                  Entrance(RR_DEKU_TREE_MQ_LOBBY,   {[]{return randoCtx->GetDungeon(DEKU_TREE)->IsMQ();}}),
                  Entrance(RR_KF_OUTSIDE_DEKU_TREE, {[]{return true;}}),
  });

  /*--------------------------
  |     VANILLA DUNGEON      |
  ---------------------------*/
  if (randoCtx->GetDungeon(DEKU_TREE)->IsVanilla()) {
  areaTable[RR_DEKU_TREE_LOBBY] = Region("Deku Tree Lobby", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->CanUse(RG_BOOMERANG));}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE));}}),
                }, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MAP_CHEST, true),
                }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_ENTRYWAY,          {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_2F_MIDDLE_ROOM,    {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_COMPASS_ROOM,      {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_LOWER,    {[]{return Here(RR_DEKU_TREE_LOBBY, []{return logic->CanAdultAttack || logic->CanChildAttack || logic->CanUse(RG_NUTS);});}}),
                  Entrance(RR_DEKU_TREE_OUTSIDE_BOSS_ROOM, {[]{return false;}}),
                  Entrance(RR_DEKU_TREE_BOSS_ENTRYWAY,     {[]{return false;}}),
  });

  areaTable[RR_DEKU_TREE_2F_MIDDLE_ROOM] = Region("Deku Tree 2F Middle Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_LOBBY,         {[]{return Here(RR_DEKU_TREE_2F_MIDDLE_ROOM, []{return logic->HasShield || logic->CanUse(RG_MEGATON_HAMMER);});}}),
                  Entrance(RR_DEKU_TREE_SLINGSHOT_ROOM,{[]{return Here(RR_DEKU_TREE_2F_MIDDLE_ROOM, []{return logic->HasShield || logic->CanUse(RG_MEGATON_HAMMER);});}}),
  });

  areaTable[RR_DEKU_TREE_SLINGSHOT_ROOM] = Region("Deku Tree Slingshot Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_SLINGSHOT_CHEST,           true),
                  LOCATION(RC_DEKU_TREE_SLINGSHOT_ROOM_SIDE_CHEST, true),
                }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_2F_MIDDLE_ROOM, {[]{return logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_HOVER_BOOTS);}}),
  });

  areaTable[RR_DEKU_TREE_COMPASS_ROOM] = Region("Deku Tree Compass Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->CanUse(RG_BOOMERANG));}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE));}}),
                }, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_COMPASS_CHEST,           true),
                  LOCATION(RC_DEKU_TREE_COMPASS_ROOM_SIDE_CHEST, true),
                  LOCATION(RC_DEKU_TREE_GS_COMPASS_ROOM,         logic->CanAdultAttack || logic->CanChildAttack),
                }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_LOBBY,     {[]{return logic->HasFireSourceWithTorch || logic->CanUse(RG_FAIRY_BOW);}}),
                  Entrance(RR_DEKU_TREE_BOSS_ENTRYWAY, {[]{return false;}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_LOWER] = Region("Deku Tree Basement Lower", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->Boomerang);}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE));},
                                    /*Glitched*/[]{return logic->CanUse(RG_MEGATON_HAMMER);}}),
                }, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_BASEMENT_CHEST,    true),
                  LOCATION(RC_DEKU_TREE_GS_BASEMENT_GATE,  logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_BOOMERANG) || logic->HasExplosives || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_HOOKSHOT) || logic->CanUse(RG_DINS_FIRE)),
                  LOCATION(RC_DEKU_TREE_GS_BASEMENT_VINES, logic->CanUseProjectile || logic->CanUse(RG_DINS_FIRE) || (randoCtx->GetTrickOption(RT_DEKU_MQ_COMPASS_GS) && logic->CanJumpslash)),
                }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_LOBBY,               {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_SCRUB_ROOM, {[]{return Here(RR_DEKU_TREE_BASEMENT_LOWER, []{return logic->HasFireSourceWithTorch || logic->CanUse(RG_FAIRY_BOW);});}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_UPPER,      {[]{return logic->IsAdult || randoCtx->GetTrickOption(RT_DEKU_B1_SKIP) || HasAccessTo(RR_DEKU_TREE_BASEMENT_UPPER);}}),
                  Entrance(RR_DEKU_TREE_OUTSIDE_BOSS_ROOM,   {[]{return false;}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_SCRUB_ROOM] = Region("Deku Tree Basement Scrub Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_LOWER,            {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_WATER_ROOM_FRONT, {[]{return Here(RR_DEKU_TREE_BASEMENT_SCRUB_ROOM, []{return logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW);});}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_WATER_ROOM_FRONT] = Region("Deku Tree Basement Water Room Front", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_SCRUB_ROOM,      {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_WATER_ROOM_BACK, {[]{return logic->Swim || randoCtx->GetTrickOption(RT_DEKU_B1_BACKFLIP_OVER_SPIKED_LOG);}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_WATER_ROOM_BACK] = Region("Deku Tree Basement Water Room Back", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_WATER_ROOM_FRONT, {[]{return logic->Swim || randoCtx->GetTrickOption(RT_DEKU_B1_BACKFLIP_OVER_SPIKED_LOG);}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_TORCH_ROOM,       {[]{return true;}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_TORCH_ROOM] = Region("Deku Tree Basement Torch Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->CanUse(RG_BOOMERANG));}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE));}}),
                }, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_WATER_ROOM_BACK, {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_BACK_LOBBY,      {[]{return Here(RR_DEKU_TREE_BASEMENT_TORCH_ROOM, []{return logic->HasFireSourceWithTorch || logic->CanUse(RG_FAIRY_BOW);});}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_BACK_LOBBY] = Region("Deku Tree Basement Back Lobby", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->CanUse(RG_BOOMERANG));}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (Here(RR_DEKU_TREE_BASEMENT_BACK_LOBBY, []{return logic->HasFireSourceWithTorch || logic->CanUse(RG_FAIRY_BOW);}) &&
                                                                            (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE)));}}),
                }, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_TORCH_ROOM, {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_BACK_ROOM,  {[]{return Here(RR_DEKU_TREE_BASEMENT_BACK_LOBBY, []{return logic->HasFireSourceWithTorch || logic->CanUse(RG_FAIRY_BOW);}) &&
                                                                     Here(RR_DEKU_TREE_BASEMENT_BACK_LOBBY, []{return logic->CanBlastOrSmash;});}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_UPPER,      {[]{return Here(RR_DEKU_TREE_BASEMENT_BACK_LOBBY, []{return logic->HasFireSourceWithTorch || logic->CanUse(RG_FAIRY_BOW);}) && logic->IsChild;}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_BACK_ROOM] = Region("Deku Tree Basement Back Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_GS_BASEMENT_BACK_ROOM, logic->HookshotOrBoomerang),
                }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_BACK_LOBBY, {[]{return true;}}),
  });

  areaTable[RR_DEKU_TREE_BASEMENT_UPPER] = Region("Deku Tree Basement Upper", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->CanUse(RG_BOOMERANG));}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE));}}),
                }, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_LOWER,      {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BASEMENT_BACK_LOBBY, {[]{return logic->IsChild;}}),
                  Entrance(RR_DEKU_TREE_OUTSIDE_BOSS_ROOM,   {[]{return Here(RR_DEKU_TREE_BASEMENT_UPPER, []{return logic->HasFireSourceWithTorch || (randoCtx->GetTrickOption(RT_DEKU_B1_BOW_WEBS) && logic->IsAdult && logic->CanUse(RG_FAIRY_BOW));});}}),
  });

  areaTable[RR_DEKU_TREE_OUTSIDE_BOSS_ROOM] = Region("Deku Tree Outside Boss Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {}, {
                  //Exits
                  Entrance(RR_DEKU_TREE_BASEMENT_UPPER, {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_BOSS_ENTRYWAY,  {[]{return (logic->Swim || Here(RR_DEKU_TREE_OUTSIDE_BOSS_ROOM, []{return logic->CanUse(RG_IRON_BOOTS);})) && Here(RR_DEKU_TREE_OUTSIDE_BOSS_ROOM, []{return logic->HasShield;});}}),
  });
  }

  /*---------------------------
  |   MASTER QUEST DUNGEON    |
  ---------------------------*/
  if (randoCtx->GetDungeon(DEKU_TREE)->IsMQ()) {
  areaTable[RR_DEKU_TREE_MQ_LOBBY] = Region("Deku Tree MQ Lobby", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {
                  //Events
                  EventAccess(&logic->DekuBabaSticks, {[]{return logic->DekuBabaSticks || (logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) || logic->CanUse(RG_BOOMERANG));}}),
                  EventAccess(&logic->DekuBabaNuts,   {[]{return logic->DekuBabaNuts   || (logic->CanJumpslash || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) || logic->CanUse(RG_MEGATON_HAMMER) || logic->HasExplosives || logic->CanUse(RG_DINS_FIRE));}}),
  }, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MQ_MAP_CHEST,                 true),
                  LOCATION(RC_DEKU_TREE_MQ_SLINGSHOT_CHEST,           logic->CanAdultAttack || logic->CanChildAttack),
                  LOCATION(RC_DEKU_TREE_MQ_SLINGSHOT_ROOM_BACK_CHEST, logic->HasFireSourceWithTorch || (logic->IsAdult && logic->CanUse(RG_FAIRY_BOW))),
                  LOCATION(RC_DEKU_TREE_MQ_BASEMENT_CHEST,            logic->HasFireSourceWithTorch || (logic->IsAdult && logic->CanUse(RG_FAIRY_BOW))),
                  LOCATION(RC_DEKU_TREE_MQ_GS_LOBBY,                  logic->CanAdultAttack || logic->CanChildAttack),
  }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_ENTRYWAY,                     {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_MQ_COMPASS_ROOM,              {[]{return Here(RR_DEKU_TREE_MQ_LOBBY, []{return (logic->IsChild && logic->CanUse(RG_FAIRY_SLINGSHOT)) || (logic->IsAdult && logic->CanUse(RG_FAIRY_BOW));}) &&
                                                                               Here(RR_DEKU_TREE_MQ_LOBBY, []{return logic->HasFireSourceWithTorch || (logic->IsAdult && logic->CanUse(RG_FAIRY_BOW));});}}),
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_FRONT, {[]{return Here(RR_DEKU_TREE_MQ_LOBBY, []{return (logic->IsChild && logic->CanUse(RG_FAIRY_SLINGSHOT)) || (logic->IsAdult && logic->CanUse(RG_FAIRY_BOW));}) &&
                                                                               Here(RR_DEKU_TREE_MQ_LOBBY, []{return logic->HasFireSourceWithTorch;});}}),
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_LEDGE,            {[]{return randoCtx->GetTrickOption(RT_DEKU_B1_SKIP) || Here(RR_DEKU_TREE_MQ_LOBBY, []{return logic->IsAdult;});}}),
  });

  areaTable[RR_DEKU_TREE_MQ_COMPASS_ROOM] = Region("Deku Tree MQ Compass Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MQ_COMPASS_CHEST,   true),
                  LOCATION(RC_DEKU_TREE_MQ_GS_COMPASS_ROOM, logic->HookshotOrBoomerang &&
                                                                               Here(RR_DEKU_TREE_MQ_COMPASS_ROOM, []{return logic->CanUse(RG_BOMBCHU_5) ||
                                                                               (logic->Bombs && (logic->CanUse(RG_SONG_OF_TIME) || logic->IsAdult)) ||
                                                                               (logic->IsAdult && logic->CanUse(RG_MEGATON_HAMMER) && (logic->CanUse(RG_SONG_OF_TIME) || randoCtx->GetTrickOption(RT_DEKU_MQ_COMPASS_GS)));})),
  }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_MQ_LOBBY, {[]{return true;}}),
  });

  areaTable[RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_FRONT] = Region("Deku Tree MQ Basement Water Room Front", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MQ_BEFORE_SPINNING_LOG_CHEST, true),
  }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_BACK, {[]{return randoCtx->GetTrickOption(RT_DEKU_MQ_LOG) ||  (logic->IsChild && (logic->DekuShield || logic->HylianShield)) ||
                                                                             (logic->IsAdult && (logic->CanUse(RG_LONGSHOT) || (logic->CanUse(RG_HOOKSHOT) && logic->CanUse(RG_IRON_BOOTS))));}}),
                  Entrance(RR_DEKU_TREE_MQ_LOBBY,                    {[]{return true;}}),
  });

  areaTable[RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_BACK] = Region("Deku Tree MQ Basement Water Room Back", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MQ_AFTER_SPINNING_LOG_CHEST, logic->CanUse(RG_SONG_OF_TIME)),
  }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_BACK_ROOM,        {[]{return Here(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_BACK, []{return (logic->IsChild && logic->CanUse(RG_STICKS)) || logic->CanUse(RG_DINS_FIRE) ||
                                                                               Here(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_FRONT, []{return logic->IsAdult && logic->CanUse(RG_FIRE_ARROWS);});}) &&
                                                                                 Here(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_BACK, []{return logic->CanUse(RG_KOKIRI_SWORD) || logic->CanUse(RG_MASTER_SWORD) || logic->CanUse(RG_BIGGORON_SWORD) ||
                                                                                  logic->CanUseProjectile || (logic->CanUse(RG_NUTS) && logic->CanUse(RG_STICKS));});}}),
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_FRONT, {[]{return true;}}),
  });

  areaTable[RR_DEKU_TREE_MQ_BASEMENT_BACK_ROOM] = Region("Deku Tree MQ Basement Back Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MQ_GS_BASEMENT_GRAVES_ROOM, (logic->IsAdult && logic->CanUse(RG_LONGSHOT)) || (logic->CanUse(RG_SONG_OF_TIME) && logic->HookshotOrBoomerang)),
                  LOCATION(RC_DEKU_TREE_MQ_GS_BASEMENT_BACK_ROOM,   logic->HasFireSourceWithTorch && logic->HookshotOrBoomerang),
  }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_LEDGE,           {[]{return logic->IsChild;}}),
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_WATER_ROOM_BACK, {[]{return (logic->IsChild && logic->CanUse(RG_KOKIRI_SWORD)) || logic->CanUseProjectile || (logic->CanUse(RG_NUTS) && (logic->IsChild && logic->CanUse(RG_STICKS)));}}),
  });

  areaTable[RR_DEKU_TREE_MQ_BASEMENT_LEDGE] = Region("Deku Tree MQ Basement Ledge", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {
                  //Locations
                  LOCATION(RC_DEKU_TREE_MQ_DEKU_SCRUB, logic->CanStunDeku),
  }, {
                  //Exits
                  Entrance(RR_DEKU_TREE_MQ_BASEMENT_BACK_ROOM, {[]{return logic->IsChild;}}),
                  Entrance(RR_DEKU_TREE_MQ_LOBBY,              {[]{return true;}}),
                  Entrance(RR_DEKU_TREE_MQ_OUTSIDE_BOSS_ROOM,
                         { [] { return Here(RR_DEKU_TREE_MQ_BASEMENT_LEDGE, [] { return logic->HasFireSourceWithTorch; }); } }),
  });

    areaTable[RR_DEKU_TREE_MQ_OUTSIDE_BOSS_ROOM] =
            Region("Deku Tree MQ Outside Boss Room", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {},
                 {
                     // Exits
                     Entrance(RR_DEKU_TREE_MQ_BASEMENT_LEDGE, {[]{ return true; }}),
                     Entrance(RR_DEKU_TREE_BOSS_ENTRYWAY,     {[]{return (logic->Swim || Here(RR_DEKU_TREE_MQ_BASEMENT_LEDGE, []{return logic->CanUse(RG_IRON_BOOTS);})) && Here(RR_DEKU_TREE_MQ_BASEMENT_LEDGE, [] { return logic->HasShield; }); } }),
                 });
  }

    /*---------------------------
    |         BOSS ROOM         |
    ---------------------------*/
    areaTable[RR_DEKU_TREE_BOSS_ENTRYWAY] =
        Region("Deku Tree Boss Entryway", "Deku Tree", RA_DEKU_TREE, NO_DAY_NIGHT_CYCLE, {}, {},
             {
                 // Exits
                 Entrance(RR_DEKU_TREE_OUTSIDE_BOSS_ROOM, { [] { return randoCtx->GetDungeon(DEKU_TREE)->IsVanilla(); } }),
                 Entrance(RR_DEKU_TREE_MQ_OUTSIDE_BOSS_ROOM, { [] { return randoCtx->GetDungeon(DEKU_TREE)->IsMQ(); } }),
                 Entrance(RR_DEKU_TREE_BOSS_ROOM, { [] { return true; } }),
             });

    areaTable[RR_DEKU_TREE_BOSS_ROOM] =
        Region("Deku Tree Boss Room", "Deku Tree", RA_NONE, NO_DAY_NIGHT_CYCLE,
             {
                 // Events
                 EventAccess(&logic->DekuTreeClear, { [] {
                                                  return logic->DekuTreeClear || (logic->HasBossSoul(RG_GOHMA_SOUL) && 
                                                         (logic->CanJumpslash && (logic->CanUse(RG_NUTS) || logic->CanUse(RG_FAIRY_SLINGSHOT) || logic->CanUse(RG_FAIRY_BOW) ||
                                                                           logic->HookshotOrBoomerang)));
                                              }}),
             },
             {
                 // Locations
                 LOCATION(RC_QUEEN_GOHMA, logic->DekuTreeClear),
                 LOCATION(RC_DEKU_TREE_QUEEN_GOHMA_HEART, logic->DekuTreeClear),
             },
             {
                 // Exits
                 Entrance(RR_DEKU_TREE_BOSS_ENTRYWAY, { [] { return true; } }),
                 Entrance(RR_KF_OUTSIDE_DEKU_TREE, { [] { return logic->DekuTreeClear; } }),
             });
}
