
#ifndef SYSTEM_H
#define SYSTEM_H

#include "CompsLib.h"
#include <vector>
#include <fstream>
#include <QMutex>

namespace Electrophysiology {

const double R = 8.31;
const double F = 96500.0;

class Space {
public:
    Space();
    void InitializeSpace(double vol, bool unlimited, Comps::Geom gmtry);
    void SetVolume(double vol) { volume = vol; }
    double* GetVolumePtr() { return &volume; }
    double GetVolume() { return volume; }
    Comps::Geom GetGeometry() { return geometry; }
    void SetUnlimitedVolume(bool val) { unlimitedVolume = val; }
    bool* GetUnlimitedVolumePtr() { return &unlimitedVolume; }
    void IncrementDv(double delta) { dV += delta; }
    void IncrementDeltaOnVolume();
    ~Space() {}
private:
    double volume;
    double dV;
    bool unlimitedVolume;
    Comps::Geom geometry;
};

class System {
public:
    System();

    //SYSTEM-INITIALIZATION FUNCTIONS
    void InitializeSystem();		//creates some basic tables
    void CreateIons(std::vector<Comps::TIon> ionsToUse, int howMany);
    void CreateTransporters(std::vector<Comps::TTransporter> transportersToUse, int howMany);
    void CreateParameters(std::vector<Comps::TParameter> parametersToUse, int howMany);
    void CountNumberOfEverythingThatCanBeSaved();		//=numOfIons + numOfTransporters + numOfParams
    void CreateDataWhatToSave(std::vector<Comps::WhatToSave> data, int howMany);		//creates a table of structure containing data of what to save, id'fied by type and id
    void UpdateIons();		//calculates n from conc. and vol.
    int GetNumOfTransporters() { return numOfTransporters; }
    int GetNumOfIons() { return numOfIons; }
    void MakeListOfObjectsToBeSaved();		//makes list of all objects and another of objects that have values that have to be saved
    void CreateBigTableOfSavedValues();		//creates buffer table of tables of data that will be saved to files
    void CreateTableForTime();				//creates table for time data
    void CreateFiles();
    //FUNCTION VERIFYING THE CREATED DATA
    void DescribeTheSystem(std::vector<std::string>* allStructures, std::vector<std::string>* allStructuresToBeSaved, bool& result);

    //REALTIME FUNCTIONS
    void UpdateSurfaceArea();	//calculates surface area from cell volume
    void CalculateSumsOfConcentrations();	//osmosis issues
    void CalculateCapacitanceReal();		//calculates real cap from cap and surface area
    void CalculateDQ();						//calculates dq from a set of dn's of each ion
    void UpdateEverything();				//updates everything from dx to x and then dx = 0.0
    void UpdateTime();						//updates time variable (t+= dt)
    void SaveDataToBuffer();				//saves the data of all relevant objects to the buffer
    void RunOneLoopCycle();					//jeden obrot petli w kontekscie calego systemu
    void CheckIfNeedsSavingToFile();		//checks if buffer is full
    void CheckIfNeedsSavingToBuffer();
    bool CheckIfSimulationShouldEnd() { return simulationShouldEnd; }
    void CheckTiming();
    void SaveBufferToFiles();

    //DATA SHARING FUNCTIONS AND ENTITIES
    int GetRealNumberOfDataToSave();
    std::string GetSavedObjectNameById(int id);
    std::string GetSavedObjectUnitById(int id);
    QMutex* mutex;
    std::vector<QString>* vecShared;
    bool* stoppingVariablePtr;
    bool* vecReady;
    double* odsetek;
    bool* stoppingFillingThreadPtr;
    void SetMutex(QMutex* mt) { mutex = mt; }
    void SetVecShared(std::vector<QString>* vs) { vecShared = vs; }
    void SetVecReady(bool* vr) { vecReady = vr; }
    void SetStoppingVariablePtr(bool* v) { stoppingVariablePtr = v; }
    void SetOdsetek(double* od) { odsetek = od; }
    void SetStoppingFillingThreadPtr(bool* b) { stoppingFillingThreadPtr = b; }

    //OTHER FUNCTIONS
    double GetTime() { return t; }
    double GetMaxTime() { return tmax; }
    double GetDt() { return dt; }
    double GetCoIle() { return saveEveryWhich; }

    ~System();

    //LISTY OGÓLNE - ZAWIERAJ¥ WSZYSTKO - DOSTÊPNE POWSZECHNIE
    double** doubleList;
    Comps::Ion** ionList;	//parzyste - ecf; nieparzyste - cell
    Comps::Transporter** transporterList;
    int* noOfEachTransporter;

private:
    //LISTY SZCZEGÓ£OWE - TYLKO TO CO JEST
    Comps::Ion** realIonList;		//list of actually present ions
    Comps::Transporter** realTransporterList;		//list of actually present transporters
    Comps::Parameter** realParameterList;			//list of actually present parameters
    Comps::Object** allObjectList;			//list of all objects (ions, transportersm parameters)
    Comps::Object** objectsToBeSaved;		//sublist from allObjectList of objects that will be saved, kolejnosc taka jak dataWhatToSave[]

    Space* cell;
    Space* ecf;

    int numOfIons;		//realna liczba jonów
    int numOfTransporters;	//realna liczba transporterow
    int numOfDoubles;	//realna liczba parametrów (raczej taka sama co ogólna)
    int numOfEverythingThatCanBeSaved;

    double e;		//MP
    double capacitance;	//[F/m2]
    double capacitanceReal;		//[F]
    double temperature;
    double tmax, dt;
    double waterCond;
    double surfaceArea;
    double t;

    double sumOfConcentrationsIn;	//for osmosis
    double sumOfConcentrationsOut;	//for osmosis
    double dQ;		//wzgledem srodka blony (w komorce)

    Comps::WhatToSave* dataWhatToSave;
    int numOfDataToSave;	//number of data to be saved (e.g. Na, K and INa ->  = 3)
    int realNumOfDataToSave; 	//number of data structures that will be saved (there can be more than 1 transporter of a type)
    double saveEveryWhich;		//1 - every, 2 - every other etc.
    double savingBufferLength;		//after this is reached everything is saved to file
    //Comps::Object* findObjectByTypeAndId(char type, int id);
    bool checkIfNeedsToBeSaved(Comps::Object* object);

    double** tableOfSavedValues;	//kolejnosc odpowiada kolejnosci objectsToBeSaved[] (czyli kolejnosc dataWhatToSave[]), potencjalne wartosci NULL maja swoje podtabelki
    double* tableTime;
    int bufferCounter;

    bool readyToBeTransferedToFile;		//if false, buffer can be filled; if true, file transfer should be made and buffer cleared
    bool simulationShouldEnd;
    int simulationCounter;

    std::ofstream** fileStreamTable;	//kolejnosc odpowiada objectsToBeSaved[] czyli dataWhatToSave[], potencjalne fake-owe wpisy maja swoje obiekty
    std::ofstream* fileStreamTime;
    std::string createFilename(Comps::Object* object);

    void makeWaterFlow();
    double* decideAboutParameter_valuePtr(int id, double value);
    std::string decideAboutParameter_name(int id);
    std::string decideAboutParameter_unit(int id);
    bool sprawdzBilansLadunkow();

};

}


#endif // SYSTEM_H
