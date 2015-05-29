#pragma once

#include <vector>
#include <regex>
#include <string>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/counting_range.hpp>

#define NO_COPY(TypeName) \
		TypeName() = default;  \
		TypeName(TypeName && ) = default;  \
		TypeName& operator=(TypeName && ) = default; \
		TypeName(const TypeName & ) = delete; \
		TypeName& operator=(const TypeName & ) = delete;

#define GROUNDED(TypeName) \
		TypeName() = default;  \
		TypeName(TypeName && ) = delete;  \
		TypeName& operator=(TypeName && ) = delete; \
		TypeName(const TypeName & ) = delete; \
		TypeName& operator=(const TypeName & ) = delete; 

#define NO_COPY_SHORT(TypeName) \
		TypeName(const TypeName & ) = delete; \
		TypeName& operator=(const TypeName & ) = delete; 

#define DEFAULT_MOVE(TypeName) \
		TypeName(TypeName && ) = default;  \
		TypeName& operator=(TypeName && ) = default; 

#define WHOLE(Container) std::begin(Container),std::end(Container)

#define DEL_IF_EXISTS(pointer) if (pointer != nullptr) { delete pointer; pointer = nullptr; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file A handful of handy functions.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Common {
	/**
	* @brief renaming for boost::counting_range
	* @param[in] begin  first number in the range
	* @param[in] end    first number not in the range
	* @return  a counting range for [begin,end[
	*/
	template<typename IntegralType>
	inline auto crange(const IntegralType begin, const IntegralType end) -> decltype(boost::counting_range(begin, end)) {
		return boost::counting_range(begin, end);
	}

	/**
	* @brief an interface to boost::counting_range
	* @param[in] end    first number not in the range
	* @return  a counting range for [0,end[
	*/
	template<typename IntegralType>
	inline auto crange(const IntegralType end) -> decltype(boost::counting_range(end, end)) {
		return boost::counting_range(static_cast<IntegralType>(0), end);
	}

	/**
	* @brief A range adapter for containers with the size() operator.
	* @param[in] container   container whose size is in question.
	* @return  a counting range for [0,container.size()[
	*/
	template<typename SeqType>
	inline auto cscope(const SeqType & container) -> decltype(crange(container.size())) {
		typedef decltype(container.size()) ContSizeT;
		return crange(static_cast<ContSizeT>(0), container.size());
	}

	/**
	* @brief creates a vector with the given range
	* @param[in] begin    first number in the range
	* @param[in] end    first number not in the range
	* @return  a vector for [begin,end[
	*/
	template<typename IntegralType>
	inline std::vector<IntegralType> vrange(const IntegralType begin, const IntegralType end) {
		std::vector<IntegralType> values(end - begin);

		size_t val = begin;
		generate(values.begin(), values.end(), [&val]() {return val++; });

		return values;
	}

	/**
	* @brief creates a vector with the given range
	* @param[in] end    first number not in the range
	* @return  a vector for [0,end[
	*/
	template<typename IntegralType>
	inline std::vector<IntegralType> vrange(const IntegralType end) {
		return vrange(static_cast<IntegralType>(0), end);
	}


	/**
	* @brief Increases integral value by 1.
	* @param[in,out] val  reference to value that will be increased
	*/
	template<typename IntegralType>
	void increase(typename std::vector<IntegralType>::reference val) {
		val++;
	}


	template<>
	void increase<bool>(std::vector<bool>::reference val);

	/**
	* @brief Iterates values from left to right if it is possible. If so, return true, otherwise return false.
	* @param[in] top    vector of maximal values each component can reach
	* @param[in] bottom vector   of minimal values each component can reach
	* @param[in,out] iterated    vector of values to iterate
	* @return  true if the iteration was valid, false if it caused overflow (iterated > bottom)
	*/
	template<typename IntegralType>
	bool iterate(const std::vector<IntegralType> & top, const std::vector<IntegralType> & bottom, std::vector<IntegralType> & iterated) {
		if (iterated.empty())
			return false;
		for (size_t val_num = 0; val_num <= iterated.size(); val_num++) {
			if (val_num == iterated.size())
				return false;
			if (iterated[val_num] == top[val_num]) {
				iterated[val_num] = bottom[val_num];
			}
			else {
				increase<IntegralType>(iterated[val_num]);
				break;
			}
		}
		return true;
	}

	/**
	* @brief Iterates values from left to right if it is possible. If so, return true, otherwise return false.
	* @param[in] values	sets of values for each member, left to right    
	* @param[in,out] iterated    vector of values to iterate
	* @return  true if the iteration was valid, false if it caused overflow (iterated > bottom)
	*/
	template<typename IntegralType>
	bool iterate(const std::vector<std::vector<IntegralType>>  & values, std::vector<IntegralType> & iterated) {
		if (iterated.empty())
			return false;
		for (size_t val_num = 0; val_num <= iterated.size(); val_num++) {
			if (val_num == iterated.size())
				return false;
			if (iterated[val_num] == values[val_num].back()) {
				iterated[val_num] = values[val_num].front();
			}
			else {
				iterated[val_num] = *(++find(WHOLE(values[val_num]), iterated[val_num]));
				break;
			}
		}
		return true;
	}

	template<typename IntegralType>
	bool belongsToProduct(const std::vector<IntegralType> & element, const std::vector<std::vector<IntegralType>> & sets) {
		for (size_t set_no = 0; set_no < sets.size(); set_no++) {
			for (size_t val_no = 0; val_no <= sets[set_no].size(); val_no++) {
				if (val_no == sets[set_no].size()) {
					return false;
				}
				else if (element[set_no] == sets[set_no][val_no]) {
					break;
				}
			}
		}
		return true;
	}

	template<typename IntegralType>
	std::vector<IntegralType> min(const std::vector<std::vector<IntegralType>> & values) {
		std::vector<IntegralType>  result(values.size());

		for (size_t val_num = 0; val_num < values.size(); val_num++) {
			if (values[val_num].empty()) {
				throw runtime_error("Trying to obtain a lower bound of an empty product.");
			}
			result[val_num] = values[val_num].front();
		}

		return result;
	}

	template<typename IntegralType>
	std::vector<IntegralType> max(const std::vector<std::vector<IntegralType>> & values) {
		std::vector<IntegralType> result(values.size());

		for (size_t val_num = 0; val_num < values.size(); val_num++) {
			if (values[val_num].empty()) {
				throw runtime_error("Trying to obtain an upper bound of an empty product.");
			}
			result[val_num] = values[val_num].back();
		}

		return result;
	}

	/**
	* @brief Concatenate the two vectors -> A.B
	* @param[in] B    source vector
	* @param[in, out] A   extended vector
	* @return	ref to A
	*/
	template<typename T>
	std::vector<T> operator+(const std::vector<T> & A, const std::vector<T> & B) {
		std::vector<T> result = A;
		result.insert(result.end(), WHOLE(B));
		return result;
	}

	/**
	* @brief Obtain a vector that contains only the unique components of the original.
	* @param[in] original	the source vector
	* @return	a new vector that contains only the unique elements of the original
	*/
	template<typename T>
	std::vector<T> makeUnique(std::vector<T> original) {
		auto end_it = std::unique(WHOLE(original));
		original.resize(std::distance(begin(original), end_it));
		return original;
	}

	/**
	* @brief Obtain all matches to the regex in the source string
	* @param[in] control_regex	regular expression describing the match
	* @param[in] original	a string to search in
	* @param[in] n	number of the match to use (0 for the whole string, 1-n for the i-th parenthesis)
	* @return	vector of all the matches
	*/
	std::vector<std::string> getAllMatches(const std::string & control_regex, const std::string & original, const int n = 0);

	/**
	* @brief replace in the given string all the occurences of "from" to "to"
	* @param[in] from	original string
	* @param[in] to	replacement string
	* @param[in, out] str	string to replace in
	*/
	void replaceAll(const std::string & from, const std::string & to, std::string & str);

	/**
	* @brife return a string quoting the provided data
	* @param[in] content	value to quote
	* @return	string with the quoted data
	*/
	template<typename T>
	std::string quote(const T & content) {
		return "\"" + boost::lexical_cast<std::string, T>(content) + "\"";
	}

	/**
	* @brief Control if the map has the given key
	* @param[in] dictionary the input map
	* @param[in] key	the key to find
	* @return	true if the key is present
	*/
	template <typename KeyT, typename ValT>
	inline bool hasKey(const std::map<KeyT, ValT> & dictionary, const KeyT & key) {
		return (dictionary.find(key) != dictionary.end());
	}

	/**
	* @brief returns a zero-initiated index of value in the vector data
	* @param[in] data	the vector with elements
	* @param[in] val	the value that is searched for
	* @return	a zero-initiated index of "val" in "data" or maximal size_t value if not found
	*/
	template <typename T>
	inline std::size_t getIndex(const std::vector <T> & data, const T & val) {
		auto it = std::find(WHOLE(data), val);
		if (it != std::end(data))
			return std::distance(std::begin(data), it);
		else
			return std::numeric_limits<std::size_t>::max();
	}
}

using namespace Common;


