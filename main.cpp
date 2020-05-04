
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace space {

//! \brief A part of a spaceship
//!
//! \tparam Tag A tag to differentiate between different part types
template <typename Tag> class SpaceshipPart {
public:
    explicit SpaceshipPart(std::string description)
        : description_{std::move(description)} {
    }

    [[nodiscard]] const std::string& description() const {
        return description_;
    }

private:
    std::string description_;
};

template <typename PartTag>
std::ostream& operator<<(std::ostream& out,
                         const SpaceshipPart<PartTag>& part) {
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

using Engine = SpaceshipPart<tags::Engine>;
using Fuselage = SpaceshipPart<tags::Fuselage>;
using Cabin = SpaceshipPart<tags::Cabin>;
using Armor = SpaceshipPart<tags::Armor>;
using SmallWing = SpaceshipPart<tags::SmallWing>;
using LargeWing = SpaceshipPart<tags::LargeWing>;
using Weapon = SpaceshipPart<tags::Weapon>;

//! \brief A warehouse holding some SpaceshipParts
//!
class Warehouse {
public:
    Warehouse& add(std::string description);
    [[nodiscard]] std::string description();

    [[nodiscard]] const std::vector<Engine>& engines() const;
    [[nodiscard]] std::vector<Engine>& engines();

    [[nodiscard]] const std::vector<Fuselage>& fuselages() const;
    [[nodiscard]] std::vector<Fuselage>& fuselages();

    [[nodiscard]] const std::vector<Cabin>& cabins() const;
    [[nodiscard]] std::vector<Cabin>& cabins();

    [[nodiscard]] const std::vector<Armor>& armors() const;
    [[nodiscard]] std::vector<Armor>& armors();

    [[nodiscard]] const std::vector<SmallWing>& smallWings() const;
    [[nodiscard]] std::vector<SmallWing>& smallWings();

    [[nodiscard]] const std::vector<LargeWing>& largeWings() const;
    [[nodiscard]] std::vector<LargeWing>& largeWings();

    [[nodiscard]] const std::vector<Weapon>& weapons() const;
    [[nodiscard]] std::vector<Weapon>& weapons();

private:
    std::vector<Engine> engines_;
    std::vector<Fuselage> fuselage_;
    std::vector<Cabin> cabin_;
    std::vector<Armor> armor_;
    std::vector<SmallWing> small_wings_;
    std::vector<LargeWing> large_wings_;
    std::vector<Weapon> weapon_;
};

//! \brief A spaceship with a given set of SpaceshipParts
//!
class Spaceship {
public:
    constexpr static int max_weapon_count = 4;

    //! \brief Construct a new Spaceship using the given parts
    //!
    Spaceship(Engine engine, Fuselage fuselage, Cabin cabin, Armor armor,
              std::optional<SmallWing> small_wings,
              std::optional<LargeWing> large_wings,
              std::array<std::optional<Weapon>, max_weapon_count> weapons)
        : engine_{std::move(engine)},
          fuselage_{std::move(fuselage)},
          cabin_{std::move(cabin)},
          armor_{std::move(armor)},
          small_wings_{std::move(small_wings)},
          large_wings_{std::move(large_wings)},
          weapons_{std::move(weapons)} {
    }

    //! \brief Builds a new spaceship by taking random parts from the warehouse
    //!
    //! All the parts taken to build the spaceship will be removed from the
    //! warehouse
    //!
    //! \param warehouse The warehouse to take parts from
    //! \return std::optional<Spaceship> The built spaceship, or std::nullopt if
    //! the required parts cannot be found in the warehouse
    [[nodiscard]] static std::optional<Spaceship> Build(Warehouse& warehouse);

    [[nodiscard]] std::string description();

    [[nodiscard]] int weaponCount() const;

private:
    Engine engine_;
    Fuselage fuselage_;
    Cabin cabin_;
    Armor armor_;
    std::optional<SmallWing> small_wings_;
    std::optional<LargeWing> large_wings_;
    std::array<std::optional<Weapon>, max_weapon_count> weapons_;
};

} // namespace space

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

    std::cout << "Loading parts from " << parts_file << '\n';

    space::Warehouse warehouse;

    if (std::ifstream file{parts_file}; file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            warehouse.add(line);
        }
    } else {
        std::cerr << "Failed to open file " << parts_file << std::endl;
        std::exit(-1);
    }

    while (true) {
        std::cout << "Trying to build a spaceship using the following "
                     "available parts:\n";
        std::cout << warehouse.description() << '\n';
        auto space_ship = space::Spaceship::Build(warehouse);
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

namespace space {

const std::vector<Engine>& Warehouse::engines() const {
    return engines_;
}

std::vector<Engine>& Warehouse::engines() {
    return engines_;
}

const std::vector<Fuselage>& Warehouse::fuselages() const {
    return fuselage_;
}

std::vector<Fuselage>& Warehouse::fuselages() {
    return fuselage_;
}

const std::vector<Cabin>& Warehouse::cabins() const {
    return cabin_;
}

std::vector<Cabin>& Warehouse::cabins() {
    return cabin_;
}

const std::vector<Armor>& Warehouse::armors() const {
    return armor_;
}

std::vector<Armor>& Warehouse::armors() {
    return armor_;
}

const std::vector<SmallWing>& Warehouse::smallWings() const {
    return small_wings_;
}

std::vector<SmallWing>& Warehouse::smallWings() {
    return small_wings_;
}

const std::vector<LargeWing>& Warehouse::largeWings() const {
    return large_wings_;
}

std::vector<LargeWing>& Warehouse::largeWings() {
    return large_wings_;
}

const std::vector<Weapon>& Warehouse::weapons() const {
    return weapon_;
}

std::vector<Weapon>& Warehouse::weapons() {
    return weapon_;
}

Warehouse& Warehouse::add(std::string description) {
    if (description.ends_with("engine")) {
        engines_.emplace_back(std::move(description));
    } else if (description.ends_with("fuselage")) {
        fuselage_.emplace_back(std::move(description));
    } else if (description.ends_with("cabin")) {
        cabin_.emplace_back(std::move(description));
    } else if (description.ends_with("armor")) {
        armor_.emplace_back(std::move(description));
    } else if (description.ends_with("wings")) {
        if (description.starts_with("small")) {
            small_wings_.emplace_back(std::move(description));
        } else if (description.starts_with("large")) {
            large_wings_.emplace_back(std::move(description));
        } else {
            const auto type = description.substr(0, description.find(' '));
            std::cerr << "The wing type [" << type
                      << "] is not a valid spaceship wing type, skipping\n";
        }
    } else if (description.ends_with("weapon")) {
        weapon_.emplace_back(std::move(description));
    } else {
        const auto type = description.substr(description.rfind(' ') + 1);
        std::cerr << "The part type [" << type
                  << "] is not a valid spaceship part, skipping\n";
    }

    return *this;
}

std::string Warehouse::description() {
    std::stringstream ss;
    auto print_vector = [&ss](const auto& vec, const char* name) {
        ss << name << ":\n";
        for (const auto& part : vec) {
            ss << "\t- " << part.description() << '\n';
        }
    };
    print_vector(engines(), "engines");
    print_vector(fuselages(), "fuselage");
    print_vector(cabins(), "cabin");
    print_vector(armors(), "armor");
    print_vector(smallWings(), "small wings");
    print_vector(largeWings(), "large wings");
    print_vector(weapons(), "weapon");
    return ss.str();
}

std::string Spaceship::description() {
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
    for (size_t i = 0, remaining_weapons = weapon_count; i < weapons_.size();
         i++) {
        const auto& weapon = weapons_[i];
        if (weapon.has_value()) {
            ss << weapon.value();
            if (remaining_weapons > 2) {
                ss << ", ";
            } else if (remaining_weapons == 2) {
                ss << " and ";
            }
            --remaining_weapons;
        }
    }
    return ss.str();
}

int Spaceship::weaponCount() const {
    int count{0};
    for (const auto& weapon : weapons_) {
        if (weapon.has_value()) {
            ++count;
        }
    }
    return count;
}

std::optional<Spaceship> Spaceship::Build(Warehouse& warehouse) {
    auto has_requried_parts = [](Warehouse& warehouse) -> bool {
        return !warehouse.engines().empty() && !warehouse.fuselages().empty() &&
               !warehouse.cabins().empty() && !warehouse.armors().empty();
    };

    if (!has_requried_parts(warehouse)) {
        return std::nullopt;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    auto take_random_part =
        [&gen]<typename PartType>(std::vector<PartType>& parts) -> PartType {
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

} // namespace space
