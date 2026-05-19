#pragma once
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

class ProgressBar {
public:
    ProgressBar(int total, int width = 50, bool show_eta = true)
    : total_(total), width_(width), show_eta_(show_eta), start_time_(std::chrono::steady_clock::now()) {
    }

    void update(int current) {
        if(current > total_)
            current = total_;
        if(current < 0)
            current = 0;

        float progress = static_cast<float>(current) / total_;
        int pos        = static_cast<int>(width_ * progress);

        std::cout << "\r[";

        for(int i = 0; i < width_; ++i) {
            if(i < pos)
                std::cout << "█";
            else if(i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }

        std::cout << "] " << std::setw(3) << static_cast<int>(progress * 100) << "%"
                  << " (" << current << "/" << total_ << ")";

        if(show_eta_ && current > 0) {
            auto now     = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_).count();

            double estimated_total = elapsed * total_ / current;
            int remaining_ms       = static_cast<int>(estimated_total - elapsed);

            if(remaining_ms > 0) {
                int seconds = remaining_ms / 1000;
                std::cout << " ETA: " << seconds / 60 << "m " << seconds % 60 << "s";
            }
        }

        std::cout.flush();

        if(current == total_) {
            std::cout << std::endl; // New line when finished
        }
    }

    void finish() {
        update(total_);
    }

private:
    int total_;
    int width_;
    bool show_eta_;
    std::chrono::steady_clock::time_point start_time_;
};