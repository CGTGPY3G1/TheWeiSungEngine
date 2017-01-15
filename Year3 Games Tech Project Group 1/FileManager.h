#pragma once
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal\types\vector.hpp>
#include <fstream>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal\external\rapidxml\rapidxml.hpp>
#include "zlib.h" 
#include <atlbase.h>
//#include <atlconv.h>
#include "GameObject.h"
enum SaveFormat {
	XML = 0,
	JSON = 1,
	BINARY = 2
};

class FileManager {
public:
	//FileManager();
	//~FileManager();

	static std::string LoadTMX(const std::string path) {
		std::ifstream is("Tilesets/" + path + ".tmx");
		std::string toReturn = "";
		if(is) {
			std::stringstream buffer;
			buffer << is.rdbuf();
			toReturn = buffer.str();
			is.close();
		}
		return toReturn;
	}
	template <typename T> static void Save(const std::string path, T & data, const SaveFormat & format) {
		switch(format) {
		case SaveFormat::XML:
			SaveXML(path, data);
		break;
		case SaveFormat::JSON:
			SaveJSON(path, data);
		break;
		case SaveFormat::BINARY:
			SaveBINARY(path, data);
		break;
		default:
			break;
		}
	}
private:
	template <typename T> static void SaveXML(const std::string path, T & data) {
		std::ofstream os(path + ".xml", std::ios_base::trunc);
		cereal::XMLOutputArchive archive(os);
		archive(cereal::make_nvp("Object", data));
	}

	template <> static void SaveXML<std::shared_ptr<GameObject>>(const std::string path, std::shared_ptr<GameObject> & data) {
		std::ofstream os(path + ".xml", std::ios_base::trunc);
		cereal::XMLOutputArchive archive(os);
		archive(cereal::make_nvp("GameObject: " + data->GetName(), data));
	}

	template <typename T> static void SaveJSON(const std::string path, T & data) {
		std::ofstream os(path + ".json", std::ios_base::trunc);
		cereal::JSONOutputArchive archive(os);
		archive(cereal::make_nvp(data));
	}

	template <> static void SaveJSON<std::shared_ptr<GameObject>>(const std::string path, std::shared_ptr<GameObject> & data) {
		std::ofstream os(path + ".json", std::ios_base::trunc);
		cereal::JSONOutputArchive archive(os);
		archive(cereal::make_nvp("GameObject: " + data->GetName(), data));
	}

	template <typename T> static void SaveBINARY(const std::string path, T & data) {
		gzFile gz_file;
		gz_file = gzopen((path + ".rsd").c_str(), "wb");
		std::stringstream os;
		{
			cereal::BinaryOutputArchive archive(os);
			archive(cereal::make_nvp(data));
		}
		unsigned long int file_size = sizeof(char) * os.str().size();
		gzwrite(gz_file, (void*)&file_size, sizeof(file_size));
		gzwrite(gz_file, (void*)(os.str().data()), file_size);
		gzclose(gz_file);
	}

	template <> static void SaveBINARY<std::shared_ptr<GameObject>>(const std::string path, std::shared_ptr<GameObject> & data) {
		gzFile gz_file;
		gz_file = gzopen((path + ".rsd").c_str(), "wb");		
		std::stringstream os;
		{
			cereal::XMLOutputArchive archive(os);
			archive(cereal::make_nvp("GameObject: " + data->GetName(), data));
		}
		unsigned long int file_size = sizeof(char) * os.str().size();
		gzwrite(gz_file, (void*)&file_size, sizeof(file_size));
		gzwrite(gz_file, (void*)(os.str().data()), file_size);
		gzclose(gz_file);
	}



	//static bool DirectoryExists(std::string path) {
	//	DWORD dwAttrib = GetFileAttributes((LPCTSTR)path.c_str());
	//	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	//}

	//static void CreateNewDirectory(std::string path) {
	//	if(!CreateDirectory((LPCTSTR)path.c_str(), NULL)) {
	//		Debug::PrintImmediately("Couldn't Create Directory - " + path, DebugMessageType::DEBUG_TYPE_FAILURE_CRITICAL);
	//	}
	//}

	//static bool FileExists(std::string path) {
	//	DWORD dwAttrib = GetFileAttributes((LPCTSTR)path.c_str());
	//	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	//}
};
//
//FileManager::FileManager() {
//}
//
//FileManager::~FileManager() {
//}
