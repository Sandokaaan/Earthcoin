// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/transactiondescdialog.h>
#include <qt/forms/ui_transactiondescdialog.h>

#include <qt/transactiontablemodel.h>

#include <QModelIndex>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

TransactionDescDialog::TransactionDescDialog(const QModelIndex &idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDescDialog),
    ipfsUrlPrefix(NULL)
{
    ui->setupUi(this);
    setWindowTitle(tr("Details for %1").arg(idx.data(TransactionTableModel::TxHashRole).toString()));
    QString desc = idx.data(TransactionTableModel::LongDescriptionRole).toString();
    IPFS = idx.data(TransactionTableModel::IPFSRole).toString();
    ui->detailText->setHtml(desc);
    if (IPFS.length() < 1)
        ui->openIPFSButton->hide();
}

TransactionDescDialog::~TransactionDescDialog()
{
    delete ui;
}

void TransactionDescDialog::on_openIPFSButton_clicked()
{
    QString URL = (ipfsUrlPrefix ? (*ipfsUrlPrefix) : "https://dweb.link/ipfs/") + IPFS;
    QDesktopServices::openUrl(QUrl(URL));
}
