#ifndef MOVEMENT_SIMULATOR_H
#define MOVEMENT_SIMULATOR_H

#include <exception>
#include <vector>

struct MotionBound {

    // REQ-004
    float upper;
    float lower;

    MotionBound() : upper(0.0), lower(0.0) {}
    MotionBound(float upper, float lower) : upper(upper), lower(lower) {}
    bool operator==(const MotionBound other) const {
        return other.upper == upper && other.lower == lower;
     }
};

struct MotionPlan {

    // REQ-007 "Streamed" input effort.
    std::vector<float> effort;

    // REQ-007 Constant Position
    float start_position;
    float scale_factor;

    // REQ-004
    MotionBound bound;

    MotionPlan() {}
    MotionPlan(std::vector<float> effort, float start_position, float scale_factor, float upper, float lower) 
        : effort(effort), start_position(start_position), scale_factor(scale_factor), bound(MotionBound(upper, lower)){}

    MotionPlan(std::vector<float> effort, float start_position, float scale_factor, MotionBound bound) 
        : effort(effort), start_position(start_position), scale_factor(scale_factor), bound(bound) {}

    bool operator==(const MotionPlan& mp) const {
        return (mp.start_position == start_position && mp.scale_factor == scale_factor && mp.effort == effort && mp.bound == bound);
    }
};  

/* 
Simple simulator to estimate the end goals as the result of current position + effort * scale_factor.
Rules:
1. Effort must be between 1.0 and -1.0.
2. Subsequent efforts in a motion plan that are less than the highest
   seen are ignored.
3. Subsequent efforts in a motion plan that are less AND in the opposite 
   direction are NOT ignored. (e.g 0.9 then -0.5. The latter is not ignored)
4. Bounds of the motion can not be exceeded.
*/
class MotionSimulator {
    private:
        const float UPPER_EFFORT_LIMIT = 1.0;
        const float LOWER_EFFORT_LIMIT = -1.0;
        MotionPlan _motion_plan{};
        bool _initialized{false};
        float _current_position{0.0f};

        float CalculateState(float current_postion, float effort, float scale_factor);

    public:
    MotionSimulator();
    explicit MotionSimulator(const MotionPlan& mp);
        void SetMotionPlan(const MotionPlan &mp);
        const MotionPlan& GetMotionPlan() const {return _motion_plan;};
        void SetPosition(float pos) {_current_position = pos;};
        std::vector<float> RunMotionPlan();
};

// TODO: Consider moving this as the project grows.
// Keeping here for brevity.
class InvalidMotionPlan : public std::exception {
    public:
        explicit InvalidMotionPlan(std::string message) : _message("InvalidMotionPlan: " + std::move(message)) {}

        const char* what() const noexcept override {
            return  _message.c_str();
        }
    private:
        std::string _message;
};

// REQ-004
class InvalidLocation : public std::exception {
    public:
        explicit InvalidLocation(std::string message) : _message("InvalidLocation: " + std::move(message)) {}

        const char* what() const noexcept override {
            return _message.c_str();
        }
    private:
        std::string _message;
};

class EffortError : public std::exception {
    public:
        explicit EffortError(std::string message) : _message("EffortError" + std::move(message)) {}

        const char* what() const noexcept override {
            return _message.c_str();
        }
    private:
        std::string _message;
};
#endif // MOVEMENT_SIMULATOR_H