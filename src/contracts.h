#pragma once

#include "Contract.h"

class CContracts {
public:
	static Contract simpleFuture(void) {
		Contract res;
		res.symbol = "ES";
		res.secType = "FUT";
		res.exchange = "GLOBEX";
		res.currency = "USD";
		res.lastTradeDateOrContractMonth = "201803";
		res.includeExpired = true;
		return res;
	}

	static Contract continuoseFuture(void) {
		Contract res;
		res.symbol = "ES";
		res.secType = "CONTFUT";
		res.exchange = "GLOBEX";
		return res;
	}

	static Contract eurGbpFx(void) {
		Contract res;
		res.symbol = "EUR";
		res.secType = "CASH";
		res.currency = "GBP";
		res.exchange = "IDEALPRO";
		return res;
	}

	static Contract europeanStock(void) {
		Contract res;
		res.symbol = "NOKIA";
		res.secType = "STK";
		res.currency = "EUR";
		res.exchange = "SMART";
		res.primaryExchange = "HEX";
		return res;
	}

	// There exists also continuose and expire futures:
	// res.secType = "FUT+CONTFUT";

	static Contract crudeOil(void) {
		Contract res;
		res.symbol = "CL";
		res.secType = "FUT";
		res.exchange = "NYMEX";
		res.currency = "USD";
		res.lastTradeDateOrContractMonth = "202009";
		//res.includeExpired = true;
		return res;
	}

	static Contract crudeOilCont(void) {
		Contract res;
		res.symbol = "CL";
		res.secType = "CONTFUT";
		res.exchange = "NYMEX";
		res.currency = "USD";
		return res;
	}
};
