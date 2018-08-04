#include "mod.h"
#include "items.h"
#include "patch.h"
#include "clearcache.h"

#include <ttyd/system.h>
#include <ttyd/mario_pouch.h>
#include <ttyd/string.h>
#include <ttyd/swdrv.h>
#include <ttyd/seqdrv.h>
#include <ttyd/mariost.h>
#include <ttyd/__mem.h>

#include <cstdio>

extern uint16_t PossibleItems[];
extern uint16_t ItemArraySize;
extern bool LZRandoChallenge;
extern uint32_t TimerCount;
extern char *NextMap;
extern bool HitShineSprite;
extern bool CrystalStarIsInField;
extern bool EmeraldStarAlreadyChanged;
extern uint16_t CrystalStarNewItem;
extern bool MarioRunAwayCoinDrop;
extern bool EnemyHeldItemArrayInUse;
extern bool ItemRandoV2;
extern uint8_t EnemyHeldItemArrayCounter;
extern bool RanAwayFromBattle;
extern uint16_t EnemyHeldItemArray[8][8];
extern uint32_t GSWAddressesStart;
extern uint32_t CrystalStarPointerAddress;
extern bool RandomizeGivenItem;

extern "C" {
  void StartCrystalStarPointerWrite();
  void BranchBackCrystalStarPointerWrite();
  void StartMarioRunAwayCoinDrop();
  void BranchBackMarioRunAwayCoinDrop();
  void StartNewItemsToInventory();
  void BranchBackNewItemsToInventory();
  void StartItemFixNewItemsToInventory();
  void BranchBackItemFixNewItemsToInventory();
  void StartWriteEnemyHeldItems();
  void BranchBackWriteEnemyHeldItems();
  void StartWriteEnemyHeldItemDrop();
  void BranchBackWriteEnemyHeldItemDrop();
  void StartWriteRunAwayFlag();
  void BranchBackWriteRunAwayFlag();
  void StartRandomizeStandardShops();
  void BranchBackRandomizeStandardShops();
  void StartAudienceItem();
  void BranchBackAudienceItem();
  void StartAudienceItemsBadgeFix();
  void BranchBackAudienceItemsBadgeFix();
  void StartAdjustSPForNewCrystalStar();
  void BranchBackAdjustSPForNewCrystalStar();
  void StartRandomizeCoinBlocks();
  void BranchBackRandomizeCoinBlocks();
  void StartDisplayOverworldSPBars();
  void BranchBackDisplayOverworldSPBars();
}

namespace mod {

uint32_t randomizeItem()
{
  uint32_t NewItem;
  bool ConfirmNewItem = false;
  
  while (!ConfirmNewItem)
  {
    NewItem = PossibleItems[ttyd::system::irand(ItemArraySize)];
    
    // Prevent certain things if the challenge mode is in use
    if (LZRandoChallenge)
    {
      // Prevent the Debug Badge from being selected
      if (NewItem == DebugBadge)
      {
        continue;
      }
      
      // Prevent the Magical Map or crystal stars from spawning within the first 5 minutes
      if ((NewItem == MagicalMapBigger) || ((NewItem >= DiamondStar) && (NewItem <= CrystalStar)))
      {
        uint32_t TimerCountCutoff = 198000; // 55 Minutes
        if (TimerCount >= TimerCountCutoff)
        {
          continue;
        }
      }
    }
    
    // If current item is an Important Item, check to see if the player already has it
    if ((NewItem <= CrystalStar) && (NewItem != ShineSprite))
    {
      if (ttyd::mario_pouch::pouchCheckItem(NewItem) > 0) // pouchCheckItem gets the total number of NewItem in the player's inventory
      {
        // Player already has this Important Item, so get a new one
        continue;
      }
    }
    
    // Check the field to make sure no duplicates are placed
    #ifdef TTYD_US
      uint32_t FieldItemsAddress = 0x803DC294;
    #elif defined TTYD_JP
      uint32_t FieldItemsAddress = 0x803D8714;
    #elif defined TTYD_EU
      uint32_t FieldItemsAddress = 0x803E82F4;
    #endif
    
    uint32_t FieldItemsAddressesStart = *reinterpret_cast<uint32_t *>(FieldItemsAddress);
    bool ItemIsAlreadySpawned = false;
    
    for (int i = 0; i < 32; i++)
    {
      uint32_t tempItem = *reinterpret_cast<uint32_t *>(FieldItemsAddressesStart + 0x4);
      if (tempItem != NewItem)
      {
        // Check the next item in the field
        FieldItemsAddressesStart += 0x98;
      }
      else
      {
        // Check if item in the field is actually spawned or not
        uint16_t tempBit = *reinterpret_cast<uint16_t *>(FieldItemsAddressesStart);
        tempBit &= (1 << 0); // Check if 0 bit is active
        
        if (!tempBit)
        {
          // Item in the field is not actually spawned, so check next item
          FieldItemsAddressesStart += 0x98;
        }  
        else
        {
          // Current item is already in the field, so get a new one
          ItemIsAlreadySpawned = true;
          break;
        }
      }
    }
    
    if (!ItemIsAlreadySpawned)
    {
      ConfirmNewItem = true;
    }
  }

  return NewItem;
}

uint32_t randomizeItemWithChecks(uint32_t currentItemId)
{
  // Check if the current item is a Shine Sprite, as it will appear twice in the field, but the first one shouldn't be randomized
  if (currentItemId == ShineSprite)
  {
    // Randomize if currently receiving from Bub
    if (ttyd::string::strncmp(NextMap, "rsh_04", 6) == 0)
    {
      return randomizeItem();
    }
    
    if (!HitShineSprite)
    {
      // First instance of the Shine Sprite from a block, so don't randomize
      HitShineSprite = true;
      return currentItemId;
    }
    else
    {
      // Second instance of the Shine Sprite from a block, so randomize
      HitShineSprite = false;
      return randomizeItem();
    }
  }
  
  // Randomize the Strange Sack from the Pit chest
  if (currentItemId == StrangeSack)
  {
    return randomizeItem();
  }
  
  // Don't change the current item if it's an Important Item, excluding the crystal stars
  if (currentItemId < DiamondStar)
  {
    return currentItemId;
  }
  
  // Check if the current item is a crystal star
  if ((currentItemId >= DiamondStar) && (currentItemId <= CrystalStar))
  {
    // Set necessary pointer for items to properly act like crystal stars
    CrystalStarIsInField = true;
    
    // Check if current item is the Emerald Star, and if the player is currently in Magnus 1.0's room
    if ((currentItemId == EmeraldStar) && (ttyd::string::strcmp(NextMap, "mri_01") == 0))
    {
      if (!EmeraldStarAlreadyChanged)
      {
        // First instance of the Emerald Star, so randomize it
        EmeraldStarAlreadyChanged = true;
        CrystalStarNewItem = randomizeItem();
      }
      else
      {
        // Second instance of the Emerald Star, so don't randomize it
        EmeraldStarAlreadyChanged = false;
      }
      
      return CrystalStarNewItem;
    }
    
    CrystalStarNewItem = randomizeItem();
    return CrystalStarNewItem;
  }
  
  // Don't randomize coins dropped when running away from a battle
  if (MarioRunAwayCoinDrop)
  {
    MarioRunAwayCoinDrop = false;
    return currentItemId;
  }
  
  // Don't randomize Coconuts during the time that they are needed in Chapter 5
  uint32_t SequencePosition = ttyd::swdrv::swByteGet(0);
  if ((currentItemId == Coconut) && (SequencePosition >= 232) && (SequencePosition <= 236))
  {
    return currentItemId;
  }
  
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Battle = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kBattle);
  
  // Prevent changing the item if currently in a battle
  if (NextSeq != Battle)
  {
    // Don't randomize enemy item drop
    // Must also not run while in a battle
    if (EnemyHeldItemArrayInUse && (currentItemId >= GoldBar))
    {
      EnemyHeldItemArrayInUse = false;
      return currentItemId;
    }
    
    // Prevent changing the item if currently using an item (counts as being in the Pause Menu)
    uint32_t SystemLevel = ttyd::mariost::marioStGetSystemLevel();
    if (SystemLevel != 15)
    {
      bool ItemRandoV2Coin = ItemRandoV2 && (currentItemId == Coin);
      if (ItemRandoV2Coin || ((currentItemId >= StarPiece) && (currentItemId <= SuperChargeP)))
      {
        return randomizeItem();
      }
    }
  }
  
  return currentItemId;
}

void *Mod::getRandomItem(const char *itemName, uint32_t itemId, uint32_t itemMode, int32_t wasCollectedExpr, void *itemPickupScript, float itemCoordinateX, float itemCoordinateY, float itemCoordinateZ)
{
  itemId = randomizeItemWithChecks(itemId);
  
  // Call original function
  return mPFN_itemEntry_trampoline(const_cast<char *>(itemName), itemId, itemMode, wasCollectedExpr, reinterpret_cast<uint32_t *>(itemPickupScript), itemCoordinateX, itemCoordinateY, itemCoordinateZ);
}

void manageEnemyHeldItemArray()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t Battle = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kBattle);
  
  if (NextSeq == Battle)
  {
    // Increment EnemyHeldItemArrayCounter upon entering a battle
    if (!EnemyHeldItemArrayInUse)
    {
      EnemyHeldItemArrayInUse = true;
      EnemyHeldItemArrayCounter++;
    }
  }
  else
  {
    EnemyHeldItemArrayInUse = false;
    
    if (RanAwayFromBattle)
    {
      RanAwayFromBattle = false;
      
      // Clear current part of array upon running from a battle
      EnemyHeldItemArrayCounter--;
      ttyd::__mem::memset(&EnemyHeldItemArray[EnemyHeldItemArrayCounter][0], 0, sizeof(uint16_t) * 8);
    }
  }
}

extern "C" {
uint32_t assignEnemyHeldItem(void *OriginalEnemyHeldItemArray, uint32_t ArrayIndex)
{
  uint32_t NewItem = 0;
  
  // Make sure the item isn't an important item or the Debug Badge
  while ((NewItem < GoldBar) || (NewItem == DebugBadge))
  {
    NewItem = randomizeItem();
  }
  
  // Get current slot in the array to use
  uint32_t NewArrayIndex = (ArrayIndex - 0x1C) / 0x4;
  
  // Store NewItem in new array
  EnemyHeldItemArray[EnemyHeldItemArrayCounter - 1][NewArrayIndex] = NewItem;
  
  // Store NewItem in the original array
  *reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(OriginalEnemyHeldItemArray) + ArrayIndex) = NewItem;
  
  return NewItem;
}
}

extern "C" {
uint32_t getEnemyItemDrop()
{
  // Decrement EnemyHeldItemArrayCounter
  EnemyHeldItemArrayCounter--;
  
  uint32_t ItemsInArray = 0;
  
  // Get total number of current held items
  for (int i = 0; i < 8; i++)
  {
    uint16_t tempitem = EnemyHeldItemArray[EnemyHeldItemArrayCounter][i];
    if (tempitem != 0)
    {
      ItemsInArray++;
    }
    else
    {
      break;
    }
  }
  
  // Pick a random item to drop from the array
  uint32_t DropItem = EnemyHeldItemArray[EnemyHeldItemArrayCounter][ttyd::system::irand(ItemsInArray)];
  
  // Clear current part of array
  ttyd::__mem::memset(&EnemyHeldItemArray[EnemyHeldItemArrayCounter][0], 0, sizeof(uint16_t) * 8);
  
  // Prevent dropped item from being randomized
  EnemyHeldItemArrayInUse = true; // This bool will be used before it is reset by manageEnemyHeldItemArray
  
  return DropItem;
}
}

void randomizeShopRewards()
{
  // Randomize shop rewards (leave the number of purchases needed for them alone for now)
  // Set up array with new items to use (10 items in the array)
  uint16_t NewItem[10] = { 0 };
  
  for (int i = 0; i < 10; i++)
  {
    bool GetNewItem = true;
    while (GetNewItem)
    {
      NewItem[i] = randomizeItem();
      if ((NewItem[i] < GoldBar) || (NewItem[i] == DebugBadge))
      {
        // Get a new item if these occur
        continue;
      }
      
      // Make sure NewItem isn't already in the array
      bool FoundItem = false;
      for (int x = 0; x < 10; x++)
      {
        // Prevent index comparing against itself
        if (i == x)
        {
          x++;
        }
        
        if (NewItem[i] == NewItem[x])
        {
          FoundItem = true;
          break;
        }
      }
      
      if (!FoundItem)
      {
        GetNewItem = false;
      }
    }
  }
  
  #ifdef TTYD_US
    uint32_t ShopRewardsArrayStart = 0x8033C950;
  #elif defined TTYD_JP
    uint32_t ShopRewardsArrayStart = 0x8033A610;
  #elif defined TTYD_EU
    uint32_t ShopRewardsArrayStart = 0x80348708;
  #endif
  
  // Replace items in the array with the items in the new array (10 items in the array)
  for (int i = 0; i < 10; i++)
  {
    *reinterpret_cast<uint32_t *>(ShopRewardsArrayStart + 0x4) = NewItem[i];
    
    // Get next item in array
    ShopRewardsArrayStart += 0x8;
  }
}

void setValuesMapChange()
{
  int32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
  int32_t MapChange = static_cast<int32_t>(ttyd::seqdrv::SeqIndex::kMapChange);
  
  // Only set on map change
  if (NextSeq != MapChange)
  {
    return;
  }
  
  // Randomize shop rewards
  randomizeShopRewards();
  
  // Set GSWF(1230), which is the flag that opens the shop door leading to Don Pianta
  uint32_t GSWFAddress = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  *reinterpret_cast<uint32_t *>(GSWFAddress + 0x210) |= (1 << 14); // Turn on the 14 bit
  
  // Reset EnemyHeldItemArray, in the event that it bugs out for some reason
  // Need to figure out why it bugs out in the first place
  ttyd::__mem::memset(EnemyHeldItemArray, 0, sizeof(EnemyHeldItemArray));
  EnemyHeldItemArrayInUse = false;
  EnemyHeldItemArrayCounter = 0;
}

extern "C" {
void randomizeStandardShopItems(int32_t CurrentShopItemIndex, void *ShopItemsArray, uint32_t OffsetToCurrentItem)
{
  uint32_t ShopArray = reinterpret_cast<uint32_t>(ShopItemsArray);
  uint32_t CurrentItem = *reinterpret_cast<uint32_t *>(ShopArray + OffsetToCurrentItem);
  
  // Don't randomize any important items
  if (CurrentItem < GoldBar)
  {
    return;
  }
  
  bool GetNewItem = true;
  while (GetNewItem)
  {
    CurrentItem = randomizeItem();
    if ((CurrentItem < GoldBar) || (CurrentItem == DebugBadge))
    {
      // Get a new item if these occur
      continue;
    }
    
    // Check for duplicates
    if (CurrentShopItemIndex > 0)
    {
      bool FoundDuplicate = false;
      for (int i = 0; i < CurrentShopItemIndex; i++)
      {
        uint32_t tempItem = *reinterpret_cast<uint32_t *>(ShopArray + (i * 0x8));
        if (tempItem == CurrentItem)
        {
          FoundDuplicate = true;
          break;
        }
      }
      
      if (!FoundDuplicate)
      {
        GetNewItem = false;
      }
    }
    else
    {
      GetNewItem = false;
    }
  }
  
  // Load Item price
  #ifdef TTYD_US
    uint32_t itemDataTable = 0x803108A8;
  #elif defined TTYD_JP
    uint32_t itemDataTable = 0x8030EE58;
  #elif defined TTYD_EU
    uint32_t itemDataTable = 0x8031C638;
  #endif
  
  uint32_t CurrentItemAddress = itemDataTable + (CurrentItem * 0x28);
  uint16_t ItemBuyPrice = *reinterpret_cast<uint16_t *>(CurrentItemAddress + 0x14);
  uint16_t ItemSellPrice = *reinterpret_cast<uint16_t *>(CurrentItemAddress + 0x1A);
  uint32_t ItemNewBuyPrice = ItemSellPrice * 2;
  
  // Check if normal price is higher than new price
  if (ItemBuyPrice > ItemNewBuyPrice)
  {
    ItemNewBuyPrice = ItemBuyPrice;
  }
  
  // Make sure the new buy price does not exceed 999 coins
  if (ItemNewBuyPrice > 999)
  {
    ItemNewBuyPrice = 999;
  }
  
  *reinterpret_cast<uint32_t *>(ShopArray + OffsetToCurrentItem) = CurrentItem;
  *reinterpret_cast<uint32_t *>(ShopArray + OffsetToCurrentItem + 0x4) = ItemNewBuyPrice;
}
}

extern "C" {
void adjustSPForNewCrystalStar(uint32_t CurrentItem)
{
  bool Comparisons = (CurrentItem == MagicalMapBigger) || ((CurrentItem >= DiamondStar) && (CurrentItem <= CrystalStar));
  
  // Only run if the current item is the magical map or a crystal star
  if (!Comparisons)
  {
    return;
  }
  
  uint32_t PouchPointer = reinterpret_cast<uint32_t>(ttyd::mario_pouch::pouchGetPtr());
  uint32_t SPBarAddress = PouchPointer + 0x8C;
  uint16_t Multiplier = 0;
  
  if (CurrentItem != MagicalMapBigger)
  {
    // Item is a crystal star
    Multiplier = CurrentItem - DiamondStar + 1;
    *reinterpret_cast<uint16_t *>(SPBarAddress) |= (1 << Multiplier);
  }
  else
  {
    // Item is the Magical Map
    *reinterpret_cast<uint16_t *>(SPBarAddress) |= (1 << 0); // Turn on the 0 bit
  }
  
  // Get new Max SP
  uint16_t NewMaxSP = (Multiplier + 1) * 100;
  uint16_t CurrentMaxSP = *reinterpret_cast<uint16_t *>(PouchPointer + 0x7C);
  
  // Check if new Max SP is higher than current Max SP
  if (NewMaxSP > CurrentMaxSP)
  {
    *reinterpret_cast<uint16_t *>(PouchPointer + 0x7C) = NewMaxSP;
    *reinterpret_cast<uint16_t *>(PouchPointer + 0x7A) = NewMaxSP; // Current SP
  }
  else
  {
    *reinterpret_cast<uint16_t *>(PouchPointer + 0x7A) = CurrentMaxSP; // Current SP
  }
}
}

extern "C" {
uint32_t audienceFixMarioReceivingBadges(uint32_t pouchGetEmptyHaveItemCnt, uint32_t currentCrowdItem)
{
  // Only run if currentCrowdItem is a badge
  if (currentCrowdItem < PowerJump)
  {
    return pouchGetEmptyHaveItemCnt;
  }
  
  // Get the total number of badges the player currently has
  uint32_t BadgeCount = ttyd::mario_pouch::pouchGetHaveBadgeCnt();
  
  if (BadgeCount < 200)
  {
    // There is currently free space for badges, so return a number higher than 0 free spaces
    return 1;
  }
  else
  {
    // There is currently no free space for badges, so return 0 free spaces
    return 0;
  }
}
}

extern "C" {
void *writeCrystalStarPointer(void *CurrentItemLocation)
{
  if (CrystalStarIsInField)
  {
    CrystalStarIsInField = false;
    
    // Get pointer
    uint32_t CrystalStarPointer = *reinterpret_cast<uint32_t *>(CrystalStarPointerAddress);
    CrystalStarPointer = *reinterpret_cast<uint32_t *>(CrystalStarPointer + 0x4);
    
    // Write pointer
    *reinterpret_cast<uint32_t *>(reinterpret_cast<uint32_t>(CurrentItemLocation) + 0x1C) = CrystalStarPointer;
  }
  
  return CurrentItemLocation;
}
}

extern "C" {
uint32_t newItemToInventory(uint32_t currentItem)
{
  // Return CrystalStarNewItem if the current item is a crystal star
  if ((currentItem >= DiamondStar) && (currentItem <= CrystalStar))
  {
    return CrystalStarNewItem;
  }
  else if (currentItem == HotDog)
  {
    // Set flag to prevent swSet from being ran during the pouchGetItem function
    RandomizeGivenItem = true;
    
    // Randomize the hotdog from the Glitzville hotdog seller
    uint32_t NewItem = 0;
    while ((NewItem < GoldBar) || (NewItem == DebugBadge))
    {
      // Make sure the new item is a standard item/badge
      NewItem = randomizeItem();
    }
    
    return NewItem;
  }
  
  return currentItem;
}
}

extern "C" {
uint32_t newItemsPrevent_swSet(uint32_t currentItem)
{
  if (RandomizeGivenItem)
  {
    RandomizeGivenItem = false;
    
    // Return arbitrary value less than 240 (PowerJump)
    return 1;
  }
  else
  {
    // Return original value
    return currentItem;
  }
}
}

extern "C" {
uint32_t getAudienceItem()
{
  uint32_t NewItem = 0;
  
  // Make sure the new item is a standard item/badge
  while ((NewItem < GoldBar) || (NewItem == DebugBadge))
  {
    NewItem = randomizeItem();
  }
  
  return NewItem;
}
}

extern "C" {
bool displayOverworldSPBars(uint32_t CurrentSpecialMoves, int32_t CurrentSPBar)
{
  // Check for how many SP bars to display
  for (int i = 7; i >= CurrentSPBar; i--)
  {
    if (CurrentSpecialMoves & (1 << i))
    {
      return true;
    }
  }
  
  return false;
}
}

void Mod::writeItemRandoAssemblyPatches()
{
  #ifdef TTYD_US
    uint32_t CrystalStarPointer = 0x800ADFBC;
    
    uint32_t RunAwayCoinDrop = 0x800AEAEC;
    
    uint32_t NewItemToInventory = 0x800F0E3C;
    
    uint32_t FixNewItemToInventory = 0x800D52E8;
    
    uint32_t EnemyHeldItems = 0x800F8C18;
    
    uint32_t EnemyHeldItemDrop = 0x8004EC14;
    
    uint32_t DisableEnemyHeldItemArrayInUseFlag = 0x800470A8;
    
    uint32_t RandomizeStandardShops = 0x800EF700;
    
    uint32_t RandomizeAudienceItems = 0x801A5C70;
    
    uint32_t RandomizeAudienceItemsBadgeFix = 0x801A5424;
    
    uint32_t PouchStoreImportantItemAddress = 0x800D552C;
    
    uint32_t CoinBlockAddress = 0x800668E4;
    
    uint32_t EnemyItemCanUseCheck1 = 0x80125D54;
    uint32_t EnemyItemCanUseCheck2 = 0x801A5414;
    uint32_t EnemyItemCanUseCheck3 = 0x801A5484;
    
    uint32_t EnableAppeal = 0x801239EC;
    
    uint32_t EnableAudience1 = 0x801A173C;
    uint32_t EnableAudience2 = 0x801A61B4;
    uint32_t EnableAudience3 = 0x801A6CB8;
    uint32_t EnableAudience4 = 0x801A6B70;
    
    uint32_t PreventUpgradesCrash = 0x800ABCD8;
    
    uint32_t DisplaySPBarsOverworld = 0x8013D5B8;
    uint32_t DisplaySPBars1 = 0x8013D498;
    uint32_t DisplaySPBars2 = 0x80170F24;
    uint32_t DisplaySPBars3 = 0x80170F44;
    uint32_t DisplaySPBars4 = 0x80170F64;
    uint32_t DisplaySPBars5 = 0x80170F84;
    uint32_t DisplaySPBars6 = 0x80170FA4;
    uint32_t DisplaySPBars7 = 0x80170FC4;
    uint32_t DisplaySPBars8 = 0x80170FE4;
    uint32_t DisplaySPBars9 = 0x80171004;
    
    uint32_t KoopaCurseEffectsAddress = 0x8036AC3C;
    
    uint32_t SuperChargeEffectsAddress = 0x80355228;
    
    uint32_t itemDataTable = 0x803108A8;
    uint16_t KoopaCurseIcon = 390;
    uint16_t DebugBadgeIcon = 108;
    
    uint32_t SweetFeastAddress = 0x803559A8;
    uint32_t ShowstopperAddress = 0x80355BE8;
    
    uint32_t ShineSpriteAddress = 0x800D51FC;
  #elif defined TTYD_JP
    uint32_t CrystalStarPointer = 0x800AC284;
    
    uint32_t RunAwayCoinDrop = 0x800ACDB4;
    
    uint32_t NewItemToInventory = 0x800EC180;
    
    uint32_t FixNewItemToInventory = 0x800D10C8;
    
    uint32_t EnemyHeldItems = 0x800F3CC8;
    
    uint32_t EnemyHeldItemDrop = 0x8004DFB4;
    
    uint32_t DisableEnemyHeldItemArrayInUseFlag = 0x800469FC;
    
    uint32_t RandomizeStandardShops = 0x800EAEEC;
    
    uint32_t RandomizeAudienceItems = 0x8019FCC4;
    
    uint32_t RandomizeAudienceItemsBadgeFix = 0x8019F478;
    
    uint32_t PouchStoreImportantItemAddress = 0x800D130C;
    
    uint32_t CoinBlockAddress = 0x800659E0;
    
    uint32_t EnemyItemCanUseCheck1 = 0x80120894;
    uint32_t EnemyItemCanUseCheck2 = 0x8019F468;
    uint32_t EnemyItemCanUseCheck3 = 0x8019F4D8;
    
    uint32_t EnableAppeal = 0x8011E524;
    
    uint32_t EnableAudience1 = 0x8019B9D0;
    uint32_t EnableAudience2 = 0x801A0208;
    uint32_t EnableAudience3 = 0x801A0CEC;
    uint32_t EnableAudience4 = 0x801A0BC4;
    
    uint32_t PreventUpgradesCrash = 0x800AA01C;
    
    uint32_t DisplaySPBarsOverworld = 0x80137FEC;
    uint32_t DisplaySPBars1 = 0x80137ECC;
    uint32_t DisplaySPBars2 = 0x8016BC70;
    uint32_t DisplaySPBars3 = 0x8016BC90;
    uint32_t DisplaySPBars4 = 0x8016BCB0;
    uint32_t DisplaySPBars5 = 0x8016BCD0;
    uint32_t DisplaySPBars6 = 0x8016BCF0;
    uint32_t DisplaySPBars7 = 0x8016BD10;
    uint32_t DisplaySPBars8 = 0x8016BD30;
    uint32_t DisplaySPBars9 = 0x8016BD50;
    
    uint32_t KoopaCurseEffectsAddress = 0x803682B4;
    
    uint32_t SuperChargeEffectsAddress = 0x80352B50;
    
    uint32_t itemDataTable = 0x8030EE58;
    uint16_t KoopaCurseIcon = 382;
    uint16_t DebugBadgeIcon = 100;
    
    uint32_t SweetFeastAddress = 0x803532D0;
    uint32_t ShowstopperAddress = 0x80353510;
    
    uint32_t ShineSpriteAddress = 0x800D0FDC;
  #elif defined TTYD_EU
    uint32_t CrystalStarPointer = 0x800AF38C;
    
    uint32_t RunAwayCoinDrop = 0x800AFEBC;
    
    uint32_t NewItemToInventory = 0x800F1CA8;
    
    uint32_t FixNewItemToInventory = 0x800D60F0;
    
    uint32_t EnemyHeldItems = 0x800F9A84;
    
    uint32_t EnemyHeldItemDrop = 0x8004ECE0;
    
    uint32_t DisableEnemyHeldItemArrayInUseFlag = 0x80047190;
    
    uint32_t RandomizeStandardShops = 0x800F056C;
    
    uint32_t RandomizeAudienceItems = 0x801A794C;
    
    uint32_t RandomizeAudienceItemsBadgeFix = 0x801A7100;
    
    uint32_t PouchStoreImportantItemAddress = 0x800D6334;
    
    uint32_t CoinBlockAddress = 0x8006717C;
    
    uint32_t EnemyItemCanUseCheck1 = 0x8012783C;
    uint32_t EnemyItemCanUseCheck2 = 0x801A70F0;
    uint32_t EnemyItemCanUseCheck3 = 0x801A7160;
    
    uint32_t EnableAppeal = 0x8012492C;
    
    uint32_t EnableAudience1 = 0x801A3418;
    uint32_t EnableAudience2 = 0x801A7E90;
    uint32_t EnableAudience3 = 0x801A8994;
    uint32_t EnableAudience4 = 0x801A884C;
    
    uint32_t PreventUpgradesCrash = 0x800AD0A8;
    
    uint32_t DisplaySPBarsOverworld = 0x8013F0A0;
    uint32_t DisplaySPBars1 = 0x8013EF80;
    uint32_t DisplaySPBars2 = 0x801729C4;
    uint32_t DisplaySPBars3 = 0x801729E4;
    uint32_t DisplaySPBars4 = 0x80172A04;
    uint32_t DisplaySPBars5 = 0x80172A24;
    uint32_t DisplaySPBars6 = 0x80172A44;
    uint32_t DisplaySPBars7 = 0x80172A64;
    uint32_t DisplaySPBars8 = 0x80172A84;
    uint32_t DisplaySPBars9 = 0x80172AA4;
    
    uint32_t KoopaCurseEffectsAddress = 0x80376B04;
    
    uint32_t SuperChargeEffectsAddress = 0x803610D8;
    
    uint32_t itemDataTable = 0x8031C638;
    uint16_t KoopaCurseIcon = 390;
    uint16_t DebugBadgeIcon = 108;
    
    uint32_t SweetFeastAddress = 0x80361858;
    uint32_t ShowstopperAddress = 0x80361A98;
    
    uint32_t ShineSpriteAddress = 0x800D6004;
  #endif
  
  // Write Crystal Star pointer
  patch::writeBranch(reinterpret_cast<void *>(CrystalStarPointer), reinterpret_cast<void *>(StartCrystalStarPointerWrite));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackCrystalStarPointerWrite), reinterpret_cast<void *>(CrystalStarPointer + 0x4));
  
  // Write run away coin drop
  patch::writeBranch(reinterpret_cast<void *>(RunAwayCoinDrop), reinterpret_cast<void *>(StartMarioRunAwayCoinDrop));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackMarioRunAwayCoinDrop), reinterpret_cast<void *>(RunAwayCoinDrop + 0x4));
  
  // Write New Item to inventory
  patch::writeBranch(reinterpret_cast<void *>(NewItemToInventory), reinterpret_cast<void *>(StartNewItemsToInventory));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackNewItemsToInventory), reinterpret_cast<void *>(NewItemToInventory + 0x4));
  
  // Prevent New Item from running swSet
  patch::writeBranch(reinterpret_cast<void *>(FixNewItemToInventory), reinterpret_cast<void *>(StartItemFixNewItemsToInventory));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackItemFixNewItemsToInventory), reinterpret_cast<void *>(FixNewItemToInventory + 0x4));
  
  // Write enemy held items
  patch::writeBranch(reinterpret_cast<void *>(EnemyHeldItems), reinterpret_cast<void *>(StartWriteEnemyHeldItems));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackWriteEnemyHeldItems), reinterpret_cast<void *>(EnemyHeldItems + 0x4));
  
  // Write enemy held item drop
  patch::writeBranch(reinterpret_cast<void *>(EnemyHeldItemDrop), reinterpret_cast<void *>(StartWriteEnemyHeldItemDrop));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackWriteEnemyHeldItemDrop), reinterpret_cast<void *>(EnemyHeldItemDrop + 0x4));
  
  // Disable EnemyHeldItemArrayInUse
  patch::writeBranch(reinterpret_cast<void *>(DisableEnemyHeldItemArrayInUseFlag), reinterpret_cast<void *>(StartWriteRunAwayFlag));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackWriteRunAwayFlag), reinterpret_cast<void *>(DisableEnemyHeldItemArrayInUseFlag + 0x4));
  
  // Randomize standard shop items
  patch::writeBranch(reinterpret_cast<void *>(RandomizeStandardShops), reinterpret_cast<void *>(StartRandomizeStandardShops));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackRandomizeStandardShops), reinterpret_cast<void *>(RandomizeStandardShops + 0x4));
  
  // Randomize audience items
  patch::writeBranch(reinterpret_cast<void *>(RandomizeAudienceItems), reinterpret_cast<void *>(StartAudienceItem));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackAudienceItem), reinterpret_cast<void *>(RandomizeAudienceItems + 0x4));
  
  // Fix audience items bug with badges
  patch::writeBranch(reinterpret_cast<void *>(RandomizeAudienceItemsBadgeFix), reinterpret_cast<void *>(StartAudienceItemsBadgeFix));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackAudienceItemsBadgeFix), reinterpret_cast<void *>(RandomizeAudienceItemsBadgeFix + 0x4));
  
  // Adjust SP when getting a new crystal star
  patch::writeBranch(reinterpret_cast<void *>(PouchStoreImportantItemAddress), reinterpret_cast<void *>(StartAdjustSPForNewCrystalStar));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackAdjustSPForNewCrystalStar), reinterpret_cast<void *>(PouchStoreImportantItemAddress + 0x4));
  
  // Randomize single coins from coin blocks
  patch::writeBranch(reinterpret_cast<void *>(CoinBlockAddress), reinterpret_cast<void *>(StartRandomizeCoinBlocks));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackRandomizeCoinBlocks), reinterpret_cast<void *>(CoinBlockAddress + 0x4));
  
  // Properly display SP Bars
  patch::writeBranch(reinterpret_cast<void *>(DisplaySPBarsOverworld), reinterpret_cast<void *>(StartDisplayOverworldSPBars));
  patch::writeBranch(reinterpret_cast<void *>(BranchBackDisplayOverworldSPBars), reinterpret_cast<void *>(DisplaySPBarsOverworld + 0x4));
  
  // Allow enemies to hold items that they can't use
  *reinterpret_cast<uint32_t *>(EnemyItemCanUseCheck1) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(EnemyItemCanUseCheck2) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(EnemyItemCanUseCheck3) = 0x60000000; // nop
  
  // Allow appealing in battles, even if the player has no special moves yet
  *reinterpret_cast<uint32_t *>(EnableAppeal) = 0x60000000; // nop
  
  // Allow the audience to appear, even if the player has no special moves yet
  *reinterpret_cast<uint32_t *>(EnableAudience1) = 0x4800000C; // b 0xC
  *reinterpret_cast<uint32_t *>(EnableAudience2) = 0x48000010; // b 0x10
  *reinterpret_cast<uint32_t *>(EnableAudience3) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(EnableAudience4) = 0x60000000; // nop
  
  // Prevent the game from crashing if the player obtains a hammer or boot upgrade while not in the proper rooms for their cutscenes to take place
  *reinterpret_cast<uint32_t *>(PreventUpgradesCrash) = 0x48000030; // b 0x30
  
  // Display all of the SP bars by default, assuming you have at least one of the special moves
  *reinterpret_cast<uint32_t *>(DisplaySPBars1) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars2) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars3) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars4) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars5) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars6) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars7) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars8) = 0x60000000; // nop
  *reinterpret_cast<uint32_t *>(DisplaySPBars9) = 0x60000000; // nop
  
  // Make Koopa Curse target all enemies rather than just one
  *reinterpret_cast<uint8_t *>(KoopaCurseEffectsAddress + 0x64) = 0x2;
  
  // Make Super Charge and Super Charge P increase attack by 4 instead of 2
  *reinterpret_cast<uint8_t *>(SuperChargeEffectsAddress + 0x9F) = 0x4; // Super Charge
  *reinterpret_cast<uint8_t *>(SuperChargeEffectsAddress + 0xC0 + 0x9F) = 0x4; // Super Charge P
  
  // Change the amount of SP needed for Sweet Feast from 5 to 3
  *reinterpret_cast<uint8_t *>(SweetFeastAddress + 0x12) = 0x3;
  
  // Change the amount of SP needed for Showstopper from 2 to 4
  *reinterpret_cast<uint8_t *>(ShowstopperAddress + 0x12) = 0x4;
  
  // Modify the pouchGetItem function to give 3 Shine Sprites when you collect one
  *reinterpret_cast<uint8_t *>(ShineSpriteAddress + 0x3) = 0x3;
  
  // Change icons for Koopa Curse and Debug Badge
  *reinterpret_cast<uint16_t *>(itemDataTable + (KoopaCurse * 0x28) + 0x20) = KoopaCurseIcon;
  *reinterpret_cast<uint16_t *>(itemDataTable + (DebugBadge * 0x28) + 0x20) = DebugBadgeIcon;
  
  // Change sell price for Trade Off
  *reinterpret_cast<uint16_t *>((itemDataTable + (TradeOff * 0x28)) + 0x1A) = 10;
  
  // Change sell price for Trial Stew
  *reinterpret_cast<uint16_t *>((itemDataTable + (TrialStew * 0x28)) + 0x1A) = 25;
  
  // Change buy and sell prices for Triple Dip
  uint32_t TripleDipAddress = itemDataTable + (TripleDip * 0x28);
  *reinterpret_cast<uint16_t *>(TripleDipAddress + 0x14) = 200; // Buy price
  *reinterpret_cast<uint16_t *>(TripleDipAddress + 0x16) = 140; // Discounted Buy price
  *reinterpret_cast<uint16_t *>(TripleDipAddress + 0x1A) = 100; // Sell price
  
  // Change buy and sell prices for Super Charge
  uint32_t SuperChargeAddress = itemDataTable + (SuperCharge * 0x28);
  *reinterpret_cast<uint16_t *>(SuperChargeAddress + 0x14) = 100; // Buy price
  *reinterpret_cast<uint16_t *>(SuperChargeAddress + 0x16) = 70; // Discounted Buy price
  *reinterpret_cast<uint16_t *>(SuperChargeAddress + 0x1A) = 50; // Sell price
  
  // Change buy and sell prices for Super Charge P
  uint32_t SuperChargePAddress = itemDataTable + (SuperChargeP * 0x28);
  *reinterpret_cast<uint16_t *>(SuperChargePAddress + 0x14) = 150; // Buy price
  *reinterpret_cast<uint16_t *>(SuperChargePAddress + 0x16) = 105; // Discounted Buy price
  *reinterpret_cast<uint16_t *>(SuperChargePAddress + 0x1A) = 75; // Sell price
  
  // Clear the cache for the pouchGetItem address for Shine Sprites, as it gets used during the boot process
  clearcache::clearCache(ShineSpriteAddress, sizeof(uint32_t));
}

void Mod::itemRandoStuff()
{
  manageEnemyHeldItemArray();
  setValuesMapChange();
}

}