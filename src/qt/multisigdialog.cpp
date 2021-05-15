// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/multisigdialog.h>
#include <qt/forms/ui_multisigdialog.h>
#include <qt/multisigentry.h>

#include <qt/guiutil.h>

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QClipboard>
#include <QScrollBar>

#include <random>
#include <chrono>
#include <rpc/util.h>
#include <script/script.h>
#include <outputtype.h>
#include <utilstrencodings.h>
#include <key_io.h>
#include <wallet/wallet.h>
#include <validation.h>


MultisigDialog::MultisigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultisigDialog)
{
    ui->setupUi(this);
    connect(ui->setButton, SIGNAL(clicked()), this, SLOT(setKeysRequired()));
    connect(ui->createButton, SIGNAL(clicked()), this, SLOT(createMultisig()));
    ui->createButton->setEnabled(false);
}

MultisigDialog::~MultisigDialog()
{
    delete ui;
}

void MultisigDialog::setKeysRequired()
{
    ui->setButton->setEnabled(false);
    nKeys = ui->spinBoxKeys->value();
    nRequired = ui->spinBoxRequired->value();
    if (nRequired > nKeys)
    {
        nRequired = nKeys;
        ui->spinBoxRequired->setValue(nRequired);
    }
    ui->spinBoxKeys->setEnabled(false);
    ui->spinBoxRequired->setEnabled(false);
    for (int i=0; i<nKeys; i++)
        addEntry(i);
    ui->createButton->setEnabled(true);
    QString sublabel = ui->lineEditLabel->text();
    if (sublabel.length() < 1 )
    {
         std::minstd_rand rd;
         rd.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
         uint32_t value = rd();
         sublabel = QString::number(value);
    }
    label = "MULTISIG_" + sublabel;
    ui->lineEditLabel->setText(label);
    ui->lineEditLabel->setEnabled(false);
}

void MultisigDialog::addEntry(int i)
{
    MultisigEntry *entry = new MultisigEntry(this);
    QString label = "Key ";
    label += (49+i);
    label += "     ";
    entry->setLabel(label);
    ui->entries->addWidget(entry);
    entry->clear();
}

void MultisigDialog::createMultisig()
{
    QString s = "";
    std::vector<CPubKey> pubkeys;
    bool allKeysValid = true;
    for (int i=0; i<nKeys; i++) 
    {
	MultisigEntry * entry = (MultisigEntry *)ui->entries->itemAt(i)->widget();
        QString xkey = entry->getValue();
        if (xkey.length()==0) 
        {
            entry->clear();
            allKeysValid = false;
            break;
        }
        if (!(entry->isValid()))
        {
            ui->lineEditRedeemScript->setPlainText("");
            ui->lineEditAddress->setText("");
            allKeysValid = false;
            break;
        }
        std::string strKey = xkey.toStdString();
        if (!IsHex(strKey))
        {
            allKeysValid = false;
            break;
        }
        CPubKey vchPubKey(ParseHex(strKey));
        if (!vchPubKey.IsFullyValid())
        {
            allKeysValid = false;
            break;
        }
        pubkeys.push_back(HexToPubKey(strKey));
    }
    if (!allKeysValid)
    {
        QMessageBox msgBox;
        msgBox.setText("Please, enter valid public keys");
        msgBox.exec();
        return;
    }
    for (int i=0; i<nKeys; i++) 
	((MultisigEntry *)ui->entries->itemAt(i)->widget())->disableChanges();
    ui->createButton->setEnabled(false);
    CBasicKeyStore keystore;
    CWallet* const pwallet = (GetWallets().size()>0) ?
                            (GetWallets()[0] ? GetWallets()[0].get() : nullptr) :
                            nullptr;
    if (pwallet)
        LOCK2(cs_main, pwallet->cs_wallet);
    OutputType ot = pwallet->m_default_address_type;
    const CScript inner = CreateMultisigRedeemscript(nRequired, pubkeys);
    std::string ss = HexStr(inner.begin(), inner.end());
    s = QString::fromStdString(ss);
    ui->lineEditRedeemScript->setPlainText(s);
    CTxDestination dest = AddAndGetDestinationForScript((pwallet?(*pwallet):keystore), inner, ot);
    if (pwallet)
        pwallet->SetAddressBook(dest, label.toStdString(), "multisig");
    std::string sa = EncodeDestination(dest);
    s = QString::fromStdString(sa);
    ui->lineEditAddress->setText(s);
    QString res = pwallet ? "Multisig address was stored into wallet as a watch-only address." :
                  "Multisig address was not stored into wallet. Please copy address and redeemScript manually.";
    QMessageBox msgBox;
    msgBox.setText(res);
    msgBox.exec();
}

void MultisigDialog::on_copyAddressButton_clicked() 
{
    QApplication::clipboard()->setText(ui->lineEditAddress->text());
}

void MultisigDialog::on_copyScriptButton_clicked() 
{
    QApplication::clipboard()->setText(ui->lineEditRedeemScript->toPlainText());
}

