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
#include <wallet/wallet.h>
#include <validation.h>

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
            std::string strKey = input.toStdString();
            CPubKey vchPubKey(ParseHex(strKey));
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
    ui->copyKeyButton->setEnabled(isValid());
}

void MultisigEntry::on_copyKeyButton_clicked()
{
    QApplication::clipboard()->setText(ui->key->text());
}

void MultisigEntry::on_ownKey_clicked()
{
    QString s;
    CWallet* const pwallet = (GetWallets().size()>0) ? 
                            (GetWallets()[0] ? GetWallets()[0].get() : nullptr) :
                            nullptr;
    if (!pwallet || pwallet->IsWalletFlagSet(WALLET_FLAG_DISABLE_PRIVATE_KEYS))
        s = "Error - wallet can not get a key";
    else
    {
        LOCK2(cs_main, pwallet->cs_wallet);
        std::string label = "MULTI_PRIV_KEYS";
        OutputType ot = pwallet->m_default_address_type;
        if (!pwallet->IsLocked())
            pwallet->TopUpKeyPool();
        CPubKey newKey;
        if (!pwallet->GetKeyFromPool(newKey))
            s =  "Error - keygen failed";
        else
        {
            pwallet->LearnRelatedScripts(newKey, ot);
            CTxDestination dest = GetDestinationForKey(newKey, ot);
            pwallet->SetAddressBook(dest, label, "keystore");
            std::string sk = HexStr(newKey.begin(), newKey.end());
            s = QString::fromStdString(sk);
        }
    }
    ui->key->setFocus();
    ui->key->setText(s);
    ui->key->clearFocus();
    if (isValid())
        disableChanges();
}

void MultisigEntry::disableChanges() 
{
    ui->key->setEnabled(false);
    ui->pasteButton->setEnabled(false);
    ui->ownKey->setEnabled(false);
    ui->copyKeyButton->setEnabled(true);
}

void MultisigEntry::setLabel(QString &label) 
{
    ui->labelKey->setText(label);
}
