#pragma once
#include <pch.h>

#include <drogon/HttpAppFramework.h>
#include <drogon/utils/Utilities.h>
#include <trantor/net/EventLoopThread.h>
#include <trantor/net/TcpClient.h>

using namespace trantor;

namespace gaboot
{
    struct Email 
    {
        enum states 
        {
            Init,
            HandShake,
            Tls,
            Auth,
            User,
            Pass,
            Mail,
            Rcpt,
            Data,
            Body,
            Quit,
            Close
        };
        std::string m_from;
        std::string m_to;
        std::string m_subject;
        std::string m_content;
        std::string m_user;
        std::string m_passwd;
        states m_status;
        std::string m_uuid;
        bool m_is_html{ false };
        std::shared_ptr<trantor::TcpClient> m_socket;

        Email(std::string from, std::string to, std::string subject,
            std::string content, std::string user, std::string passwd, bool isHTML,
            std::shared_ptr<trantor::TcpClient> socket)
            : m_from(std::move(from)), m_to(std::move(to)),
            m_subject(std::move(subject)), m_content(std::move(content)),
            m_user(std::move(user)), m_passwd(std::move(passwd)),
            m_socket(std::move(socket)), m_is_html(isHTML),
            m_uuid(drogon::utils::getUuid()) {
            m_status = Init;
        }

        ~Email() = default;

        static std::unordered_map<std::string, std::shared_ptr<Email>> m_emails; // Container for processing emails
    };

    std::unordered_map<std::string, std::shared_ptr<Email>> Email::m_emails;

    inline void messagesHandle(const trantor::TcpConnectionPtr& connPtr,
        trantor::MsgBuffer* msg,
        const std::shared_ptr<Email>& email,
        const std::function<void(const std::string& msg)>& cb) {
        std::string receivedMsg;
        while (msg->readableBytes() > 0) {
            std::string buf(msg->peek(), msg->readableBytes());
            receivedMsg.append(buf);
            //        LOG_INFO << buf;
            msg->retrieveAll();
        }
        LOG_TRACE << "receive: " << receivedMsg;
        std::string responseCode(receivedMsg.begin(), receivedMsg.begin() + 3);
        //    std::string responseMsg(receivedMsg.begin() + 4, receivedMsg.end());

        if (email->m_status == Email::Init && responseCode == "220") 
        {
            std::string outMsg;
            trantor::MsgBuffer out;

            outMsg.append("EHLO smtpclient.qw");
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::HandShake;
        }
        else if (email->m_status == Email::HandShake && responseCode == "220") 
        {
            std::string outMsg;
            trantor::MsgBuffer out;

            outMsg.append("EHLO smtpclient.qw");
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            auto policy = TLSPolicy::defaultClientPolicy();
            policy->setValidate(false);
            connPtr->startEncryption(policy, false,
                [out](const trantor::TcpConnectionPtr& connPtr) {
                    // LOG_TRACE << "SSL established";
                    connPtr->send(out);
                });

            email->m_status = Email::Auth;
        }
        else if (email->m_status == Email::HandShake && responseCode == "250") 
        {
            std::string outMsg;
            trantor::MsgBuffer out;

            outMsg.append("STARTTLS");
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::HandShake;
        }
        else if (email->m_status == Email::Auth && responseCode == "250") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            outMsg.append("AUTH LOGIN");
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::User;
        }
        else if (email->m_status == Email::User && responseCode == "334") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            std::string secret(email->m_user);

            // outMsg.append(base64_encode(reinterpret_cast<const unsigned
            // char*>(secret.c_str()), secret.length()));
            outMsg.append(drogon::utils::base64Encode(
                reinterpret_cast<const unsigned char*>(secret.c_str()),
                secret.length()));

            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Pass;
        }
        else if (email->m_status == Email::Pass && responseCode == "334") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            std::string secret(email->m_passwd);

            outMsg.append(drogon::utils::base64Encode(
                reinterpret_cast<const unsigned char*>(secret.c_str()),
                secret.length()));
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Mail;
        }
        else if (email->m_status == Email::Mail && responseCode == "235") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            outMsg.append("MAIL FROM:<");
            outMsg.append(email->m_from);
            outMsg.append(">\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Rcpt;
        }
        else if (email->m_status == Email::Rcpt && responseCode == "250") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            outMsg.append("RCPT TO:<");
            outMsg.append(email->m_to);
            outMsg.append(">\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Data;
        }
        else if (email->m_status == Email::Data && responseCode == "250") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            outMsg.append("DATA");
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Body;
        }
        else if (email->m_status == Email::Body && responseCode == "354") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            outMsg.append("To: " + email->m_to + "\r\n");
            outMsg.append("From: " + email->m_from + "\r\n");
            if (email->m_is_html) {
                outMsg.append("Content-Type: text/html;\r\n");
            }
            outMsg.append("Subject: " + email->m_subject + "\r\n\r\n");

            outMsg.append(email->m_content);
            outMsg.append("\r\n.\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Quit;
        }
        else if (email->m_status == Email::Quit && responseCode == "250") 
        {
            trantor::MsgBuffer out;
            std::string outMsg;

            outMsg.append("QUIT");
            outMsg.append("\r\n");

            out.append(outMsg.data(), outMsg.size());

            connPtr->send(std::move(out));

            email->m_status = Email::Close;
        }
        else if (email->m_status == Email::Close) 
        {
            /*Callback here for succeed delivery is probable*/
            cb("Email sent. ID : " + email->m_uuid);
            return;
        }
        else 
        {
            email->m_status = Email::Close;
            /*Callback here for notification is probable*/
            cb(receivedMsg);
        }
    }
}