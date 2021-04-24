// Copyright (c) 2011-2021 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_MULTISIGSIGN_H
#define EARTHCOIN_QT_MULTISIGSIGN_H

#include <QDialog>

namespace Ui {
    class MultisigSign;
}

/** Dialog for creation multisig address.
 */
class MultisigSign : public QDialog
{
    Q_OBJECT

public:
    explicit MultisigSign(QWidget *parent = 0);
    ~MultisigSign();

public Q_SLOTS:
    void on_pasteTransactionButton_clicked();
    void decodeTransaction();
    void on_pasteRedeemScript_clicked();
    void on_pastePrivateKeyButton_clicked();
    void signTransaction();
    void on_copyTransactionButton_clicked();
    void sendTransaction();
    void on_copyTxidButton_clicked();

private:
    Ui::MultisigSign *ui;
};

#endif // EARTHCOIN_Q_MULTISIGSIGN_H
