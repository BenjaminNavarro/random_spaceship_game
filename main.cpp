
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

#include <string_view>
#include <sstream>

char partsFileName[128] = "vehicle_parts.txt";

std::vector<std::string> allParts;

template <typename Tag> class Part {
public:
    explicit Part(std::string description)
        : description_{std::move(description)} {
    }

    [[nodiscard]] const std::string& description() const {
        return description_;
    }

private:
    std::string description_;
};

template <typename PartTag>
std::ostream& operator<<(std::ostream& out, const Part<PartTag>& part) {
    out << part.description();
    return out;
}

namespace tags {
class Engine;
class Fuselage;
class Cabin;
class Armor;
class SmallWing;
class LargeWing;
class Weapon;
} // namespace tags

using Engine = Part<tags::Engine>;
using SmallWing = Part<tags::SmallWing>;
using LargeWing = Part<tags::LargeWing>;
using Fuselage = Part<tags::Fuselage>;
using Cabin = Part<tags::Cabin>;
using Armor = Part<tags::Armor>;
using Weapon = Part<tags::Weapon>;

class Warehouse {
public:
    [[nodiscard]] const std::vector<Engine>& engines() const {
        return engines_;
    }

    [[nodiscard]] std::vector<Engine>& engines() {
        return engines_;
    }

    [[nodiscard]] const std::vector<SmallWing>& smallWings() const {
        return small_wings_;
    }

    [[nodiscard]] std::vector<SmallWing>& smallWings() {
        return small_wings_;
    }

    [[nodiscard]] const std::vector<LargeWing>& largeWings() const {
        return large_wings_;
    }

    [[nodiscard]] std::vector<LargeWing>& largeWings() {
        return large_wings_;
    }

    [[nodiscard]] const std::vector<Fuselage>& fuselages() const {
        return fuselage_;
    }

    [[nodiscard]] std::vector<Fuselage>& fuselages() {
        return fuselage_;
    }

    [[nodiscard]] const std::vector<Cabin>& cabins() const {
        return cabin_;
    }

    [[nodiscard]] std::vector<Cabin>& cabins() {
        return cabin_;
    }

    [[nodiscard]] const std::vector<Armor>& armors() const {
        return armor_;
    }

    [[nodiscard]] std::vector<Armor>& armors() {
        return armor_;
    }

    [[nodiscard]] const std::vector<Weapon>& weapons() const {
        return weapon_;
    }

    [[nodiscard]] std::vector<Weapon>& weapons() {
        return weapon_;
    }

    Warehouse& add(std::string description) {
        if (description.ends_with("engine")) {
            engines_.emplace_back(std::move(description));
        } else if (description.ends_with("wings")) {
            if (description.starts_with("small")) {
                small_wings_.emplace_back(std::move(description));
            } else if (description.starts_with("large")) {
                large_wings_.emplace_back(std::move(description));
            } else {
                const auto type = [&description]() -> std::string {
                    if (const auto type_end = description.find(' ');
                        type_end != std::string::npos) {
                        return description.substr(0, type_end);
                    } else {
                        return {};
                    }
                }();
                std::cerr << "The wing type [" << type
                          << "] is not a valid spaceship wing type, skipping\n";
            }
        } else if (description.ends_with("fuselage")) {
            fuselage_.emplace_back(std::move(description));
        } else if (description.ends_with("cabin")) {
            cabin_.emplace_back(std::move(description));
        } else if (description.ends_with("armor")) {
            armor_.emplace_back(std::move(description));
        } else if (description.ends_with("weapon")) {
            weapon_.emplace_back(std::move(description));
        } else {
            const auto type = [&description]() -> std::string {
                if (const auto type_start = description.rfind(' ');
                    type_start != std::string::npos) {
                    return description.substr(type_start + 1);
                } else {
                    return {};
                }
            }();
            std::cerr << "The part type [" << type
                      << "] is not a valid spaceship part, skipping\n";
        }

        return *this;
    }

    [[nodiscard]] std::string description() {
        std::stringstream ss;
        auto print_vector = [&ss](const auto& vec, const char* name) {
            ss << name << ":\n";
            for (const auto& part : vec) {
                ss << "\t- " << part.description() << '\n';
            }
        };
        print_vector(engines(), "engines");
        print_vector(smallWings(), "small wings");
        print_vector(largeWings(), "large wings");
        print_vector(fuselages(), "fuselage");
        print_vector(cabins(), "cabin");
        print_vector(armors(), "armor");
        print_vector(weapons(), "weapon");
        return ss.str();
    }

private:
    std::vector<Engine> engines_;
    std::vector<SmallWing> small_wings_;
    std::vector<LargeWing> large_wings_;
    std::vector<Fuselage> fuselage_;
    std::vector<Cabin> cabin_;
    std::vector<Armor> armor_;
    std::vector<Weapon> weapon_;
};

class Spaceship {
public:
    constexpr static int max_weapon_count = 4;

    //! \brief Builds a new spaceship by taking random parts from the warehouse
    //!
    //! All the parts taken to build the spaceship will be removed from the
    //! warehouse
    //!
    //! \param warehouse The warehouse to take parts from
    //! \return std::optional<Spaceship> The built spaceship, or std::nullopt if
    //! the required parts cannot be found in the warehouse
    [[nodiscard]] static std::optional<Spaceship> Build(Warehouse& warehouse);

    Spaceship(Engine engine, Fuselage fuselage, Cabin cabin, Armor armor,
              std::optional<SmallWing> small_wings = std::nullopt,
              std::optional<LargeWing> large_wings = std::nullopt,
              std::array<std::optional<Weapon>, max_weapon_count> weapons = {})
        : engine_{std::move(engine)},
          fuselage_{std::move(fuselage)},
          cabin_{std::move(cabin)},
          armor_{std::move(armor)},
          small_wings_{std::move(small_wings)},
          large_wings_{std::move(large_wings)},
          weapons_{std::move(weapons)} {
    }

    [[nodiscard]] std::string description() {
        std::stringstream ss;
        ss << "A ship with ";
        ss << engine_ << ", ";
        ss << fuselage_ << ", ";
        ss << cabin_ << ", ";
        if (large_wings_.has_value()) {
            ss << large_wings_.value() << ", ";
        }
        if (small_wings_.has_value()) {
            ss << small_wings_.value() << ", ";
        }
        ss << armor_ << " and ";
        const auto weapon_count = weaponCount();
        if (weapon_count == 0) {
            ss << "no weapons";
        } else if (weapon_count == 1) {
            ss << "1 weapon: ";
        } else {
            ss << weapon_count << " weapons: ";
        }
        for (const auto& weapon : weapons_) {
            if (weapon.has_value()) {
                ss << weapon.value() << ", ";
            }
        }
        return ss.str();
    }

    [[nodiscard]] constexpr int weaponCount() const {
        int count{0};
        for (const auto& weapon : weapons_) {
            if (weapon.has_value()) {
                ++count;
            }
        }
        return count;
    }

private:
    Engine engine_;
    Fuselage fuselage_;
    Cabin cabin_;
    Armor armor_;
    std::optional<SmallWing> small_wings_;
    std::optional<LargeWing> large_wings_;
    std::array<std::optional<Weapon>, max_weapon_count> weapons_;
};

std::optional<Spaceship> Spaceship::Build(Warehouse& warehouse) {
    const auto has_requried_parts = [&warehouse]() -> bool {
        return !warehouse.engines().empty() && !warehouse.fuselages().empty() &&
               !warehouse.cabins().empty() && !warehouse.armors().empty();
    }();

    if (!has_requried_parts) {
        return std::nullopt;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    auto take_random_part = [&gen](auto& parts) {
        auto dist = std::uniform_int_distribution<int>(0, parts.size() - 1);
        const auto part_index = dist(gen);
        const auto part = parts[part_index];
        parts.erase(parts.begin() + part_index);
        return part;
    };

    auto take_random_optional =
        [&gen]<typename PartType>(std::vector<PartType>& parts,
                                  double probability =
                                      1.) -> std::optional<PartType> {
        if (auto skip_part =
                std::uniform_real_distribution<>(0, 1)(gen) > probability;
            skip_part || parts.empty()) {
            return std::nullopt;
        } else {
            auto dist = std::uniform_int_distribution<int>(0, parts.size() - 1);
            const auto part_index = dist(gen);
            const auto part = parts[part_index];
            parts.erase(parts.begin() + part_index);
            return part;
        }
    };

    decltype(Spaceship::weapons_) weapons;
    for (auto& weapon : weapons) {
        weapon = take_random_optional(warehouse.weapons(), 0.25);
    }

    return Spaceship{take_random_part(warehouse.engines()),
                     take_random_part(warehouse.fuselages()),
                     take_random_part(warehouse.cabins()),
                     take_random_part(warehouse.armors()),
                     take_random_optional(warehouse.smallWings(), 0.5),
                     take_random_optional(warehouse.largeWings(), 0.5),
                     std::move(weapons)};
}

int main(int argc, char* argv[]) {
    const auto parts_file = [argc, argv]() -> std::string {
        if (argc == 2) {
            return argv[1];
        } else {
#ifdef PARTS_FILE
            return PARTS_FILE;
#else
            return "vehicle_parts.txt";
#endif
        }
    }();

    std::cout << "Loading parts from: " << parts_file << '\n';

    Warehouse warehouse;

    if (std::ifstream file{parts_file}; file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            warehouse.add(line);
            allParts.push_back(line);
        }
    } else {
        std::cerr << "Failed to open file " << parts_file << std::endl;
        std::exit(-1);
    }

    while (true) {
        std::cout << "Trying to build a spaceship using the following "
                     "available parts:\n";
        std::cout << warehouse.description() << '\n';
        auto space_ship = Spaceship::Build(warehouse);
        if (space_ship.has_value()) {
            std::cout << "We built the following spaceship: ";
            std::cout << space_ship->description() << "\n\n";
        } else {
            std::cout
                << "Sorry, not enough parts to build a spaceship, exiting\n";
            break;
        }
    }
}