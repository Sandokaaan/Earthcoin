// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/multisigentry.h>
#include <qt/forms/ui_multisigentry.h>

#include <qt/guiutil.h>

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QClipboard>

#include <utilstrencodings.h>
#include <pubkey.h>
#include <qt/walletmodel.h>

#include <keystore.h>
#include <key.h>

PubKeyValidator::PubKeyValidator(QObject * parrent)
  : QValidator(parrent) {}

PubKeyValidator::~PubKeyValidator() {}

QValidator::State PubKeyValidator::validate(QString &input, int &pos) const
{
    int n = input.length();
    if ( (n==0) || ( (n > 0) && (input.at(0) != '0')) )
        return QValidator::Invalid;
    if ( n > 1)
    {
        int maxLen = (input.at(1)=='4') ? 130 :
            (input.at(1)=='2' || input.at(1)=='3') ? 66 : 0;
        if (n > maxLen)
            return QValidator::Invalid;
        const QString allowedChars = "0123456789abcdefghABCDEFGH";
        for (int i=2; i<n; i++)
        {
            if (allowedChars.indexOf(input.at(i))<0)
                return QValidator::Invalid;
        }
        if (n == maxLen) 
        {
	    const char *c_str2 = input.toLatin1().data();
            CPubKey vchPubKey(ParseHex(c_str2));
            if (vchPubKey.IsFullyValid())
                return QValidator::Acceptable;
            else
                return QValidator::Invalid;
        }
    }
    return QValidator::Intermediate;
}

MultisigEntry::MultisigEntry(QWidget *parent) :
    QWidget(parent),
    model(0),
    ui(new Ui::MultisigEntry)
{
    ui->setupUi(this);
    ui->key->setValidator(new PubKeyValidator(this));
    ui->key->setCheckValidator(new PubKeyValidator(this));
}

MultisigEntry::~MultisigEntry()
{
    delete ui;
}

void MultisigEntry::clear()
{
    ui->key->clear();
}

QString MultisigEntry::getValue() const
{
    return ui->key->text();
}

bool MultisigEntry::isValid() const
{
    return ui->key->isValid();
}

void MultisigEntry::on_pasteButton_clicked()
{
    ui->key->setFocus();
    ui->key->setText(QApplication::clipboard()->text());
    ui->key->clearFocus();
}

void MultisigEntry::on_ownKey_clicked()
{
    CBasicKeyStore keystore;
    CKey xkey;
    xkey.MakeNewKey(true);
    keystore.AddKey(xkey);
    const CPubKey pkey = xkey.GetPubKey();
    std::string sk = HexStr(pkey.begin(), pkey.end());
    QString s = QString::fromStdString(sk);
    ui->key->setFocus();
    ui->key->setText(s);
    ui->key->clearFocus();
}

