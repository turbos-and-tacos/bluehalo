#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "movement_simulator.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << '<csv_file>' << std::endl;
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
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        
        int cnt = 0;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
            ++cnt;
        }
        if (row.size() == 0) {
            return 1;
        }
        
        test_case++;
        
        float start = (float)std::atof(row[0].c_str());
        float scale = (float)std::atof(row[1].c_str());
        
        // Support exactly 5 effort inputs and goal states
        std::vector<float> effort;
        for (int i = 2; i <= 6; ++i) {
            effort.push_back((float)std::atof(row[i].c_str()));
        }

        std::vector<float> results;
        for (int i = 7; i <= 11; ++i) {
            results.push_back((float)std::atof(row[i].c_str()));
        }

        MotionProfile mp{effort, start, scale};
        MovementSimulator ms(mp);
        std::vector<float> goals = ms.Run();

        bool failed = false;
        for (int i = 0; i <= goals.size(); ++i) {
            if (goals[i] != results[i]) {
                std::cout << "[RESULT MISMATCH] " << goals[i] << " != " << results[i] << std::endl;
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
