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

#include <rpc/util.h>
#include <script/script.h>
#include <outputtype.h>
#include <utilstrencodings.h>
#include <key_io.h>

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
        addEntry();
    ui->createButton->setEnabled(true);
}

void MultisigDialog::addEntry()
{
    MultisigEntry *entry = new MultisigEntry(this);
    ui->entries->addWidget(entry);
}

void MultisigDialog::createMultisig()
{
    QString s = "";
    std::vector<CPubKey> pubkeys;
    for (int i=0; i<nKeys; i++) 
    {
	MultisigEntry * entry = (MultisigEntry *)ui->entries->itemAt(i)->widget();
        QString xkey = entry->getValue();
        if (xkey.length()==0) 
        {
            entry->clear();
            return;
        }
        if (!(entry->isValid()))
        {
            ui->lineEditRedeemScript->setPlainText("");
            ui->lineEditAddress->setText("");
            return;
        }
        const char *c_str2 = xkey.toLatin1().data();
        if (!IsHex(c_str2))
            return;
        CPubKey vchPubKey(ParseHex(c_str2));
        if (!vchPubKey.IsFullyValid())
            return;
        pubkeys.push_back(HexToPubKey(c_str2));
    }
    const CScript inner = CreateMultisigRedeemscript(nRequired, pubkeys);
    std::string ss = HexStr(inner.begin(), inner.end());
    s = QString::fromStdString(ss);
    ui->lineEditRedeemScript->setPlainText(s);
    CBasicKeyStore keystore;
    const CTxDestination dest = AddAndGetDestinationForScript(keystore, inner, OutputType::LEGACY);
    std::string sa = EncodeDestination(dest);
    s = QString::fromStdString(sa);
    ui->lineEditAddress->setText(s);
}

void MultisigDialog::on_copyAddressButton_clicked() 
{
    QApplication::clipboard()->setText(ui->lineEditAddress->text());
}

void MultisigDialog::on_copyScriptButton_clicked() 
{
    QApplication::clipboard()->setText(ui->lineEditRedeemScript->toPlainText());
}
