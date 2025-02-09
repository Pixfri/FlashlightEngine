// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_APPLICATION_HPP
#define FL_CORE_APPLICATION_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <FlashlightEngine/Core/Assert.hpp>
#include <FlashlightEngine/Core/ApplicationComponent.hpp>
#include <FlashlightEngine/Core/World.hpp>
#include <FlashlightEngine/DataStructures/Bitset.hpp>

#include <spdlog/async_logger.h>

#include <memory>

namespace Fl {
    struct FrameTimeInfo {
        F32 DeltaTime;     // Time elapsed since the last application loop iteration.
        F32 GlobalTime;    // Time elapsed since the application started, in seconds.
        I32 SubStepCount;  // Amount of fixed time steps to process.
        F32 SubStepTime;   // Time used by each fixed time step, in seconds.
    };

    class Application {
    public:
        explicit Application(U64 worldCount = 1);
        
        inline const std::vector<ApplicationComponentPtr>& GetAppComponents() const;
        inline std::vector<ApplicationComponentPtr>& GetAppComponents();
        inline const std::vector<WorldPtr>& GetWorlds() const;
        inline std::vector<WorldPtr>& GetWorlds();
        inline FrameTimeInfo GetTimeInfo() const;

        inline void SetFixedTimeStep(F32 fixedTimeStep);

        /**
         * @brief Adds a world to the application.
         * @tparam Args Types of the arguments to forward to the world.
         * @param args Arguments to forward to the world.
         * @return A reference to the new world.
         */
        template <typename... Args>
        World& AddWorld(Args&&... args);

        /**
         * @brief Adds a component to the application.
         * @tparam Args Types of the arguments to forward to the application component.
         * @param args Arguments to forward to the application component.
         * @return A reference to the new application component.
         */
        template <typename... Args>
        ApplicationComponent& AddComponent(Args&&... args);

        /**
         * @brief Runs the application.
         */
        void Run();

        /**
         * @brief Runs the application and calls the given callback on each iteration.
         * @tparam FuncType Type of the callback to call on each cycle.
         * @param callback The callback to call on each cycle.
         */
        template <typename FuncType>
        void Run(FuncType&& callback);

        /**
         * @brief Runs one cycle of the application.
         * @return Whether the application is still running or not.
         */
        bool RunOnce();

        /**
         * @brief Tells the application to stop running.
         */
        inline void Quit();
    
    private:
        std::shared_ptr<spdlog::async_logger> m_AppLogger;

        std::vector<WorldPtr> m_Worlds{};
        Bitset m_ActiveWorlds{};

        std::vector<ApplicationComponentPtr> m_Components{};
        Bitset m_ActiveComponents{};

        FrameTimeInfo m_TimeInfo{ 0.0f, 0.0f, 0, 0.016666f };
        std::chrono::time_point<std::chrono::system_clock> m_LastFrameTime = std::chrono::system_clock::now();
        F32 m_RemainingTime{}; // Extra time remaining after executing the systems' fixed step update.

        bool m_IsRunning = true;

        static bool m_IsLoggerSetup;
        void SetupLogger();
    };
}

#include <FlashlightEngine/Core/Application.inl>

#endif // FL_CORE_APPLICATION_HPP
