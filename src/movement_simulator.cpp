#include <algorithm>
#include <cassert>
#include <iostream>
#include "movement_simulator.hpp"


MovementSimulator::MovementSimulator(MotionProfile mp) : _profile(mp), _current_position(mp._start_position) {}

std::vector<float> MovementSimulator::Run(){
    std::vector<float> result_states;
    float last_state = _current_position;
    float state = 0.0;
    float max_impulse = 0.0; 
    for (auto effort : _profile._effort_input) {

        // Effort is positive, is it more than the max?
        if (max_impulse >= 0.0 && effort >= 0.0) {
            max_impulse = std::max(effort, max_impulse);

        // Effort is negative, is it more than the max?
        } else if (max_impulse < 0.0 && effort < 0.0) {
            max_impulse = std::min(effort, max_impulse);

        // Signs of the effort and max are different. Must apply this effort.
        } else {
            max_impulse = effort;
        }

        if (max_impulse != 0.0){
            state = CalculateState(_current_position, max_impulse, _profile._scale);
            last_state = state;
        } else {
            state = last_state;
        }
        result_states.push_back(state);
    }
    return result_states;
}

float MovementSimulator::CalculateState(float current_pos, float effort_input, float scale) {
    assert(effort_input <= UPPER_EFFORT_LIMIT);
    assert(effort_input >= LOWER_EFFORT_LIMIT); 
    float result = current_pos + effort_input * scale;
    return result;
}