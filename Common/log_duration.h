#pragma once

#include <chrono>
#include <iostream>
#include <QString>
#include <unordered_map>

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define LOG_DURATION_T(x,t) LogDuration<t> UNIQUE_VAR_NAME_PROFILE(x)

template<typename Period = std::chrono::microseconds>
class LogDuration {
public:
    // заменим имя типа std::chrono::steady_clock
    // с помощью using для удобства
    using Clock = std::chrono::steady_clock;

    LogDuration(const QString& id)
        : id_(id){
    }

	~LogDuration() {
		using namespace std::chrono;
		using namespace std::literals;

		const auto end_time = Clock::now();
		const auto dur = end_time - start_time_;
        const auto duration_in_period = duration_cast<Period>( dur ).count( );
        total_duration_[id_] += duration_in_period;
        ++count_[id_];

        //qDebug( ) << id_ << ": " << duration_cast<Period>( dur ).count( ) << " " << period_name( );
    }

    static std::unordered_map<QString, double> mean_duration( ) {
        std::unordered_map<QString, double> result;
        for ( const auto& [id, total] : total_duration_ ) {
            result[id] = total / count_[id];
        }
        return result;
    }

    static void reset( const std::string& id ) {
        total_duration_[id] = 0.0;
        count_[id] = 0;
    }

    static void resetAll( ) {
        for ( auto& [id, total] : total_duration_ ) {
            total_duration_[id] = 0;
            count_[id] = 0;
        }

        //total_duration_.clear( );
        //count_.clear( );
    }
private:
    QString period_name( ) const {
        if constexpr ( std::is_same_v<Period, std::chrono::nanoseconds> )
            return "ns";
        else if constexpr ( std::is_same_v<Period, std::chrono::microseconds> )
            return "us";
        else if constexpr ( std::is_same_v<Period, std::chrono::milliseconds> )
            return "ms";
        else if constexpr ( std::is_same_v<Period, std::chrono::seconds> )
            return "s";
        else if constexpr ( std::is_same_v<Period, std::chrono::minutes> )
            return "min";
        else if constexpr ( std::is_same_v<Period, std::chrono::hours> )
            return "h";
        else
            return "unknown";
    }
    const QString id_;
    const Clock::time_point start_time_ = Clock::now();
    // Структура для хранения общего времени выполнения и количества выполнений
    static std::unordered_map<QString, double> total_duration_;
    static std::unordered_map<QString, int> count_;

};

// Инициализация статического члена класса
template<typename Period>
std::unordered_map<QString, double> LogDuration<Period>::total_duration_;

template<typename Period>
std::unordered_map<QString, int> LogDuration<Period>::count_;