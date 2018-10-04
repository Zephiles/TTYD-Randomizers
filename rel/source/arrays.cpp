#include "items.h"
#include "maps.h"

#include <cstdint>

extern "C" {

// Item Rando
uint16_t PossibleItems[] = { StrangeSack, MagicalMapBigger, ShineSprite, DiamondStar, EmeraldStar, GoldStar, RubyStar, SapphireStar, GarnetStar, CrystalStar, StarPiece, GoldBar, GoldBarx3, ThunderBolt, ThunderRage, ShootingStar, IceStorm, FireFlower, EarthQuake, BoosSheet, VoltShroom, RepelCape, RuinPowder, SleepySheep, POWBlock, Stopwatch, DizzyDial, PowerPunch, CourageShell, HPDrainItem, TradeOff, MiniMrMini, MrSoftener, Mushroom, SuperShroom, UltraShroom, LifeShroom, DriedShroom, TastyTonic, HoneySyrup, MapleSyrup, JamminJelly, SlowShroom, GradualSyrup, HotDog, Cake, PointSwap, FrightMask, Mystery, InnCoupon, WhackaBump, Coconut, DriedBouquet, MysticEgg, GoldenLeaf, KeelMango, FreshPasta, CakeMix, HotSauce, TurtleyLeaf, Horsetail, PeachyPeach, SpitePouch, KoopaCurse, ShroomFry, ShroomRoast, ShroomSteak, Mistake, HoneyShroom, MapleShroom, JellyShroom, HoneySuper, MapleSuper, JellySuper, HoneyUltra, MapleUltra, JellyUltra, SpicySoup, ZessDinner, ZessSpecial, ZessDeluxe, ZessDynamite, ZessTea, SpaceFood, IciclePop, ZessFrappe, SnowBunny, CoconutBomb, CourageMeal, ShroomCake, ShroomCrepe, MousseCake, FriedEgg, FruitParfait, EggBomb, InkPasta, Spaghetti, ShroomBroth, PoisonShroom, ChocoCake, MangoDelight, LovePudding, MeteorMeal, TrialStew, CouplesCake, InkySauce, Omelettemeal, KoopaTea, Koopasta, SpicyPasta, HeartfulCake, PeachTart, ElectroPop, FirePop, HoneyCandy, CocoCandy, JellyCandy, ZessCookie, HealthySalad, KoopaBun, FreshJuice, PowerJump, Multibounce, PowerBounce, TornadoJump, ShrinkStomp, SleepyStomp, SoftStomp, PowerSmash, QuakeHammer, HammerThrow, PiercingBlow, HeadRattle, FireDrive, IceSmash, DoubleDip, DoubleDipP, Charge, ChargeP, SuperAppeal, SuperAppealP, PowerPlus, PowerPlusP, PUpDDown, PUpDDownP, AllorNothing, AllorNothingP, MegaRush, MegaRushP, PowerRush, PowerRushP, PDownDUp, PDownDUpP, LastStand, LastStandP, DefendPlus, DefendPlusP, DamageDodge, DamageDodgeP, HPPlus, HPPlusP, FPPlus, FlowerSaver, FlowerSaverP, IcePower, SpikeShield, FeelingFine, FeelingFineP, ZapTap, DoublePain, Jumpman, Hammerman, ReturnPostage, HappyHeart, HappyHeartP, HappyFlower, HPDrainBadge, HPDrainBadgeP, FPDrain, FPDrainP, CloseCall, CloseCallP, PrettyLucky, PrettyLuckyP, LuckyDay, LuckyDayP, Refund, PityFlower, PityFlowerP, QuickChange, Peekaboo, TimingTutor, HeartFinder, FlowerFinder, MoneyMoney, AttackFXR, AttackFXB, AttackFXG, AttackFXY, AttackFXP, ChillOut, FirstAttack, BumpAttack, SlowGo, Simplifier, Unsimplifier, LuckyStart, LEmblem, WEmblem, TripleDip, LuckyStartPLuckyStartIcon, DebugBadge, UpgradedPowerJump, UpgradedPowerSmash, UpgradedQuakeHammer, InvalidItemBadgeNoKnownEffect, InvalidItemBadgePNoKnownEffect, SuperCharge, SuperChargeP };
uint16_t ItemArraySize = sizeof(PossibleItems) / sizeof(PossibleItems[0]);

// LZ Rando
uint32_t PossibleLZMaps[] = { aji_00, aji_01, aji_02, aji_03, aji_04, aji_05, aji_06, aji_07, aji_08, aji_09, aji_10, aji_11, aji_12, aji_13, aji_14, aji_15, aji_16, aji_17, aji_18, aji_19, bom_00, bom_01, bom_02, bom_03, bom_04, dou_00, dou_01, dou_02, dou_03, dou_04, dou_05, dou_06, dou_07, dou_08, dou_09, dou_10, dou_11, dou_12, dou_13, eki_00, eki_01, eki_02, eki_03, eki_04, eki_05, eki_06, gon_00, gon_01, gon_02, gon_03, gon_04, gon_05, gon_06, gon_07, gon_08, gon_09, gon_10, gon_11, gon_12, gon_13, gor_00, gor_01, gor_02, gor_03, gor_04, gra_00, gra_01, gra_02, gra_03, gra_04, gra_05, gra_06, hei_00, hei_01, hei_02, hei_03, hei_04, hei_05, hei_06, hei_07, hei_08, hei_09, hei_10, hei_11, hei_12, hei_13, hom_00, jin_00, jin_01, jin_02, jin_03, jin_04, jin_05, jin_06, jin_07, jin_08, jin_09, jin_10, jin_11, jon_00, las_00, las_01, las_02, las_03, las_04, las_05, las_06, las_07, las_08, las_09, las_10, las_11, las_12, las_13, las_14, las_15, las_16, las_17, las_18, las_19, las_20, las_21, las_22, las_23, las_24, las_25, las_26, las_27, las_28, las_29, las_30, moo_00, moo_01, moo_02, moo_05, moo_06, moo_07, mri_00, mri_01, mri_02, mri_03, mri_04, mri_05, mri_06, mri_07, mri_08, mri_09, mri_10, mri_11, mri_12, mri_13, mri_14, mri_15, mri_16, mri_17, mri_18, mri_19, mri_20, muj_00, muj_01, muj_02, muj_03, muj_04, muj_05, muj_10, muj_11, muj_12, nok_00, nok_01, pik_00, pik_01, pik_02, pik_03, pik_04, rsh_00_a, rsh_00_b, rsh_00_c, rsh_01_a, rsh_01_b, rsh_01_c, rsh_02_a, rsh_02_b, rsh_02_c, rsh_03_a, rsh_03_b, rsh_03_c, rsh_04_a, rsh_04_b, rsh_04_c, rsh_05_a, rsh_06_a, rsh_07_a, rsh_07_b, rsh_07_c, rsh_08_a, tik_00, tik_01, tik_02, tik_03, tik_04, tik_05, tik_06, tik_07, tik_08, tik_11, tik_12, tik_13, tik_15, tik_16, tik_17, tik_18, tik_19, tik_20, tik_21, tou_01, tou_02, tou_03, tou_04, tou_05, tou_06, tou_07, tou_08, tou_10, tou_12, usu_00, usu_01, win_00, win_01, win_02, win_03, win_04, win_05, win_06 };
uint16_t LZMapArraySize = sizeof(PossibleLZMaps) / sizeof(PossibleLZMaps[0]);

// LZ Challenge Mode
uint32_t PossibleChallengeMaps[] = { aji_02, aji_03, aji_05, aji_07, aji_11, aji_12, aji_13, aji_14, bom_00, bom_01, bom_02, bom_03, bom_04, dou_00, dou_02, dou_04, dou_05, dou_06, dou_07, dou_09, dou_10, eki_00, eki_02, eki_03, eki_04, eki_05, eki_06, gon_01, gon_02, gon_03, gon_04, gon_05, gon_06, gon_07, gon_08, gon_09, gon_11, gon_12, gon_13, gor_00, gor_01, gor_02, gor_03, gor_04, gra_00, gra_01, gra_02, gra_03, gra_04, gra_05, gra_06, hei_00, hei_01, hei_02, hei_04, hei_06, hei_08, hei_10, hei_12, hei_13, hom_00, jin_00, jin_03, jin_04, jin_05, jin_06, jin_07, jin_08, jin_09, jin_10, jin_11, jon_00, las_01, las_02, las_03, las_04, las_06, las_07, las_09, las_11, las_12, las_13, las_14, las_15, las_16, las_17, las_18, las_19, las_20, las_22, las_24, las_25, las_26, las_28, las_29, las_30, moo_01, moo_02, moo_06, moo_07, mri_00, mri_01, mri_02, mri_03, mri_04, mri_05, mri_06, mri_07, mri_09, mri_10, mri_12, mri_13, mri_15, mri_16, mri_17, mri_19, mri_20, muj_00, muj_01, muj_02, muj_03, muj_04, muj_05, muj_12, nok_00, nok_01, pik_00, pik_01, pik_03, pik_04, rsh_00_c, rsh_02_c, rsh_03_c, rsh_04_c, rsh_05_a, rsh_06_a, tik_00, tik_01, tik_02, tik_03, tik_04, tik_05, tik_06, tik_07, tik_11, tik_12, tik_13, tik_15, tik_16, tik_17, tik_18, tik_19, tik_20, tik_21, tou_01, tou_02, tou_03, tou_04, tou_05, tou_06, tou_07, tou_08, tou_12, usu_00, usu_01, win_00, win_01, win_02, win_03, win_05 };
uint16_t ChallengeMapArraySize = sizeof(PossibleChallengeMaps) / sizeof(PossibleChallengeMaps[0]);

// Set up array for GSWFs to turn on upon starting a new file with the Loading Zone randomizer/Challenge Mode
uint16_t GSWF_Array[] = {
  // Turn on GSWF(0) to skip shop tutorials
  0,
  
  // Turn on GSWF(37) and GSWF(38) to prevent emails from being explained for the first time
  37,
  38,
  
  // Turn on GSWF(233) to skip save blocks from being explained for the first time
  233,
  
  // Turn on GSWF(234) to skip recovery blocks from being explained for the first time
  234,

  // Turn on GSWF(235) to skip items from being explained for the first time
  235,
  
  // Turn on GSWF(1187) to set Zess T. to be blocking the west entrance
  1187,
  
  // Turn on GSWF(1189) to have the Contact Lens ordered already
  1189,
  
  // Turn on GSWF(1197) to skip Zess T. explaining that he will cook stuff for the player now
  1197,
  
  // Turn on GSWF(1200) to prevent partners from explaining Save Blocks in central Rogueport
  1200,
  
  // Turn on GSWF(1334) to have the entrances revealed already in tik_03
  1334,
  
  // Turn on GSWF(1353) to skip having to talk to the Plane Mode curse chest for the first time
  1353,
  
  // Turn on GSWF(1492) to skip having to talk to the Paper Mode curse chest for the first time
  1492,
  
  // Turn on GSWF(1781) and GSWF(1782) to skip the Koopie Koo cutscene in Petal Meadows
  1781,
  1782,
  
  // Turn on GSWF(1805) to skip the cutscene of Goombella explaining her tattles on the bridge screen in Petal Meadows
  1805,
  
  // Turn on GSWF(1932) to skip having to talk to the Tube Mode curse chest for the first time
  1932,
  
  // Turn on GSWF(2075) to skip Vivian's textbox in Twilight Trail
  2075,
  
  // Turn on GSWF(2228) to prevent the player from being able to use the Steeple Key
  2228,
  
  // Turn on GSWF(2401) to skip the cutscene of entering Grubba's office through the grate for the first time
  2401,
  
  // Turn on GSWF(2500) to skip the cutscene of re-signing up to be a fighter
  2500,
  
  // Turn on GSWF(2867) to drain the water in the Great Tree
  2867,
  
  // Turn on GSWF(2872) to skip the cutscene on the bottom floor of mri_17
  2872,
  
  // Turn on GSWF(2878) to prevent the player from being able to talk to Jabble
  2878,
  
  // Turn on GSWF(2982), GSWF(2983), and GSWF(2984) to activate the blue switches in Pirate's Grotto
  2982,
  2983,
  2984,
  
  // Turn on GSWF(3131) to skip the cutscene with Four-Eyes after Bobbery is taken by the Embers
  3131,
  
  // Turn on GSWF(3574) to skip the cutscene of the bridge being shown when talking to the conductor at Riverside
  3574,
  
  // Turn on GSWF(3884) to skip the cutscene on the first screen of the area leading to Fahr Outpost
  3884,
  
  // Turn on GSWF(4196) and GSWF(4197) to allow the player to use the crane without needing to insert the Cog
  4196,
  4197,
  
  // Turn on GSWF(4218) to skip the crane game tutorial
  4218,
  
  // Turn on GSWF(5374) to skip the Trouble Center tutorial
  5374
};
uint16_t GSWF_Array_Size = sizeof(GSWF_Array) / sizeof(GSWF_Array[0]);

const char *HelpMenuArray[] = {
  // Page 1
  "This menu displays information about the following:\n\n1. Item Randomizer\n   Pages 2-9\n\n2. Loading Zone Randomizer\n   Pages 10-19\n\n3. Loading Zone Randomizer - 1 Hour Challenge\n   Pages 20-28\n\nPress D-Pad Left/Right to change pages\nPress L + Start to close the menu",
  
  // Page 2
  "The following will be randomized:\n\n1. Items/Badges that spawn in the field\n\n2. Items/Badges from chests and blocks\n\n3. Single Coin Blocks\n\n4. Star Pieces in floor panels\n\n5. Coins from the overworld (only when the Randomize\n   Coins option is On). The coins dropped when\n   running away from a battle are not randomized.",
  
  // Page 3
  "6. Items held by enemies in battles. All enemies will\n   be holding a random item/badge, and one of these\n   will be dropped at random. Neither the Debug\n   Badge nor important items will ever be\n   held/dropped.\n\n7. Items held by the audience. Any item that spawns\n   in the audience will be randomized (even rocks,\n   bones, etc.), although you will still only get one of\n   the items/badges that is thrown.\n\n8. Standard Shop Items (the Contact Lens and\n   Luigi Books will still appear as normal)\n\n9. Shop Rewards (will never be randomized into the\n   Debug Badge nor important items)",
  
  // Page 4
  "10. Hot Dogs bought from Glitzville (will never be\n    randomized into the Debug Badge nor\n    important items)\n\nTo avoid softlocks, the Action Commands and Special\nMoves tutorials are inaccessible\n\nDue to story progression requirements, the door in\nWestside Goods leading to Don Pianta will always be\nunlocked.\n\nDue to story progression requirements, the Coconuts\nfrom the tree at Keelhaul Key will not be randomized\nduring the time that they are needed.",
  
  // Page 5
  "In addition to the regular items and badges, the\nfollowing can be obtained:\n\n1. Strange Sack\n2. Crystal Stars (will not advance the Sequence, but\n   will still give you the special move)\n3. Magical Map (gives you Sweet Treat)\n\nWhen the game is about to spawn an item and the\nitem is an important item/upgrade, it first checks to\nsee if you already have that item, and picks a\ndifferent item if you do\n\nThe SP needed for Sweet Feast has been changed\nfrom 5 to 3",
  
  // Page 6
  "The SP needed for Showstopper has been changed\nfrom 2 to 4\n\nObtaining a Shine Sprite gives you 3 instead of 1\n\nCollecting the crystal star reward at the end of\nchapters (assuming the item is not a crystal star/\nMagical Map) no longer adjusts your SP, nor does\nit give you a special move\n\nThe following unused items/badges have been added:\n\n1. Koopa Curse - Has the chance to apply the Slow\n   status to all enemies for 3 turns. Icon has been\n   changed to the green Koopa skull.",
  
  // Page 7
  "2. Cake - Recovers 10 HP\n\n3. Trade Off - Increases all enemies' levels by 5\n   and is stackable.\n\n4. Debug Badge - Automatically does most action\n   commands and superguards. Costs 1 BP to equip.\n   Icon has been changed to the A button.\n\n5. Mega Jump - Jump attack that costs 4 FP and\n   ((Jump Power x 2) + 4) damage to one enemy.\n   Costs 2 BP to equip.\n\n6. Mega Smash - Hammer attack that costs 4 FP and\n   does (Hammer Power + 4) damage to one enemy.\n   Costs 2 BP to equip.",
  
  // Page 8
  "7. Mega Quake - Hammer attack that costs 7 FP and\n   does 10 damage to all ground enemies. Costs 4 BP\n   to equip\n\n8. Triple Dip - Same as equipping 2 Double Dips.\n   Costs 6 BP to equip.\n\n9. All or Nothing P - All or Nothing for partners\n\n10. FP Drain P - FP Drain for partners\n\n11. Lucky Day P - Lucky Day for partners\n\n12. Lucky Start P - Lucky Start for partners. Uses\n    the same icon as Lucky Start. Costs 2 BP to\n    equip.",
  
  // Page 9
  "13. Pity Flower P - Pity Flower for partners\n\n14. Super Charge - Boosts attack by +4. Costs 2 BP\n    to equip.\n\n15. Super Charge P - Boosts attack by +4. Costs 2 BP\n    to equip.\n\n16. Art Attack Hitboxes Badge - Displays hitboxes\n    around enemies when using Art Attack. Has the\n    icon of the small Ruby Star. Costs 1 BP to equip.\n\n17. Run Meter Badge - Automatically fills the run\n    meter. Has the icon of the OK image displayed\n    when running away. Costs 1 BP to equip.",
  
  // Page 10
  "This randomizes all of the loading zones.\n\nStarting from a new file, you will be placed at\nRogueport Square. You are given the Strange Sack,\nall of the partners, the Yoshi color is random, and\nyou start with Yoshi out. After this, any loading zone\nyou enter will be random. The loading zone chosen\nwhen entering a room will also be random. Along with \nthis, current sequence position, loading zone used,\nand current map will be displayed on-screen.\n\nThe Sequence (Seq) is where the game thinks you\ncurrently are in the story\n\nThe Loading Zone (LZ) is the entrance that Mario\ncame out of upon entering the room",
  
  // Page 11
  "The Map is the name of the current room\n\nWhen entering a room, LZ will be temporarily set to\n'Pending' until a proper loading zone is chosen\n\nIf you end up in either the X-Naut Fortress room\njust before Magnus 2 or the Grotto room with the\nPlane Panel, then you will be able to use the Plane\nMode curse, regardless of whether you have it or not\n\nIf you end up in either east Twilight Town, the Creepy\nSteeple storage room, the Creepy Steeple room prior\nto the Parrot, the room with the Ultra Boots, or the\nX-Naut Fortress room just before Magnus 2, then you\nwill be able to use the Paper and Tube Mode curses,\nregardless of whether you have them or not",
  
  // Page 12
  "If you end up in either the Rogueport Sewers room\nwith the 3 Shine Sprites or the Grotto room with the\nchest that requires Boat Mode to get to, then you\nwill be able to use the Boat Mode curse, regardless\nof whether you have it or not\n\nIf you enter a room through a loading zone that\nwould require you to be in Boat Mode, then Mario will\nautomatically transform into the Boat\n\nWhen loading an existing file, you will be sent to a\nrandom room with a random loading zone instead of\nappearing at the save block that you saved at\n\nThe Pianta Parlor minigames rooms are inaccessible",
  
  // Page 13
  "In the event of a Game Over, you can wait for the\nintro to start playing again to resume gameplay on\nthe current file\n\nSeveral cheat codes have been added to this mode\nto avoid softlocks:\n\n  1. L + Z will force Mario off of Yoshi\n\n  2. The game will never allow Mario to be\n     transformed into Peach nor Bowser\n\nWhen warping to a room that has a boss, the\nSequence will be adjusted to allow you to fight that\nboss, assuming that the Sequence was previously\nbelow or at the necessary value for that boss",
  
  // Page 14
  "The following Bosses are included:\n\n1. Blooper\n2. Gold Fuzzy\n3. Hooktail\n4. Shadow Sirens (Ch2)\n5. Magnus 1\n6. Grubba\n7. Atomic Boo\n8. Doopliss 1\n9. Doopliss 2\n10. Cortez\n11. Crump (Ch5)\n12. Smorg\n13. Magnus 2\n14. Gloomtail",
  
  // Page 15
  "The Escape Timer will never appear in the rooms that\nuse it\n\nThe Sequence will be automatically adjusted to allow\nyou to get the Paper curses, regardless of what value\nit is currently at. The same is true for collecting\nthe Black Keys for them if necessary.\n\nWhen loading into the room with the Ultra Boots, the\nSequence will be automatically adjusted to allow you\nto get them if it's currently above the necessary\nlevel. Additionally, the loading zone will be\nautomatically set if you are currently able to obtain\nthem.",
  
  // Page 16
  "The Cog is automatically inserted upon starting a new\nfile. It will still appear in the overworld, however.\n\nZess T. is now automatically blocking the west\nentrance upon starting a new file. The Contact Lens\nare also automatically ordered. The Sequence needs\nto be past 55 to be able to buy them, however.\n\nFor the US and EU versions of the game, the Power\nRush and Power Rush P icons will be randomized\nupon starting a new file. They will use the icons from\nthe other versions of the game.\n\nIn the event that you enter a battle with no partner\nout, Goombella will automatically be brought out. The\nexception to this is the Doopliss 2 fight.",
  
  // Page 17
  "When going to fight Doopliss 2, Vivian is forced out\nboth before and after the fight. This is to prevent\nthe game from crashing.\n\nWhen warping to Doopliss's room, whether you fight\nhim the first or second time is based on the Sequence.\nIf the Sequence is before or at the first fight, then\none of the fights will be chosen at random. If the\nSequence is past the first fight but before or at the\nsecond fight, then the second fight will be chosen.\n\nWhen warping to Glitzville, the Sequence will be\nautomatically adjusted for you to be able to get the\nSuper Hammer if you don't have it already. This\nhappens even if you have the Ultra Hammer already.",
  
  // Page 18
  "When warping into either the Glitz Pit Arena or one\nof the locker rooms, any current matches are\nautomatically cleared. This is to prevent softlocks\nas well as a match trying to start at the same time\nas the cutscene for fighting Grubba is playing.\n\nYou will be warped to the credits after defeating\nthe Shadow Queen.\n\nIf you warp into the X-Naut Fortress room right\nbefore Magnus 2, then you can exit via the west\nentrance by walking/jumping off of the platform with\nthe pipe that is right next to the conveyor belt.\nThis will put you onto the conveyor belt. You can\nthen use Paper Mode to go through the bars.",
  
  // Page 19
  "When obtaining a follower, it will be randomized into\none of the following:\n\n1. Yoshi Egg\n2. Flavio\n3. Punio\n4. Frankly\n5. Gus",
  
  // Page 20
  "The 1 hour challenge is a challenge mode in which\nyou play the game for 1 hour to try to gain as many\npoints as possible. If any of the settings in this\nconflict with the settings of the standard Loading\nZone randomizer, then these settings will take\npriority.\n\nThe points given are as follows:\n\n1. 2 points are given for collecting a crystal star/\n   Magical Map\n\n2. 6 points are given for collecting a curse",
  
  // Page 21
  "3. 1 point is given for collecting an important item.\n   The exception to this is the Battle Trunks,\n   which give 1 point per 10 of them.\n\n4. 2 points are given for having a follower. These\n   points are lost if you lose the follower.\n\n5. 2 points are given for each level up\n\n6. 10 points are given for defeating a boss, with the\n   following exceptions:\n\n   i.  25 points are given for the Shadow Queen, and\n      30 points are given for Bonetail\n   ii. 8 points are given for Grodus and Bowser &\n      Kammy",
  
  // Page 22
  "7. 1 point is given for every 100 coins that you\n   have. An additional point is given for having\n   exactly 999 coins.\n\n8. 1 point is given for every 10 badges in the badge\n   log. This also includes badges that normally do not\n   show up in the log, such as Triple Dip.\n\n9. 5 points are given for every partner upgrade\n\n10. 5 points are lost for every Game Over that occurs",
  
  // Page 23
  "Several cheat codes have been added to this mode:\n\n1. Pressing the following changes the loading zone:\n\n  i.  L + D-Pad Up - changes it to North\n  ii.  L + D-Pad Down - changes it to South\n  iii. L + D-Pad Left - changes it to West\n  iv. L + D-Pad Right - changes it to East\n\n2. L + B reloads the room that you're currently in.\n   There are some instances where you will not be\n   able to reload the room, such as when fighting/\n   about to fight a boss, or being in a Pit room.\n   Reloading the room will also cause the items in the\n   room to respawn.",
  
  // Page 24
  "3. You are able to walk and jump on water. Spin\n   Jumping and going onto water with Yoshi will\n   still cause you to fall into it normally.\n\nIf you manually change the loading zone via the\ncheat, then the loading zone will no longer be\nrandomized until you leave the current room. The LZ\nwill also no longer say 'Pending' if you reload the room.\n\nWhen using this mode, Mario will no longer transform\ninto the Boat, as being able to walk on water makes\nit unnecessary",
  
  // Page 25
  "The current Score and Time are displayed on-screen.\nWhen the timer reaches 0, the game will state that\nthe time is up, and will give the options to either\ncontinue playing or to return to the title screen. The\nFinal Score will also be displayed, as well as where\neach of the points came from. The order of the\npoints are the same as the list prior. If you choose\nto continue playing, then the timer will reset to 1\nhour, but the Final Score will remain displayed. Both\nthe Score and the sources for the Score will still\nupdate, but the Final Score will remain unchanged.\n\nL + R can be pressed at anytime during gameplay to\ntoggle the display for where the points came from\n\nMost cutscenes are skipped or shortened",
  
  // Page 26
  "Many maps from the standard Loading Zone\nrandomizer are excluded\n\nThe Debug Badge will never spawn\n\nCrystal stars and/or the Magical Map cannot be\nobtained within the first 5 minutes\n\nThe Pit cannot be accessed within the first 10 minutes\n\nBosses cannot be accessed within the first 20 minutes\n\nSaying No to the Shadow Queen will warp you to a\ndifferent room instead of resulting in a Game Over.\nHowever, doing so will set the Sequence to 405.",
  
  // Page 27
  "Defeating the Shadow Queen will warp you to a\ndifferent room\n\nWhen a boss that does not require a specific\nSequence (such as the Atomic Boo) is defeated, you\nmust go to a different room in order for them to\nrespawn. Warping to the same room will not respawn\nthem; you must warp to a different room first.",
  
  // Page 28
  "While on the File Select Screen, L + Y can be pressed\nto toggle a menu that displays the Final Scores from\nyour previous challenge runs. This menu can only be\nopened while using the Challenge Mode. The menu\ndisplays the scores in the order that they occured,\nso the first one on the list is the first run\ncompleted, etc. If you got a Game Over and then\nproceeded to start a new run, then that entry will\nbe marked with Reset instead of an actual score. In\nthe event that the scores exceed the total amount\non the page (currently supports up to 64 scores), then\nD-Pad Left/Right can be used to navigate to the\nother pages. It is important to note that the scores\nwill be cleared upon resetting the game, as resetting\nclears all of the game's memory."  
};
uint8_t HelpMenuArraySize = sizeof(HelpMenuArray) / sizeof(HelpMenuArray[0]);

}