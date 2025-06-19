#pragma once

#include "../Includes.h"

class CLogger
{
public:
    void Debug(std::string message)
    {
        // Green
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2 | FOREGROUND_INTENSITY);

        auto Time = std::chrono::system_clock::now();
        std::time_t ToTime = std::chrono::system_clock::to_time_t(Time);
        std::tm LocalTime = *std::localtime(&ToTime);

        std::cout << "[" << std::put_time(&LocalTime, "%H:%M") << "] [DEBUG]: " << message << "\n\n";
    };

    void Error(std::string message)
    {
        // Red
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4 | FOREGROUND_INTENSITY);

        auto Time = std::chrono::system_clock::now();
        std::time_t ToTime = std::chrono::system_clock::to_time_t(Time);
        std::tm LocalTime = *std::localtime(&ToTime);

        std::cout << "[" << std::put_time(&LocalTime, "%H:%M") << "] [ERROR]: " << message << "\n\n";
    };

	void WaitAndClose()
	{
		Debug("Closing in 10 seconds...");
		Sleep(10000);
        ExitProcess(0);
	};
};

static auto Logger = std::make_unique<CLogger>;