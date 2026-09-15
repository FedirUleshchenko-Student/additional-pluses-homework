#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <ranges>
#include <algorithm>


enum class ItemType { WEAPON, ARMOR, POTION, SCROLL };


struct Item {
    std::string name;
    ItemType type;
    int level;
    double price;
    bool isBroken;
};


class Hero {
private:
    std::string name;
    std::vector<Item> inventory;


public:
    Hero(std::string name, std::vector<Item> items)
        : name(std::move(name)), inventory(std::move(items)) {}


    const std::vector<Item>& getInventory() const { return inventory; }


    // =========================================================================
    // TODO 1: Реалізувати обробник подій для інвентарю.
    // Метод приймає:
    // 1. Predicate (умову): std::function<bool(const Item&)>
    // 2. Action (дію): std::function<void(const Item&)>
    // Пройтися по всіх items в inventory. Якщо predicate(item) == true, викликати action(item).
    // =========================================================================
    void triggerEvents(std::function<bool(const Item&)> predicate,
        std::function<void(const Item&)> action)
    {
        // --- НАПИШІТЬ ВАШ КОД ТУТ ---

        std::for_each(getInventory().begin(), getInventory().end(),
            [pr = predicate, ac = action](auto x) {
                if (pr(x)) ac(x);
            });
        // ----------------------------
    }
};


// =========================================================================
// TODO 5: Реалізувати пошук найкращого предмета за типом.
// Повертає std::optional<Item>.
// Якщо в інвентарі є предмети заданого типу `targetType`, повернути той,
// у якого ціна (price) найвища. Якщо таких предметів немає — повернути std::nullopt.
// =========================================================================
std::optional<Item> findBestItem(const std::vector<Item>& inventory, ItemType targetType) {
    // --- НАПИШІТЬ ВАШ КОД ТУТ ---

    bool flag = false;
    Item t = Item{ "None", ItemType::WEAPON, 0, 0, true };
    std::for_each(inventory.begin(), inventory.end(),
        [targetType, &t, &flag](const Item& x) {
            if (x.type != targetType) return;
            if (t.price < x.price) { t = x; flag = true; }
        });

    if (flag) return t;
    return std::nullopt;
    // ----------------------------
}


int main() {
    std::vector<Item> initialItems = {
        {"Excalibur", ItemType::WEAPON, 10, 500.0, false},
        {"Wooden Shield", ItemType::ARMOR, 2, 40.0, true},
        {"Health Potion", ItemType::POTION, 1, 15.0, false},
        {"Dragon Slayer Sword", ItemType::WEAPON, 8, 350.0, false},
        {"Rusty Dagger", ItemType::WEAPON, 1, 5.0, true},
        {"Mana Potion", ItemType::POTION, 1, 20.0, false},
        {"Iron Plate", ItemType::ARMOR, 6, 200.0, false}
    };


    Hero hero("Arthur", initialItems);


    std::cout << "=== TASK 1 & 2: std::function & Basic Lambda ===" << std::endl;
    // =========================================================================
    // TODO 2: Викликати hero.triggerEvents()
    // 1-ий аргумент (умова): лямбда, яка перевіряє, що предмет це WEAPON і його level > 5.
    // 2-ий аргумент (дія): лямбда, яка виводить у консоль: "[ALERT] High level weapon found: <назва>"
    // =========================================================================


    // --- НАПИШІТЬ ВАШ КОД ТУТ ---
    hero.triggerEvents(
        [](const Item& i) -> bool {
            return i.type == ItemType::WEAPON && i.level > 5;
        },
        [](const Item& i) {
            std::cout << "[ALERT] High level weapon found: <" << i.name << ">" << std::endl;
        });



    std::cout << "\n=== TASK 3: Closures & Mutable Lambda ===" << std::endl;
    // =========================================================================
    // TODO 3: Створити лямбду `sellItem`, яка захоплює змінну `double totalRevenue = 0.0;`
    // за значенням ([totalRevenue]), але використовує `mutable`.
    // Лямбда приймає `double itemPrice`, додає її до внутрішнього totalRevenue
    // та повертає оновлену суму.
    // Симулювати продаж 3 предметів з цінами 50.0, 120.0, 30.0 і вивести результат кожного продажу.
    // =========================================================================


    // --- НАПИШІТЬ ВАШ КОД ТУТ ---

    double totalRevenue = 0.0;
    auto sellItem = [totalRevenue](double itemPrice) mutable {
        return totalRevenue += itemPrice;
        };

    std::vector<Item> itemsForSale = {
        {"Mace", ItemType::WEAPON, 2, 50.0, true},
        {"Scroll of Funny Hat Creation", ItemType::SCROLL, 15, 120.0, true},
        {"Broken Shield", ItemType::ARMOR, 1, 30.0, true}
    };

    std::for_each(itemsForSale.begin(), itemsForSale.end(),
        [sellItem](Item i) mutable { std::cout << "Total selled value = " << sellItem(i.price) << std::endl; });


    std::cout << "\n=== TASK 4: std::ranges & Views (C++20) ===" << std::endl;
    // =========================================================================
    // TODO 4: Використовуючи std::views та оператор |, обробити hero.getInventory():
    // 1. filter: залишити тільки цілі предмети (!isBroken).
    // 2. transform: перетворити предмет у рядок (std::string) формату:
    //    "<Name> (New Price: <price * 1.2>)"
    // 3. take: взяти перші 3 елементи.
    // Вивести результат за допомогою циклу for (auto&& item_info : pipeline)
    // =========================================================================


    // --- НАПИШІТЬ ВАШ КОД ТУТ ---

    for (auto&& item_info :
        std::views::take(
            std::views::transform(std::views::filter(hero.getInventory(), [](Item i) -> bool {return !i.isBroken; }),
                [](Item i) {
                    return i.name + " (New Price : " + std::to_string(int(i.price * 1.2)) + ")"; }), 3)) {
        std::cout << item_info << std::endl;
    }


    std::cout << "\n=== TASK 5: std::optional ===" << std::endl;
    // =========================================================================
    // Тестування TODO 5
    // =========================================================================
    auto bestScroll = findBestItem(hero.getInventory(), ItemType::SCROLL);
    if (bestScroll.has_value()) {
        std::cout << "Best Scroll: " << bestScroll->name << std::endl;
    }
    else {
        std::cout << "No scrolls found in inventory!" << std::endl;
    }


    auto bestWeapon = findBestItem(hero.getInventory(), ItemType::WEAPON);
    std::cout << "Best Weapon: " << bestWeapon.value_or(Item{ "None", ItemType::WEAPON, 0, 0, true }).name << std::endl;


    return 0;
}