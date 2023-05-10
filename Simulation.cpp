
#include "Simulation.h"
#include <iostream>

namespace Electrophysiology {

    Simulation::Simulation() {
        system = NULL;
        stoppingVariablePtr = NULL;
    }

    Simulation::Simulation(Electrophysiology::System* sys, bool* sv) {
        system = sys;
        stoppingVariablePtr = sv;
    }

    void Simulation::StartSimulation() {
        //std::cout << "Simulation started !!!" << std::endl;
        runningTheSimulation();
        //std::cout << "Simulation finished !!!" << std::endl;
        //std::cout << "Czas: " << system->GetTime() << std::endl;
    }

    void Simulation::runningTheSimulation() {
        system -> SetStoppingVariablePtr(stoppingVariablePtr);
        while (!(system->CheckIfSimulationShouldEnd()) || (*stoppingVariablePtr)) { //oba musza byc false zeby sie krecilo
            system->RunOneLoopCycle();			//runs one loop and actualizes everything, including time
            system->CheckIfNeedsSavingToBuffer();	//checks if current loop round should save data to buffer - if yes, saves and checks if needs to be saved to file then saves if needed
            system->CheckTiming();
        }
        *(system -> stoppingFillingThreadPtr) = true;   //kończymy wypełniający tablicę wątek
    }

    Simulation::~Simulation() {
        if (system != NULL) {
            delete system;
            system = NULL;
        }
    }

}
