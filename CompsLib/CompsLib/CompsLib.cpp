// Comps.cpp : Definiuje funkcje biblioteki statycznej.
//

#include "pch.h"
#include "framework.h"
#include "CompsLib.h"
#include <cstdlib>
#include <cmath>
#include <sstream>

namespace Comps {

	COMPSLIB_API const std::string transportersNames[TRANSPORTERS_COUNT] = {
		"SimpleNaChannel",
		"SimpleKChannel",
		"SimpleClChannel",
		"NaKATPase",
		"SimpleNaChannel2",	//user added experiment
		"TRPChannel",
		"ClvChannel"
	};
	COMPSLIB_API const int transportersParamsNumbers[TRANSPORTERS_COUNT] = { 1,1,1,1,1,1,1 };	//user modified (from 4) experiment
	COMPSLIB_API const std::string ionsNames[IONS_COUNT] = {
		"Na",
		"K",
		"Cl",
		"Impermeant",
		"Ca"
	};

	COMPSLIB_API const std::string parameterNames[PARAMETERS_COUNT] = {
		"MP", "temperatura", "krok czasowy", "objetosc komorki", "objetosc ECF", "nieskonczonosc ECF", "czas trwania symulacji",
		"pojemnosc elektryczna blony", "przewodnosc blony dla wody", "pole powierzchni komorki", "zapisuj co ... iteracje", "dlugosc bufora zapisu"
	};
	COMPSLIB_API const std::string parameterUnits[PARAMETERS_COUNT] = {
		"mV", "K", "s", "pl", "pl", "1", "s", "F/m2", "l^2/(Js)", "m^2", "1", "1"
	};

	COMPSLIB_API std::string* paramNamesPointers[TRANSPORTERS_COUNT] = { NULL, NULL, NULL, NULL, NULL, NULL };	//user modified (from 4) experiment
	COMPSLIB_API std::string* paramUnitsPointers[TRANSPORTERS_COUNT] = { NULL, NULL, NULL, NULL, NULL, NULL };	//user modified (from 4) experiment

	COMPSLIB_API void (*fun_ptr_createTransporter_arr[TRANSPORTERS_COUNT])(double*, int*, Transporter*&, int) = { 
		createSimpleNaChannel, 
		createSimpleKChannel, 
		createSimpleClChannel, 
		createNaKATPase,
		createSimpleNaChannel2,	//user added experiment
		createTRPChannel,
		createClvChannel
	};

	COMPSLIB_API std::string SimpleNaChannel::paramNames[] = { "przewodnosc" };
	COMPSLIB_API std::string SimpleKChannel::paramNames[] = { "przewodnosc" };
	COMPSLIB_API std::string SimpleClChannel::paramNames[] = { "przewodnosc" };
	COMPSLIB_API std::string NaKATPase::paramNames[] = { "moc" };
	COMPSLIB_API std::string SimpleNaChannel2::paramNames[] = { "przewodnosc" };	//user added experiment
	COMPSLIB_API std::string TRPChannel::paramNames[] = { "przewodnosc" };
	COMPSLIB_API std::string ClvChannel::paramNames[] = { "przewodnosc" };

	COMPSLIB_API std::string SimpleNaChannel::paramUnits[] = { "pS" };
	COMPSLIB_API std::string SimpleKChannel::paramUnits[] = { "pS" };
	COMPSLIB_API std::string SimpleClChannel::paramUnits[] = { "pS" };
	COMPSLIB_API std::string NaKATPase::paramUnits[] = { "pA" };
	COMPSLIB_API std::string SimpleNaChannel2::paramUnits[] = { "pS" };	//user added experiment
	COMPSLIB_API std::string TRPChannel::paramUnits[] = { "pS" };
	COMPSLIB_API std::string ClvChannel::paramUnits[] = { "pS" };

	//LIBRARY INITIALIZATION FUNCTION
	//CREATES TABLE OF POINTERS TO PARAM LISTS OF EACH TRANSPORTER

	void InitializeLibrary() {
		paramNamesPointers[0] = SimpleNaChannel::paramNames;
		paramNamesPointers[1] = SimpleKChannel::paramNames;
		paramNamesPointers[2] = SimpleClChannel::paramNames;
		paramNamesPointers[3] = NaKATPase::paramNames;
		paramNamesPointers[4] = SimpleNaChannel2::paramNames;	//user added experiment
		paramNamesPointers[5] = TRPChannel::paramNames;
		paramNamesPointers[6] = ClvChannel::paramNames;

		paramUnitsPointers[0] = SimpleNaChannel::paramUnits;
		paramUnitsPointers[1] = SimpleKChannel::paramUnits;
		paramUnitsPointers[2] = SimpleClChannel::paramUnits;
		paramUnitsPointers[3] = NaKATPase::paramUnits;
		paramUnitsPointers[4] = SimpleNaChannel2::paramUnits;
		paramUnitsPointers[5] = TRPChannel::paramUnits;
		paramUnitsPointers[6] = ClvChannel::paramUnits;
	}

	//FUNCTIONS CREATING COMPONENTS
	void createSimpleNaChannel(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {
		std::string channelName("");
		if (ktoryToJuz == 1)
			channelName = "SimpleNaChannel";
		else {
			channelName = "SimpleNaChannel_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12; //entry is in pS, so needs to be converted to basic form
		ptr = new SimpleNaChannel(channelName, conductance);
	}

	void createSimpleKChannel(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {
		std::string channelName("");
		if (ktoryToJuz == 1)
			channelName = "SimpleKChannel";
		else {
			channelName = "SimpleKChannel_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12;	//as above
		ptr = new SimpleKChannel(channelName, conductance);
	}

	void createSimpleClChannel(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {
		std::string channelName("");
		if (ktoryToJuz == 1)
			channelName = "SimpleClChannel";
		else {
			channelName = "SimpleClChannel_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12;	//as above
		ptr = new SimpleClChannel(channelName, conductance);
	}

	void createNaKATPase(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {
		std::string pumpName("");
		if (ktoryToJuz == 1)
			pumpName = "NaKATPase";
		else {
			pumpName = "NaKATPase_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12;	//as above - here entry is in pA
		ptr = new NaKATPase(pumpName, conductance);
	}

	void createSimpleNaChannel2(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {		//user added experiment
		std::string channelName("");
		if (ktoryToJuz == 1)
			channelName = "SimpleNaChannel2";
		else {
			channelName = "SimpleNaChannel2_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12;	//as above
		ptr = new SimpleNaChannel(channelName, conductance);
	}

	void createTRPChannel(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {		//user added experiment
		std::string channelName("");
		if (ktoryToJuz == 1)
			channelName = "TRPChannel";
		else {
			channelName = "TRPChannel_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12;	//as above
		ptr = new TRPChannel(channelName, conductance);
	}

	void createClvChannel(double* doubleList, int* intList, Transporter*& ptr, int ktoryToJuz) {		//user added experiment
		std::string channelName("");
		if (ktoryToJuz == 1)
			channelName = "ClvChannel";
		else {
			channelName = "ClvChannel_" + std::to_string(ktoryToJuz);
		}
		double conductance = doubleList[0] * 1e-12;	//as above
		ptr = new ClvChannel(channelName, conductance);
	}

	//GEOMETRY ISSUES

	double CountSurfaceAreaFromVolume(double volume, Geom geometry) {
		if (geometry == Geom::GEOM_SPHERE) {
			double r = std::cbrt(((volume * 0.75) / PI));
			r *= 0.1;	//z dm na m
			double surface = 4.0 * PI * (pow(r, 2));
			return surface;
		}
		else {
			return -1.0;
		}
	}

	//WATER FLOW ISSUES

	double MakeWaterFlow(double sumOfIn, double sumOfOut, double** doubleList) {
		double temp = *(doubleList[1]);
		double pressureIn = R * temp * sumOfIn;
		double pressureOut = R * temp * sumOfOut;
		double cond = *(doubleList[8]);
		double flow = cond * (pressureOut - pressureIn);	//dodatnie dV idzie poza komorke
		double dt = *(doubleList[2]);
		double dV = flow * dt;
		return dV;
	}

	//MY TRANSFORMATION FROM DOUBLE TO STRING
	std::string my_to_string(const double a_value, const int n = 6)
	{
		std::ostringstream out;
		out.precision(10);
		out << std::scientific << a_value;
		return std::move(out).str();
	}

	//OBJECT CLASS
	std::string Object::GetName() {
		return (std::to_string(type) + std::to_string(id));
	}

	std::string Object::DescribeMyself() {
		return ("EmptyObject");
	}

	//PARAMETER CLASS
	std::string Parameter::DescribeMyself() {
		std::string s = name + " = " + my_to_string(*valuePtr) + unit;
		return s;
	}

	//ION CLASS
	Ion::Ion(double ionConc, double ionMoles, double ionCharge, std::string ionName, char t, int id) : Object(t, id, false) {
		c = ionConc * 1e-3;	//entry in mmol/l
		n = ionMoles;
		charge = ionCharge;
		name = ionName;
		dn = 0.0;
	}

	std::string Ion::DescribeMyself() {
		return (name + "; charge=" + std::to_string(charge) + "; concentration=" + std::to_string(c*1e3) + "mM");
	}

	void Ion::IncrementDeltaOnConcentration(double volume) {
		n += dn;
		dn = 0.0;
		c = n / volume;
	}

	//TRANSPORTER CLASS
	Transporter::Transporter(std::string transporterName, char t, int i) : Object(t, i, false) {
		name = transporterName;
		current = 0.0;
	}

	std::string Transporter::DescribeMyself() {
		return name;
	}

	//SIMPLE_Na_CHANNEL CLASS
	SimpleNaChannel::SimpleNaChannel(std::string channelName, double cond) : Transporter(channelName, 'T', 0) {
		conductance = cond;
	}

	void SimpleNaChannel::Run(double** doubleList, Ion** ionList) {
		double e = *(doubleList[0]);
		double temp = *(doubleList[1]);
		double dt = *(doubleList[2]);
		double i = conductance * (e - ((R * temp / F) * log((ionList[0]->GetC()) / (ionList[1]->GetC()))));
		double dn = (i * dt) / F;
		current = i;
		ionList[0]->IncrementDn(dn);
		ionList[1]->IncrementDn(-dn);
	}

	std::string SimpleNaChannel::DescribeMyself() {
		std::string s = "Simple Na channel, conductance: " + std::to_string(conductance*1e12) + "pS";
		return s;
	}

	//SIMPLE_K_CHANNEL CLASS

	SimpleKChannel::SimpleKChannel(std::string channelName, double cond) : Transporter(channelName, 'T', 1) {
		conductance = cond;
	}

	void SimpleKChannel::Run(double** doubleList, Ion** ionList) {
		double e = *(doubleList[0]);
		double temp = *(doubleList[1]);
		double dt = *(doubleList[2]);
		double i = conductance * (e - ((R * temp / F) * log((ionList[2]->GetC()) / (ionList[3]->GetC()))));
		double dn = (i * dt) / F;
		current = i;
		ionList[2]->IncrementDn(dn);
		ionList[3]->IncrementDn(-dn);
	}

	std::string SimpleKChannel::DescribeMyself() {
		std::string s = "Simple K channel, conductance: " + std::to_string(conductance*1e12) + "pS";
		return s;
	}

	//SIMPLE_CL_CHANNEL CLASS

	SimpleClChannel::SimpleClChannel(std::string channelName, double cond) : Transporter(channelName, 'T', 2) {
		conductance = cond;
	}

	void SimpleClChannel::Run(double** doubleList, Ion** ionList) {
		double e = *(doubleList[0]);
		double temp = *(doubleList[1]);
		double dt = *(doubleList[2]);
		double i = conductance * (e - ((R * temp / F) * log((ionList[5]->GetC()) / (ionList[4]->GetC()))));
		double dn = (i * dt) / F;
		current = i;
		ionList[5]->IncrementDn(dn);
		ionList[4]->IncrementDn(-dn);
	}

	std::string SimpleClChannel::DescribeMyself() {
		std::string s = "Simple Cl channel, conductance: " + std::to_string(conductance*1e12) + "pS";
		return s;
	}

	//NAKATPASE CLASS

	NaKATPase::NaKATPase(std::string pumpName, double p) : Transporter(pumpName, 'T', 3) {
		power = p;
	}

	void NaKATPase::Run(double** doubleList, Ion** ionList) {
		double dt = *(doubleList[2]);
		double i = power;
		double dn = (i * dt) / F;
		current = i;
		ionList[0]->IncrementDn(3 * dn);
		ionList[1]->IncrementDn(-3 * dn);
		ionList[2]->IncrementDn(-2 * dn);
		ionList[3]->IncrementDn(2 * dn);
	}

	std::string NaKATPase::DescribeMyself() {
		std::string s = "Na-K ATPase, power: " + std::to_string(power*1e12) + "pA";
		return s;
	}

	//SIMPLE_Na_CHANNEL2 CLASS						//user added experiment
	SimpleNaChannel2::SimpleNaChannel2(std::string channelName, double cond) : Transporter(channelName, 'T', 4) {
		conductance = cond;
	}

	void SimpleNaChannel2::Run(double** doubleList, Ion** ionList) {
		double e = *(doubleList[0]);
		double temp = *(doubleList[1]);
		double dt = *(doubleList[2]);
		double i = conductance * (e - ((R * temp / F) * log((ionList[0]->GetC()) / (ionList[1]->GetC()))));
		double dn = (i * dt) / F;
		current = i;
		ionList[0]->IncrementDn(dn);
		ionList[1]->IncrementDn(-dn);
	}

	std::string SimpleNaChannel2::DescribeMyself() {
		std::string s = "Simple Na channel 2, conductance: " + std::to_string(conductance*1e12) + "pS";
		return s;
	}

	//TRP Channel
	TRPChannel::TRPChannel(std::string channelName, double cond) : Transporter(channelName, 'T', 5) {
		conductance = cond;
	}

	void TRPChannel::Run(double** doubleList, Ion** ionList) {
		double e = *(doubleList[0]);
		if (e < 0.0) {
			current = 0.0;
			return;
		}
		double temp = *(doubleList[1]);
		double dt = *(doubleList[2]);

		double eNernst = (R * temp / F) * log((ionList[0]->GetC()) / (ionList[1]->GetC()));
		double eNernstCa = (R * temp / (2.0 * F)) * log((ionList[8]->GetC()) / (ionList[9]->GetC()));
		
		double i = conductance * (e - eNernst) * (exp(temp - 308.0));
		double iCa = conductance * (e - eNernstCa) * (exp(temp - 308.0));
		double dnNa = (i * dt) / F;
		double dnCa = (iCa * dt) / (2.0 * F);
		current = i;
		ionList[0]->IncrementDn(dnNa);
		ionList[1]->IncrementDn(-dnNa);
		ionList[8]->IncrementDn(dnCa);
		ionList[9]->IncrementDn(-dnCa);
	}

	std::string TRPChannel::DescribeMyself() {
		std::string s = "TRP channel for Na and Ca, conductance: " + std::to_string(conductance * 1e12) + "pS";
		return s;
	}

	//Clv channel
	ClvChannel::ClvChannel (std::string channelName, double cond) : Transporter(channelName, 'T', 6) {
		conductance = cond;
	}

	void ClvChannel::Run(double** doubleList, Ion** ionList) {
		double e = *(doubleList[0]);
		if (e < 0.0) {
			current = 0.0;
			return;
		}
		double temp = *(doubleList[1]);
		double dt = *(doubleList[2]);

		double eNernst = (R * temp / (F)) * log((ionList[5]->GetC()) / (ionList[4]->GetC()));
		double i = conductance * (e - eNernst);
		double dn = (i * dt) / F;
		current = i;
		ionList[5]->IncrementDn(dn);
		ionList[4]->IncrementDn(-dn);
	}

	std::string ClvChannel::DescribeMyself() {
		std::string s = "Clv channel, conductance: " + std::to_string(conductance * 1e12) + "pS";
		return s;
	}

}
