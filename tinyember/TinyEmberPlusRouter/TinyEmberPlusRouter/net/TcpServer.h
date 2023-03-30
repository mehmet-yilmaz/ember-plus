/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __TINYEMBERROUTER_NET_TCPSERVER_H
#define __TINYEMBERROUTER_NET_TCPSERVER_H

#include <vector>
#include <QtNetwork/qtcpserver.h>
#include <QtCore/qmutex.h>
#include <QtCore/qthread.h>

namespace net
{
    class TcpClient;
    class TcpClientFactory;

    /**
     * Implementation of a tcp/ip server which listens to a specified port and
     * uses a factory to create clients for accepted connections.
     */
    class TcpServer : public QTcpServer
    {
        Q_OBJECT
        public:
            /**
             * Initializes a new TcpServer.
             * @param parent Pointer to the application object.
             * @param factory Pointer to the client factory which is used when a new connection
             *      has been accepted.
             * @param port The tcp/ip port to listen to.
             */
            TcpServer(QObject* parent, TcpClientFactory* factory, short port);

            /** Destructor */
            virtual ~TcpServer();

            /**
             * Sends the passed data to all currently connected clients.
             * @param array The array to transmit.
             */
            void write(QByteArray const& array);

            /**
             * Sends the buffer defined by the iterators to all connected clients.
             * @param first An iterator that points to the first item to copy.
             * @param last An iterator that points one past the last item to copy.
             */
            template<typename InputIterator>
            void write(InputIterator first, InputIterator last);

        private slots:
            /**
             * Handles an accepted connection.
             */
            void clientAccepted();

            /**
             * Handles a client's disconnect event.
             * @param client The client that signaled a disconnect.
             */
            void clientDisconnected(TcpClient* client);

        private:
            typedef std::vector<TcpClient*> ClientCollection;
            ClientCollection m_clients;
            TcpClientFactory *const m_factory;
            QMutex m_mutex;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline void TcpServer::write(InputIterator first, InputIterator last)
    {
        auto array = QByteArray();
        std::copy(first, last, std::back_inserter(array));
        write(array);
    }
}

#endif//__TINYEMBERROUTER_NET_TCPSERVER_H
