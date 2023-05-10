
#include "control.h"
#include "qdebug.h"

#include <filesystem>

//CONSTRUCTOR
Control::Control() {
    system = NULL;
    simulation = NULL;
    tabIonDoStworzenia = new std::vector<Comps::TIon>;
    tabParametryDoStworzenia = new std::vector<Comps::TParameter>;
    tabTransporterDoStworzenia = new std::vector<Comps::TTransporter>;
    tabNazwyDoZapisywania = new std::vector<Comps::WhatToSave>;
    ileNazwDoZapisywania = 0;
    ileIons = 0;
    ileParameters = 0;
    ileTransporters = 0;

    Comps::InitializeLibrary();
}

//IONS SETUP
std::vector<QString> Control::GetListOfIonsFromLibrary() {  //prepares list of ion names from ionNames[] arr
    int ileWszystkichJonow = Comps::IONS_COUNT;
    QString name("");
    std::vector<QString> list;
    for (int i=0; i<ileWszystkichJonow; ++i) {
        name = QString::fromStdString(Comps::ionsNames[i]);
        list.push_back(name);
    }
    return list;
}

bool Control::findIonDoStworzenia(int id) { //sprawdza czy jon o id istnieje w tablicy jonów do stworzenia
    for (auto x : *tabIonDoStworzenia) {
        if (x.id == id)
            return true;
    }
    return false;
}

bool Control::AddIonToDoStworzenia(int id, double cin, double cout, double charge) {
    Comps::TIon newIonDoStworzenia;
    newIonDoStworzenia.charge = charge;
    newIonDoStworzenia.concentrationIn = cin;
    newIonDoStworzenia.concentrationOut = cout;
    newIonDoStworzenia.id = id;
    if (findIonDoStworzenia(id))    //nie wstawiamy powtórek tych samych jonów
        return false;
    tabIonDoStworzenia -> push_back(newIonDoStworzenia);
    ileIons++;
    return true;
}

bool Control::RemoveIonFromDoStworzenia(int id) {
    int i(0);
    for (auto x : *tabIonDoStworzenia) {
        if (x.id == id) {
            tabIonDoStworzenia -> erase(tabIonDoStworzenia->begin() + i);
            ileIons--;
            return true;
        }
        i++;
    }
    return false;
}

std::vector<QString> Control::GetListOfUsedIonsFromLibrary() {
    std::vector<QString> list;
    if (tabIonDoStworzenia == NULL)
        return list;
    int id(-1);
    QString name("");
    for (auto x : *tabIonDoStworzenia) {
        id = x.id;
        name = QString::fromStdString(Comps::ionsNames[id]);
        list.push_back(name);
    }
    return list;
}

//TRANSPORTERS SETUP
std::vector<QString> Control::GetListOfTransportersFromLibrary() {  //prepares list of ion names from ionNames[] arr
    int ileWszystkichTransporterow = Comps::TRANSPORTERS_COUNT;
    QString name("");
    std::vector<QString> list;
    for (int i=0; i<ileWszystkichTransporterow; ++i) {
        name = QString::fromStdString(Comps::transportersNames[i]);
        list.push_back(name);
    }
    return list;
}

bool Control::AddTransporterToDoStworzenia(int id, std::vector<double> params) {
    Comps::TTransporter newTransporterDoStworzenia;
    newTransporterDoStworzenia.id = id;
    newTransporterDoStworzenia.params = params;
    tabTransporterDoStworzenia -> push_back(newTransporterDoStworzenia);
    ileTransporters++;
    return true;
}

void Control::GetListOfTransporterParameters(
    std::vector<QString>* paramNames,
    std::vector<QString>* paramUnits,
    int& paramNumber,
    int id) {

    if (paramNames == NULL || paramUnits == NULL)
        return;
    paramNumber = Comps::transportersParamsNumbers[id];
    std::string* paramNamesPointer(NULL),* paramUnitsPointer(NULL);
    paramNamesPointer = Comps::paramNamesPointers[id];
    paramUnitsPointer = Comps::paramUnitsPointers[id];
    if (paramNamesPointer == NULL || paramUnitsPointer == NULL)
        return;
    for (int i=0; i<paramNumber; ++i) {
        paramNames -> push_back(QString::fromStdString(paramNamesPointer[i]));
        paramUnits -> push_back(QString::fromStdString(paramUnitsPointer[i]));
    }
}

bool Control::RemoveTransporterFromDoStworzenia(int id) {
    int i(0);
    for (auto x : *tabTransporterDoStworzenia) {
        if (x.id == id) {
            tabTransporterDoStworzenia -> erase(tabTransporterDoStworzenia->begin() + i);
            ileTransporters--;
            return true;
        }
        i++;
    }
    return false;
}

std::vector<QString> Control::GetListOfUsedTransportersFromLibrary() {
    std::vector<QString> list;
    if (tabTransporterDoStworzenia == NULL)
        return list;
    int id(-1);
    QString name("");
    for (auto x : *tabTransporterDoStworzenia) {
        id = x.id;
        name = QString::fromStdString(Comps::transportersNames[id]);
        list.push_back(name);
    }
    return list;
}

//PARAMETERS

void Control::GetParameterNamesAndUnits(
    std::vector<QString>* paramNames,
    std::vector<QString>* paramUnits,
    int& paramNumber) {

    if (paramNames == NULL || paramUnits == NULL)
        return;
    paramNumber = Comps::PARAMETERS_COUNT;
    for (int i=0; i<paramNumber; ++i) {
        paramNames -> push_back(QString::fromStdString(Comps::parameterNames[i]));
        paramUnits -> push_back(QString::fromStdString(Comps::parameterUnits[i]));
    }
}

bool Control::AddParameterToDoStworzenia(int id, double value) {
    Comps::TParameter newParameter;
    newParameter.id = id;
    newParameter.value = value;
    tabParametryDoStworzenia -> push_back(newParameter);
    ileParameters++;
    return true;
}

void Control::GetInsertedParameters(std::vector<QString>* data, std::vector<int>* ids) {
    std::vector<QString> resultData;
    if (tabParametryDoStworzenia == NULL)
        return;
    for (auto x : *tabParametryDoStworzenia) {
        data -> push_back(QString::number(x.value));
        ids -> push_back(x.id);
    }
}

void Control::ClearParametryDoStworzenia() {
    tabParametryDoStworzenia -> clear();
    ileParameters = 0;
}

//WYBÓR CO ZAPISYWAĆ A CO NIE

void Control::PrepareListOfUsedIons(std::vector<DoZapisywania>* data) {
    if (data == NULL)
        return;
    QString nameOut(""), nameIn("");
    int idOut(-1), idIn(-1);
    Control::DoZapisywania doZapisywaniaOut, doZapisywaniaIn;
    for (auto x : *tabIonDoStworzenia) {
        nameOut = QString::fromStdString(Comps::ionsNames[x.id]) + "_OUT";
        idOut = x.id * 2;   //tutaj zmienia się indeksowanie: out = 2x id, in = 2x id + 1
        doZapisywaniaOut.name = nameOut;
        doZapisywaniaOut.id = idOut;

        nameIn = QString::fromStdString(Comps::ionsNames[x.id]) + "_IN";
        idIn = x.id * 2 + 1;
        doZapisywaniaIn.name = nameIn;
        doZapisywaniaIn.id = idIn;

        data -> push_back(doZapisywaniaOut);
        data -> push_back(doZapisywaniaIn);
    }
}

void Control::PrepareListOfUsedTransporters(std::vector<DoZapisywania>* data) {
    if (data == NULL)
        return;
    QString name("");
    int id(-1);
    Control::DoZapisywania doZapisywania;
    for (auto x : *tabTransporterDoStworzenia) {
        name = QString::fromStdString(Comps::transportersNames[x.id]);
        id = x.id;
        doZapisywania.name = name;
        doZapisywania.id = id;
        data -> push_back(doZapisywania);
    }
}

void Control::PrepareListOfUsedParameters(std::vector<DoZapisywania>* data) {
    if (data == NULL)
        return;
    QString paramName(""), nameTotal(""), paramUnit("");
    double value(0.0);
    Control::DoZapisywania doZapisywania;
    for (auto x : *tabParametryDoStworzenia) {
        paramName = QString::fromStdString(Comps::parameterNames[x.id]);
        paramUnit = QString::fromStdString(Comps::parameterUnits[x.id]);
        value = x.value;
        nameTotal = paramName + ("::: (początkowo: ") + QString::number(value) + paramUnit + ")";
        doZapisywania.name = nameTotal;
        doZapisywania.id = x.id;
        data -> push_back(doZapisywania);
    }
}

void Control::AddIonDoZapisywania(int id) {
    Comps::WhatToSave wts;
    wts.id = id;
    wts.type = 'I';
    tabNazwyDoZapisywania -> push_back(wts);
    ileNazwDoZapisywania++;
}

void Control::AddTransporterDoZapisywania(int id) {
    Comps::WhatToSave wts;
    wts.id = id;
    wts.type = 'T';
    tabNazwyDoZapisywania -> push_back(wts);
    ileNazwDoZapisywania++;
}

void Control::AddParameterDoZapisywania(int id) {
    Comps::WhatToSave wts;
    wts.id = id;
    wts.type = 'P';
    tabNazwyDoZapisywania -> push_back(wts);
    ileNazwDoZapisywania++;
}

void Control::ClearTableDoZapisywania() {
    tabNazwyDoZapisywania -> clear();
    ileNazwDoZapisywania = 0;
}

//PRZYGOTOWANIE DO SYMULACJI
bool Control::CheckIfTablesAreFull() {
    if (tabIonDoStworzenia == NULL)
        return false;
    if (tabIonDoStworzenia->empty())
        return false;
    if (tabTransporterDoStworzenia == NULL)
        return false;
    if (tabTransporterDoStworzenia->empty())
        return false;
    if (tabParametryDoStworzenia == NULL)
        return false;
    if (tabParametryDoStworzenia->empty())
        return false;
    return true;
}

//CREATING THE SYSTEM AND VERIFYING RESULTS

void Control::CreateSystem() {
    system = new Electrophysiology::System();
}

void Control::removePreviousFilesWithData() {
    const std::string dir_path = "OutputFiles";
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
        std::filesystem::remove_all(entry.path());
}

void Control::CreateEverythingInTheSystem() {
    system->InitializeSystem();		//creates some tables in memory

    system->CreateIons(*tabIonDoStworzenia, ileIons);
    system->CreateTransporters(*tabTransporterDoStworzenia, ileTransporters);
    system->CreateParameters(*tabParametryDoStworzenia, ileParameters);		//downloads params to the system, eg capacitance

    system->CountNumberOfEverythingThatCanBeSaved();		//probably useless

    system->CreateDataWhatToSave(*tabNazwyDoZapisywania, ileNazwDoZapisywania);

    system->UpdateIons();		//calculates n from concentrations and volumes
    system->CalculateSumsOfConcentrations();	//calculates sums of conc in and out
    system->UpdateSurfaceArea();		//calculates surface area from volume and geometry
    system->CalculateCapacitanceReal();	//calculates real capacitance (multiplied by surface area)

    system->MakeListOfObjectsToBeSaved();
    system->CreateBigTableOfSavedValues();
    system->CreateTableForTime();

    removePreviousFilesWithData();
    system->CreateFiles();
}

void Control::DescribeTheSystem(std::vector<std::string>* allStructures, std::vector<std::string>* allStructuresToBeSaved, bool& result) {
    system -> DescribeTheSystem(allStructures, allStructuresToBeSaved, result);
}

void Control::ClearEverything() {
    tabIonDoStworzenia -> clear();
    tabParametryDoStworzenia -> clear();
    tabTransporterDoStworzenia -> clear();
    ileIons = 0;
    ileTransporters = 0;
    ileParameters = 0;
}

//CREATING SIMULATION

void Control::CreateSimulation(bool* sv) {
    simulation = new Electrophysiology::Simulation(system, sv);
}

std::vector<QString> Control::PrepareDataToBeShown() {
    std::vector<QString> data;
    QString str("");
    int numOfData = system -> GetRealNumberOfDataToSave();
    for (int i=0; i<numOfData; ++i) {
        str = QString::fromStdString(system -> GetSavedObjectNameById(i));
        data.push_back(str);
    }
    return data;
}

std::vector<QString> Control::PrepareUnitsToBeShown() {
    std::vector<QString> data;
    QString str("");
    int numOfData = system -> GetRealNumberOfDataToSave();
    for (int i=0; i<numOfData; ++i) {
        str = QString::fromStdString(system -> GetSavedObjectUnitById(i));
        data.push_back(str);
    }
    return data;
}

void Control::SetMutexAndVecShared(QMutex* mt, std::vector<QString>* v, bool* vecReady, double* odsetek, bool* secPtr) {
    system -> SetMutex(mt);
    system -> SetVecShared(v);
    system -> SetVecReady(vecReady);
    system -> SetOdsetek(odsetek);
    system -> SetStoppingFillingThreadPtr(secPtr);
}

void Control::GetTimeVariables(double& dt, double& tmax, double& coile) {
    tmax = system ->GetMaxTime();
    dt = system ->GetDt();
    coile = system ->GetCoIle();
}

//RUNNING SIMULATION

void Control::RunSimulation() {
    simulation -> StartSimulation();
}

Control::~Control() {
    if (simulation != NULL) {
        delete simulation;      //generally deletes system too
        simulation = NULL;
        system = NULL;
    }
    if (system != NULL) {
        delete system;
        system = NULL;
    }
    if (tabIonDoStworzenia != NULL) {delete tabIonDoStworzenia; tabIonDoStworzenia = NULL; }
    if (tabParametryDoStworzenia != NULL) {delete tabParametryDoStworzenia; tabParametryDoStworzenia = NULL;}
    if (tabTransporterDoStworzenia != NULL) {delete tabTransporterDoStworzenia; tabTransporterDoStworzenia = NULL;}
    if (tabNazwyDoZapisywania != NULL) {delete tabNazwyDoZapisywania; tabNazwyDoZapisywania = NULL;}
}

