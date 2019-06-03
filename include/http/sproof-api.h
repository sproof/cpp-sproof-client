
#ifndef SPROOF_CLIENT_SPROOF_API_H
#define SPROOF_CLIENT_SPROOF_API_H

#include <memory>
#include <thread>
#include <functional>
#include <unordered_map>

#include <jsoncpp/json/json.h>

#include <sproof-utils/signature.h>

#include "http/http-client.h"
#include "http/http-error.h"
#include "config.h"
#include "model/events/sproof-event.h"
#include "model/profile.h"

namespace sproof {


    const std::string API_URL{"api/v1/"};


    class SproofApi {

    public:

        using EventList = std::vector<std::unique_ptr<SproofEvent>>;
        using ResultCb = std::function<void(Json::Value)>;
        using ErrorCb = std::function<void(HttpException)>;

        /**
         * erstellt ein neues SproofApi Object mit der gegebenen Config
         * @param config zu verwendende Sproof Config
         */
        explicit SproofApi(sproof::Config  config);

        /**
         * erzeugt eine Signatur der Zugangsdaten
         * @param credentials die verwendeten Zugangsdaten
         * @return Signatur der Zugangsdaten
         */
        sproof::Signature sign_credentials(const sproof::Credentials& credentials);

        /**
         * fuehrt ein blockierendes GET Request durch
         * @param path der verwendete Pfad
         * @return antwort des Servers
         */
        Json::Value get(const std::string& path);

        /**
         * fuehrt ein blockierendes GET Request durch
         * @param path der verwendete Pfad
         * @param params GET Parameter Paare
         * @return antwort des Servers
         */
        Json::Value get(const std::string& path, const std::unordered_map<std::string,std::string>& params);

        /**
         * fuehrt ein nicht blockierendes GET Request durch und ruft je nach Serveranwort callbacks auf
         * @param path der verwendete Pfad
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback  wird im Fehlerfall aufgerufen
         * @return Threadhandle für den ausfuehrenden Thread
         */
        std::thread get(const std::string& path, ResultCb success_callback, ErrorCb error_callback);

        /**
         * fuehrt ein nicht blockierendes GET Request durch und ruft je nach Serveranwort callbacks auf
         * @param path der verwendete Pfad
         * @param params GET Parameter Paare
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback  wird im Fehlerfall aufgerufen
         * @return Threadhandle für den ausfuehrenden Thread
         */
        std::thread get(const std::string& path, const std::unordered_map<std::string,std::string>& params,
                        ResultCb success_callback, ErrorCb error_callback);

        /**
         * fuehrt ein blockierendes POST Request durch
         * @param path der verwendete Pfad
         * @param data die versendeten Daten
         * @return antwort des Servers
         */
        Json::Value post(const std::string& path, const Json::Value& data);

        /**
         * fuehrt ein nicht blockierendes POST Request durch und ruft je nach Serveranwort callbacks auf
         * @param path der verwendete Pfad
         * @param data die versendeten Daten
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return Threadhandle für den ausfuehrenden Thread
         */
        std::thread post(const std::string& path, const Json::Value& data,
                         ResultCb result_callback, ErrorCb error_callback);

        /**
         * laedt die gegebene Datei zum Server hoch
         * @param url_path Pfad des Servers
         * @param file_path Pfad zur lokalen Datei
         * @param credentials die verwendeten Zugangsdaten
         * @return antwort des Servers
         */
        Json::Value post_file(const std::string& url_path, const std::string& file_path, const Json::Value &credentials);

        /**
         * laedt die gegebene Datei zum Server hoch
         * @param url_path Pfad des Servers
         * @param file_stream Imput-Stream der zu sendenden Datei
         * @param file_name Name der zu sendenden Datei
         * @param credentials die verwendeten Zugangsdaten
         * @return antwort des Servers
         */
        Json::Value post_file(const std::string &url_path, std::ifstream &file_stream, const std::string& file_name,
                              const Json::Value &credentials);

        /**
         * laedt die gegebene Datei zum Server hoch
         * @param url_path Pfad des Servers
         * @param file_path Pfad zur lokalen Datei
         * @param credentials die verwendeten Zugangsdaten
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return antwort des Servers
         */
        std::thread post_file(const std::string& url_path, const std::string& file_path, const Json::Value &credentials,
                              ResultCb result_callback, ErrorCb error_callback);

        Json::Value get_status();
        std::thread get_status(ResultCb result_callback, ErrorCb error_callback);

        Json::Value register_premium_user(const Profile& profile);
        std::thread register_premium_user(const Profile& profile, ResultCb result_callback, ErrorCb error_callback);

        /**
         * hasht die gegebenen Events
         * @param events Events die gehasht werden sollen
         * @return Hash der Events vom Server
         */
        Json::Value get_hash(const EventList& events);

        /**
         * hasht die gegebenen Events
         * @param events Events die gehasht werden sollen
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return Hash der Events vom Server
         */
        std::thread get_hash(const EventList& events, ResultCb result_callback, ErrorCb error_callback);

        /**
         * holt eine Raw-Transaction fuer die gegebenen Events vom Server
         * @param events Events fuer die die Transaktion erstellt werden soll
         * @return antwort des Servers
         */
        Json::Value get_raw_transaction(const EventList& events);

        /**
         * holt eine Raw-Transaction fuer die gegebenen Events vom Server
         * @param events Events fuer die die Transaktion erstellt werden soll
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return antwort des Servers
         */
        std::thread get_raw_transaction(const EventList& events, ResultCb result_callback, ErrorCb error_callback);

        /**
         * uebermittelt die signierte Transaktion und die Events an den Server zur weiteren Verarbeitung
         * @param events Events fuer die Transaktion
         * @param tx_sign_result Resultat von Utils.signTx(...)
         * @return antwort des Servers
         */
        Json::Value submit(const EventList& events, const std::string& tx_hex, const std::string& tx_hash);

        /**
         * uebermittelt die signierte Transaktion und die Events an den Server zur weiteren Verarbeitung
         * @param events Events fuer die Transaktion
         * @param tx_sign_result Resultat von Utils.signTx(...)
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return antwort des Servers
         */
        std::thread submit(const EventList& events, const std::string& tx_hex, const std::string& tx_hash,
                ResultCb result_callback, ErrorCb error_callback);

        /**
         * uebermittelt die Events an den Server zur weiteren Verarbeitung
         * @param events Events die verarbeitet werden sollen
         * @param from Adresse des Absenders
         * @param hash Hash der Daten (via getHash(...))
         * @param signature Signatur des Hashes (Utils.sign(hash, credentials))
         * @return antwort des Servers
         */
        Json::Value submit_premium(const EventList& events, const std::string& from,
                const std::string& hash, const Signature& signature);

        /**
         * uebermittelt die Events an den Server zur weiteren Verarbeitung
         * @param events Events die verarbeitet werden sollen
         * @param from Adresse des Absenders
         * @param hash Hash der Daten (via getHash(...))
         * @param signature Signatur des Hashes (Utils.sign(hash, credentials))
         * @param result_callback wird aufgerufen wenn kein Fehler auftritt
         * @param error_callback wird im Fehlerfall aufgerufen
         * @return antwort des Servers
         */
        std::thread submit_premium(const EventList& events, const std::string& from,
                                  const std::string& hash, const Signature& signature,
                                  ResultCb result_callback, ErrorCb error_callback);

    private:


        sproof::Config m_config;
        sproof::HttpClient m_http_client;

        std::string get_final_url(const std::string &path);

        Json::Value get_hash_data(const EventList& events,
                                  const Credentials& credentials);

        Json::Value get_raw_transaction_data(const EventList& events,
                                             const std::string& address);

        Json::Value get_submit_data(const EventList &events, const std::string& tx_hex, const std::string& tx_hash);

        Json::Value get_submit_premium_data(const EventList &events, const std::string &from,
                                            const std::string &hash, const Signature &signature);

        friend class Sproof;
        const sproof::Config& get_config() const;
    };


}


#endif //SPROOF_CLIENT_SPROOF_API_H
