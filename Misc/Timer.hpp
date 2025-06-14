#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <mutex>

/**
 * @author: huxint
 */
namespace timer {
    class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
        bool running;

    public:
        constexpr Timer() : running(false) {}

        constexpr auto start() -> void {
            start_time = std::chrono::high_resolution_clock::now();
            running = true;
        }

        constexpr auto stop() -> void {
            end_time = std::chrono::high_resolution_clock::now();
            running = false;
        }

        // 返回微秒数
        constexpr auto elapsedMicroseconds() const -> double {
            auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
            return std::chrono::duration<double, std::micro>(end - start_time).count();
        }

        // 返回毫秒数
        constexpr auto elapsedMilliseconds() const -> double {
            return elapsedMicroseconds() / 1000.0;
        }

        // 返回秒数
        constexpr auto elapsedSeconds() const -> double {
            return elapsedMicroseconds() / 1000000.0;
        }

    };

    // 全局计时器管理类
    class TimerManager {
    private:
        static std::map<std::string, std::vector<double>> timings;
        static std::mutex mutex;

    public:
        static constexpr auto addTiming(const std::string &name, double elapsed) -> void {
            std::lock_guard<std::mutex> lock(mutex);
            timings[name].push_back(elapsed);
        }

        static constexpr auto printResults() -> void {
            std::lock_guard<std::mutex> lock(mutex);
            std::cerr << "===== 计时器统计结果 =====" << std::endl;
            std::cerr << std::left << std::setw(30) << "名称"
                      << std::setw(15) << "调用次数"
                      << std::setw(15) << "总时间(ms)"
                      << std::setw(15) << "平均时间(ms)"
                      << std::setw(15) << "最小时间(ms)"
                      << std::setw(15) << "最大时间(ms)" << std::endl;

            for (const auto &[name, times] : timings) {
                if (times.empty()) {
                    continue;
                }

                double total = 0.0;
                double min_time = times[0];
                double max_time = times[0];

                for (double t : times) {
                    total += t;
                    min_time = std::min(min_time, t);
                    max_time = std::max(max_time, t);
                }

                double avg = total / times.size();

                std::cerr << std::left << std::setw(30) << name
                          << std::setw(15) << times.size()
                          << std::setw(15) << std::fixed << std::setprecision(3) << total
                          << std::setw(15) << std::fixed << std::setprecision(3) << avg
                          << std::setw(15) << std::fixed << std::setprecision(3) << min_time
                          << std::setw(15) << std::fixed << std::setprecision(3) << max_time << std::endl;
            }
            std::cerr << "=========================" << std::endl;
        }

        static constexpr auto clear() -> void {
            std::lock_guard<std::mutex> lock(mutex);
            timings.clear();
        }

    };

    std::map<std::string, std::vector<double>> TimerManager::timings;
    std::mutex TimerManager::mutex;

    // 作用域计时器
    class ScopeTimer {
    private:
        Timer timer;
        std::string name;
        bool silent;

    public:
        constexpr ScopeTimer(const std::string &timer_name, bool silent_mode = false) : name(timer_name), silent(silent_mode) {
            timer.start();
        }

        constexpr ~ScopeTimer() {
            timer.stop();
            double elapsed = timer.elapsedMilliseconds();
            TimerManager::addTiming(name, elapsed);
            if (not silent) {
                std::cerr << name << ": " << std::fixed << std::setprecision(3) << elapsed << " ms" << std::endl;
            }
        }

    };

}

#define TIMER_START(name) timer::Timer name; name.start()
#define TIMER_STOP(name) name.stop()
#define TIMER_ELAPSED_MS(name) name.elapsedMilliseconds()
#define SCOPE_TIMER(name) timer::ScopeTimer scope_timer_##__LINE__(name)
#define SILENT_SCOPE_TIMER(name) timer::ScopeTimer scope_timer_##__LINE__(name, true)
#define PRINT_TIMER_RESULTS() timer::TimerManager::printResults()
#define CLEAR_TIMER_RESULTS() timer::TimerManager::clear()