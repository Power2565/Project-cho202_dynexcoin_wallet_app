// Copyright (c) 2021-2022, The TuringX Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2012-2016 The Cryptonote developers

#pragma once

#include <QCommandLineParser>
#include <QObject>

namespace WalletGui {

class CommandLineParser : public QObject {
  Q_OBJECT

public:
  CommandLineParser(QObject* _parent);
  ~CommandLineParser();

  bool process(const QStringList& _argv);

  bool hasHelpOption() const;
  bool hasVersionOption() const;
  bool hasTestnetOption() const;
  bool hasMinimizedOption() const;
  bool hasAllowLocalIpOption() const;
  bool hasHideMyPortOption() const;
  QString getErrorText() const;
  QString getHelpText() const;
  QString getP2pBindIp() const;
  quint16 getP2pBindPort() const;
  quint16 getP2pExternalPort() const;
  QStringList getPeers() const;
  QStringList getPiorityNodes() const;
  QStringList getExclusiveNodes() const;
  QStringList getSeedNodes() const;
  QString getDataDir() const;

private:
  QCommandLineParser m_parser;
  QCommandLineOption m_helpOption;
  QCommandLineOption m_versionOption;
  QCommandLineOption m_testnetOption;
  QCommandLineOption m_p2pBindIpOption;
  QCommandLineOption m_p2pBindPortOption;
  QCommandLineOption m_p2pExternalOption;
  QCommandLineOption m_allowLocalIpOption;
  QCommandLineOption m_addPeerOption;
  QCommandLineOption m_addPriorityNodeOption;
  QCommandLineOption m_addExclusiveNodeOption;
  QCommandLineOption m_seedNodeOption;
  QCommandLineOption m_hideMyPortOption;
  QCommandLineOption m_dataDirOption;
  QCommandLineOption m_minimized;
};

}
