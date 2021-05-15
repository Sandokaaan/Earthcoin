// Copyright (c) 2011-2021 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_MULTISIGSIGN_H
#define EARTHCOIN_QT_MULTISIGSIGN_H

#include <QDialog>
#include <QString>
#include <univalue.h>
#include <key.h>

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
    void on_redeemPasteBT_clicked();
    void on_pastePrivateKeyButton_clicked();
    void signTransaction();
    void on_copyTransactionButton_clicked();
    void sendTransaction();
    void on_copyTxidButton_clicked();
    void showInputs(int i);
    void showOutputs(int i);
    void useExtPrivKey(int i);
    void on_lineEditPrivateKey_textChanged(const QString &hexKey);

private:
    Ui::MultisigSign *ui;
    UniValue decodedTransaction;
    UniValue txinputs;
    UniValue txoutputs;
    QString lastSrcAddress;
    double valueIn;
    double valueOut;
    double valueBack;
    bool allInputsValid;
    bool ownPrivkey;
    CKey privKey;
    void decodeInputs();
    void decodeOutputs();
    UniValue getSourceTransaction(const std::string & txid, int vout);
    bool readSrcAddressInfo();
    bool extPrivkeyValid();
    void signWithKey();
    void signWithWallet();
    void showSignResult(const UniValue & rts);
};

#endif // EARTHCOIN_Q_MULTISIGSIGN_H
