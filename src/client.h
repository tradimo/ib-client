#pragma once

#include "EWrapper.h"
#include "EReader.h"
#include "EClientSocket.h"

enum class state {
	connect,
	contractDetails,
	continuoseFuture,
	headTimespamp,
	historicalData,
	delayedTickData,
	idle
};

class CClient : public EWrapper
{
public:
	CClient(void);
	
	void setConnectOptions(const std::string& connectOptions) { m_pClientSocket->setConnectOptions(connectOptions); }
	bool connect(std::string addr, int port);
	bool isConnected(void) const { return m_pClientSocket->isConnected(); }
	void processMessages(void);
	

	// From EWrapper
	virtual void tickPrice(TickerId tickerId, TickType field, double price, const TickAttrib& attrib) override
	{
		printf("Tick Price. Ticker Id: %ld, Field: %d, Price: %g, CanAutoExecute: %d, PastLimit: %d, PreOpen: %d\n", tickerId, static_cast<int>(field), price, attrib.canAutoExecute, attrib.pastLimit, attrib.preOpen);
	}
	virtual void tickSize(TickerId tickerId, TickType field, int size) override
	{
		printf("Tick Size. Ticker Id: %ld, Field: %d, Size: %d\n", tickerId, static_cast<int>(field), size);
	}
	virtual void tickOptionComputation(TickerId tickerId, TickType tickType, double impliedVol, double delta, double optPrice, double pvDividend, double gamma, double vega, double theta, double undPrice) override { printf("callback 03\n"); }
	virtual void tickGeneric(TickerId tickerId, TickType tickType, double value) override { printf("callback 04\n"); }
	virtual void tickString(TickerId tickerId, TickType tickType, const std::string& value) override
	{
		printf("Tick String. Ticker Id: %ld, Type: %d, Value: %s\n", tickerId, static_cast<int>(tickType), value.c_str());
	}
	virtual void tickEFP(TickerId tickerId, TickType tickType, double basisPoints, const std::string& formattedBasisPoints, double totalDividends, int holdDays, const std::string& futureLastTradeDate, double dividendImpact, double dividendsToLastTradeDate) override { printf("callback 06\n"); }
	virtual void orderStatus(OrderId orderId, const std::string& status, double filled, double remaining, double avgFillPrice, int permId, int parentId, double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice) override { printf("callback 07\n"); }
	virtual void openOrder(OrderId orderId, const Contract&, const Order&, const OrderState&) override { printf("callback 08\n"); }
	virtual void openOrderEnd() override { printf("callback 09\n"); }
	virtual void winError(const std::string& str, int lastError) override { printf("callback 10\n"); }
	virtual void connectionClosed() override { printf("callback 11\n"); }
	virtual void updateAccountValue(const std::string& key, const std::string& val, const std::string& currency, const std::string& accountName) override { printf("callback 12\n"); }
	virtual void updatePortfolio(const Contract& contract, double position, double marketPrice, double marketValue, double averageCost, double unrealizedPNL, double realizedPNL, const std::string& accountName) override { printf("callback 13\n"); }
	virtual void updateAccountTime(const std::string& timeStamp) override { printf("callback 14\n"); }
	virtual void accountDownloadEnd(const std::string& accountName) override { printf("callback 15\n"); }
	virtual void nextValidId(OrderId orderId) override { 
		printf("Next Valid Id: %ld\n", orderId);
		// m_state = state::contractDetails;
		// m_state = state::continuoseFuture;
		// m_state = state::headTimespamp;
		// m_state = state::historicalData;
		m_state = state::delayedTickData;
	}
	
	static void printContractMsg(const Contract& contract) 
	{
		printf("\tConId: %ld\n", contract.conId);
		printf("\tSymbol: %s\n", contract.symbol.c_str());
		printf("\tSecType: %s\n", contract.secType.c_str());
		printf("\tLastTradeDateOrContractMonth: %s\n", contract.lastTradeDateOrContractMonth.c_str());
		printf("\tStrike: %g\n", contract.strike);
		printf("\tRight: %s\n", contract.right.c_str());
		printf("\tMultiplier: %s\n", contract.multiplier.c_str());
		printf("\tExchange: %s\n", contract.exchange.c_str());
		printf("\tPrimaryExchange: %s\n", contract.primaryExchange.c_str());
		printf("\tCurrency: %s\n", contract.currency.c_str());
		printf("\tLocalSymbol: %s\n", contract.localSymbol.c_str());
		printf("\tTradingClass: %s\n", contract.tradingClass.c_str());
	}
	
	virtual void contractDetails(int reqId, const ContractDetails& contractDetails) override 
	{ 
		printf("ContractDetails begin. ReqId: %d\n", reqId);
		printContractMsg(contractDetails.contract);
		//printContractDetailsMsg(contractDetails);
		printf("ContractDetails end. ReqId: %d\n", reqId);
	}
	virtual void bondContractDetails(int reqId, const ContractDetails& contractDetails) override { printf("callback 18\n"); }
	virtual void contractDetailsEnd(int reqId) override { 
		printf("ContractDetailsEnd. %d\n", reqId);
	}
	virtual void execDetails(int reqId, const Contract& contract, const Execution& execution) override { printf("callback 20\n"); }
	virtual void execDetailsEnd(int reqId) override { printf("callback 21\n"); }
	virtual void error(int id, int errorCode, const std::string& errorString) override { 
		printf("Error. Id: %d, Code: %d, Msg: %s\n", id, errorCode, errorString.c_str());
	}
	virtual void updateMktDepth(TickerId id, int position, int operation, int side, double price, int size) override { printf("callback 23\n"); }
	virtual void updateMktDepthL2(TickerId id, int position, const std::string& marketMaker, int operation, int side, double price, int size, bool isSmartDepth) override { printf("callback 24\n"); }
	virtual void updateNewsBulletin(int msgId, int msgType, const std::string& newsMessage, const std::string& originExch) override { printf("callback 25\n"); }
	virtual void managedAccounts(const std::string& accountsList) override { 
		printf("Account List: %s\n", accountsList.c_str());
	}
	virtual void receiveFA(faDataType pFaDataType, const std::string& cxml) override { printf("callback 27\n"); }
	virtual void historicalData(TickerId reqId, const Bar& bar) override { 
		printf("HistoricalData. ReqId: %ld - Date: %s, Open: %g, High: %g, Low: %g, Close: %g, Volume: %lld, Count: %d, WAP: %g\n", reqId, bar.time.c_str(), bar.open, bar.high, bar.low, bar.close, bar.volume, bar.count, bar.wap);
	}
	virtual void historicalDataEnd(int reqId, const std::string& startDateStr, const std::string& endDateStr) override { 
		printf("HistoricalDataEnd. ReqId: %d - Start Date: %s, End Date: %s\n", reqId, startDateStr.c_str(), endDateStr.c_str());
	}
	virtual void scannerParameters(const std::string& xml) override { printf("callback 30\n"); }
	virtual void scannerData(int reqId, int rank, const ContractDetails& contractDetails, const std::string& distance, const std::string& benchmark, const std::string& projection, const std::string& legsStr) override { printf("callback 31\n"); }
	virtual void scannerDataEnd(int reqId) override { printf("callback 32\n"); }
	virtual void realtimeBar(TickerId reqId, long time, double open, double high, double low, double close, long volume, double wap, int count) override { printf("callback 33\n"); }
	virtual void currentTime(long time) override { printf("callback 34\n"); }
	virtual void fundamentalData(TickerId reqId, const std::string& data) override { printf("callback 35\n"); }
	virtual void deltaNeutralValidation(int reqId, const DeltaNeutralContract& deltaNeutralContract) override { printf("callback 36\n"); }
	virtual void tickSnapshotEnd(int reqId) override { printf("callback 37\n"); }
	virtual void marketDataType(TickerId reqId, int marketDataType) override
	{
		printf("MarketDataType. ReqId: %ld, Type: %d\n", reqId, marketDataType);
	}
	virtual void commissionReport(const CommissionReport& commissionReport) override { printf("callback 39\n"); }
	virtual void position(const std::string& account, const Contract& contract, double position, double avgCost) override { printf("callback 40\n"); }
	virtual void positionEnd() override { printf("callback 41\n"); }
	virtual void accountSummary(int reqId, const std::string& account, const std::string& tag, const std::string& value, const std::string& curency) override { printf("callback 42\n"); }
	virtual void accountSummaryEnd(int reqId) override { printf("callback 43\n"); }
	virtual void verifyMessageAPI(const std::string& apiData) override { printf("callback 44\n"); }
	virtual void verifyCompleted(bool isSuccessful, const std::string& errorText) override { printf("callback 45\n"); }
	virtual void displayGroupList(int reqId, const std::string& groups) override { printf("callback 46\n"); }
	virtual void displayGroupUpdated(int reqId, const std::string& contractInfo) override { printf("callback 47\n"); }
	virtual void verifyAndAuthMessageAPI(const std::string& apiData, const std::string& xyzChallange) override { printf("callback 48\n"); }
	virtual void verifyAndAuthCompleted(bool isSuccessful, const std::string& errorText) override { printf("callback 49\n"); }
	virtual void connectAck(void) override { 
		if (m_pClientSocket->asyncEConnect()) m_pClientSocket->startApi();
	}
	virtual void positionMulti(int reqId, const std::string& account, const std::string& modelCode, const Contract& contract, double pos, double avgCost) override { printf("callback 51\n"); }
	virtual void positionMultiEnd(int reqId) override { printf("callback 52\n"); }
	virtual void accountUpdateMulti(int reqId, const std::string& account, const std::string& modelCode, const std::string& key, const std::string& value, const std::string& currency) override { printf("callback 53\n"); }
	virtual void accountUpdateMultiEnd(int reqId) override { printf("callback 54\n"); }
	virtual void securityDefinitionOptionalParameter(int reqId, const std::string& exchange, int underlyingConId, const std::string& tradingClass,const std::string& multiplier, const std::set<std::string>& expirations, const std::set<double>& strikes) override { printf("callback 55\n"); }
	virtual void securityDefinitionOptionalParameterEnd(int reqId) override { printf("callback 56\n"); }
	virtual void softDollarTiers(int reqId, const std::vector<SoftDollarTier> &tiers) override { printf("callback 57\n"); }
	virtual void familyCodes(const std::vector<FamilyCode> &familyCodes) override { printf("callback 58\n"); }
	virtual void symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions) override { printf("callback 59\n"); }
	virtual void mktDepthExchanges(const std::vector<DepthMktDataDescription> &depthMktDataDescriptions) override { printf("callback 60\n"); }
	virtual void tickNews(int tickerId, time_t timeStamp, const std::string& providerCode, const std::string& articleId, const std::string& headline, const std::string& extraData) override { printf("callback 61\n"); }
	virtual void smartComponents(int reqId, const SmartComponentsMap& theMap) override { printf("callback 62\n"); }
	virtual void tickReqParams(int tickerId, double minTick, const std::string& bboExchange, int snapshotPermissions) override
	{
		printf("tickerId: %d, minTick: %g, bboExchange: %s, snapshotPermissions: %u\n", tickerId, minTick, bboExchange.c_str(), snapshotPermissions);
//		m_bboExchange = bboExchange;
	}
	virtual void newsProviders(const std::vector<NewsProvider> &newsProviders) override { printf("callback 64\n"); }
	virtual void newsArticle(int requestId, int articleType, const std::string& articleText) override { printf("callback 65\n"); }
	virtual void historicalNews(int requestId, const std::string& time, const std::string& providerCode, const std::string& articleId, const std::string& headline) override { printf("callback 66\n"); }
	virtual void historicalNewsEnd(int requestId, bool hasMore) override { printf("callback 67\n"); }
	virtual void headTimestamp(int reqId, const std::string& headTimestamp) override { 
		printf("Head time stamp. ReqId: %d - Head time stamp: %s,\n", reqId, headTimestamp.c_str());
	}
	virtual void histogramData(int reqId, const HistogramDataVector& data) override { printf("callback 69\n"); }
	virtual void historicalDataUpdate(TickerId reqId, const Bar& bar) override { printf("callback 70\n"); }
	virtual void rerouteMktDataReq(int reqId, int conid, const std::string& exchange) override { printf("callback 71\n"); }
	virtual void rerouteMktDepthReq(int reqId, int conid, const std::string& exchange) override { printf("callback 72\n"); }
	virtual void marketRule(int marketRuleId, const std::vector<PriceIncrement> &priceIncrements) override { printf("callback 73\n"); }
	virtual void pnl(int reqId, double dailyPnL, double unrealizedPnL, double realizedPnL) override { printf("callback 74\n"); }
	virtual void pnlSingle(int reqId, int pos, double dailyPnL, double unrealizedPnL, double realizedPnL, double value) override { printf("callback 75\n"); }
	virtual void historicalTicks(int reqId, const std::vector<HistoricalTick> &ticks, bool done) override { printf("callback 76\n"); }
	virtual void historicalTicksBidAsk(int reqId, const std::vector<HistoricalTickBidAsk> &ticks, bool done) override { printf("callback 77\n"); }
	virtual void historicalTicksLast(int reqId, const std::vector<HistoricalTickLast> &ticks, bool done) override { printf("callback 78\n"); }
	virtual void tickByTickAllLast(int reqId, int tickType, time_t time, double price, int size, const TickAttribLast& tickAttribLast, const std::string& exchange, const std::string& specialConditions) override { printf("callback 79\n"); }
	virtual void tickByTickBidAsk(int reqId, time_t time, double bidPrice, double askPrice, int bidSize, int askSize, const TickAttribBidAsk& tickAttribBidAsk) override { printf("callback 80\n"); }
	virtual void tickByTickMidPoint(int reqId, time_t time, double midPoint) override { printf("callback 81\n"); }
	virtual void orderBound(long long orderId, int apiClientId, int apiOrderId) override { printf("callback 82\n"); }
	virtual void completedOrder(const Contract& contract, const Order& order, const OrderState& orderState) override { printf("callback 83\n"); }
	virtual void completedOrdersEnd() override { printf("callback 84\n"); }
	

private:
	std::unique_ptr<EClientSocket>	m_pClientSocket;
	EReaderOSSignal					m_readerOSSignal;
	std::unique_ptr<EReader>		m_pReader	= nullptr;
	state							m_state		= state::connect;
};
