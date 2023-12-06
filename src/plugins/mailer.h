/**
 *
 *  mailer.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>


namespace gaboot
{
    class mailer : public drogon::Plugin<mailer>
    {
    public:
        mailer() = default;
        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        void initAndStart(const Json::Value& config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        void shutdown() override;

        std::string send_email(
            const std::string
            & mailServer, // Mail server address/dns E.g. 127.0.0.1/smtp.mail.com
            const uint16_t& port,       // Port  E.g. 587
            const std::string& from,    // Send from whom E.g. drogon@gmail.com
            const std::string& to,      // Reciever       E.g. drogon@yahoo.com
            const std::string& subject, // The email title/subject
            const std::string& content, // The email content.
            const std::string& user,    // User      (Usually same as "from")
            const std::string& passwd,  // Password
            bool isHTML,                // content type
            const std::function<void(const std::string&)>& cb = {}
            // The callback for email sent notification
        );
    };
}

