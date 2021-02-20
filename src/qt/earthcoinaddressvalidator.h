// Copyright (c) 2011-2014 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EARTHCOIN_QT_EARTHCOINADDRESSVALIDATOR_H
#define EARTHCOIN_QT_EARTHCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class EarthcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit EarthcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Earthcoin address widget validator, checks for a valid earthcoin address.
 */
class EarthcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit EarthcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // EARTHCOIN_QT_EARTHCOINADDRESSVALIDATOR_H
