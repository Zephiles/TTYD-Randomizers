#pragma once

#include <cstdint>

namespace ttyd::evt_pouch
{
    extern "C"
    {
        // evt_pouch_get_yoshiname
        // evt_pouch_set_yoshiname
        // evt_pouch_open_mail
        // evt_pouch_receive_mail
        // evt_pouch_majinai_set
        // evt_pouch_get_equipbadgecnt
        // evt_pouch_get_havebadgecnt
        // evt_pouch_get_haveitemcnt
        uint32_t evt_pouch_all_party_recovery();
        // evt_pouch_mario_recovery
        // evt_pouch_party_left
        // evt_pouch_party_join
        // evt_pouch_set_ap
        // evt_pouch_get_max_ap
        // evt_pouch_set_hp
        // evt_pouch_get_hp
        // evt_pouch_check_item
        // evt_pouch_remove_keepitem
        // evt_pouch_add_keepitem
        // evt_pouch_remove_item_index
        // evt_pouch_remove_item
        // evt_pouch_add_item
        // evt_pouch_get_starstone
        // evt_pouch_add_starpiece
        // evt_pouch_get_starpiece
        // evt_pouch_set_supercoin
        // evt_pouch_get_supercoin
        // evt_pouch_set_coin
        // evt_pouch_add_coin
        // evt_pouch_get_coin
    }
} // namespace ttyd::evt_pouch
