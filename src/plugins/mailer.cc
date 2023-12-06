/**
 *
 *  mailer.cc
 *
 */

#include "mailer.h"
#include "interfaces/email.hpp"

using namespace drogon;

namespace gaboot
{
    void mailer::initAndStart(const Json::Value& config)
    {
        LOG(INFO) << "Plugin SMTP Mailer initialized.";
    }

    void mailer::shutdown()
    {
        LOG(INFO) << "Plugin SMTP Mailer uninitialized.";
    }

    std::string mailer::send_email(const std::string& mailServer, const uint16_t& port, const std::string& from, const std::string& to, const std::string& subject, const std::string& content, const std::string& user, const std::string& passwd, bool isHTML, const std::function<void(const std::string&)>& cb)
    {
        if (mailServer.empty() || from.empty() || to.empty() || subject.empty() ||
            user.empty() || passwd.empty()) 
        {
            LOG(WARNING) << "Invalid input(s) - "
                << "\nServer : " << mailServer << "\nPort : " << port
                << "\nfrom : " << from << "\nto : " << to
                << "\nsubject : " << subject << "\nuser : " << user
                << "\npasswd : " << passwd;
            return {};
        }

        static auto hasLineBreak = [](const std::string& msg) 
        {
            if (std::string::npos != msg.find_first_of("\n") ||
                std::string::npos != msg.find_first_of("\r")) 
            {
                return true;
            }
            return false;
        };

        if (hasLineBreak(from)) 
        {
            LOG(WARNING) << "Invalid \"FROM\" data : " << from;
            return {};
        }
        if (hasLineBreak(to)) 
        {
            LOG(WARNING) << "Invalid \"TO\" data : " << to;
            return {};
        }
        if (hasLineBreak(subject)) 
        {
            LOG(WARNING) << "Invalid \"SUBJECT\" data : " << subject.data();
            return {};
        }

        LOG(DEBUG) << "New TcpClient : " << mailServer << ":" << port;

        // Create the email
        auto email = std::make_shared<Email>(from, to, subject, content, user, passwd,
            isHTML, nullptr);

        auto resolver = app().getResolver();
        resolver->resolve(
            mailServer, [email, port, cb](const trantor::InetAddress& addr) {
                constexpr size_t defaultLoopIdA = 10;
                constexpr size_t defaultLoopIdB = 9;
                auto loopA = app().getIOLoop(defaultLoopIdA);
                auto loopB = app().getIOLoop(defaultLoopIdB);

                if (loopA == loopB) 
                {
                    LOG(WARNING) << "Please provide at least 2 threads for this plugin";
                    return;
                }

                auto loop = loopA->isInLoopThread() ? loopB : loopA;

                assert(loop);                    // Should never be null
                trantor::InetAddress addr_(addr.toIp(), port, false);
                auto tcpSocket = std::make_shared<trantor::TcpClient>(loop, addr_, "SMTPMail");

                email->m_socket = tcpSocket;

                std::weak_ptr<Email> email_wptr = email;

                Email::m_emails.emplace(email->m_uuid,
                    email); // Assuming there is no uuid collision
                tcpSocket->setConnectionCallback(
                    [email_wptr](const trantor::TcpConnectionPtr& connPtr) {
                        auto email_ptr = email_wptr.lock();
                        if (!email_ptr) 
                        {
                            LOG(WARNING) << "Email pointer gone";
                            return;
                        }
                        if (connPtr->connected()) 
                        {
                            // send request;
                            LOG(DEBUG) << "Connection established!";
                        }
                        else 
                        {
                            LOG(DEBUG) << "Connection disconnect";
                            Email::m_emails.erase(email_ptr->m_uuid); // Remove the email in list
                            // thisPtr->onError(std::string("ReqResult::NetworkFailure"));
                        }
                    });
                tcpSocket->setConnectionErrorCallback([email_wptr]() {
                    auto email_ptr = email_wptr.lock();
                    if (!email_ptr) 
                    {
                        LOG(WARNING) << "Email pointer gone";
                        return;
                    }
                    // can't connect to server
                    LOG(WARNING) << "Bad Server address";
                    Email::m_emails.erase(email_ptr->m_uuid); // Remove the email in list
                    // thisPtr->onError(std::string("ReqResult::BadServerAddress"));
                    });
                auto cb_(cb ? cb : [](const std::string& msg) {
                    LOG(INFO) << "Default email callback : " << msg;
                    });
                tcpSocket->setMessageCallback(
                    [email_wptr, cb_](const trantor::TcpConnectionPtr& connPtr,
                        trantor::MsgBuffer* msg) {
                            auto email_ptr = email_wptr.lock();
                            if (!email_ptr) 
                            {
                                LOG(WARNING) << "Email pointer gone";
                                return;
                            }
                            // email->m_socket->disconnect();
                            messagesHandle(connPtr, msg, email_ptr, cb_);
                    });
                tcpSocket->connect(); // Start trying to send the email
            });
        return email->m_uuid;
    }

}