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
#include <primitives/transaction.h>

MultisigSign::MultisigSign(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultisigSign)
{
    ui->setupUi(this);
    connect(ui->decodeTransactionButton, SIGNAL(clicked()), this, SLOT(decodeTransaction()));
    connect(ui->signTransactionButton, SIGNAL(clicked()), this, SLOT(signTransaction()));
//    ui->createButton->setEnabled(false);
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


void MultisigSign::decodeTransaction()
{
    QString rawTx = ui->lineEditUnsignedTransaction->toPlainText();
    const char *csRawTx = rawTx.toLatin1().data();
    CMutableTransaction mtx;
    bool decodeOK = false;
    if (DecodeHexTx(mtx, csRawTx, true, true))
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
    
}
