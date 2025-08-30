#ifndef MOVEMENT_SIMULATOR_H
#define MOVEMENT_SIMULATOR_H


#include <vector>

struct  MotionProfile {
    std::vector<float> _effort_input;
    float _start_position;
    float _scale;

    MotionProfile(std::vector<float> effort_input, float start_position, float scale) 
        : _effort_input(effort_input), _start_position(start_position), _scale(scale) {}
};  


class MovementSimulator {
    private:
        const float UPPER_EFFORT_LIMIT = 1.0;
        const float LOWER_EFFORT_LIMIT = -1.0;
        MotionProfile _profile;
        bool _initialized;
        float _current_position;

        float CalculateState(float current_pos, float effort_input, float scale);

    public:
        void SetPosition(float pos) {_current_position = pos;};
        std::vector<float> Run();
        MovementSimulator() = default;
        MovementSimulator(MotionProfile mp);
};

#endif // MOVEMENT_SIMULATOR_H