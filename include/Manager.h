
#pragma once
#include "Serialization.h"

#define ENABLE_IF_NOT_UNINSTALLED if (isUninstalled) return;

class Manager final : public SaveLoadData, public RE::Actor::ForEachSpellVisitor {

    std::set<FormID> favorites;
    std::map<FormID, unsigned int> hotkey_map;
    std::set<FormID> temp_all_spells;

    bool isUninstalled = false;

    const std::set<unsigned int> allowed_hotkeys = {0,1,2,3,4,5,6,7};

    bool RemoveFavorite(FormID formid);

    int GetHotkey(const RE::InventoryEntryData* a_entry) const;

    [[nodiscard]] bool IsHotkeyValid(int hotkey) const;

    void UpdateHotkeyMap(FormID item_formid, const RE::InventoryEntryData* a_entry);

    void UpdateHotkeyMap(FormID spell_formid, int a_hotkey);

    [[nodiscard]] std::map<unsigned int, FormID> GetInventoryHotkeys() const;

    [[nodiscard]] std::map<FormID, unsigned int> GetMagicHotkeys() const;

    [[nodiscard]] std::map<unsigned int, FormID> GetHotkeysInUse() const;

    [[nodiscard]] FormID HotkeyIsInUse(FormID, int a_hotkey) const;

    static void HotkeySpell(RE::TESForm* form, unsigned int hotkey);

    void ApplyHotkey(FormID formid);

    void SyncHotkeys_Item();

    void SyncHotkeys_Spell();

    void SyncHotkeys();

    static bool IsSpellFavorited(FormID a_spell,const RE::BSTArray<RE::TESForm*>& favs);

    RE::BSContainer::ForEachResult Visit(RE::SpellItem* a_spell) override;

    void CollectPlayerSpells();

public:
    static Manager* GetSingleton() {
        static Manager singleton;
        return &singleton;
    }
    
    const char* GetType() override { return "Manager"; }

    void AddFavorites_Item();
    
    void AddFavorites_Spell();

    void AddFavorites();

    void SyncFavorites_Item();

    void SyncFavorites_Spell();

    void SyncFavorites();

    void FavoriteCheck_Item(FormID formid);

    void FavoriteCheck_Spell(FormID formid);

    void FavoriteCheck_Spell();

    void Uninstall() { isUninstalled = true; };

    void Reset();

    void SendData();

    void ReceiveData();

};