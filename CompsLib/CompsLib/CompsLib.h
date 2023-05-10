#pragma once

#ifdef COMPSLIB_EXPORTS
#define COMPSLIB_API __declspec(dllexport)
#else
#define COMPSLIB_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

namespace Comps {

	class Transporter;
	class Ion;

	extern "C" COMPSLIB_API void createSimpleNaChannel(double*, int*, Transporter*&, int);
	extern "C" COMPSLIB_API void createSimpleKChannel(double*, int*, Transporter*&, int);
	extern "C" COMPSLIB_API void createSimpleClChannel(double*, int*, Transporter*&, int);
	extern "C" COMPSLIB_API void createNaKATPase(double*, int*, Transporter*&, int);
	extern "C" COMPSLIB_API void createSimpleNaChannel2(double*, int*, Transporter*&, int);	//user added experiment
	extern "C" COMPSLIB_API void createTRPChannel(double*, int*, Transporter*&, int);
	extern "C" COMPSLIB_API void createClvChannel(double*, int*, Transporter*&, int);

	static const double R = 8.31;
	static const double F = 96500.0;
	static const double PI = 3.14;

	static const int TRANSPORTERS_COUNT = 7;	//user modified (from 4) experiment
	static const int IONS_COUNT = 5;
	static const int DOUBLES_COUNT = 12;
	static const int PARAMETERS_COUNT = 12;

	COMPSLIB_API extern const std::string transportersNames[TRANSPORTERS_COUNT];
	COMPSLIB_API extern const int transportersParamsNumbers[TRANSPORTERS_COUNT];
	COMPSLIB_API extern std::string* paramNamesPointers[TRANSPORTERS_COUNT];
	COMPSLIB_API extern std::string* paramUnitsPointers[TRANSPORTERS_COUNT];

	COMPSLIB_API extern const std::string ionsNames[IONS_COUNT];

	COMPSLIB_API extern const std::string parameterNames[PARAMETERS_COUNT];
	COMPSLIB_API extern const std::string parameterUnits[PARAMETERS_COUNT];

	extern "C" COMPSLIB_API void InitializeLibrary();

	COMPSLIB_API extern void (*fun_ptr_createTransporter_arr[TRANSPORTERS_COUNT])(double*, int*, Transporter*&, int);

	//STRUCTURES TO SEND/RECEIVE DATA

	struct COMPSLIB_API TIon {
		int id;
		double concentrationOut;
		double concentrationIn;
		double charge;
	};

	struct COMPSLIB_API TTransporter {
		int id;
		std::vector<double> params;
	};

	struct COMPSLIB_API TParameter {
		int id;
		double value;
	};

	struct COMPSLIB_API WhatToSave {
		char type;	//transporters, ions, params (T, I, P)
		int id;
	};

	//ULTRA BASE CLASS - FOR SAVING PURPOSES MAINLY

	class COMPSLIB_API Object {
	public:
		Object() : type('X'), id(0), needsToBeSaved(false) {}
		Object(char t, int i, bool saved) : type(t), id(i), needsToBeSaved(saved) {}
		char GetType() { return type; }
		int GetId() { return id; }
		void SetNeedToBeSaved(bool b) { needsToBeSaved = b; }
		bool GetNeedToBeSaved() { return needsToBeSaved; }
		virtual double ReturnDataToSave() = 0;
		virtual std::string GetName();
		virtual std::string DescribeMyself();
		virtual std::string ReturnUnit() { return "1"; }
		virtual ~Object() {}
	protected:
		char type;
		int id;
		bool needsToBeSaved;
	};

	//GENERAL BASE CLASSES

	class COMPSLIB_API Ion : public Object {
	public:
		Ion() : c(0.0), n(0.0), charge(0.0), dn(0.0), name(""), Object('I', -1, false) {}
		Ion(double, double, double, std::string, char, int);
		double GetC() { return c; }
		double GetN() { return n; }
		double GetCharge() { return charge; }
		void SetC(double concentration) { c = concentration; }
		void SetN(double moles) { n = moles; }
		void SetCharge(double ionCharge) { charge = ionCharge; }
		void SetName(std::string ionName) { name = ionName; }
		std::string GetName() { return name; }	//override
		void IncrementDn(double inc) { dn += inc; }
		double GetDN() { return dn; }
		void IncrementDeltaOnConcentration(double volume);
		double ReturnDataToSave() { return c; }	//override
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return "M"; }	//override
		~Ion() {}
	private:
		double c, n, charge, dn;
		std::string name;
	};

	class COMPSLIB_API Transporter : public Object {
	public:
		Transporter() : name(""), current(0.0), Object('T', -1, false) {}
		Transporter(std::string, char, int);
		virtual void Run(double** doubleList, Ion** ionList) = 0;
		double ReturnDataToSave() { return current; }	//override
		double GetCurrent() { return current; }
		void SetName(std::string transporterName) { name = transporterName; }
		std::string GetName() { return name; }	//override
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return "A"; }	//override
		virtual ~Transporter() {}
	protected:
		std::string name;
		double current;
	};

	class COMPSLIB_API Parameter : public Object {
	public:
		Parameter() : valuePtr(NULL), Object('P', -1, false) {}
		Parameter(double* ptr, char c, int i, std::string s, std::string u) : name(s), valuePtr(ptr), unit(u), Object(c, i, false) {}
		double ReturnDataToSave() { return *valuePtr; }	//override
		std::string GetName() { return name; }	//override
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~Parameter() {}
	private:
		std::string name;
		std::string unit;
		double* valuePtr;
	};

	//GEOMETRY ISSUES
	enum class COMPSLIB_API Geom { GEOM_SPHERE, GEOM_CUBE, GEOM_CYLINDER };
	extern "C" COMPSLIB_API double CountSurfaceAreaFromVolume(double volume, Geom geometry);

	//WATER FLOW ISSUES
	extern "C" COMPSLIB_API double MakeWaterFlow(double sumOfIn, double sumOfOut, double** doubleList);		//zwraca dV - bardzo wazna roznica!

	//PARTICULAR CLASSES

	//TRANSPORTERS
	class COMPSLIB_API SimpleNaChannel : public Transporter {
	public:
		SimpleNaChannel() : conductance(0.0), Transporter() {}
		SimpleNaChannel(std::string channelName, double cond);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~SimpleNaChannel() {}
	private:
		double conductance;
		const std::string unit = "A";
	};

	class COMPSLIB_API SimpleKChannel : public Transporter {
	public:
		SimpleKChannel() {}
		SimpleKChannel(std::string channelName, double cond);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~SimpleKChannel() {}
	private:
		double conductance;
		const std::string unit = "A";
	};

	class COMPSLIB_API SimpleClChannel : public Transporter {
	public:
		SimpleClChannel() {}
		SimpleClChannel(std::string channelName, double cond);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~SimpleClChannel() {}
	private:
		double conductance;
		const std::string unit = "A";
	};

	class COMPSLIB_API NaKATPase : public Transporter {
	public:
		NaKATPase() {}
		NaKATPase(std::string pumpName, double p);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~NaKATPase() {}
	private:
		double power;
		const std::string unit = "A";
	};

	class COMPSLIB_API SimpleNaChannel2 : public Transporter {		//user added experiment
	public:
		SimpleNaChannel2() : conductance(0.0), Transporter() {}
		SimpleNaChannel2(std::string channelName, double cond);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~SimpleNaChannel2() {}
	private:
		double conductance;
		const std::string unit = "A";
	};

	class COMPSLIB_API TRPChannel : public Transporter {		//user added experiment
	public:
		TRPChannel() : conductance(0.0), Transporter() {}
		TRPChannel(std::string channelName, double cond);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~TRPChannel() {}
	private:
		double conductance;
		const std::string unit = "A";
	};

	class COMPSLIB_API ClvChannel : public Transporter {		//user added experiment
	public:
		ClvChannel() : conductance(0.0), Transporter() {}
		ClvChannel(std::string channelName, double cond);
		static std::string paramNames[1];
		static std::string paramUnits[1];
		void Run(double** doubleList, Ion** ionList);
		std::string DescribeMyself();	//override
		std::string ReturnUnit() { return unit; }	//override
		~ClvChannel() {}
	private:
		double conductance;
		const std::string unit = "A";
	};

}
