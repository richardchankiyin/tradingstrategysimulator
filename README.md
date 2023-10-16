# tradingstrategysimulator
To illustrate the trading strategy driven by order book change events via order checking. Events will be triggered by strategy through updating position and sending orders

Prerequsite
=========
g++ supporting c++20  

Detail
=======
This is a trading simulation application which support different trading strategy implementations. In our example we have a simple market making algo.  
Assuming there are two markets, primary market with market data in the orderbook and secondary market with best bid/ask only. The concept of market making is to buy low from one market and sell high in another market for the same asset.  

You are assummed to have basic knowledge of FIX message because this is the protocol for order/execution transmission between components.

Build and Run
=========
Run command ***./buildandrun.sh test/SimpleMMTradingStrategyTest***
```
richard@richard-linux:$ ./buildandrun.sh test/SimpleMMTradingStrategyTest
Running all tests.
Running [Global/SimpleMMTradingStrategyInit]: Passed 22 out of 22 tests in 6.2e-05 seconds
Running [Global/SimpleMMTradingStrategyUpdateSecMarket]: Passed 4 out of 4 tests in 1.2e-05 seconds
Running [Global/SimpleMMTradingStrategyOnExecutionIrrelevantSymNoStrategyTriggered]: Passed 1 out of 1 tests in 0.000124 seconds
Running [Global/SimpleMMTradingStrategyOnExecutionNoPriceDiffNoStrategyTriggered]: Passed 1 out of 1 tests in 0.000123 seconds
Running [Global/SimpleMMTradingStrategyOnExecutionPrimaryBidSlightlyHigherThanSecondaryAskNoStrategyTriggered]: Passed 1 out of 1 tests in 0.000152 seconds
Running [Global/SimpleMMTradingStrategyOnExecutionPrimaryBidHigherThanSecondaryAskStrategyTriggered]This is a test case to demonstrate the selling through order book and buy from secondary market
Pre MM Cash Balance: 1e+06 Instrument Balance: 0
preparing to buy secondary and sell primary...
Post Trading processing....
Post MM Cash Balance: 1.00031e+06 Instrument Balance: 0
: Passed 7 out of 7 tests in 0.000267 seconds
Running [Global/SimpleMMTradingStrategyOnExecutionPrimaryAskSlightltLowerThanSecondaryBidNoStrategyTriggered]: Passed 1 out of 1 tests in 0.000142 seconds
Running [Global/SimpleMMTradingStrategyOnExecutionPrimaryAskLowerThanSecondaryBidStrategyTriggered]This is a test case to demonstrate the buying through order book and selling from secondary market
Pre MM Cash Balance: 1e+06 Instrument Balance: 0
preparing to sell secondary and buy primary...
Post Trading processing....
Post MM Cash Balance: 1.00048e+06 Instrument Balance: 0
: Passed 7 out of 7 tests in 0.000244 seconds
Running [Global/SimpleMMTradingStrategyOnMarketDataChangenPrimaryBidtHigherThanSecondaryAskStrategyTriggered]preparing to buy secondary and sell primary...
: Passed 11 out of 11 tests in 0.000158 seconds
9 Tests finished. All 55 assertions are passing.
```

Remarks: there are compile,build and debug scripts to faciliate the development. 
