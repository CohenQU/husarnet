// Copyright (c) 2017 Husarion Sp. z o.o.
// author: Michał Zieliński (zielmicha)
// Wrapper around OS sockets
#pragma once

#include "ipaddress.h"
#include <functional>
#include <vector>
#include <memory>
#include "string_view.h"
#include "threads_port.h"

using mutex_guard = std::lock_guard<std::recursive_mutex>;

namespace OsSocket {
struct FramedTcpConnection;

using PacketCallack = std::function<void(InetAddress, string_view)>;
using TcpDataCallback = std::function<void(const std::string&)>;
using TcpErrorCallback =
    std::function<void(std::shared_ptr<FramedTcpConnection>)>;

bool udpListenUnicast(int port, PacketCallack callback, bool setAsDefault=true);
void udpSend(InetAddress address, string_view data, int fd=-1);
bool udpListenMulticast(InetAddress address, PacketCallack callback);
void udpSendMulticast(InetAddress address, const std::string& data);
int bindUdpSocket(InetAddress addr, bool reuse);
void bindCustomFd(int fd, std::function<void()> readyCallback);
void bindCustomDgramFd(int fd, PacketCallack callback);
IpAddress resolveToIp(std::string hostname);
InetAddress ipFromSockaddr(struct sockaddr_storage st);
struct sockaddr_in6 sockaddrFromIp(InetAddress ip);
int connectTcpSocket(InetAddress addr);

bool write(std::shared_ptr<FramedTcpConnection> conn, const std::string& data, bool queue);
// Write a data packet.
//
// If the socket is not ready and queue is true, queue it anyway.
// Otherwise discard it.
//
// Returns true if the packet was sent.

void close(std::shared_ptr<FramedTcpConnection> conn);

std::shared_ptr<FramedTcpConnection> tcpConnect(InetAddress address,
                                                TcpDataCallback dataCallback,
                                                TcpErrorCallback errorCallback);

void runOnce(int timeout);
void init();

}
