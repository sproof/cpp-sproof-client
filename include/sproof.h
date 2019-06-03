
#ifndef _SPROOF_H
#define _SPROOF_H

#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <thread>
#include <functional>

#include <sproof-utils/credentials.h>

#include "model/events/sproof-event.h"
#include "model/events/document-receiver-add.h"
#include "model/events/document-receiver-revoke.h"
#include "model/events/document-revoke.h"
#include "model/events/document-register.h"
#include "model/events/profile-revoke.h"
#include "model/events/profile-register.h"
#include "model/events/profile-confirm.h"
#include "model/events/profile-update.h"
#include "model/profile.h"
#include "model/document.h"
#include "config.h"
#include "http/sproof-api.h"

namespace sproof {

    class Sproof {

    public:

        using EventList = std::vector<std::unique_ptr<SproofEvent>>;
        using ResultCb = std::function<void(Json::Value)>;
        using ErrorCb = std::function<void(HttpException)>;

        /**
        * erstellt ein neues Sproof Object und laedt die Konfiguration aus der gegebenen Datei
        * @param path_to_config Dateipfad der Konfiguration
        */
        explicit Sproof(const std::string &path_to_config);

        /**
        * erstellt ein neues Sproof Object mit der gegebenen Konfiguration
        * @param input_stream Imput-Stream aus einer Konfiguration
        */
        explicit Sproof(const std::ifstream &input_stream);

        /**
        * erstellt ein neues Sproof Object mit der gegebenen Konfiguration
        * @param config zu verwendende Konfigurationsdaten
        */
        explicit Sproof(const sproof::Config& config);

        /**
        * fuegt das gegebene event zur internen Event-Liste hinzu
        * @param event ein neues event
        */
        void add_event(std::unique_ptr<SproofEvent> event);

        /**
        * fuegt das gegebene 'PROFILE_REGISTER' event zur internen Event-Liste hinzu
        * @param profile ein neues Profile
        */
        void register_profile(const Profile& profile);

        /**
        * fuegt das gegebene 'PROFILE_REVOKE' event zur internen Event-Liste hinzu
        * @param reason der Grund für den revoke
        */
        void revoke_profile(const std::string& reason);

        /**
        * fuegt das gegebene 'PROFILE_CONFIRM' event zur internen Event-Liste hinzu
        * @param profile_id stellt den Wert "to" im event dar
        * @param value stellt ein Boolean im event dar
        */
        void confirm_profile(const std::string& profile_id, bool value);

        /**
        * fuegt das gegebene 'PROFILE_UPDATE' event zur internen Event-Liste hinzu
        * @param profile ein neues Profile
        */
        void update_profile(const Profile& profile);

        /**
        * fuegt das gegebene 'DOCUMENT_REGISTER' event zur internen Event-Liste hinzu
        * @param document das zu registrierende Dokument
        */
        void register_document(const Document& document);

        /**
        * fuegt das gegebene 'DOCUMENT_REVOKE' event zur internen Event-Liste hinzu
        * @param document_hash der Hash des Dukumentes
        * @param reason der Grund für den revoke
        */
        void revoke_document(const std::string& document_hash, const std::string& reason);

        /**
        * fuegt das gegebene 'DOCUMENT_RECEIVER_ADD' event zur internen Event-Liste hinzu
        * @param receiver_id die Addresse des Empfängers
        * @param document_hash der Hash des Dukumentes
        */
        void add_document_receiver(const std::string& receiver_id, const std::string& document_hash);

        /**
        * fuegt das gegebene 'DOCUMENT_RECEIVER_REVOKE' event zur internen Event-Liste hinzu
        * @param receiver_id die Addresse des Empfängers
        * @param reason der Grund für den revoke
        */
        void revoke_document_receiver(const std::string& receiver_id, const std::string& reason);

        /**
        * committet alle Events in der Liste
        * @param result_callback callback um das Ergebnis zu erhalten
        * @param error_callback callback um Fehler zu erhalten
        * @return
        */
        std::thread commit(std::function<void(Json::Value)> result_callback,
                std::function<void(HttpException)> error_callback);

        /**
        * committet alle Events in der Liste ueber die premium API
        * @param result_callback callback um das Ergebnis zu erhalten
        * @param error_callback callback um Fehler zu erhalten
        * @return
        */
        std::thread commit_premium( std::function<void(Json::Value)> result_callback,
                                    std::function<void(HttpException)> error_callback);

        /**
         * liefert die liste der aktuell ge-addeten events
         * @return liste der aktuell ge-addeten events
         */
        const std::vector<std::unique_ptr<SproofEvent>>& get_events();


        std::thread get_user(ResultCb result_callback, ErrorCb error_callback);

        std::thread get_state(ResultCb result_callback, ErrorCb error_callback);

        std::thread get_events(const std::unordered_map<std::string,std::string>& params, ResultCb result_callback,
                ErrorCb error_callback);

        std::thread get_transactions(const std::unordered_map<std::string,std::string>& params, ResultCb result_callback,
                       ErrorCb error_callback);

        std::thread get_profiles(const std::unordered_map<std::string,std::string>& params, ResultCb result_callback,
                        ErrorCb error_callback);

        std::thread get_receivers(const std::unordered_map<std::string,std::string>& params, ResultCb result_callback,
                                          ErrorCb error_callback);

        std::thread get_registrations(const std::unordered_map<std::string,std::string>& params, ResultCb result_callback,
                        ErrorCb error_callback);

        std::thread get_documents(const std::unordered_map<std::string,std::string>& params, ResultCb result_callback,
                        ErrorCb error_callback);

        std::thread get_validation(const std::string& id, ResultCb result_callback,
                        ErrorCb error_callback);

    private:
        std::unique_ptr<SproofApi> m_api;

        std::vector<std::unique_ptr<SproofEvent>> m_events;
    };

}

#endif