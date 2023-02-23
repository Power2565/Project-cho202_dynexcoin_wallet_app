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

#include <QApplication>
#include <QClipboard>

#include "CurrencyAdapter.h"
#include "AddressBookModel.h"
#include "AddressBookFrame.h"
#include "MainWindow.h"
#include "NewAddressDialog.h"
#include "WalletEvents.h"

#include "ui_addressbookframe.h"

namespace WalletGui {

AddressBookFrame::AddressBookFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::AddressBookFrame) {
  m_ui->setupUi(this);
  m_ui->m_addressBookView->setModel(&AddressBookModel::instance());

  connect(m_ui->m_addressBookView->selectionModel(), &QItemSelectionModel::currentChanged, this, &AddressBookFrame::currentAddressChanged);
}

AddressBookFrame::~AddressBookFrame() {
}

void AddressBookFrame::addClicked() {
  NewAddressDialog dlg(&MainWindow::instance());
  if (dlg.exec() == QDialog::Accepted) {
    QString label = dlg.getLabel();
    QString address = dlg.getAddress();
    if (!CurrencyAdapter::instance().validateAddress(address)) {
      QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Invalid address"), QtCriticalMsg));
      return;
    }

    AddressBookModel::instance().addAddress(label, address);
  }
}

void AddressBookFrame::copyClicked() {
  QApplication::clipboard()->setText(m_ui->m_addressBookView->currentIndex().data(AddressBookModel::ROLE_ADDRESS).toString());
}

void AddressBookFrame::deleteClicked() {
  int row = m_ui->m_addressBookView->currentIndex().row();
  AddressBookModel::instance().removeAddress(row);
}

void AddressBookFrame::currentAddressChanged(const QModelIndex& _index) {
  m_ui->m_copyAddressButton->setEnabled(_index.isValid());
  m_ui->m_deleteAddressButton->setEnabled(_index.isValid());
}

}
