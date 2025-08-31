#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "movement_simulator.hpp"

constexpr float EPS = 1e-5f;

void visualizeComparison(float goal, float result, float lower=-5.0f, float upper=5.0f) {
    constexpr int WIDTH = 60;
    std::string line(WIDTH, '-');

    // Scale values into [0, WIDTH-1]
    auto scale = [&](float val) {
        val = std::clamp(val, lower, upper);
        return static_cast<int>((val - lower) / (upper - lower) * (WIDTH - 1));
    };

    int gpos = scale(goal);
    int rpos = scale(result);

    if (gpos == rpos) {
        // Goal and Result match
        line[gpos] = '*';
    } else {
        // Goal indicator
        line[gpos] = 'G';

        // Result indocator
        line[rpos] = 'R';
    }

    std::cout << "[" << std::setw(6) << lower << "]"
              << line
              << "[" << std::setw(6) << upper << "]\n";
    std::cout << "Goal: " << goal << "  Result: " << result << "\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << "<csv_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Supplied file could not be opened " << argv[1] << std::endl;
        return 1;
    }
    int test_case = 0;
    bool any_failed = false;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        if (row.size() == 0) {
            continue;
        }

        test_case++;
        
        float start        = std::atof(row[0].c_str());
        float scale_factor = std::atof(row[1].c_str());
        
        // Support exactly 5 effort inputs and goal states
        int len = (row.size() - 2) / 2;
        std::vector<float> effort;
        effort.reserve(len);
        for (size_t i = 2; i < len+2; ++i) {
            effort.push_back(std::atof(row[i].c_str()));
        }

        std::vector<float> results;
        results.reserve(len);
        for (size_t i = len+2; i < row.size(); ++i) {
            results.push_back(std::atof(row[i].c_str()));
        }

        MotionBound mb{10.0,-10.0};
        MotionPlan mp{effort, start, scale_factor, mb};
        MotionSimulator ms(mp);
        auto goals = ms.RunMotionPlan();

        bool failed = false;
        for (size_t i = 0; i < goals.size(); ++i) {
            if (std::fabs(goals[i] - results[i]) > EPS) {
                std::cout << "[RESULT MISMATCH]" << std::endl;
                visualizeComparison(goals[i], results[i], mb.lower, mb.upper);
                failed = true;
            }
        }
        if (failed) {
            std::cout << "Test case " << test_case << " failed " << std::endl;
            any_failed = true;
        }
    }
    if (!any_failed) {
        std::cout << "All " << test_case << " test cases passed" << std::endl;
    }

    file.close();
    return 0;
}
