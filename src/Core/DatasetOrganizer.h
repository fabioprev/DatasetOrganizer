#pragma once

#include <string>

/**
 * @class DatasetOrganizer
 * 
 * @brief Class that implements a tool for separating a given data set in all his classes.
 */
class DatasetOrganizer
{
	private:
		/**
		 * @brief name of the data set chosen.
		 */
		std::string dataset;
		
	public:
		/**
		 * @brief Empty constructor.
		 */
		DatasetOrganizer();
		
		/**
		 * @brief Destructor.
		 */
		~DatasetOrganizer();
		
		/**
		 * @brief Function that starts the separating of the data set in the directory given as input.
		 * 
		 * @param directory name of the directory which contains the data set.
		 */
		void exec(const std::string& directory);
};
