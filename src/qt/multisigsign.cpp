// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/multisigsign.h>
#include <qt/forms/ui_multisigsign.h>

#include <qt/guiutil.h>

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QClipboard>
#include <QScrollBar>

#include <rpc/util.h>
#include <script/script.h>
#include <outputtype.h>
#include <utilstrencodings.h>
#include <key_io.h>
#include <core_io.h>
#include <pubkey.h>
#include <validation.h>
#include <rpc/rawtransaction.h>
#include <wallet/rpcwallet.h>
#include <primitives/transaction.h>
#include <wallet/wallet.h>

MultisigSign::MultisigSign(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultisigSign)
{
    ui->setupUi(this);
    connect(ui->signTransactionButton, SIGNAL(clicked()), this, SLOT(signTransaction()));
    connect(ui->sendTransactionButton, SIGNAL(clicked()), this, SLOT(sendTransaction()));
    connect(ui->inputsCB,SIGNAL(activated(int)),this,SLOT(showInputs(int)));
    connect(ui->outputsCB,SIGNAL(activated(int)),this,SLOT(showOutputs(int)));
    connect(ui->exprivkeyCB,SIGNAL(stateChanged(int)),this,SLOT(useExtPrivKey(int)));
    decodedTransaction = UniValue(UniValue::VOBJ);
    txinputs = UniValue(UniValue::VARR);
    txoutputs = UniValue(UniValue::VARR);
    lastSrcAddress = "";
    valueIn = 0.0;
    valueOut = 0.0;
    valueBack = 0.0;
    allInputsValid = false;
    ownPrivkey = false;
}

MultisigSign::~MultisigSign()
{
    delete ui;
}

void MultisigSign::on_pasteTransactionButton_clicked()
{
    ui->lineEditPrivateKey->clear();
    ui->rawTxLineEdit->clear();
    ui->lineEditSendStatus->clear();
    ui->rawTxLineEdit->setPlainText(QApplication::clipboard()->text());
    decodeTransaction();
}


void MultisigSign::on_redeemPasteBT_clicked()
{
    ui->redeemED->setPlainText(QApplication::clipboard()->text());
}


void MultisigSign::on_pastePrivateKeyButton_clicked()
{
    ui->lineEditPrivateKey->setText(QApplication::clipboard()->text());
    ui->signTransactionButton->setEnabled(extPrivkeyValid());
}

void MultisigSign::on_copyTransactionButton_clicked()
{
    QApplication::clipboard()->setText(ui->rawTxLineEdit->toPlainText());
}

void MultisigSign::on_copyTxidButton_clicked()
{
    QApplication::clipboard()->setText(ui->lineEditSendStatus->text());
}

void MultisigSign::showOutputs(int i)
{
    int n = ui->outputsCB->count();
    ui->xofnOutputLB->setText(QString::number(i+1)+QString("/")+QString::number(n));
}

void MultisigSign::showInputs(int i)
{
    int n = ui->inputsCB->count();
    ui->xofnInputLB->setText(QString::number(i+1)+QString("/")+QString::number(n));
}

UniValue MultisigSign::getSourceTransaction(const std::string & txid, int vout)
{
    UniValue rts =  UniValue(UniValue::VOBJ);
    try {
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back(txid);
        request.params.push_back(true);
        UniValue tmp = getrawtransaction(request);
        rts = tmp["vout"].get_array()[vout];
        rts.pushKV("confirmations",tmp["confirmations"].get_int());
        return rts;
    }
    catch(...) {
        return rts;
    }
}

void MultisigSign::decodeInputs()
{
    txinputs = UniValue(UniValue::VARR);
    valueIn = 0.0;
    allInputsValid = true;
    try {
        ui->inputsCB->clear();
        if (decodedTransaction.exists("vin") && decodedTransaction["vin"].isArray())
        {
             int nVin = decodedTransaction["vin"].size();
             ui->xofnInputLB->setText(QString("0/")+QString::number(nVin));
             for (int i=0; i<nVin; i++)
             {
                 const UniValue & output = decodedTransaction["vin"].get_array()[i];
                 QString txid = QString::fromStdString(output["txid"].get_str());
                 int vout = output["vout"].get_int();
                 UniValue srcTx = getSourceTransaction(txid.toStdString(), vout);
                 UniValue txi =  UniValue(UniValue::VOBJ);
                 txi.pushKV("output",output);
                 txi.pushKV("prevtx",srcTx);
                 txinputs.push_back(txi);
                 try {
                    const UniValue & scriptPubKey = srcTx["scriptPubKey"];
                    const UniValue & addresses = scriptPubKey["addresses"];
                    lastSrcAddress = QString::fromStdString(addresses[0].get_str());
                    QString txShortInfo = "✔ " + lastSrcAddress;
                    txShortInfo += (addresses.size()>1) ? ", ... → " : " → ";
                    double amount = srcTx["value"].get_real();
                    valueIn += amount;
                    txShortInfo += QString::number(amount) + " EAC";
                    txShortInfo += "  (⛏ + " + QString::number(srcTx["confirmations"].get_real()) + ")";
                    const UniValue & scriptSig = output["scriptSig"];
                    const UniValue & hex = scriptSig["hex"];
                    if (hex.get_str().length()>0)
                        txShortInfo += " 🔑";
                    ui->inputsCB->addItem(txShortInfo,i);
                 } catch(...) {
                    QString txShortInfo = "❌ Invalid input " + txid;
                    ui->inputsCB->addItem(txShortInfo,i);
                    allInputsValid = false;                    
                 }
             }
        }
    }
    catch(...) {
        ui->inputsCB->clear();
        txinputs = UniValue(UniValue::VARR);
        allInputsValid = false;
    }
    showInputs(0);
}

void MultisigSign::decodeOutputs()
{
    txoutputs = UniValue(UniValue::VARR);
    valueOut = 0.0;
    valueBack = 0.0;
    try {
        ui->outputsCB->clear();
        if (decodedTransaction.exists("vout") && decodedTransaction["vout"].isArray())
        {
            int nVout = decodedTransaction["vout"].size();
            ui->xofnOutputLB->setText(QString("0/")+QString::number(nVout));
            for (int i=0; i<nVout; i++)
            {
                const UniValue & output = decodedTransaction["vout"].get_array()[i];
                txoutputs.push_back(output);
                double amount = output["value"].get_real();
                QString txShortInfo = QString::number(amount) + " EAC";
                const UniValue & scriptPubKey = output["scriptPubKey"];
                const UniValue & addresses = scriptPubKey["addresses"];
                QString address = QString::fromStdString(addresses[0].get_str());
                txShortInfo += " → " + address;
                if (addresses.size()>1)
                    txShortInfo += ", ...";
                if (lastSrcAddress == address)
                {
                    txShortInfo += " ⏎";
                    valueBack += amount;
                }
                else
                    valueOut += amount;
                ui->outputsCB->addItem(txShortInfo,i);
            }
        }
    }
    catch(...) {
        ui->outputsCB->clear();
        txoutputs = UniValue(UniValue::VARR);
    }
    showOutputs(0);
}

bool MultisigSign::readSrcAddressInfo()
{
    ui->pubkeysCB->clear();
    ownPrivkey = false;
    try {
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back(lastSrcAddress.toStdString());
        UniValue rts = getaddressinfo(request);
        int sigsrequired = rts["sigsrequired"].get_int();
        UniValue pubkeys = rts["pubkeys"].get_array();
        std::string hex = rts["hex"].get_str();
        ui->redeemED->setPlainText(QString::fromStdString(hex));
        ui->redeemPasteBT->setEnabled(false);
        int nKeys = pubkeys.size();
        ui->allKeyED->setText(QString::number(nKeys));
        ui->reqKeyED->setText(QString::number(sigsrequired));
        CWallet* const pwallet = (GetWallets().size()>0) ?
                                (GetWallets()[0] ? GetWallets()[0].get() : nullptr) :
                                nullptr;
        for (int i=0; i<nKeys; i++) {
            std::string hexPubKey = pubkeys[i].get_str();
            QString keyInfo = "🔑 " + QString::number(i+1) + " - ";
            keyInfo += QString::fromStdString(hexPubKey);
            if (pwallet)
            {
                CPubKey vchPubKey(ParseHex(hexPubKey));
                if (vchPubKey.IsFullyValid() && pwallet->HaveKey(vchPubKey.GetID()))
                {
                    keyInfo += "    👁";
                    ownPrivkey = true;
                }
            }

            ui->pubkeysCB->addItem(keyInfo, i);
        }
        return true;
    }
    catch(...) {
        ui->redeemPasteBT->setEnabled(true);
        return false;
    }
}

void MultisigSign::useExtPrivKey(int i)
{
    ui->lineEditPrivateKey->setEnabled(allInputsValid && i);
    ui->pastePrivateKeyButton->setEnabled(allInputsValid && i);
    ui->signTransactionButton->setEnabled( allInputsValid && (!i && ownPrivkey) || (i && extPrivkeyValid()) );
    if (allInputsValid && ui->lineEditPrivateKey->text().isEmpty())
        ui->lineEditPrivateKey->setPlaceholderText(i ? "Enter private key here ..." : "");
}

void MultisigSign::decodeTransaction()
{
    try {
        std::string rawTx = ui->rawTxLineEdit->toPlainText().toStdString();
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back(rawTx);
        decodedTransaction = decoderawtransaction(request);        
    }
    catch(std::exception& e) {
        ui->lineEditSendStatus->setText("❌ " + QString::fromStdString(e.what()));
    }
    catch(UniValue& e) {
        ui->lineEditSendStatus->setText("❌ " + QString::fromStdString(e.write()));
    }
    catch(...) {
        ui->lineEditSendStatus->setText("❌ Unknown error");
    }
    decodeInputs();
    decodeOutputs();
    ui->totalED->setText(QString::number(valueIn, 'f', 8));
    ui->tosendED->setText(QString::number(valueOut, 'f', 8));
    ui->recbackED->setText(QString::number(valueBack, 'f', 8));
    double fee = valueIn - valueBack - valueOut;
    allInputsValid &= (fee>0);
    ui->feeED->setText(QString::number(fee, 'f', 8));
    ui->statusLB->setText(allInputsValid?"✔":"❌");
    ui->lineEditSendStatus->setText(allInputsValid ? "" : "❌ Invalid transaction");
    bool doNotAskRedeemScript = readSrcAddressInfo();
    ui->redeemPasteBT->setEnabled(!doNotAskRedeemScript && allInputsValid);
    ui->exprivkeyCB->setCheckState(ownPrivkey ? Qt::Unchecked : Qt::Checked);
    ui->exprivkeyCB->setEnabled(ownPrivkey && allInputsValid);
    ui->lineEditPrivateKey->setEnabled(!ownPrivkey && allInputsValid);
    ui->pastePrivateKeyButton->setEnabled(!ownPrivkey && allInputsValid);
    ui->signTransactionButton->setEnabled(ownPrivkey && allInputsValid);
    bool hasMsg = decodedTransaction.exists("txComment");
    ui->messageED->setEnabled(hasMsg);
    ui->messageLB->setEnabled(hasMsg);
    if (hasMsg)
        ui->messageED->setText(QString::fromStdString(decodedTransaction["txComment"].get_str()));
}

bool MultisigSign::extPrivkeyValid()
{
    std::string csKey = ui->lineEditPrivateKey->text().toStdString();
    privKey = DecodeSecret(csKey);
    return privKey.IsValid();
}

void MultisigSign::on_lineEditPrivateKey_textChanged(const QString &hexKey)
{
    privKey = DecodeSecret(hexKey.toStdString());
    ui->signTransactionButton->setEnabled(privKey.IsValid());
}

void MultisigSign::signWithKey()
{
    if (!extPrivkeyValid())
    {
        ui->lineEditSendStatus->setText("❌ Invalid private key");
        return;
    }
    std::string redeemScript = ui->redeemED->toPlainText().toStdString();
    if (!(IsHex(redeemScript)))
    {
        ui->lineEditSendStatus->setText("❌ Invalid redeem script");
        return;
    }
    CBasicKeyStore keystore;
    keystore.AddKey(privKey);
    CMutableTransaction mtx;
    std::string rawTx = ui->rawTxLineEdit->toPlainText().toStdString();
    if (!DecodeHexTx(mtx, rawTx, true, true))
    {
        ui->lineEditSendStatus->setText("❌ Invalid transaction");
        return;
    }
    try {
        UniValue prevTxUnival(UniValue::VARR);
        for (const UniValue & input : txinputs.getValues())
        {
            UniValue source(UniValue::VOBJ);
            source.pushKV("txid", input["output"]["txid"]);
            source.pushKV("vout", input["output"].get_obj()["vout"]);
            source.pushKV("scriptPubKey", input["prevtx"].get_obj()["scriptPubKey"].get_obj()["hex"]);
            source.pushKV("redeemScript", redeemScript);
            prevTxUnival.push_back(source);
        }
        UniValue rts = SignTransaction(mtx, prevTxUnival, &keystore, true, "ALL");
        showSignResult(rts);
    } catch(...) {
        ui->lineEditSendStatus->setText("❌ Invalid transaction inputs");
        return;
    }
}

void MultisigSign::showSignResult(const UniValue & rts)
{
    try {
        if (rts.exists("complete"))
        {
            bool complete = rts["complete"].get_bool();
            ui->lineEditSendStatus->setText(complete ? "✔ Sign complete - you can send the transaction" : "½ A partial sign - more signs required");
            if (rts.exists("errors") && rts["errors"][0].exists("error"))
            {
                std::string error = rts["errors"][0]["error"].get_str();
                if (error.find("CHECK(MULTI)SIG") == std::string::npos)
                {
                    ui->lineEditSendStatus->setText("❌ Sig error - bad key");
                    return;
                }
            }
            ui->rawTxLineEdit->setPlainText(QString::fromStdString(rts["hex"].get_str()));
            ui->sendTransactionButton->setEnabled(complete);
            ui->pastePrivateKeyButton->setEnabled(false);
            ui->exprivkeyCB->setEnabled(false);
            ui->lineEditPrivateKey->setEnabled(false);
            ui->signTransactionButton->setEnabled(false);
            ui->copyTransactionButton->setEnabled(true);
            ui->pasteTransactionButton->setEnabled(false);
        }
        else
        {
            ui->lineEditSendStatus->setText("❌ Sign error - unknown");
            return;
        }
    }
    catch(std::exception& e) {
        ui->rawTxLineEdit->setPlainText(QString::fromStdString(e.what()));
        return;
    }
}

void MultisigSign::signWithWallet()
{
    std::string rawTx = ui->rawTxLineEdit->toPlainText().toStdString();
    try {
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back(rawTx);
        UniValue rts = signrawtransactionwithwallet(request);
        showSignResult(rts);
    } catch(...) {
        ui->lineEditSendStatus->setText("❌ Invalid transaction inputs");
        return;
    }
}

void MultisigSign::signTransaction()
{
    ui->signTransactionButton->setEnabled(false);
    ui->sendTransactionButton->setEnabled(false);
    bool useExtKey = (ui->exprivkeyCB->checkState() == Qt::Checked);
    if (useExtKey)
        signWithKey();
    else
        signWithWallet();
}

void MultisigSign::sendTransaction()
{
    ui->copyTransactionButton->setEnabled(false);
    ui->sendTransactionButton->setEnabled(false);
    ui->copyTxidButton->setEnabled(true);
    CMutableTransaction mtx;
    std::string rawTx = ui->rawTxLineEdit->toPlainText().toStdString();
    if (!DecodeHexTx(mtx, rawTx, true, true)) 
    {
        ui->lineEditSendStatus->setText("❌ Invalid transaction");
    }
    else
    {
        try {
            JSONRPCRequest request;
            request.params = UniValue(UniValue::VARR);
            request.params.push_back(rawTx);
            UniValue rts = sendrawtransaction(request);
            QString answer = QString::fromStdString(rts.write());
            answer.replace(QString('"'), QString(""));
            ui->lineEditSendStatus->setText(answer);
        }
        catch(std::exception& e) {
            ui->lineEditSendStatus->setText("❌ " + QString::fromStdString(e.what()));
        }
        catch(UniValue& e) {
            ui->lineEditSendStatus->setText("❌ " + QString::fromStdString(e.write()));
        }
        catch(...) {
            ui->lineEditSendStatus->setText("❌ Unknown error");
        }
    }
}
