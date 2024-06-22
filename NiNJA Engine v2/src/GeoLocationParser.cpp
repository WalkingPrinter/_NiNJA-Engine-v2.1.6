#include "GeoLocationParser.h"
#include "SecureStrings.h"
#include "Globals.h"

namespace GeoIpParser
{
	//Example return from geo-ip api: {"as":"AS7922 Comcast Cable Communications, LLC", "city" : "Lisle", "country" : "United States", "countryCode" : "US", "isp" : "Comcast Cable", "lat" : 41.787, "lon" : -88.0833, 
	//"org" : "Comcast Cable", "query" : "13.37.13.37", "region" : "IL", "regionName" : "Illinois", "status" : "success", "timezone" : "America/Chicago", "zip" : "60532"}
	char IP[0x100];

	static DWORD dwNextRequestTime = 0;
	void GeoIpParser::ParseIntenal(string ip, GeoIpInfo *clientInfo) {
		//sprintf(IP, "/json/%s", ip.c_str());
		sprintf(IP, "/?query=%s", ip.c_str());
		BOOL httpSuccess = FALSE, bUsedAlt = FALSE;
		HttpRequest req;
		string ipStr(IP);
		//printf("Attempting web request to ip-api.com%s\n", ipStr.c_str());
		//BYTE *ret =  req.SendRequest(!CG::bUseAltGeoIPApi ? "ip-api.com" : "extreme-ip-lookup.com", ipStr, FALSE, &httpSuccess, 80, 0x1000) ;
		BYTE *ret = req.SendRequest("api.geoiplookup.net", ipStr, FALSE, &httpSuccess, 80, 0x500);
		if (httpSuccess) {
			//printf("Successful ip lookup!\n");
			XMLDocument doc;
			doc.Parse((PCHAR)ret);

			if (doc.FirstChildElement("ip")) {
				XMLElement* result = doc.FirstChildElement("ip")->FirstChildElement("results")->FirstChildElement("result");
				if (result) {
					ZeroMemory(IP, 0x100);

					sprintf(IP, "/xml.gp?ip=%s", ip.c_str());

					XMLElement *isp = result->FirstChildElement("isp");
					if (isp) {
						memcpy(clientInfo->isp, isp->GetText(), sizeof(clientInfo->isp) - 1);
					}
					else {
						memcpy(clientInfo->isp, "Unavailable", sizeof(clientInfo->isp) - 1);
					}

					// Free the return from our initial web request, it's not needed anymore.
					XSAFE_FREE(ret);
					ret = NULL;

					ipStr.clear();
					string reverseString(IP);

					// Sleep until a second has passed since our last request to the geoplugin api, they rate limit us to 120 requests per minute so half a second between requests is all we need
					while (dwNextRequestTime > GetTickCount()) {
						Sleep(10);
					}

					BYTE *info = req.SendRequest("geoplugin.net", reverseString, FALSE, &httpSuccess, 80, 0x500);
					dwNextRequestTime = GetTickCount() + 500;
					if (httpSuccess) {
						XMLDocument doc2;
						if (doc2.Parse((PCHAR)info) == XML_SUCCESS) {
							XMLElement* geoInfo = doc2.FirstChildElement("geoPlugin");
							if (geoInfo) {
								XMLElement *city = geoInfo->FirstChildElement("geoplugin_city");
								if (city && city->GetText()) {
									memcpy(clientInfo->city, city->GetText(), strlen(city->GetText()) < sizeof(clientInfo->city) - 1 ? strlen(city->GetText()) : sizeof(clientInfo->city) - 1);
								}
								else {
									strcpy(clientInfo->city, "Unavailable");
								}

								XMLElement *state = geoInfo->FirstChildElement("geoplugin_region");
								if (state && state->GetText()) {
									memcpy(clientInfo->stateFull, state->GetText(), strlen(state->GetText()) < sizeof(clientInfo->stateFull) - 1 ? strlen(state->GetText()) : sizeof(clientInfo->stateFull) - 1);
								}
								else {
									strcpy(clientInfo->stateFull, "Unavailable");
								}

								XMLElement *country = geoInfo->FirstChildElement("geoplugin_countryName");
								if (country && country->GetText()) {
									memcpy(clientInfo->country, country->GetText(), strlen(country->GetText()) < sizeof(clientInfo->country) - 1 ? strlen(country->GetText()) : sizeof(clientInfo->country) - 1);
								}
								else {
									strcpy(clientInfo->country, "Unavailable");
								}
							}
						}
						else {
							DbgPrint("Failed to parse geoplugin.net XML!");
						}

						XSAFE_FREE(info);
						success = true;
					}
					return;
				}
			}
			XSAFE_FREE(ret);
		}
		success = false;
	}

	GeoIpParser::GeoIpParser(string ip, GeoIpInfo *clientInfo) {
		ParseIntenal(ip, clientInfo);
	}

	GeoIpParser::GeoIpParser(IN_ADDR ip, GeoIpInfo *clientInfo) {
		sprintf(IP, "%d.%d.%d.%d", ip.S_un.S_un_b.s_b1, ip.S_un.S_un_b.s_b2, ip.S_un.S_un_b.s_b3, ip.S_un.S_un_b.s_b4);
		ParseIntenal(IP, clientInfo);
	}

	GeoIpParser::GeoIpParser(DWORD ip, GeoIpInfo *clientInfo) {
		IN_ADDR ipT;
		ipT.S_un.S_addr = ip;
		sprintf(IP, "%d.%d.%d.%d", ipT.S_un.S_un_b.s_b1, ipT.S_un.S_un_b.s_b2, ipT.S_un.S_un_b.s_b3, ipT.S_un.S_un_b.s_b4);
		ParseIntenal(IP, clientInfo);
	}
};