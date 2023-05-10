
#ifndef SIMULATION_H
#define SIMULATION_H

#include "System.h"

namespace Electrophysiology {

    class Simulation
    {
    public:
        Simulation();
        Simulation(Electrophysiology::System* sys, bool* sv);
        void StartSimulation();
        bool* stoppingVariablePtr;
        ~Simulation();
    private:
        Electrophysiology::System* system;
        void runningTheSimulation();
    };

}

#endif // SIMULATION_H
