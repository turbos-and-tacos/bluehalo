#include "gtest/gtest.h"
#include "movement_simulator.hpp"


TEST(EffortSimulator, ProvidedCase1) {
    std::vector<float> goals{10.3, 11.5, 13.0, 13.0, 13.0, 13.0, 8.5, 7.0, 7.0};

    MotionPlan mp{
        {0.1, 0.5, 1.0, 0.5, 0.0, 0.0, -0.5, -1.0, 0.0},
        10.0,
        3.0,
        MotionBound(13.0, 0.0)
    };

    MotionSimulator ms(mp);
    std::vector<float> output_states = ms.RunMotionPlan();

    EXPECT_EQ(goals, output_states);
}


TEST(EffortSimulator, ProvidedCase2) {
    std::vector<float> goals{4.6, 4.0, 3.0, 3.0, 3.0, 5.6, 6.4, 6.4};

    MotionPlan mp{
        {-0.2, -0.5, -1.0, -0.5, 0.0, 0.3, 0.7, 0.0},
        5.0,
        2.0,
        MotionBound(7.0, -5.0)
    };

    MotionSimulator ms(mp);
    std::vector<float> output_states = ms.RunMotionPlan();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, CustomCase1) {
    std::vector<float> goals{1.0, 2.0, 0.0};

    MotionPlan mp{
        // No movement, move to 2.0, then
        // move to 0.0 because the direction changed.
        {0.0, 1.0, -1.0},
        1.0,
        1.0,
        MotionBound(5.0, -5.0)
    };

    MotionSimulator ms(mp);
    std::vector<float> output_states = ms.RunMotionPlan();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, CustomCase2) {
    std::vector<float> goals{1.0, 0.0, -0.0};

    MotionPlan mp{
        {0.0, -1.0, -0.1},
        1.0,
        1.0,
        MotionBound(5.0, -5.0)
    };

    MotionSimulator ms(mp);
    std::vector<float> output_states = ms.RunMotionPlan();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, NoMovement) {
    std::vector<float> goals{1.0, 1.0, 1.0};

    MotionPlan mp{
        {0.0, 0.0, 0.0},
        1.0,
        0.1,
        MotionBound(5.0, -5.0)
    };

    MotionSimulator ms(mp);
    std::vector<float> output_states = ms.RunMotionPlan();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, MoveBackwards) {
    std::vector<float> goals{1.0, 0.75, 0.65};

    MotionPlan mp{
        // Start at 1.0, move backward by 0.25, move backward by 0.35.
        {0.0, -0.5, -0.7},
        1.0,
        0.5,
        MotionBound(5.0, -5.0)
    };

    MotionSimulator ms(mp);
    std::vector<float> output_states = ms.RunMotionPlan();

    EXPECT_EQ(goals, output_states);
}

TEST(EffortSimulator, NotInitialized) {
    MotionSimulator ms;
    EXPECT_THROW(ms.RunMotionPlan(), InvalidMotionPlan);
}

TEST(EffortSimulator, InitializedDefaultConstructor) {
    MotionSimulator ms;
    MotionPlan mp{
        {0.0, 0.0, 0.0},
        1.0,
        0.1,
        MotionBound(5.0, -5.0)
    };
    ms.SetMotionPlan(mp);
    EXPECT_NO_THROW(ms.RunMotionPlan());
}

TEST(EffortSimulator, GetSetMotionPlan) {
    MotionSimulator ms;
    MotionPlan mp{
        {0.0, 0.0, 0.0},
        1.0,
        0.1,
        MotionBound(5.0, -5.0)
    };
    MotionPlan expected{
        {0.0, 0.0, 0.0},
        1.0,
        0.1,
        MotionBound(5.0, -5.0)
    };
    ms.SetMotionPlan(mp);
    auto plan = ms.GetMotionPlan();
    EXPECT_EQ(plan, expected);
}

TEST(EffortSimulator, ExceedMotionBounds) {
    std::vector<float> goals{10.3, 11.5, 13.0, 13.0, 13.0, 13.0, 8.5, 7.0, 7.0};

    MotionPlan mp{
        {0.1, 0.5, 1.0, 0.5, 0.0, 0.0, -0.5, -1.0, 0.0},
        10.0,
        3.0,
        MotionBound(9.0, 0.0)
    };

    // Invalid Start location thrown first. Invalid location because of bounds later.
    EXPECT_THROW(MotionSimulator ms(mp), InvalidLocation);
}

TEST(EffortSimulator, InvalidEffort) {
    MotionSimulator ms;
    MotionPlan mp{
        {10.0, 0.0, 0.0},
        1.0,
        0.1,
        MotionBound(5.0, -5.0)
    };
    ms.SetMotionPlan(mp);
    EXPECT_THROW(ms.RunMotionPlan(), EffortError);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
