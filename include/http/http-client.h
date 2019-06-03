
#ifndef SPROOF_HTTP_CLIENT_H
#define SPROOF_HTTP_CLIENT_H


#include <string>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <thread>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "http/http-error.h"


namespace sproof {

    class HttpClient {
    public:
        /**
        * erstellt einen neuen HTTP Clienen mit dem gegebenen Host als basis
        */
        HttpClient();

        ~HttpClient() = default;

        /**
         * fuehrt ein blockierendes GET Request durch
         * @param path der verwendete Pfad
         * @param params GET Parameter Paare
         * @return antwort des Servers
         */
        Json::Value get(const std::string &path,
                        const std::unordered_map<std::string, std::string> &params);

        /**
         * fuehrt ein nicht blockierendes GET Request durch und ruft je nach Serveranwort callbacks auf
         * @param path der verwendete Pfad
         * @param params GET Parameter Paare
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback  wird im Fehlerfall aufgerufen
         * @return Threadhandle für den ausfuehrenden Thread
         */
        std::thread get(const std::string &path, const std::unordered_map<std::string, std::string> &params,
                        std::function<void(Json::Value)> result_callback,
                        std::function<void(HttpException)> error_callback);

        /**
         * fuehrt ein blockierendes POST Request durch
         * @param path der verwendete Pfad
         * @param data die versendeten Daten
         * @return antwort des Servers
         */
        Json::Value post(const std::string &path, const Json::Value &data);

        /**
         * fuehrt ein nicht blockierendes POST Request durch und ruft je nach Serveranwort callbacks auf
         * @param path der verwendete Pfad
         * @param data die versendeten Daten
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return Threadhandle für den ausfuehrenden Thread
         */
        std::thread post(const std::string &path, const Json::Value &data,
                         std::function<void(Json::Value)> result_callback,
                         std::function<void(HttpException)> error_callback);

        /**
         * sendet die die angegebene Datei per form-enooded POST Request an den Server
         * @param url_path Pfad des Servers
         * @param file_path lokaler Dateipfad
         * @param credentials die verwendeten Zugangsdaten
         * @return antwort des Servers
         */
        Json::Value
        post_file(const std::string &url_path, const std::string &file_path, const Json::Value &credentials);

        /**
         * sendet die die angegebene Datei per form-enooded POST Request an den Server
         * @param url_path Pfad des Servers
         * @param file_stream Imput-Stream der zu sendenden Datei
         * @param file_name Name der zu sendenden Datei
         * @param credentials die verwendeten Zugangsdaten
         * @return antwort des Servers
         */
        Json::Value post_file(const std::string &url_path, std::ifstream &file_stream, const std::string &file_name,
                              const Json::Value &credentials);

        /**
         * sendet die die angegebene Datei per form-enooded POST Request an den Server nichtblockierend
         * @param url_path Pfad des Servers
         * @param file_path lokaler Dateipfad
         * @param credentials die verwendeten Zugangsdaten
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return Threadhandle für den ausfuehrenden Thread
         */
        std::thread post_file(const std::string &url_path, const std::string &file_path, const Json::Value &credentials,
                              std::function<void(Json::Value)> result_callback,
                              std::function<void(HttpException)> error_callback);

    };


} //namespace sproof

#endif