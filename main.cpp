
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

#include <string_view>

char partsFileName[128] = "vehicle_parts.txt";

std::vector<std::string> allParts;

template <typename Tag> class Part {
public:
    explicit Part(std::string description)
        : description_{std::move(description)} {
    }

    const std::string& description() const {
        return description_;
    }

private:
    std::string description_;
};

namespace tags {
class Engine;
class Wing;
class Fuselage;
class Cabin;
class Armor;
class Shield;
class Weapon;
} // namespace tags

using Engine = Part<tags::Engine>;
using Wing = Part<tags::Wing>;
using Fuselage = Part<tags::Fuselage>;
using Cabin = Part<tags::Cabin>;
using Armor = Part<tags::Armor>;
using Shield = Part<tags::Shield>;
using Weapon = Part<tags::Weapon>;

class Warehouse {
public:
    const std::vector<Engine>& engines() const {
        return engines_;
    }

    const std::vector<Wing>& wings() const {
        return wings_;
    }

    const std::vector<Fuselage>& fuselages() const {
        return fuselage_;
    }

    const std::vector<Cabin>& cabins() const {
        return cabin_;
    }

    const std::vector<Armor>& armors() const {
        return armor_;
    }

    const std::vector<Shield>& shields() const {
        return shield_;
    }

    const std::vector<Weapon>& weapons() const {
        return weapon_;
    }

    Warehouse& add(std::string description) {
        if (description.ends_with("engine")) {
            engines_.emplace_back(std::move(description));
        } else if (description.ends_with("wings")) {
            wings_.emplace_back(std::move(description));
        } else if (description.ends_with("fuselage")) {
            fuselage_.emplace_back(std::move(description));
        } else if (description.ends_with("cabin")) {
            cabin_.emplace_back(std::move(description));
        } else if (description.ends_with("armor")) {
            armor_.emplace_back(std::move(description));
        } else if (description.ends_with("shield")) {
            shield_.emplace_back(std::move(description));
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
            throw std::out_of_range("The part type " + type +
                                    " is not a valid spaceship part");
        }

        return *this;
    }

    void printContent() {
        auto print_vector = [](const auto& vec, const char* name) {
            if (!vec.empty()) {
                std::cout << name << ":\n";
                for (const auto& part : vec) {
                    std::cout << "\t- " << part.description() << '\n';
                }
            }
        };
        std::cout << "The warehouse currently have these elements in stock:\n";
        print_vector(engines(), "engines");
        print_vector(wings(), "wings");
        print_vector(fuselages(), "fuselage");
        print_vector(cabins(), "cabin");
        print_vector(armors(), "armor");
        print_vector(shields(), "shield");
        print_vector(weapons(), "weapon");
    }

private:
    std::vector<Engine> engines_;
    std::vector<Wing> wings_;
    std::vector<Fuselage> fuselage_;
    std::vector<Cabin> cabin_;
    std::vector<Armor> armor_;
    std::vector<Shield> shield_;
    std::vector<Weapon> weapon_;
};

class Spaceship {
public:
    static void GenerateShip(Spaceship* pOutShip);

    void Print() {
        std::cout << "A ship with ";
        std::cout << _engine << ", ";
        std::cout << _fuselage << ", ";
        std::cout << _cabin << ", ";
        if (!_large_wings.empty())
            std::cout << _large_wings << ", ";
        if (!_small_wings.empty())
            std::cout << _small_wings << ", ";
        std::cout << _armor << ", ";
        std::cout << "weapons: ";
        for (int i = 0; i < 4; ++i)
            if (!_weapons[i].empty())
                std::cout << _weapons[i] << ", ";
    }

private:
    std::string _engine;
    std::string _fuselage;
    std::string _cabin;
    std::string _large_wings; // optional
    std::string _small_wings; // optional
    std::string _armor;
    std::string _weapons[4]; // max weapon count is 4
};

void Spaceship::GenerateShip(Spaceship* pOutShip) {
    std::vector<std::string> engineParts;
    std::vector<std::string> fuselageParts;
    std::vector<std::string> cabinParts;
    std::vector<std::string> wingsParts;
    std::vector<std::string> armorParts;
    std::vector<std::string> weaponParts;

    for (const auto& str : allParts) {
        if (str.rfind("engine") != std::string::npos)
            engineParts.push_back(str);
        else if (str.rfind("fuselage") != std::string::npos)
            fuselageParts.push_back(str);
        else if (str.rfind("cabin") != std::string::npos)
            cabinParts.push_back(str);
        else if (str.rfind("wings") != std::string::npos)
            wingsParts.push_back(str);
        else if (str.rfind("armor") != std::string::npos)
            armorParts.push_back(str);
        else if (str.rfind("weapon") != std::string::npos)
            weaponParts.push_back(str);
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(engineParts.begin(), engineParts.end(), g);
    std::shuffle(fuselageParts.begin(), fuselageParts.end(), g);
    std::shuffle(cabinParts.begin(), cabinParts.end(), g);
    std::shuffle(wingsParts.begin(), wingsParts.end(), g);
    std::shuffle(armorParts.begin(), armorParts.end(), g);
    std::shuffle(weaponParts.begin(), weaponParts.end(), g);

    // select parts:
    pOutShip->_engine = engineParts[0];
    pOutShip->_fuselage = fuselageParts[0];
    pOutShip->_cabin = cabinParts[0];
    pOutShip->_armor = armorParts[0];
    pOutShip->_large_wings = wingsParts[0];
    pOutShip->_weapons[0] = weaponParts[0];
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

    warehouse.printContent();

    Spaceship sp;
    Spaceship::GenerateShip(&sp);
    sp.Print();
}