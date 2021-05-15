// Copyright (c) 2011-2021 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_MULTISIGENTRY_H
#define EARTHCOIN_QT_MULTISIGENTRY_H

#include <QDialog>
#include <QString>
#include <QValidator>

class WalletModel;

namespace Ui {
    class MultisigEntry;
}

/** Validator to check a pub-key validity
 */
class PubKeyValidator : public QValidator
{
public:
    PubKeyValidator(QObject * parrent);
    ~PubKeyValidator();
    QValidator::State validate(QString &input, int &pos) const;
};

/** Dialog for creation multisig address.
 */
class MultisigEntry : public QWidget
{
    Q_OBJECT

public:

    explicit MultisigEntry(QWidget *parent = 0);
    ~MultisigEntry();
    QString getValue() const;
    bool isValid() const;
    void setLabel(QString &label);
    void disableChanges();

public Q_SLOTS:
    void clear();
    void on_pasteButton_clicked();
    void on_copyKeyButton_clicked();
    void on_ownKey_clicked();

private:
    Ui::MultisigEntry *ui;
    WalletModel *model;
};

#endif // EARTHCOIN_QT_MULTISIGENTRY_H
