#include "Utils.h"

#undef GetObject

namespace Utils {
    std::string DecodeTypeCode(const std::uint32_t typeCode) {
        char buf[4];
        buf[3] = static_cast<char>(typeCode);
        buf[2] = static_cast<char>(typeCode >> 8);
        buf[1] = static_cast<char>(typeCode >> 16);
        buf[0] = static_cast<char>(typeCode >> 24);
        return std::string(buf, buf + 4);
    }

    namespace MsgBoxesNotifs {
        namespace Windows {
            int Po3ErrMsg() {
                MessageBoxA(nullptr, po3_err_msgbox.c_str(), "Error", MB_OK | MB_ICONERROR);
                return 1;
            }
        };
    };

    bool FavoriteItem(const RE::TESForm* a_item, const RE::TESObjectREFR::InventoryItemMap& inv) {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (const auto invChanges = player->GetInventoryChanges()) {
            for (const auto& [item, data] : inv) {
                const auto& [count, entry] = data;
                if (count > 0 && item == a_item) {
                    if (entry->IsFavorited()) {
                        return true;
                    }
                    const auto extralist = entry->extraLists && !entry->extraLists->empty() ? entry->extraLists->front() : nullptr;
                    invChanges->SetFavorite(entry.get(), extralist);
                    return true;
                }
            }
        }
        return false;
    }

    RE::InventoryEntryData* GetSelectedEntryInMenu() {
        if (const auto ui = RE::UI::GetSingleton()) {
            if (const auto menu_c = ui->GetMenu<RE::ContainerMenu>()) {
                if (const auto a_itemList = menu_c->GetRuntimeData().itemList) {
                    if (const auto item = a_itemList->GetSelectedItem()) {
                        return item->data.objDesc;
                    }
                }
            } else if (const auto menu_i = ui->GetMenu<RE::InventoryMenu>()) {
                if (const auto a_itemList = menu_i->GetRuntimeData().itemList) {
                    if (const auto item = a_itemList->GetSelectedItem()) {
                        return item->data.objDesc;
                    }
                }
            } else if (const auto menu_f = ui->GetMenu<RE::FavoritesMenu>()) {
                RE::GFxValue selectedIndex;
                const auto& runtime_data = menu_f->GetRuntimeData();
                if (runtime_data.root.GetMember("selectedIndex", &selectedIndex) && selectedIndex.IsNumber()) {
                    const std::int32_t selected_index = static_cast<std::int32_t>(selectedIndex.GetNumber());
                    const auto& items = runtime_data.favorites;
                    if (selected_index >= 0 && static_cast<uint32_t>(selected_index) < items.size()) {
                        return items[selected_index].entryData;
                    }
                }
            }
        }
        return nullptr;
    }
};