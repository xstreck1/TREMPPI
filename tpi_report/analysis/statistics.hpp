#pragma once

#include <tpi_common/header.h>

// Some basic statistical functions templates
namespace Statistics{
	// 
	template<typename T>
	inline double expected_val(const vector<T> & data) {
		return accumulate(WHOLE(data), static_cast<T>(0), std::plus<T>()) / static_cast<double>(data.size());
	}

	// 
	template<typename T>
	double expected_prod_val(const vector<T> & A, const vector<T> & B) {
		vector<T> product(A.size(), 0.);
		transform(WHOLE(A), begin(B), begin(product), multiplies<T>());
		return expected_val(product);
	}

	//
	template<typename T>
	vector<double> deviations(const vector<T> & data, const double mean) {
		vector<double> result = vector<double>(data.size(), 0.);
		transform(WHOLE(data), begin(result), [&mean](const double val) {
			return mean - val;
		});
		return result;
	}

	// 
	template<typename T>
	double variance(const vector<T> & dev) {
		return accumulate(WHOLE(dev), 0.0, [](double variance, const double dev){
			return variance + pow(dev, 2.);
		}) / dev.size();
	}

	// 
	template<typename T>
	double correlation(const vector<T> & X, const vector<T> & Y) {
		// Compute the correlation
		vector<double> X_dev = deviations(X, expected_val(X));
		vector<double> Y_dev = deviations(Y, expected_val(Y));
		double divident = expected_prod_val(X_dev, Y_dev);
		double divisor = pow(variance(X_dev), 0.5) * pow(variance(Y_dev), 0.5);
		double correlation = (divisor == 0.) ? 0. : (divident / divisor);

		return correlation;
	}
}