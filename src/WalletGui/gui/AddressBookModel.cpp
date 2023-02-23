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

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "WalletAdapter.h"
#include "AddressBookModel.h"
#include "Settings.h"

namespace WalletGui {

AddressBookModel& AddressBookModel::instance() {
  static AddressBookModel inst;
  return inst;
}

AddressBookModel::AddressBookModel() : QAbstractItemModel() {
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &AddressBookModel::walletInitCompleted, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &AddressBookModel::reset, Qt::QueuedConnection);
}

AddressBookModel::~AddressBookModel() {
}

int AddressBookModel::columnCount(const QModelIndex& _parent) const {
  return 2;
}

QVariant AddressBookModel::data(const QModelIndex& _index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  QJsonObject address = m_addressBook.at(_index.row()).toObject();

  switch (_role) {
  case Qt::DisplayRole:
    switch (_index.column()) {
    case COLUMN_LABEL:
      return _index.data(ROLE_LABEL);
    case COLUMN_ADDRESS:
      return _index.data(ROLE_ADDRESS);
    default:
      return QVariant();
    }

  case ROLE_LABEL:
    return address.value("label");
  case ROLE_ADDRESS:
    return address.value("address");
  default:
    return QVariant();
  }

  return QVariant();
}

Qt::ItemFlags AddressBookModel::flags(const QModelIndex& _index) const {
  return (Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsSelectable);
}

QVariant AddressBookModel::headerData(int _section, Qt::Orientation _orientation, int _role) const {
  if (_orientation != Qt::Horizontal || _role != Qt::DisplayRole) {
    return QVariant();
  }

  switch (_section) {
  case COLUMN_LABEL:
    return tr("Label");
  case COLUMN_ADDRESS:
    return tr("Address");
  }

  return QVariant();
}

QModelIndex AddressBookModel::index(int _row, int _column, const QModelIndex& _parent) const {
  if (_parent.isValid()) {
    return QModelIndex();
  }

  return createIndex(_row, _column, _row);
}

QModelIndex AddressBookModel::parent(const QModelIndex& _index) const {
  return QModelIndex();
}

int AddressBookModel::rowCount(const QModelIndex& _parent) const {
  return m_addressBook.size();
}

void AddressBookModel::addAddress(const QString& _label, const QString& _address) {
  beginInsertRows(QModelIndex(), m_addressBook.size(), m_addressBook.size());
  QJsonObject newAddress;
  newAddress.insert("label", _label);
  newAddress.insert("address", _address);
  m_addressBook.append(newAddress);
  endInsertRows();
  saveAddressBook();
}

void AddressBookModel::removeAddress(int _row) {
  if (_row > m_addressBook.size() - 1) {
    return;
  }

  beginRemoveRows(QModelIndex(), _row, _row);
  m_addressBook.removeAt(_row);
  endRemoveRows();
  saveAddressBook();
}

void AddressBookModel::reset() {
  beginResetModel();
  while (!m_addressBook.empty()) {
    m_addressBook.removeFirst();
  }

  endResetModel();
}

void AddressBookModel::saveAddressBook() {
  QFile addressBookFile(Settings::instance().getAddressBookFile());
  if (addressBookFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QByteArray file_content = QJsonDocument(m_addressBook).toJson(QJsonDocument::Compact);
    addressBookFile.write(file_content);
    addressBookFile.close();
  }
}

void AddressBookModel::walletInitCompleted(int _error, const QString& _error_text) {
  if (!_error) {
    QFile addressBookFile(Settings::instance().getAddressBookFile());
    if (addressBookFile.open(QIODevice::ReadOnly)) {
      QByteArray file_content = addressBookFile.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(file_content);
      if (!doc.isNull()) {
        m_addressBook = doc.array();
      }

      addressBookFile.close();
      if (!m_addressBook.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, m_addressBook.size() - 1);
        endInsertRows();
      }
    }
  }
}

}
