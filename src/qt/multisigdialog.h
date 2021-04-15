// Copyright (c) 2011-2021 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_MULTISIGDIALOG_H
#define EARTHCOIN_QT_MULTISIGDIALOG_H

#include <QDialog>

namespace Ui {
    class MultisigDialog;
}

/** Dialog for creation multisig address.
 */
class MultisigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultisigDialog(QWidget *parent = 0);
    ~MultisigDialog();
    void addEntry();

public Q_SLOTS:
    void setKeysRequired();
    void createMultisig();
    void on_copyAddressButton_clicked();
    void on_copyScriptButton_clicked();

private:
    Ui::MultisigDialog *ui;
    int nKeys;
    int nRequired;
};

#endif // EARTHCOIN_QT_MULTISIGDIALOG_H
