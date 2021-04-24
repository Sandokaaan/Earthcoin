// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_TRANSACTIONDESC_H
#define EARTHCOIN_QT_TRANSACTIONDESC_H

#include <QObject>
#include <QString>

class TransactionRecord;

namespace interfaces {
class Node;
class Wallet;
struct WalletTx;
struct WalletTxStatus;
}

/** Provide a human-readable extended HTML description of a transaction.
 */
class TransactionDesc: public QObject
{
    Q_OBJECT

public:
    TransactionDesc() {}
    ~TransactionDesc() {}
    QString toHTML(interfaces::Node& node, interfaces::Wallet& wallet, TransactionRecord *rec, int unit);
    const QString& getIPFS() const { return IPFS; }

private:

    QString IPFS;
    QString FormatTxStatus(const interfaces::WalletTx& wtx, const interfaces::WalletTxStatus& status, bool inMempool, int numBlocks, int64_t adjustedTime);
};

#endif // EARTHCOIN_QT_TRANSACTIONDESC_H
