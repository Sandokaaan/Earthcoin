// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_MULTICONTROLDIALOG_H
#define EARTHCOIN_QT_MULTICONTROLDIALOG_H

#include <amount.h>

#include <QAbstractButton>
#include <QAction>
#include <QDialog>
#include <QList>
#include <QMenu>
#include <QPoint>
#include <QString>
#include <QTreeWidgetItem>
#include <univalue.h>
#include <amount.h>

class SendCoinsEntry;
class SendCoinsRecipient;
class PlatformStyle;
class CMutableTransaction;

namespace Ui {
    class MultiControlDialog;
}

#define ASYMP_UTF8 "\xE2\x89\x88"

class CMultiControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CMultiControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CMultiControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CMultiControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};


class MultiControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiControlDialog(const PlatformStyle *platformStyle, QWidget *parent = 0);
    ~MultiControlDialog();
    void updateLabels(QDialog*);

public Q_SLOTS:
    void clear();
    SendCoinsEntry *addEntry(bool showMessage = false);
    void on_createTransactionButton_clicked();
    void on_sendTransactionButton_clicked();
    void on_copyTransactionButton_clicked();

private:
    Ui::MultiControlDialog *ui;
    int sortColumn;
    Qt::SortOrder sortOrder;
    QMenu *contextMenu;
    QTreeWidgetItem *contextMenuItem;
    QAction *copyTransactionHashAction;
    const PlatformStyle *platformStyle;
    CAmount nAvaiableAmount;
    CAmount nPayAmount;
    CAmount nChange;
    CAmount nPayFee;
    bool freeze;
    bool hideZero;

    void sortView(int, Qt::SortOrder);
    void updateView();
    bool readMultisigLabels(UniValue & labels);
    bool readMultisigAddresses(UniValue & addresses, const UniValue & labels);
    bool readAddrDetails(std::vector< UniValue > & addrDetails, const UniValue & addresses);
    bool readMultisigTransactions(UniValue & transactions, const UniValue & addrList);
    bool sortTransactions(std::vector< UniValue > & sortedTransactions, const UniValue & transactions, const UniValue & addrRevDict);
    void showTransactions(const std::vector< UniValue > & addrDetails, const std::vector< UniValue > & sortedTransactions);
    bool buildAddrDict(UniValue & addrRevDict, std::vector< UniValue > & sortedTransactions, const std::vector< UniValue > & addrDetails);
    void disableOtherChecks();
    void sendButtonColor(const QString & color, const QString & text);
    bool setInputs(UniValue & inputs, UniValue & sourceAddresses);
    bool setOutputs(UniValue & outputs, QString & txMessage);
    bool setChange(UniValue & outputs, const UniValue & sourceAddresses);
    void showError(QString msg);
    bool buildRawTransaction(CMutableTransaction & rawTx, const UniValue & inputs, const UniValue & outputs, const QString & txMessage);
    bool signRawTransaction(std::string & rts);
    CAmount calcFee(unsigned int & nBytes);
    CAmount calcAmount(unsigned int & nQuantity);

    enum
    {
        COLUMN_CHECKBOX = 0,
        COLUMN_AMOUNT,
        COLUMN_LABEL,
        COLUMN_ADDRESS,
        COLUMN_TYPE,
        COLUMN_CONFIRMATIONS,
        COLUMN_TXHASH,
        COLUMN_VOUT_INDEX,
    };
    friend class CMultiControlWidgetItem;

private Q_SLOTS:
    void viewItemChanged(QTreeWidgetItem*, int);
    void headerSectionClicked(int);
    void removeEntry(SendCoinsEntry* entry);
    void useAvailableBalance(SendCoinsEntry* entry);
    void updateAmounts();
    void showMenu(const QPoint &);
    void copyAmount();
    void copyLabel();
    void copyAddress();
    void copyTransactionHash();
    void hideEmptyChecked(int);
    void addToOutputs(UniValue & outputs, const std::string & address, const CAmount & amount);
};

#endif // EARTHCOIN_QT_MULTICONTROLDIALOG_H
