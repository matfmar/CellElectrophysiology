
#ifndef USEFULFUNCTIONS_H
#define USEFULFUNCTIONS_H

#include <vector>
#include "CompsLib.h"

void utworzPrzykladoweDaneWejsciowe(std::vector<Comps::TIon>* tabIonDoStworzenia,
                                    std::vector<Comps::TTransporter>* tabTransporterDoStworzenia,
                                    std::vector<Comps::TParameter>* tabParametryDoStworzenia,
                                    std::vector<Comps::WhatToSave>* tabNazwyDoZapisywania,
                                    int* ileIons, int* ileTransporters, int* ileParameters, int* ileNazwDoZapisywania);

#endif // USEFULFUNCTIONS_H
