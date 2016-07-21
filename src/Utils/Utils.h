#pragma once

#include <Utils/DebugUtils.h>
#include <libxml/xmlreader.h>
#include <string.h>
#include <cmath>
#include <stdexcept>

/// Macros used by the XML reader function.
#define XML_TAG_DATASET			BAD_CAST"idaxs"
#define XML_TAG_PROJECT			BAD_CAST"project"
#define XML_TAG_SUBJECT			BAD_CAST"subject"
#define XML_TAG_SUBJECT_INFO	BAD_CAST"subjectInfo"

/**
 * @class Utils
 * 
 * @brief Class that defines several useful functions.
 */
class Utils
{
	public:
		/**
		 * @brief Enumerator representing all the possible Alzheimer's state.
		 */
		enum PatientClass
		{
			AD = 0,
			MCI,
			LMCI,
			CN,
			Unknown
		};
		
		/**
		 * @brief Function that returns the class of the Alzheimer's state.
		 * 
		 * @param patient state of the patient.
		 * 
		 * @return the class of the Alzheimer's state.
		 */
		inline static PatientClass getPatientClass(const std::string patient)
		{
			if (strcmp(patient.c_str(),"2") == 0) return Utils::AD;
			else if (strcmp(patient.c_str(),"0.5") == 0) return Utils::MCI;
			else if (strcmp(patient.c_str(),"1") == 0) return Utils::LMCI;
			else if (strcmp(patient.c_str(),"0") == 0) return Utils::CN;
			else return Utils::Unknown;
		}
		
		/**
		 * @brief Function that returns the class of the Alzheimer's state.
		 * 
		 * @param patient state of the patient.
		 * 
		 * @return the class of the Alzheimer's state.
		 */
		inline static PatientClass getPatientClass(const xmlChar* patient)
		{
			if (strcmp((char*) patient,"AD") == 0) return Utils::AD;
			else if (strcmp((char*) patient,"MCI") == 0) return Utils::MCI;
			else if (strcmp((char*) patient,"LMCI") == 0) return Utils::LMCI;
			else if (strcmp((char*) patient,"Normal") == 0) return Utils::CN;
			else return Utils::Unknown;
		}
		
		/**
		 * @brief Function that returns the class of the Alzheimer's state in a string format.
		 * 
		 * @param patient state of the patient.
		 * 
		 * @return the class of the Alzheimer's state in a string format.
		 */
		inline static std::string getPatientString(const PatientClass& patient)
		{
			if (patient == Utils::AD) return "AD";
			else if (patient == Utils::MCI) return "MCI";
			else if (patient == Utils::LMCI) return "LMCI";
			else if (patient == Utils::CN) return "CN";
			else return "Unknown";
		}
		
		/**
		 * @brief Function that reads the xml file containing all information of the patient.
		 * 
		 * @param filename file of the patient.
		 * 
		 * @return the class of the patient (i.e., AD, MCI, LMCI, CN, Unknown).
		 */
		inline static PatientClass readPatientFile(const std::string& filename)
		{
			xmlDocPtr file;
			xmlNodePtr frame;
			
			file = xmlReadFile(filename.c_str(),"UTF-8",XML_PARSE_RECOVER);
			
			if (file == 0)
			{
				ERR("Error reading file '" << filename << "'. Exiting..." << std::endl);
				
				exit(-1);
			}
			
			frame = xmlDocGetRootElement(file);
			
			if ((frame == 0) || xmlStrcmp(frame->name,XML_TAG_DATASET))
			{
				ERR("File '" << filename << "' in a wrong format. Exiting..." << std::endl);
				
				exit(-1);
			}
			
			while (frame != 0)
			{
				try
				{
					frame = frame->children->next;
					
					if (!xmlStrcmp(frame->name,XML_TAG_PROJECT))
					{
						frame = frame->children->next->next->next->next->next->next->next;
						
						if (!xmlStrcmp(frame->name,XML_TAG_SUBJECT))
						{
							frame = frame->children->next->next->next->next->next->next->next;
							
							if (!xmlStrcmp(frame->name,XML_TAG_SUBJECT_INFO))
							{
								return getPatientClass(xmlNodeListGetString(file,frame->xmlChildrenNode,1));
							}
						}
					}
				}
				catch (const std::runtime_error& e)
				{
					ERR(std::endl << "XML file malformed. Exiting..." << std::endl);
					
					exit(-1);
				}
				
				frame = frame->next;
			}
			
			return Utils::Unknown;
		}
		
		/**
		 * @brief Function that truncates a floating number to a specified decimal position.
		 * 
		 * @param d floating number to be truncated.
		 * @param n desired decimal position.
		 * 
		 * @return the truncated floating number.
		 */
		inline static float roundN(float d, int n)
		{
			if (n == 0) return round(d);
			else if (n > 0)
			{
				float p;
				int temp;
				
				p = std::pow(10.0,n);
				temp = (int) (d * p);
				
				return (((float) temp) / p);
			}
			else return d;
		}
};
