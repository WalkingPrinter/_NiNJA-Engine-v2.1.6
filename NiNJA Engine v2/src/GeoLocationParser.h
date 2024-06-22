#pragma once
#include "HttpRequest.h"
#include "Utilities.h"

//#include "rapidjson\rapidjson.h"
//#include "rapidjson\document.h"
//#include "rapidjson\reader.h"
//#include "rapidjson\writer.h"
//#include "rapidjson\stringbuffer.h"

#include "tinyxml\tinyxml2.h"

using namespace tinyxml2;
//using namespace rapidjson;

namespace GeoIpParser 
{

	class GeoIpParser {// : public rapidjson::Document {
	protected:
		//internal parsing function
		void ParseIntenal(string ip, GeoIpInfo *clientInfo);

		////json flags
		//BOOL isValid;
	public:
		GeoIpParser() {
			//isValid = FALSE;
			success = false;
			// make the document an object (root)
			//SetObject();
		}

		//constructors
		GeoIpParser(IN_ADDR ip, GeoIpInfo *clientInfo);
		GeoIpParser(DWORD ip, GeoIpInfo *clientInfo);
		GeoIpParser(string ip, GeoIpInfo *clientInfo);

		//public vars
		bool success;
	};

}