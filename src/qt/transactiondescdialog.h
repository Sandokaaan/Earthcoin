// Copyright (c) 2011-2014 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_TRANSACTIONDESCDIALOG_H
#define EARTHCOIN_QT_TRANSACTIONDESCDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class TransactionDescDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Dialog showing transaction details. */
class TransactionDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDescDialog(const QModelIndex &idx, QWidget *parent = 0);
    ~TransactionDescDialog();

public Q_SLOTS:
    void on_openIPFSButton_clicked();

private:
    Ui::TransactionDescDialog *ui;
    QString IPFS;
};

#endif // EARTHCOIN_QT_TRANSACTIONDESCDIALOG_H
