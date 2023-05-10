#include "UsefulFunctions.h"

void utworzPrzykladoweDaneWejsciowe(std::vector<Comps::TIon>* tabIonDoStworzenia,
                                    std::vector<Comps::TTransporter>* tabTransporterDoStworzenia,
                                    std::vector<Comps::TParameter>* tabParametryDoStworzenia,
                                    std::vector<Comps::WhatToSave>* tabNazwyDoZapisywania,
                                    int* ileIons, int* ileTransporters, int* ileParameters, int* ileNazwDoZapisywania) {
    Comps::TIon ion;
    Comps::TTransporter transporter;
    Comps::TParameter parameter;
    Comps::WhatToSave dataToSave;
    std::vector<double> params1, params2, params3, params4, params5;
    params1.clear();
    params2.clear();
    params3.clear();
    params4.clear();
    params5.clear();
    //CLEAR EVERYTHING
    tabIonDoStworzenia->clear();
    tabTransporterDoStworzenia->clear();
    tabNazwyDoZapisywania->clear();
    tabParametryDoStworzenia->clear();
    //IONS
    //Na
    ion.charge = 1.0;
    ion.concentrationIn = 34.9;
    ion.concentrationOut = 34.9;
    ion.id = 0;
    tabIonDoStworzenia -> push_back(ion);
    //K
    ion.charge = 1.0;
    ion.concentrationIn = 117.3;
    ion.concentrationOut = 117.3;
    ion.id = 1;
    tabIonDoStworzenia -> push_back(ion);
    //Cl
    ion.charge = -1.0;
    ion.concentrationIn = 2.2;
    ion.concentrationOut = 117.2;
    ion.id = 2;
    tabIonDoStworzenia -> push_back(ion);
    //Impermeants
    ion.charge = -1.0;
    ion.concentrationIn = 150.0;
    ion.concentrationOut = 35.0;
    ion.id = 3;
    tabIonDoStworzenia -> push_back(ion);
    (*ileIons) = 4;
    //TRANSPORTERS
    //Na channel
    transporter.id = 0;
    params1.push_back(4e-9);
    transporter.params = params1;
    tabTransporterDoStworzenia -> push_back(transporter);
    //K channel
    transporter.id = 1;
    params2.push_back(4e-7);
    transporter.params = params2;
    tabTransporterDoStworzenia -> push_back(transporter);
    //Cl channel
    transporter.id = 2;
    params3.push_back(4e-9);
    transporter.params = params3;
    tabTransporterDoStworzenia -> push_back(transporter);
    //NaK pump
    transporter.id = 3;
    params4.push_back(1e-9);
    transporter.params = params4;
    tabTransporterDoStworzenia -> push_back(transporter);
    //Na channel2 - added from user code test
    transporter.id = 4;
    params5.push_back(9e-9);
    transporter.params = params5;
    tabTransporterDoStworzenia->push_back(transporter);

    (*ileTransporters) = 5;	//includes test of user code
    //DOUBLES(PARAMS)
    (*ileParameters) = 0;
    parameter.id = 0; parameter.value = 0.0; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//e
    parameter.id = 1; parameter.value = 298.0; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//temp
    parameter.id = 2; parameter.value = 0.000000035; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//dt
    parameter.id = 3; parameter.value = 25.84e-12; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//v cell
    parameter.id = 4; parameter.value = 5.182e-12; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//v ecf
    parameter.id = 5; parameter.value = 0.0; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//no infinite ecf
    parameter.id = 6; parameter.value = 0.00035; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;	//tmax
    parameter.id = 7; parameter.value = 0.0050; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;//capacitance per m2
    parameter.id = 8; parameter.value = 1.7e-10; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;//waterCond
    parameter.id = 9; parameter.value = 15.34e-9; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;//surfaceArea -> fake
    parameter.id = 10; parameter.value = 1.0; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;//save every.... (int)
    parameter.id = 11; parameter.value = 50.0; tabParametryDoStworzenia->push_back(parameter); (*ileParameters)++;//saving buffer length (int)
    //NAZWY DO ZAPISYWANIA
    (*ileNazwDoZapisywania) = 0;
    dataToSave.type = 'I'; dataToSave.id = 1; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//Na_in
    dataToSave.type = 'I'; dataToSave.id = 2; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//K_out
    dataToSave.type = 'T'; dataToSave.id = 0; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//SimpleNaChannel
    dataToSave.type = 'T'; dataToSave.id = 3; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//NaKATPase
    dataToSave.type = 'T'; dataToSave.id = 4; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//SimpleNaChannel2
    dataToSave.type = 'P'; dataToSave.id = 0; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//MP
    dataToSave.type = 'P'; dataToSave.id = 3; tabNazwyDoZapisywania->push_back(dataToSave); (*ileNazwDoZapisywania)++;		//v_cell
}
