#include "gtest/gtest.h"
#include "movement_simulator.hpp"


TEST(EffortSimulator, ProvidedCase1) {
    std::vector<float> goals{10.3, 11.5, 13.0, 13.0, 13.0, 13.0, 8.5, 7.0, 7.0};

    MotionProfile mp{
        {0.1, 0.5, 1.0, 0.5, 0.0, 0.0, -0.5, -1.0, 0.0},
        10.0,
        3.0
    };

    MovementSimulator ms(mp);
    std::vector<float> output_states = ms.Run();

    EXPECT_EQ(goals, output_states);
}


TEST(EffortSimulator, ProvidedCase2) {
    std::vector<float> goals{4.6, 4.0, 3.0, 3.0, 3.0, 5.6, 6.4, 6.4};

    MotionProfile mp{
        {-0.2, -0.5, -1.0, -0.5, 0.0, 0.3, 0.7, 0.0},
        5.0,
        2.0
    };

    MovementSimulator ms(mp);
    std::vector<float> output_states = ms.Run();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, CustomCase1) {
    std::vector<float> goals{1.0, 1.0, 1.0};

    MotionProfile mp{
        {0.0, 0.0, 0.0},
        1.0,
        0.1
    };

    MovementSimulator ms(mp);
    std::vector<float> output_states = ms.Run();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, CustomCase2) {
    std::vector<float> goals{1.0, 2.0, 0.0};

    MotionProfile mp{
        // No movement, move to 2.0, then
        // move to 0.0 because the direction changed.
        {0.0, 1.0, -1.0},
        1.0,
        1.0
    };

    MovementSimulator ms(mp);
    std::vector<float> output_states = ms.Run();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, CustomCase3) {
    std::vector<float> goals{1.0, 0.0, -0.0};

    MotionProfile mp{
        {0.0, -1.0, -0.1},
        1.0,
        1.0
    };

    MovementSimulator ms(mp);
    std::vector<float> output_states = ms.Run();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, MoveBackwards) {
    std::vector<float> goals{1.0, 0.75, 0.65};

    MotionProfile mp{
        // Start at 1.0, move backward by 0.25, move backward by 0.35.
        {0.0, -0.5, -0.7},
        1.0,
        0.5
    };

    MovementSimulator ms(mp);
    std::vector<float> output_states = ms.Run();

    EXPECT_EQ(goals, output_states);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
