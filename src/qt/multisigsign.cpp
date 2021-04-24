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
#include <validation.h>
#include <rpc/rawtransaction.h>
#include <primitives/transaction.h>

MultisigSign::MultisigSign(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultisigSign)
{
    ui->setupUi(this);
    connect(ui->decodeTransactionButton, SIGNAL(clicked()), this, SLOT(decodeTransaction()));
    connect(ui->signTransactionButton, SIGNAL(clicked()), this, SLOT(signTransaction()));
    connect(ui->sendTransactionButton, SIGNAL(clicked()), this, SLOT(sendTransaction()));
}

MultisigSign::~MultisigSign()
{
    delete ui;
}

void MultisigSign::on_pasteTransactionButton_clicked()
{
    ui->lineEditUnsignedTransaction->setPlainText(QApplication::clipboard()->text());
}

void MultisigSign::on_pasteRedeemScript_clicked()
{
    ui->lineEditRedeemScript->setPlainText(QApplication::clipboard()->text());
}

void MultisigSign::on_pastePrivateKeyButton_clicked()
{
    ui->lineEditPrivateKey->setText(QApplication::clipboard()->text());
}

void MultisigSign::on_copyTransactionButton_clicked()
{
    QApplication::clipboard()->setText(ui->lineEditSignedTransaction->toPlainText());
}

void MultisigSign::on_copyTxidButton_clicked()
{
    QApplication::clipboard()->setText(ui->lineEditSendStatus->text());
}

void MultisigSign::decodeTransaction()
{
    std::string rawTx = ui->lineEditUnsignedTransaction->toPlainText().toStdString();
    CMutableTransaction mtx;
    bool decodeOK = false;
    if (DecodeHexTx(mtx, rawTx, true, true))
    {
        if (mtx.vin.size() == mtx.vout.size() == 1)
        {
            int prevIndex = mtx.vin[0].prevout.n;
            ui->lineEditSourceTxIndex->setText(QString::number(prevIndex));
            ui->lineEditSourceTXID->setText(QString::fromStdString(mtx.vin[0].prevout.hash.GetHex()));
            ui->lineEditAmount->setText(QString::number((1.0 * mtx.vout[0].nValue) /100000000 ));
            
            CTxDestination address;
            if (ExtractDestination(mtx.vout[0].scriptPubKey, address))
            {
                ui->lineEditReceivingAddress->setText(QString::fromStdString(EncodeDestination(address)));
                CTransactionRef prevTx;
                uint256 prevTxBlockHash;
                CBlockIndex *blockindex = nullptr;
                if (GetTransaction(mtx.vin[0].prevout.hash, prevTx, Params().GetConsensus(), prevTxBlockHash, true, blockindex))
                {
                    if (prevTx->vout.size() > prevIndex)
                    {
                        ui->lineEditScriptPubKey->setText(QString::fromStdString(HexStr(prevTx->vout[prevIndex].scriptPubKey.begin(),prevTx->vout[prevIndex].scriptPubKey.end())));
                        // disable/enable butons
                        ui->decodeTransactionButton->setEnabled(false);
                        ui->pasteTransactionButton->setEnabled(false);
                        ui->lineEditUnsignedTransaction->setEnabled(false);
                        ui->lineEditRedeemScript->setEnabled(true);
                        ui->lineEditPrivateKey->setEnabled(true);
                        ui->pasteRedeemScript->setEnabled(true);
                        ui->pastePrivateKeyButton->setEnabled(true);
                        ui->signTransactionButton->setEnabled(true);
                        decodeOK = true;
                    }
                }
            }
        }
    }
    ui->lineEditDecodeStatus->setText(decodeOK ? "OK" : "Failed");
}

void MultisigSign::signTransaction()
{
    QString signStatus = "OK";
    bool signOK = true, complete;
    std::string csKey = ui->lineEditPrivateKey->text().toStdString();
    CKey privKey = DecodeSecret(csKey);
    if (!(privKey.IsValid()))
    {
        signOK = false;
        signStatus = "Invalid private key";
    }
    if (signOK)
    {
        std::string redeemScript = ui->lineEditRedeemScript->toPlainText().toStdString();
        if (!(IsHex(redeemScript)))
        {
            signOK = false;
            signStatus = "Invalid redeem script";
        }
        else
        {
            CBasicKeyStore keystore;
            keystore.AddKey(privKey);
            CMutableTransaction mtx;
            std::string rawTx = ui->lineEditUnsignedTransaction->toPlainText().toStdString();
            if (!DecodeHexTx(mtx, rawTx, true, true)) 
            {
                signOK = false;
                signStatus = "Invalid transaction";
            }
            else
            {
                std::string txid = ui->lineEditSourceTXID->text().toStdString();
                std::string scriptPubKey = ui->lineEditScriptPubKey->text().toStdString();
                UniValue source(UniValue::VOBJ);
                source.pushKV("txid", txid);
                source.pushKV("vout", (int)(mtx.vin[0].prevout.n));
                source.pushKV("scriptPubKey", scriptPubKey);
                source.pushKV("redeemScript", redeemScript);
                UniValue prevTxUnival(UniValue::VARR); 
                prevTxUnival.push_back(source);
                UniValue rts;
                try {
                    rts = SignTransaction(mtx, prevTxUnival, &keystore, true, "ALL");
                }
                catch(std::exception& e) {
                    signOK = false;
                    ui->lineEditSignedTransaction->setPlainText(QString::fromStdString(e.what()));
                }
                if (signOK && rts.exists("complete"))
                {
                    complete = rts["complete"].get_bool();
                    signStatus = (complete ? "Complete" : "Partial - next key sign required");
                    if (rts.exists("errors") && rts["errors"][0].exists("error"))
                    {
                        std::string error = rts["errors"][0]["error"].get_str();
                        if (error.find("CHECK(MULTI)SIG") == std::string::npos) 
                        { 
                            signStatus = "Sig error - bad key";
                            signOK = false;
                        }
                    }
                    ui->lineEditSignedTransaction->setPlainText(QString::fromStdString(rts["hex"].get_str()));
                }
                else
                    signStatus = "Sign error";
            }
        }
    }
    ui->lineEditSignStatus->setText(signStatus);
    if (signOK) {
        ui->signTransactionButton->setEnabled(false);
        ui->lineEditRedeemScript->setEnabled(false);
        ui->lineEditPrivateKey->setEnabled(false);
        ui->pasteRedeemScript->setEnabled(false);
        ui->pastePrivateKeyButton->setEnabled(false);
        ui->copyTransactionButton->setEnabled(true);
        if (complete)
            ui->sendTransactionButton->setEnabled(true);
    }
}

void MultisigSign::sendTransaction()
{
    ui->copyTransactionButton->setEnabled(false);
    ui->sendTransactionButton->setEnabled(false);
    ui->copyTxidButton->setEnabled(true);
    CMutableTransaction mtx;
    std::string rawTx = ui->lineEditSignedTransaction->toPlainText().toStdString();
    if (!DecodeHexTx(mtx, rawTx, true, true)) 
    {
        ui->lineEditSendStatus->setText("Invalid transaction");
    }
    else
    {
        try {
            JSONRPCRequest request;
            request.params = UniValue(UniValue::VARR);
            request.params.push_back(rawTx);
            UniValue rts = sendrawtransaction(request);
            ui->lineEditSendStatus->setText(QString::fromStdString(rts.write()));
        }
        catch(std::exception& e) {
            ui->lineEditSendStatus->setText(QString::fromStdString(e.what()));
        }
        catch(UniValue& e) {
            ui->lineEditSendStatus->setText(QString::fromStdString(e.write()));
        }
        catch(...) {
            ui->lineEditSendStatus->setText("Unknown error");
        }
    }
}
