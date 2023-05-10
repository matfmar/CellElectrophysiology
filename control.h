//KLASA ZAPEWNIAJĄCA INTERFEJS DO BIBILIOTEKI KOMPONENTÓW

#ifndef CONTROL_H
#define CONTROL_H

#include "Simulation.h"
#include "System.h"

#include <vector>
#include <QString>

class Control {
public:
    Control();

    //IONS
    std::vector<QString> GetListOfIonsFromLibrary();
    bool AddIonToDoStworzenia(int id, double cin, double cout, double charge);
    bool RemoveIonFromDoStworzenia(int id);
    std::vector<QString> GetListOfUsedIonsFromLibrary();

    //TRANSPORTERS
    std::vector<QString> GetListOfTransportersFromLibrary();
    void GetListOfTransporterParameters(
        std::vector<QString>* paramNames,
        std::vector<QString>* paramUnits,
        int& paramNumber,
        int id);
    bool AddTransporterToDoStworzenia(int id, std::vector<double> params);
    bool RemoveTransporterFromDoStworzenia(int id);
    std::vector<QString> GetListOfUsedTransportersFromLibrary();

    //PARAMETERS
    void GetParameterNamesAndUnits(
        std::vector<QString>* paramNames,
        std::vector<QString>* paramUnits,
        int& paramNumber);
    bool AddParameterToDoStworzenia(int id, double value);
    void GetInsertedParameters(std::vector<QString>* data, std::vector<int>* ids);
    void ClearParametryDoStworzenia();

    //WYBÓR CO ZAPISAĆ A CO NIE
    struct DoZapisywania {
        QString name;
        int id;
    };
    void PrepareListOfUsedIons(std::vector<DoZapisywania>* data);
    void PrepareListOfUsedTransporters(std::vector<DoZapisywania>* data);
    void PrepareListOfUsedParameters(std::vector<DoZapisywania>* data);
    void AddIonDoZapisywania(int id);
    void AddTransporterDoZapisywania(int id);
    void AddParameterDoZapisywania(int id);
    void ClearTableDoZapisywania();

    //PRZYGOTOWANIE DO ROZPOCZECIA SYMULACJI
    bool CheckIfTablesAreFull();

    //CREATING THE SYSTEM AND VERIFYING RESULTS
    void CreateSystem();
    void CreateEverythingInTheSystem();
    void DescribeTheSystem(std::vector<std::string>* allStructures, std::vector<std::string>* allStructuresToBeSaved, bool& result);
    void ClearEverything(); //when something fucks up user could do it

    //CREATING SIMULATION
    void CreateSimulation(bool* sv);
    std::vector<QString> PrepareDataToBeShown();
    std::vector<QString> PrepareUnitsToBeShown();
    void SetMutexAndVecShared(QMutex* mt, std::vector<QString>* v, bool* vecReady, double* odsetek, bool* secPtr);
    void GetTimeVariables(double& dt, double& tmax, double& coile);

    //RUNNING SIMULATION
    void RunSimulation();

    ~Control();
private:
    Electrophysiology::System* system;
    Electrophysiology::Simulation* simulation;

    std::vector<Comps::TIon>* tabIonDoStworzenia;
    std::vector<Comps::TTransporter>* tabTransporterDoStworzenia;
    std::vector<Comps::TParameter>* tabParametryDoStworzenia;
    std::vector<Comps::WhatToSave>* tabNazwyDoZapisywania;
    int ileIons, ileTransporters, ileParameters, ileNazwDoZapisywania;

    bool findIonDoStworzenia(int id);
    void removePreviousFilesWithData();



};

#endif // CONTROL_H
