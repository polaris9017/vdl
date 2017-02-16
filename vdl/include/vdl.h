#ifndef VDL_H
#define VDL_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <iomanip>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <json/json.h>
#include <rlutil/rlutil.h>
#include <cpr/cpr.h>
#include <curl/curl.h>

#define MODL_MAIN 0x01
#define TYPE_VLIVE 0x10
#define TYPE_VLIVE_CHANNEL 0x11
#define TYPE_VLIVE_LIVE 0x12
#define TYPE_TVCAST 0x13
#define TYPE_TVPOT 0x14
#define TYPE_FB 0x15
#define TYPE_INSTAGRAM 0x16

#define POST_FFMPEG 0x20

#define ERR 0xfe
#define WRN 0xff

#define VER_MAJOR 2
#define VER_MINOR 0
#define VER_REV 0

using namespace std;

namespace VDLDefault {
	//Input Urls
	const string URL_VLIVE = "http://vlive.tv/video";
	const string URL_VLIVE_CH = "http://channels.vlive.tv";
	const string URL_NAVER = "http://tvcast.naver.com";
	const string URL_DAUM = "http://tvpot.daum.net";
	const string URL_FACEBOOK = "http://facebook.com";
	const string URL_TWITTER = "http://twitter.com";

	//Internal Urls
	const string URL_INTERNAL_NAVER = "http://play.rmcnmv.naver.com/vod/play/";
	const string URL_INTERNAL_FACEBOOK = "https://graph.facebook.com/v2.7/";
	const string URL_INTERNAL_VLIVE = "http://global.apis.naver.com/rmcnmv/rmcnmv/vod_play_videoInfo.json";
	const string URL_INTERNAL_VLIVE_HLS = "http://vlive.hls.edgesuite.net/";
	const string URL_INTERNAL_VLIVE_LIVE = "http://www.vlive.tv/video/init/view";
	const string VLIVE_CH_INTERNAL_INFO_JSON = "http://api.vfan.vlive.tv/vproxy/channelplus/decodeChannelCode";
	const string VLIVE_CH_INTERNAL_VOD_LIST = "http://api.vfan.vlive.tv/vproxy/channelplus/getChannelVideoList";

	//Internal Constants
	const string VDL_CODENAME = "R U Ready";
	const string VDL_VERSION = "019902";
	const string VLIVE_VIDEO_PARAM = "\\bvlive\\.video\\.init\\(([^)]+)";
	const string VLIVE_VIDEO_CREATOR = "<div[^>]+class=\"info_area\"[^>]*>\\s*<a\\s+[^>]*>([^<]+)";
	const string VLIVE_VIDEO_TITLE = "<meta[^\\n]+=\\\"og:title\\\" content=\\\"([^]+)\"\\/>[^]+\\\"og:url\"";
	const string VLIVE_LIVE_STREAM = "\"liveStreamInfo\"\\s*:\\s*\"(.*)\",";
	const string VLIVE_LIVE_M3U8 = "#EXTINF:([\\d\\.]+),\\n([a-z\\d-_.]+)\\n#EXTINF:([\\d\\.]+),\\n([a-z\\d-_.]+)\\n#EXTINF:([\\d\\.]+),\\n([a-z\\d-_.]+)";
	const string VLIVE_LIVE_CONST_PLAYLIST = "playlist\\.m3u8\\?__gda";
	const string VLIVE_LIVE_CONST_CHUNKLIST = "chunklist.m3u8?__agda";
	const string VLIVE_CH_CHANNEL_CODE = "http:\\/\\/channels\\.vlive\\.tv\\/([\\w\\D]+)\\/video";
	const string VLIVE_CH_APP_JS = "<script[^>]+src=[\\\"\\'](http.+?\\/app\\.js)";
	const string VLIVE_CH_APPID = "Global\\.VFAN_APP_ID\\s*=\\s*[\\'\"]([^\\'\"]+)[\\'\"]";

	//Functions
	void printInit();
	bool Download(string url, wstring fname);
	string print(string str, unsigned int level = 0);
	string print(wstring str, unsigned int level = 0);
	unsigned int Categorize(string url);
	cpr::Response LoadPage(string url);
	cpr::Response LoadPage(string url, cpr::Parameters param);
	cpr::Response LoadPage(string url, cpr::Payload param);
	string replace(string str, string r, string sbst);
	void decodeHtmlEntity(wstring &str);
	size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata);
	string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale(""));
}


class VDLModule {	
protected:
	int PostProcess(wstring f, size_t flag) {

	}
	string url;
	unsigned int ver[3];	
public:
	VDLModule(unsigned int major = VER_MAJOR, unsigned int minor = VER_MINOR, unsigned int rev = VER_REV) {
		ver[0] = major, ver[1] = minor, ver[2] = rev;
	}
	void printInit();
	void setUrl(string url);
	bool LoadModule(uint16_t magic);
	vector<string> Split(string s, const string &re);
	virtual vector<string> FetchVideoList();
	virtual vector<string> Parse(const string &s, const string r);
	Json::Value ParseJson(const string & s);
	friend ostream & operator<<(ostream & os, VDLModule & m);
	virtual bool Run() { return false; }
};


#endif // !VDL_H