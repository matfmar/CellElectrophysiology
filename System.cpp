#include "System.h"
#include <QString>
#include <iomanip>

namespace Electrophysiology {

Space::Space() {//przestrzenie
    volume = 0.0;
    unlimitedVolume = false;
    geometry = Comps::Geom::GEOM_SPHERE;
    dV = 0.0;
}


void Space::InitializeSpace(double vol, bool unlimited, Comps::Geom gmtry) {
    volume = vol;
    unlimitedVolume = unlimited;
    geometry = gmtry;
    dV = 0.0;
}

System::System() {
    doubleList = NULL;
    ionList = NULL;
    transporterList = NULL;
    realIonList = NULL;
    realTransporterList = NULL;
    realParameterList = NULL;
    allObjectList = NULL;
    objectsToBeSaved = NULL;
    tableOfSavedValues = NULL;
    cell = NULL;
    ecf = NULL;
    numOfIons = 0;
    numOfTransporters = 0;
    numOfDoubles = 0;
    e = 0.0;
    capacitance = 0.0;
    temperature = 0.0;
    tmax = 0.0;
    dt = 0.0;
    waterCond = 0.0;
    numOfEverythingThatCanBeSaved = 0;
    dataWhatToSave = NULL;
    numOfDataToSave = 0;
    realNumOfDataToSave = 0;
    surfaceArea = 0.0;
    sumOfConcentrationsIn = 0.0;
    sumOfConcentrationsOut = 0.0;
    capacitanceReal = 0.0;
    dQ = 0.0;
    saveEveryWhich = 0;
    savingBufferLength = 0;
    bufferCounter = 0;
    readyToBeTransferedToFile = false;
    t = 0.0;
    tableTime = NULL;
    simulationShouldEnd = false;
    simulationCounter = 0;
    fileStreamTable = NULL;
    fileStreamTime = NULL;
    mutex = NULL;
    vecShared = NULL;
    vecReady = NULL;
    stoppingVariablePtr = NULL;
    odsetek = NULL;
    stoppingFillingThreadPtr = NULL;
}

void System::InitializeSystem() {
    cell = new Space();
    ecf = new Space();
    doubleList = (double**)malloc(Comps::DOUBLES_COUNT * sizeof(double*));
    ionList = (Comps::Ion**)malloc(2 * Comps::IONS_COUNT * sizeof(Comps::Ion*));
    transporterList = (Comps::Transporter**)malloc(Comps::TRANSPORTERS_COUNT * sizeof(Comps::Transporter*));
    noOfEachTransporter = (int*)malloc(Comps::TRANSPORTERS_COUNT * sizeof(int));
    for (int i = 0; i < Comps::DOUBLES_COUNT; ++i)
        doubleList[i] = NULL;
    for (int i = 0; i < Comps::TRANSPORTERS_COUNT; ++i) {
        transporterList[i] = NULL;
        noOfEachTransporter[i] = 0;
    }
    for (int i = 0; i < 2 * Comps::IONS_COUNT; ++i)
        ionList[i] = NULL;
    //std::cout << "Zinicjowano system" << std::endl;
}

void System::CreateIons(std::vector<Comps::TIon> ionsToUse, int howMany) {
    numOfIons = 2 * howMany;
    realIonList = (Comps::Ion**)malloc(numOfIons * sizeof(Comps::Ion*));
    Comps::Ion* currentIonOut(NULL), * currentIonIn(NULL);
    double concOut(0.0), concIn(0.0), ch(0.0);
    std::string name("");
    for (int i = 0; i < howMany; ++i) {
        concOut = ionsToUse[i].concentrationOut;
        concIn = ionsToUse[i].concentrationIn;
        ch = ionsToUse[i].charge;
        name = Comps::ionsNames[ionsToUse[i].id] + "_OUT";
        currentIonOut = new Comps::Ion(concOut, 0.0, ch, name, 'I', (2 * ionsToUse[i].id));
        ionList[2 * ionsToUse[i].id] = currentIonOut;
        realIonList[2 * i] = currentIonOut;	//po chuj taka kolejnosc w tej tablicy to nie wiem
        name = Comps::ionsNames[ionsToUse[i].id] + "_IN";
        currentIonIn = new Comps::Ion(concIn, 0.0, ch, name, 'I', (2 * ionsToUse[i].id + 1));
        ionList[2 * ionsToUse[i].id + 1] = currentIonIn;
        realIonList[2 * i + 1] = currentIonIn;	//jw
    }
    //std::cout << "Utworzono jonow: " << numOfIons << std::endl;
}

int checkKtoryToJuz(int* tablicaWszystkichTransporterow, int id) {
    tablicaWszystkichTransporterow[id]++;
    return tablicaWszystkichTransporterow[id];
}

void System::CreateTransporters(std::vector<Comps::TTransporter> transportersToUse, int howMany) {
    numOfTransporters = howMany;
    realTransporterList = (Comps::Transporter**)malloc(numOfTransporters * sizeof(Comps::Transporter*));
    Comps::Transporter* currentTransporter(NULL);
    std::vector<double> parameters;
    parameters.clear();
    double* parametersTable(NULL);
    int id(0), noOfParameters(0), ktoryToJuz(0);
    int* tablicaWszystkichTransporterow = (int*)malloc(Comps::TRANSPORTERS_COUNT * sizeof(int));
    for (int i = 0; i < Comps::TRANSPORTERS_COUNT; ++i) {
        tablicaWszystkichTransporterow[i] = 0;
    }
    for (int i = 0; i < howMany; ++i) {
        id = transportersToUse[i].id;
        ktoryToJuz = checkKtoryToJuz(tablicaWszystkichTransporterow, id);
        parameters = transportersToUse[i].params;
        noOfParameters = Comps::transportersParamsNumbers[id];
        parametersTable = (double*)malloc(noOfParameters * sizeof(double));
        for (int j = 0; j < noOfParameters; ++j) {
            parametersTable[j] = parameters[j];
        }
        Comps::fun_ptr_createTransporter_arr[id](parametersTable, NULL, currentTransporter, ktoryToJuz);
        transporterList[id] = currentTransporter;		//fake
        noOfEachTransporter[id] = ktoryToJuz;
        realTransporterList[i] = currentTransporter;
        free(parametersTable);
        parametersTable = NULL;
    }
    free(tablicaWszystkichTransporterow);
    //std::cout << "Utworzono transporterow: " << numOfTransporters << std::endl;
}

double* System::decideAboutParameter_valuePtr(int id, double value) {
    double* result(NULL);
    switch (id) {
    case 0: {e = value; result = &e;} break;
    case 1: {temperature = value; result = &temperature;} break;
    case 2: {dt = value; result = &dt;} break;
    case 3: {cell->SetVolume(value*1e-12); result = cell->GetVolumePtr();} break; //entry in pl
    case 4: {ecf->SetVolume(value*1e-12); result = ecf->GetVolumePtr();} break;   //..pl
    case 5: {if (value == 0.0) {
            ecf->SetUnlimitedVolume(false);
        }
        else {
            ecf->SetUnlimitedVolume(true);
        }
        result = (double*)ecf->GetUnlimitedVolumePtr();
    }
        break;
    case 6: {tmax = value; result = &tmax;} break;
    case 7: {capacitance = value; result = &capacitance;} break;    //entry in F/m2
    case 8: {waterCond = value; result = &waterCond;} break;
    case 9: {surfaceArea = value; result = &surfaceArea;} break;    //entry in m^2
    case 10: {saveEveryWhich = value; result = &saveEveryWhich;} break;
    case 11: {savingBufferLength = value; result = &savingBufferLength;} break;
    default: result = NULL;
    }
    return result;
}

std::string System::decideAboutParameter_name(int id) {
    std::string result("");
    switch (id) {
    case 0: result = "MP"; break;
    case 1: result = "temperature"; break;
    case 2: result = "dt"; break;
    case 3: result = "cell_volume"; break;
    case 4: result = "ecf_volume"; break;
    case 5: result = "ecf_if_unlimited_volume"; break;
    case 6: result = "tmax"; break;
    case 7: result = "capacitance"; break;
    case 8: result = "waterCond"; break;
    case 9: result = "surfaceArea"; break;
    case 10: result = "saveEveryWhich"; break;
    case 11: result = "savingBufferLength"; break;
    default: result = "";
    }
    return result;
}

std::string System::decideAboutParameter_unit(int id) {
    std::string result("");
    switch (id) {
    case 0: result = "V"; break;
    case 1: result = "K"; break;
    case 2: result = "s"; break;
    case 3: result = "l"; break;
    case 4: result = "l"; break;
    case 5: result = ""; break;
    case 6: result = "s"; break;
    case 7: result = "F/m2"; break;
    case 8: result = "l^2/(Js)"; break;
    case 9: result = "m2"; break;
    case 10: result = ""; break;
    case 11: result = ""; break;
    default: result = "";
    }
    return result;
}

void System::CreateParameters(std::vector<Comps::TParameter> parametersToUse, int howMany) {
    numOfDoubles = howMany;
    Comps::Parameter* newParam(NULL);
    realParameterList = (Comps::Parameter**)malloc(numOfDoubles * sizeof(Comps::Parameter*));
    double* ptrToDouble(NULL);
    std::string paramName(""), paramUnit("");
    for (int i = 0; i < howMany; ++i) {
        ptrToDouble = decideAboutParameter_valuePtr(parametersToUse[i].id, parametersToUse[i].value);
        paramName = decideAboutParameter_name(parametersToUse[i].id);
        paramUnit = decideAboutParameter_unit(parametersToUse[i].id);
        doubleList[parametersToUse[i].id] = ptrToDouble;
        newParam = new Comps::Parameter(ptrToDouble, 'P', parametersToUse[i].id, paramName, paramUnit);
        realParameterList[i] = newParam;
    }
    //std::cout << "Utworzono parametrow: " << numOfDoubles << std::endl;
}

void System::CountNumberOfEverythingThatCanBeSaved() {
    numOfEverythingThatCanBeSaved = numOfIons + numOfTransporters + numOfDoubles;
}

void System::CreateDataWhatToSave(std::vector<Comps::WhatToSave> data, int howMany) {
    numOfDataToSave = howMany;
    dataWhatToSave = (Comps::WhatToSave*)malloc(numOfDataToSave * sizeof(Comps::WhatToSave));
    for (int i = 0; i < howMany; ++i) {
        dataWhatToSave[i].type = data[i].type;
        dataWhatToSave[i].id = data[i].id;
    }
    //std::cout << "Przekopiowano struktutre dotyczaca zapisywanych danych" << std::endl;
}

bool System::checkIfNeedsToBeSaved(Comps::Object* object) {
    if (object == NULL)
        return false;
    for (int i=0; i<numOfDataToSave; ++i) {
        if (dataWhatToSave[i].type == object->GetType()) {
            if (dataWhatToSave[i].id == object->GetId())
                return true;
        }
    }
    return false;
}

void System::MakeListOfObjectsToBeSaved() {
    //FIRST ALL OBJECTS - LIST
    allObjectList = (Comps::Object**)malloc(numOfEverythingThatCanBeSaved * sizeof(Comps::Object*));
    int iterator(0);
    //najpierw jony 'I'
    for (int i = 0; i < numOfIons; ++i) {
        allObjectList[iterator] = realIonList[i];
        iterator++;
    }
    //teraz transportery 'T'
    for (int i = 0; i < numOfTransporters; ++i) {
        allObjectList[iterator] = realTransporterList[i];
        iterator++;
    }
    //teraz parametry 'P'
    for (int i = 0; i < numOfDoubles; ++i) {
        allObjectList[iterator] = realParameterList[i];
        iterator++;
    }

    //NOW OBJECTS TO BE SAVED
    realNumOfDataToSave = 0;
    for (int i=0; i<numOfEverythingThatCanBeSaved; ++i) {
        if (checkIfNeedsToBeSaved(allObjectList[i])) {
            realNumOfDataToSave++;
            allObjectList[i] -> SetNeedToBeSaved(true);
        }
    }
    //std::cout << "Liczba struktur rzeczywiscie do zapisywania: " << realNumOfDataToSave << std::endl;

    objectsToBeSaved = (Comps::Object**)malloc(realNumOfDataToSave * sizeof(Comps::Object*));
    int iterator2(0);
    for (int i = 0; i < numOfEverythingThatCanBeSaved; ++i) {
        if (allObjectList[i] -> GetNeedToBeSaved()) {
            objectsToBeSaved[iterator2] = allObjectList[i];
            iterator2++;
        }
    }


    //std::cout << "Utworzono tabelke z danymi do rzeczywistego zapisywania" << std::endl;
}

void System::CreateBigTableOfSavedValues() {
    tableOfSavedValues = (double**)malloc(realNumOfDataToSave * sizeof(double*));
    double* tab(NULL);
    for (int i = 0; i < realNumOfDataToSave; ++i) {
        tab = (double*)malloc(savingBufferLength * sizeof(double));
        //teraz wypelnianie liczbami poczatkowymi (zerami) - beda czytane nawet potencjalnie fake-owe tabelki wiec trzeba zeby zawsze cos bylo
        for (int j = 0; j < savingBufferLength; ++j) {
            tab[j] = 0.0;
        }
        tableOfSavedValues[i] = tab;
    }
    //std::cout << "Utworzono wielki bufor do zapisywania danych w plikach" << std::endl;
    bufferCounter = 0;		//na wszelki wypadek
}

void System::CreateTableForTime() {
    tableTime = (double*)malloc(savingBufferLength * sizeof(double));
    //std::cout << "Utworzono bufor dla czasu" << std::endl;
}

void System::UpdateIons() {
    double v(0.0);
    for (int i = 0; i < numOfIons; ++i) {
        if (i % 2 == 0) {
            v = ecf->GetVolume();
        }
        else {
            v = cell->GetVolume();
        }
        realIonList[i]->SetN(v * realIonList[i]->GetC());
    }
}

void System::CalculateDQ() {
    double totalDQ_eq(0.0);
    for (int i = 0; i < numOfIons; i += 2) {	//tylko jony na zewnatrz
        totalDQ_eq += (realIonList[i]->GetDN()) * (realIonList[i]->GetCharge());	//[Eq]
    }
    double totalDQ_coulombs = totalDQ_eq * Comps::F;
    dQ = -totalDQ_coulombs;		//wzgledem srodka blony
}

void Space::IncrementDeltaOnVolume() {
    volume += dV;
    dV = 0.0;
}

void System::UpdateEverything() {
    e += (dQ / capacitanceReal);		//MP
    dQ = 0.0;
    cell->IncrementDeltaOnVolume();		//cell vol
    ecf->IncrementDeltaOnVolume();		//ecf vol
    for (int i = 0; i < numOfIons; ++i) {	//ions in and out
        if (i % 2 == 0) {
            realIonList[i]->IncrementDeltaOnConcentration(ecf->GetVolume());
        }
        else {
            realIonList[i]->IncrementDeltaOnConcentration(cell->GetVolume());
        }
    }
    UpdateSurfaceArea();		//surface are
    CalculateCapacitanceReal();	//capacitance real
    CalculateSumsOfConcentrations();	//sums of conc.
}

void System::UpdateTime() {
    t += dt;
}

void System::UpdateSurfaceArea() {
    surfaceArea = Comps::CountSurfaceAreaFromVolume(cell->GetVolume(), cell->GetGeometry());
}

void System::CalculateCapacitanceReal() {
    capacitanceReal = capacitance * surfaceArea;
}

void System::CalculateSumsOfConcentrations() {
    double sumOfIn(0.0), sumOfOut(0.0);
    for (int i = 0; i < Comps::IONS_COUNT; ++i) {
        if (ionList[i] != NULL) {
            if (i % 2 == 0)
                sumOfOut += ionList[i]->GetC();
            else
                sumOfIn += ionList[i]->GetC();
        }
    }
    sumOfConcentrationsIn = sumOfIn;
    sumOfConcentrationsOut = sumOfOut;
}

void System::makeWaterFlow() {
    double dV = Comps::MakeWaterFlow(sumOfConcentrationsIn, sumOfConcentrationsOut, doubleList);
    cell->IncrementDv(-dV);
    ecf->IncrementDv(dV);
}

void System::SaveDataToBuffer() {
    for (int i = 0; i < realNumOfDataToSave; ++i) {
        if (objectsToBeSaved[i] != NULL) {		//na wypadek debila ¿¹daj¹cego zapisywania danych z nieistniej¹cego obiektu
            (tableOfSavedValues[i])[bufferCounter] = objectsToBeSaved[i]->ReturnDataToSave();
        }
    }
    tableTime[bufferCounter] = t;
    bufferCounter++;
    int savingBufferLength2 = (int) savingBufferLength;
    readyToBeTransferedToFile = (bufferCounter == savingBufferLength2);

    //thread case
    mutex -> lock();
    if (*stoppingVariablePtr) {
        mutex -> unlock();
        return;
    }
    for (int i=0; i < realNumOfDataToSave; ++i) {
        (*vecShared)[i] = QString::number(objectsToBeSaved[i] -> ReturnDataToSave(), 'e', 10);
    }
    *odsetek = t / tmax;
    *vecReady = true;
    mutex -> unlock();
    //end of thread case
}

void System::SaveBufferToFiles() {
    for (int i = 0; i < realNumOfDataToSave; ++i) {			//jedziemy z kazda tabelka z danymi z osobna
        if (fileStreamTable[i] == NULL)
            continue;
        for (int j = 0; j < bufferCounter - 1; ++j) {		//bufferCounter przekracza o 1 gorny indeks tablicy, bo zostal inc. przed sprawdzeniem
            //musi byc buffer counter, bo koniec bufora moze nie zejsc sie z koncem symulacji
            *(fileStreamTable[i]) << std::scientific << std::setprecision(15) << (tableOfSavedValues[i])[j] << std::endl;
        }
    }
    for (int j = 0; j < bufferCounter - 1; ++j) {
        if (fileStreamTime != NULL) {
            *(fileStreamTime) << std::scientific << std::setprecision(15) << tableTime[j] << std::endl;
        }
    }
}

int System::GetRealNumberOfDataToSave() {
    return realNumOfDataToSave;
}

std::string System::GetSavedObjectNameById(int id) {
    if (id >= realNumOfDataToSave)
        return "";
    if (objectsToBeSaved[id] == NULL)
        return "";
    return (objectsToBeSaved[id] -> DescribeMyself());
}

std::string System::GetSavedObjectUnitById(int id) {
    if (id >= realNumOfDataToSave)
        return "";
    if (objectsToBeSaved[id] == NULL)
        return "";
    return (objectsToBeSaved[id] -> ReturnUnit());
}

void System::RunOneLoopCycle() {
    //NAJPIERW RUSZAJA TRANSPORTERY
    for (int i = 0; i < numOfTransporters; ++i) {
        realTransporterList[i]->Run(doubleList, ionList);
    }
    CalculateDQ();	//chyba powinno to tutaj byc ???
    //NASTEPNIE IDZIE WODA
    makeWaterFlow();
    //AKTUALIZUJEMY DANE
    UpdateEverything();
    //AKTUALIZUJEMY CZAS
    UpdateTime();
    //GOTOWE !!!
    simulationCounter++;
}

void System::CheckIfNeedsSavingToFile() {
    if (readyToBeTransferedToFile) {
        SaveBufferToFiles();
        //nie potrzeba oddzielnej funkcji do czyszczenia buforu, po prostu dane beda nadpisywane
        readyToBeTransferedToFile = false;			//resets the flag variable
        bufferCounter = 0;				//resets the buffer; data can be just overwritten, no special 'clearing' is necessary
    }
}

void System::CheckIfNeedsSavingToBuffer() {
    int saveEveryWhich2 = (int) saveEveryWhich;
    if (simulationCounter == saveEveryWhich2) {		//first checks if simulationCounter == saveEveryWhich
        SaveDataToBuffer();				//then saves data to buffer
        simulationCounter = 0;			//and resets simulationCounter to approach saveEveryWhich once again
        CheckIfNeedsSavingToFile();		//and checks if buffer is full and needs being saved to file
    }
}

std::string System::createFilename(Comps::Object* object) {
    std::string s("");
    if (object == NULL)
        return "";
    if ((object->GetType()) == 'P') {
        s = "P_" + object->GetName();
    }
    else if ((object->GetType()) == 'T') {
        s = "C_" + object->GetName();
    }
    else if ((object->GetType()) == 'I') {
        s = "S_" + object->GetName();
    }
    else {
        s = "";
    }
    return s;
}

void System::CreateFiles() {
    fileStreamTable = (std::ofstream**)malloc(realNumOfDataToSave * sizeof(std::ofstream*));
    std::ofstream* fs(NULL);
    std::string filename(""), path("");
    for (int i = 0; i < realNumOfDataToSave; ++i) {

        filename = createFilename(objectsToBeSaved[i]);
        path = "OutputFiles/" + filename + ".txt";
        fs = new std::ofstream();
        fs->open(path.c_str(), std::ios::out);
        if (!(fs -> is_open())) {
            //std::cout << "Failed to create file: " << path << " !!!" << std::endl;
            delete fs;
            fs = NULL;
        }
        fileStreamTable[i] = fs;
        //std::cout << "Utworzono plik: " << path << std::endl;
    }
    fileStreamTime = new std::ofstream();
    path = "OutputFiles/t.txt";
    fileStreamTime->open(path.c_str(), std::ios::out);
    if (!fileStreamTime->is_open()) {
        //std::cout << "Failed to create time file!!!" << std::endl;
        delete fileStreamTime;
        fileStreamTime = NULL;
    }
    //std::cout << "Utworzono plik " << path << std::endl;
}

void System::CheckTiming() {
    simulationShouldEnd = (t > tmax);
}

bool System::sprawdzBilansLadunkow() {
    double ladunekIn(0.0), ladunekOut(0.0);
    for (int i=0; i<numOfIons; ++i) {
        if (i % 2 == 0) {
            ladunekOut += (realIonList[i] ->GetCharge()) * (realIonList[i] -> GetC());
        }
        else {
            ladunekIn += (realIonList[i] ->GetCharge()) * (realIonList[i] -> GetC());
        }
    }
    return (ladunekOut < 0.000000001 && ladunekIn < 0.000000001);   //uwzgledniamy blad zapisu zmiennoprzecinkowego
}

void System::DescribeTheSystem(std::vector<std::string>* allStructures, std::vector<std::string>* allStructuresToBeSaved, bool& result) {
    result = sprawdzBilansLadunkow();
    if (allStructures == NULL || allStructuresToBeSaved == NULL)
        return;
    for (int i=0; i<numOfEverythingThatCanBeSaved; ++i) {
        allStructures -> push_back((allObjectList[i])->DescribeMyself());
    }
    for (int i=0; i<realNumOfDataToSave; ++i) {
        allStructuresToBeSaved -> push_back((objectsToBeSaved[i])->GetName());
    }
}

System::~System() {
    //DELETING IONS FROM MEMORY
    for (int i = 0; i < numOfIons; ++i) {
        if (realIonList[i] != NULL) {
            delete realIonList[i];
            realIonList[i] = NULL;
        }
    }
    if (realIonList != NULL) {
        free(realIonList);
        realIonList = NULL;
    }
    if (ionList != NULL) {
        free(ionList);
        ionList = NULL;
    }
    //DELETING TRANSPORTERS FROM MEMORY
    for (int i = 0; i < numOfTransporters; ++i) {
        if (realTransporterList[i] != NULL) {
            delete realTransporterList[i];
            realTransporterList[i] = NULL;
        }
    }
    if (realTransporterList != NULL) {
        free(realTransporterList);
        realTransporterList = NULL;
    }
    if (transporterList != NULL) {
        free(transporterList);
        transporterList = NULL;
    }
    if (noOfEachTransporter != NULL) {
        free(noOfEachTransporter);
        noOfEachTransporter = NULL;
    }
    //DELETING CELL AND ECF
    if (cell != NULL) {
        delete cell; cell = NULL;
    }
    if (ecf != NULL) {
        delete ecf; ecf = NULL;
    }
    //DELETING LIST OF DOUBLES
    if (doubleList != NULL) {
        free(doubleList);
        doubleList = NULL;
    }
    //DELETING LIST OF PARAMETERS
    for (int i = 0; i < numOfDoubles; ++i) {
        if (realParameterList[i] != NULL) {
            delete realParameterList[i];
            realParameterList[i] = NULL;
        }
    }
    if (realParameterList != NULL) {
        free(realParameterList);
        realParameterList = NULL;
    }
    //DELETING LIST OF WHAT TO SAVE
    if (dataWhatToSave != NULL) {
        free(dataWhatToSave);
        dataWhatToSave = NULL;
    }
    //DELETING LIST OF ALL OBJECTS
    if (allObjectList != NULL) {
        free(allObjectList);
        allObjectList = NULL;
    }
    //DELETING LIST OF OBJECTS TO BE SAVED
    if (objectsToBeSaved != NULL) {
        free(objectsToBeSaved);
        objectsToBeSaved = NULL;
    }
    //DELETING BUFFER OF SAVED DATA
    if (tableOfSavedValues != NULL) {
        for (int i = 0; i < realNumOfDataToSave; ++i) {
            if (tableOfSavedValues[i] != NULL) {
                free(tableOfSavedValues[i]);
                tableOfSavedValues[i] = NULL;
            }
        }
        free(tableOfSavedValues);
        tableOfSavedValues = NULL;
    }
    //DELETING TABLE OF TIME
    if (tableTime != NULL) {
        free(tableTime);
        tableTime = NULL;
    }
    //DELETING FILESTREAM TABLE AND FILESTREAMS
    for (int i = 0; i < realNumOfDataToSave; ++i) {
        if (fileStreamTable[i] != NULL) {
            fileStreamTable[i]->close();		//file should be closed first
            delete fileStreamTable[i];
            fileStreamTable[i] = NULL;
        }
    }
    if (fileStreamTable != NULL) {
        free(fileStreamTable);
        fileStreamTable = NULL;
    }
    if (fileStreamTime != NULL) {
        fileStreamTime->close();
        delete fileStreamTime;
        fileStreamTime = NULL;
    }
}

}

