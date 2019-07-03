#include <iostream>
#include <cstdlib>
#include <ql/quantlib.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

using namespace QuantLib;

namespace po = boost::program_options;

int main(int argc, char **argv) {
	Real strike, timeToMaturity, spot, dividendYield, riskFree;
	Volatility sigma;

	// Parse options

	po::options_description desc("Parameters");
	desc.add_options()
		("help", "show help")
		("strike", po::value<Real>(&strike)->default_value(110.0), "strike")
		("timeToMaturity", po::value<Real>(&timeToMaturity)->default_value(0.5), "time to maturity")
		("spot", po::value<Real>(&spot)->default_value(100.0), "spot")
		("dividendYield", po::value<Real>(&dividendYield)->default_value(0.0), "dividend yield")
		("riskFree", po::value<Real>(&riskFree)->default_value(0.03), "risk free rate")
		("sigma", po::value<Volatility>(&sigma)->default_value(0.20), "strike");
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
	if (vm.count("help")) {
	    std::cout << desc << "\n";
	    return 1;
	}

	// Compute volatility and discount factor

	Real vol = sigma * std::sqrt(timeToMaturity);
	DiscountFactor discount = std::exp(-riskFree * timeToMaturity);
	DiscountFactor growth = std::exp(-dividendYield * timeToMaturity);

	// Calculate option price with Black-Scholes

	boost::shared_ptr<PlainVanillaPayoff> vanillaCallPayoff = boost::shared_ptr<PlainVanillaPayoff>(new PlainVanillaPayoff(Option::Type::Call, strike));
	BlackScholesCalculator bsCalculator(vanillaCallPayoff, spot, growth, vol, discount);

	// Show results

	std::cout << boost::format("initial value: %.4f") % bsCalculator.value() << std::endl;
	std::cout << boost::format("delta: %.4f") % bsCalculator.delta() << std::endl;
	std::cout << boost::format("gamma: %.4f") % bsCalculator.gamma() << std::endl;
	std::cout << boost::format("vega: %.4f") % (bsCalculator.vega(timeToMaturity) / 100) << std::endl;
	std::cout << boost::format("theta per day: %.4f") % bsCalculator.thetaPerDay(timeToMaturity) << std::endl;

	return 0;
}