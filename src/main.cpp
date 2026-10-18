#include <fstream>
#include <iostream>

#include "algo/sampling.hpp"
#include "core/mineshaft.hpp"
#include "expedition/expedition.hpp"
#include "expedition/generator.hpp"
#include "io/parser.hpp"
#include "io/printer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::print(std::cerr, "Usage: {} <map_file> <seed>\n", argv[0]);
        return 1;
    }

    try {
        std::ifstream input(argv[1]);
        if (!input) {
            throw std::invalid_argument(argv[1]);
        }

        uint64_t seed = std::stoull(argv[2]);

        expedition::Mineshaft mineshaft = io::parse(input);
        expedition::PassagesData passages_data = expedition::generate_passages_data(mineshaft, seed);

        io::print_world(std::cout, mineshaft, passages_data);

        expedition::Expedition expedition(std::move(mineshaft), std::move(passages_data));
        std::cout << expedition.simulation(algo::get_sampling_algo(seed)) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
