// Copyright (c) 2011-2018 The Earthcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/multicontroldialog.h>
#include <qt/forms/ui_multicontroldialog.h>

#include <qt/addresstablemodel.h>
#include <qt/earthcoinunits.h>
#include <qt/guiutil.h>
#include <qt/optionsmodel.h>
#include <qt/platformstyle.h>
#include <txmempool.h>
#include <qt/walletmodel.h>
#include <qt/sendcoinsentry.h>

#include <wallet/coincontrol.h>
#include <interfaces/node.h>
#include <key_io.h>
#include <core_io.h>
#include <policy/fees.h>
#include <policy/policy.h>
#include <validation.h> // For mempool
#include <wallet/fees.h>
#include <wallet/wallet.h>
#include <rpc/server.h>
#include <wallet/rpcwallet.h>
#include <rpc/rawtransaction.h>
#include <rpc/blockchain.h>

#include <QApplication>
#include <QCheckBox>
#include <QCursor>
#include <QDialogButtonBox>
#include <QFlags>
#include <QIcon>
#include <QSettings>
#include <QTreeWidget>
#include <QScrollBar>
#include <QClipboard>

const CAmount MINFEE = 1000;
const double SATOSHIVALUE = 1.0/COIN;

bool CMultiControlWidgetItem::operator<(const QTreeWidgetItem &other) const {
    int column = treeWidget()->sortColumn();
    if (column == MultiControlDialog::COLUMN_AMOUNT || column == MultiControlDialog::COLUMN_TYPE || column == MultiControlDialog::COLUMN_CONFIRMATIONS)
        return data(column, Qt::UserRole).toLongLong() < other.data(column, Qt::UserRole).toLongLong();
    return QTreeWidgetItem::operator<(other);
}

MultiControlDialog::MultiControlDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiControlDialog),
    platformStyle(_platformStyle),
    nAvaiableAmount(0),
    nPayAmount(0),
    nChange(0),
    nPayFee(0),
    freeze(false),
    hideZero(true)            // Sando: updated initialization order to fix a compiler warning
{
    ui->setupUi(this);

    // context menu actions
    QAction *copyAddressAction = new QAction(tr("Copy address"), this);
    QAction *copyLabelAction = new QAction(tr("Copy label"), this);
    QAction *copyAmountAction = new QAction(tr("Copy amount"), this);
    copyTransactionHashAction = new QAction(tr("Copy transaction ID"), this);  // we need to enable/disable this

    // context menu
    contextMenu = new QMenu(this);
    contextMenu->addAction(copyAddressAction);
    contextMenu->addAction(copyLabelAction);
    contextMenu->addAction(copyAmountAction);
    contextMenu->addAction(copyTransactionHashAction);

    // context menu signals
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
    connect(copyAddressAction, SIGNAL(triggered()), this, SLOT(copyAddress()));
    connect(copyLabelAction, SIGNAL(triggered()), this, SLOT(copyLabel()));
    connect(copyAmountAction, SIGNAL(triggered()), this, SLOT(copyAmount()));
    connect(copyTransactionHashAction, SIGNAL(triggered()), this, SLOT(copyTransactionHash()));

    // click on checkbox
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(viewItemChanged(QTreeWidgetItem*, int)));

    // click on header
    ui->treeWidget->header()->setSectionsClickable(true);
    connect(ui->treeWidget->header(), SIGNAL(sectionClicked(int)), this, SLOT(headerSectionClicked(int)));

    ui->treeWidget->setColumnWidth(COLUMN_CHECKBOX, 84);
    ui->treeWidget->setColumnWidth(COLUMN_AMOUNT, 110);
    ui->treeWidget->setColumnWidth(COLUMN_LABEL, 190);
    ui->treeWidget->setColumnWidth(COLUMN_ADDRESS, 320);
    ui->treeWidget->setColumnWidth(COLUMN_TYPE, 130);
    ui->treeWidget->setColumnWidth(COLUMN_CONFIRMATIONS, 110);
    ui->treeWidget->setColumnHidden(COLUMN_TXHASH, true);         // store transaction hash in this column, but don't show it
    ui->treeWidget->setColumnHidden(COLUMN_VOUT_INDEX, true);     // store vout index in this column, but don't show it

    // default view is sorted by amount desc
    sortView(COLUMN_AMOUNT, Qt::DescendingOrder);

    // restore list mode and sortorder as a convenience feature
    QSettings settings;
    if (settings.contains("nCoinControlSortColumn") && settings.contains("nCoinControlSortOrder"))
        sortView(settings.value("nCoinControlSortColumn").toInt(), (static_cast<Qt::SortOrder>(settings.value("nCoinControlSortOrder").toInt())));
    updateView();

    // Connect signals
    connect(ui->checkBoxChange, SIGNAL(stateChanged(int)), this, SLOT(changeChecked(int)));
    connect(ui->lineEditChange, SIGNAL(textEdited(const QString &)), this, SLOT(changeEdited(const QString &)));

    ui->addButton->setIcon(_platformStyle->SingleColorIcon(":/icons/add"));
    ui->clearButton->setIcon(_platformStyle->SingleColorIcon(":/icons/remove"));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->checkBoxHideEmpty, SIGNAL(stateChanged(int)), this, SLOT(hideEmptyChecked(int)));
    ui->copyTransactionButton->setEnabled(false);
    sendButtonColor("lightGray", "unsigned transaction");
}

MultiControlDialog::~MultiControlDialog()
{
    QSettings settings;
    settings.setValue("nCoinControlSortColumn", sortColumn);
    settings.setValue("nCoinControlSortOrder", (int)sortOrder);

    delete ui;
}

void MultiControlDialog::disableOtherChecks()
{
    ui->treeWidget->setEnabled(false);
    int donotDisable = -1;
    ui->lineEditChange->setText("");
    bool oneChecked = false;
    //  Qt::CheckState state = Qt::Checked;     // Sando: unused variable
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        if (ui->treeWidget->topLevelItem(i)->checkState(COLUMN_CHECKBOX) != Qt::Unchecked)
        {
            donotDisable = i;
            oneChecked = true;
            ui->lineEditChange->setText(ui->treeWidget->topLevelItem(i)->text(COLUMN_ADDRESS));
            break;
        }
    }    
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
    {
        CMultiControlWidgetItem *itemWalletAddress = (CMultiControlWidgetItem*)ui->treeWidget->topLevelItem(i);
        if (itemWalletAddress)
            itemWalletAddress->setDisabled((i!=donotDisable && oneChecked) || itemWalletAddress->childCount()<1);
    }
    ui->treeWidget->setEnabled(true);
    ui->copyTransactionButton->setEnabled(false);
    sendButtonColor("lightGray", "unsigned transaction");
}

// treeview: sort
void MultiControlDialog::sortView(int column, Qt::SortOrder order)
{
    sortColumn = column;
    sortOrder = order;
    ui->treeWidget->sortItems(column, order);
    ui->treeWidget->header()->setSortIndicator(sortColumn, sortOrder);
}

// treeview: clicked on header
void MultiControlDialog::headerSectionClicked(int logicalIndex)
{
    if (logicalIndex == COLUMN_CHECKBOX) // click on most left column -> do nothing
    {
        ui->treeWidget->header()->setSortIndicator(sortColumn, sortOrder);
    }
    else
    {
        if (sortColumn == logicalIndex)
            sortOrder = ((sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder);
        else
        {
            sortColumn = logicalIndex;
            sortOrder = ((sortColumn == COLUMN_LABEL || sortColumn == COLUMN_ADDRESS) ? Qt::AscendingOrder : Qt::DescendingOrder); // if label or address then default => asc, else default => desc
        }

        sortView(sortColumn, sortOrder);
    }
}

// checkbox clicked by user
void MultiControlDialog::viewItemChanged(QTreeWidgetItem* item, int column)
{
    bool noTx = true;
    if (column == COLUMN_CHECKBOX && item->text(COLUMN_TXHASH).length() == 64) // transaction hash is 64 characters (this means it is a child node, so it is not a parent node in tree mode)
    {
        noTx = false;
        if (item->isDisabled())
            item->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);
    }
    else if (column == COLUMN_CHECKBOX && item->childCount() > 0)
    {
        if (item->checkState(COLUMN_CHECKBOX) == Qt::PartiallyChecked && item->child(0)->checkState(COLUMN_CHECKBOX) == Qt::PartiallyChecked)
            item->setCheckState(COLUMN_CHECKBOX, Qt::Checked);
    }
    // selection changed -> update labels
    if (ui->treeWidget->isEnabled()) // do not update on every click for (un)select all
    {        
        if (noTx)
            disableOtherChecks();
        updateLabels(this);
    }
}

bool MultiControlDialog::readAddrDetails(std::vector< UniValue > & addrDetails, const UniValue & addresses)
{
    addrDetails.clear();
    try {
        if (!addresses.isArray())
            return false;
        for (const UniValue & address : addresses.getValues())
        {
            JSONRPCRequest request;
            request.params = UniValue(UniValue::VARR);
            request.params.push_back(address.get_str());
            UniValue rts = getaddressinfo(request);
            bool ismultisig = (rts.exists("script") && rts["script"].isStr() && rts["script"].get_str().compare("multisig")==0);
            int required = ismultisig ? rts["sigsrequired"].get_int() : 1;
            int keys = ismultisig ? rts["pubkeys"].get_array().size() : 1;
            QString req_key = QString::number(required) + "/" + QString::number(keys);
            rts.pushKV("multisigtype", req_key.toStdString());
            rts.pushKV("transactions", UniValue(UniValue::VARR));
            addrDetails.push_back(rts);
        }
        return true;
    } catch(...) {
        addrDetails.clear();
        return false;
    }
}

bool MultiControlDialog::readMultisigLabels(UniValue & labels)
{
    try {
        labels = UniValue(UniValue::VARR);
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        UniValue rts = listlabels(request);
        if (!rts.isArray())
            return false;
        for (const UniValue & rec : rts.getValues())
        {
            std::string label = rec.get_str();
            if (label.find("MULTISIG_") == 0)
                labels.push_back(label);
        }
        return true;
    } catch(...) {
        return false;
    }
}

bool MultiControlDialog::readMultisigAddresses(UniValue & addresses, const UniValue & labels)
{
    try {
        addresses = UniValue(UniValue::VARR);
        if (!labels.isArray())
            return false;
        for (const UniValue & label : labels.getValues())
        {
            JSONRPCRequest request;
            request.params = UniValue(UniValue::VARR);
            request.params.push_back(label.get_str());
            UniValue rts = getaddressesbylabel(request);
            const std::vector<std::string> & keys = rts.getKeys();
            for (const std::string & key : keys)
                addresses.push_back(key);
        }
        return true;
    } catch(...) {
        return false;
    }
}

bool MultiControlDialog::readMultisigTransactions(UniValue & transactions, const UniValue & addresses)
{
    try {
        UniValue scanobjects = UniValue(UniValue::VARR);
        if (!addresses.isArray())
            return false;
        for (const UniValue & address : addresses.getValues())
            scanobjects.push_back("addr(" + address.get_str() + ")");
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back("start");
        request.params.push_back(scanobjects);
        transactions = scantxoutset(request);
        return true;
    } catch(...) {
        transactions = UniValue(UniValue::VOBJ);
        return false;
    }
}

bool MultiControlDialog::buildAddrDict(UniValue & addrRevDict, std::vector< UniValue > & sortedTransactions, const std::vector< UniValue > & addrDetails)
{
    addrRevDict = UniValue(UniValue::VOBJ);
    sortedTransactions.clear();
    try {
        int nAddrs = addrDetails.size();
        for (int i=0; i<nAddrs; i++)
        {
            const UniValue & record = addrDetails[i];
            if (record.exists("scriptPubKey"))
                addrRevDict.pushKV(record["scriptPubKey"].get_str(), i);
            sortedTransactions.push_back(UniValue(UniValue::VARR));
        }
        return true;
    } catch(...) {
        addrRevDict = UniValue(UniValue::VOBJ);
        sortedTransactions.clear();
        return false;
    }
}

void MultiControlDialog::showTransactions(const std::vector< UniValue > & addrDetails, const std::vector< UniValue > & sortedTransactions)
{
    ui->lineEditChange->setText("");
    ui->treeWidget->clear();
    ui->treeWidget->setEnabled(false); // performance, otherwise updateLabels would be called for every checked checkbox
    ui->treeWidget->setAlternatingRowColors(false);
    QFlags<Qt::ItemFlag> flgCheckbox = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    QFlags<Qt::ItemFlag> flgTristate = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsTristate;
    LOCK(cs_main);
    int nHeight = chainActive.Height();
    try {
        int nAddr = addrDetails.size();
        for (int j=0; j<nAddr; j++)
        {
            const UniValue & txs = sortedTransactions[j];
            int nTxs = txs.size();
            if (nTxs || !hideZero)
            {
                const UniValue & record = addrDetails[j];
                std::string address = record["address"].get_str();
                std::string label = record["label"].get_str();
                std::string mtype = record["multisigtype"].get_str();
                CMultiControlWidgetItem *itemWalletAddress = new CMultiControlWidgetItem();
                itemWalletAddress->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);
                ui->treeWidget->addTopLevelItem(itemWalletAddress);
                itemWalletAddress->setFlags(flgTristate);
                itemWalletAddress->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);
                itemWalletAddress->setText(COLUMN_LABEL, QString::fromStdString(label));
                itemWalletAddress->setText(COLUMN_ADDRESS, QString::fromStdString(address));
                itemWalletAddress->setText(COLUMN_TYPE, QString::fromStdString(mtype));
                double sumBalance = 0.0;
                for (const UniValue & tx : txs.getValues())
                {
                    CMultiControlWidgetItem *itemOutput;
                    itemOutput = new CMultiControlWidgetItem(itemWalletAddress);
                    itemOutput->setFlags(flgCheckbox);
                    itemOutput->setCheckState(COLUMN_CHECKBOX,Qt::Unchecked);
                    itemOutput->setText(COLUMN_ADDRESS, QString::fromStdString(address));
                    itemOutput->setText(COLUMN_LABEL, QString::fromStdString(label));
                    double balance = tx["amount"].get_real();
                    sumBalance += balance;
                    itemOutput->setText(COLUMN_AMOUNT, QString::number(balance));
                    itemOutput->setText(COLUMN_CONFIRMATIONS, QString::number(nHeight - tx["height"].get_int()));
                    itemOutput->setText(COLUMN_TXHASH, QString::fromStdString(tx["txid"].get_str()));
                    itemOutput->setText(COLUMN_VOUT_INDEX, QString::number(tx["vout"].get_int()));
                }
                itemWalletAddress->setText(COLUMN_AMOUNT, QString::number(sumBalance));
                itemWalletAddress->setDisabled(sumBalance == 0.0);
                itemWalletAddress->setHidden(hideZero && sumBalance == 0.0);
            }
        }
    } catch(...) {
        return;
    }
    sortView(sortColumn, sortOrder);
    ui->treeWidget->setEnabled(true);
}

bool MultiControlDialog::sortTransactions(std::vector< UniValue > & sortedTransactions, const UniValue & transactions, const UniValue & addrRevDict)
{
    try {
        const UniValue & unspents = transactions["unspents"];
        if (!unspents.isArray() || unspents.size()<1)
            return false;
        for (const UniValue & tx : unspents.getValues())
        {
            std::string scriptPubKey = tx["scriptPubKey"].get_str();
            int index = addrRevDict[scriptPubKey].get_int();
            sortedTransactions[index].push_back(tx);
        }
        return true;
    } catch(...) {
        return false;
    }
}

void MultiControlDialog::updateView()
{
    UniValue labels, addresses, transactions, addrRevDict;
    std::vector< UniValue > addrDetails;
    std::vector< UniValue > sortedTransactions;
    ui->lineEditChange->setText("");
    if (    readMultisigLabels(labels)
         && readMultisigAddresses(addresses, labels)
         && readMultisigTransactions(transactions, addresses)
         && readAddrDetails(addrDetails, addresses)
         && buildAddrDict(addrRevDict, sortedTransactions, addrDetails)
         && sortTransactions(sortedTransactions, transactions, addrRevDict)
    )
    {
        showTransactions(addrDetails, sortedTransactions);
    }
    updateLabels(this);
    clear();
}

// context menu action: copy amount
void MultiControlDialog::copyAmount()
{
    GUIUtil::setClipboard(EarthcoinUnits::removeSpaces(contextMenuItem->text(COLUMN_AMOUNT)));
}

// context menu action: copy label
void MultiControlDialog::copyLabel()
{
    if (contextMenuItem->text(COLUMN_LABEL).length() == 0 && contextMenuItem->parent())
        GUIUtil::setClipboard(contextMenuItem->parent()->text(COLUMN_LABEL));
    else
        GUIUtil::setClipboard(contextMenuItem->text(COLUMN_LABEL));
}

// context menu action: copy address
void MultiControlDialog::copyAddress()
{
    if (contextMenuItem->text(COLUMN_ADDRESS).length() == 0 && contextMenuItem->parent())
        GUIUtil::setClipboard(contextMenuItem->parent()->text(COLUMN_ADDRESS));
    else
        GUIUtil::setClipboard(contextMenuItem->text(COLUMN_ADDRESS));
}

// context menu action: copy transaction id
void MultiControlDialog::copyTransactionHash()
{
    GUIUtil::setClipboard(contextMenuItem->text(COLUMN_TXHASH));
}

// context menu
void MultiControlDialog::showMenu(const QPoint &point)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(point);
    if(item)
    {
        contextMenuItem = item;
        // disable some items (like Copy Transaction ID, lock, unlock) for tree roots in context menu
        if (item->text(COLUMN_TXHASH).length() == 64) // transaction hash is 64 characters (this means it is a child node, so it is not a parent node in tree mode)
            copyTransactionHashAction->setEnabled(true);
        else // this means click on parent node in tree mode -> disable all
            copyTransactionHashAction->setEnabled(false);
        // show context menu
        contextMenu->exec(QCursor::pos());
    }
}

void MultiControlDialog::updateLabels(QDialog* dialog)
{
    ui->copyTransactionButton->setEnabled(false);
    sendButtonColor("lightGray", "unsigned transaction");
    nChange = 0;
    unsigned int nBytes = 0;
    // unsigned int nBytesInputs = 0;       // Sando: unused variable
    unsigned int nQuantity = 0;
    // bool fWitness = false;               // Sando: unused variable
    bool fDust = false;
    CAmount nAmount = calcAmount(nQuantity);
    nChange = 0;
    nPayFee = calcFee(nBytes);
    CAmount nAfterFee = nAmount - nPayFee;
    if (nAfterFee && nAfterFee < MINFEE)
    {
        nAfterFee = 0;
        fDust = true;
    }
    nAvaiableAmount = nAfterFee;
    nChange = nAfterFee - nPayAmount;
    if (nChange && nChange < MINFEE)
    {
        nChange = 0;
        fDust = true;
    }
    int nDisplayUnit = EarthcoinUnits::EAC;
    QLabel *l1 = dialog->findChild<QLabel *>("labelCoinControlQuantity");
    QLabel *l2 = dialog->findChild<QLabel *>("labelCoinControlAmount");
    QLabel *l3 = dialog->findChild<QLabel *>("labelCoinControlFee");
    QLabel *l4 = dialog->findChild<QLabel *>("labelCoinControlAfterFee");
    QLabel *l5 = dialog->findChild<QLabel *>("labelCoinControlBytes");
    QLabel *l7 = dialog->findChild<QLabel *>("labelCoinControlLowOutput");
    QLabel *l8 = dialog->findChild<QLabel *>("labelCoinControlChange");
    // stats
    l1->setText(QString::number(nQuantity));                                   // Quantity
    l2->setText(EarthcoinUnits::formatWithUnit(nDisplayUnit, nAmount));        // Amount
    l3->setText(EarthcoinUnits::formatWithUnit(nDisplayUnit, nPayFee));        // Fee
    l4->setText(EarthcoinUnits::formatWithUnit(nDisplayUnit, nAfterFee));      // After Fee
    l5->setText(((nBytes > 0) ? ASYMP_UTF8 : "") + QString::number(nBytes));   // Bytes
    l7->setText(fDust ? tr("yes") : tr("no"));                                 // Dust
    l8->setText(EarthcoinUnits::formatWithUnit(nDisplayUnit, nChange));        // Change
    if (nPayFee > 0)
    {
        l3->setText(ASYMP_UTF8 + l3->text());
        l4->setText(ASYMP_UTF8 + l4->text());
    }
    // turn label red when dust
    l7->setStyleSheet((fDust) ? "color:red;" : "");
    // tool tips
    QString toolTipDust = tr("This label turns red if any recipient receives an amount smaller than the current dust threshold.");
    // how many satoshis the estimated fee can vary per byte we guess wrong
    double dFeeVary = (nBytes != 0) ? (double)nPayFee / nBytes : 0;
    QString toolTip4 = tr("Can vary +/- %1 satoshi(s) per input.").arg(dFeeVary);
    l3->setToolTip(toolTip4);
    l4->setToolTip(toolTip4);
    l7->setToolTip(toolTipDust);
    l8->setToolTip(toolTip4);
    dialog->findChild<QLabel *>("labelCoinControlFeeText")      ->setToolTip(l3->toolTip());
    dialog->findChild<QLabel *>("labelCoinControlAfterFeeText") ->setToolTip(l4->toolTip());
    dialog->findChild<QLabel *>("labelCoinControlBytesText")    ->setToolTip(l5->toolTip());
    dialog->findChild<QLabel *>("labelCoinControlLowOutputText")->setToolTip(l7->toolTip());
    dialog->findChild<QLabel *>("labelCoinControlChangeText")   ->setToolTip(l8->toolTip());
    // Insufficient funds
    QLabel *label = dialog->findChild<QLabel *>("labelCoinControlInsuffFunds");
    if (label)
        label->setVisible(nChange < 0);
}

void MultiControlDialog::clear()
{
    // Remove entries until only one left
    while(ui->entries->count())
    {
        ui->entries->takeAt(0)->widget()->deleteLater();
    }
    addEntry();
    updateAmounts();
}


SendCoinsEntry *MultiControlDialog::addEntry(bool showMessage)
{
    ui->copyTransactionButton->setEnabled(false);
    sendButtonColor("lightGray", "unsigned transaction");
    SendCoinsEntry *entry = new SendCoinsEntry(platformStyle, this, true);
    entry->setModel(0);
    ui->entries->addWidget(entry);
    connect(entry, SIGNAL(removeEntry(SendCoinsEntry*)), this, SLOT(removeEntry(SendCoinsEntry*)));
    connect(entry, SIGNAL(useAvailableBalance(SendCoinsEntry*)), this, SLOT(useAvailableBalance(SendCoinsEntry*)));
    connect(entry, SIGNAL(payAmountChanged()), this, SLOT(updateAmounts()));

    // Focus the field, so that entry can start immediately
    entry->clear(showMessage || ui->entries->count() < 2);
    entry->setFocus();
    ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->sizeHint());
    qApp->processEvents();
    QScrollBar* bar = ui->scrollArea->verticalScrollBar();
    if(bar)
        bar->setSliderPosition(bar->maximum());
    return entry;
}

void MultiControlDialog::removeEntry(SendCoinsEntry* entry)
{
    entry->setAmount(0);
    entry->hide();
    // If the last entry is about to be removed add an empty one
    if (ui->entries->count() == 1)
        addEntry(true);
    else if (entry == qobject_cast<SendCoinsEntry*>(ui->entries->itemAt(0)->widget())) {
        SendCoinsEntry * nextEntry = qobject_cast<SendCoinsEntry*>(ui->entries->itemAt(1)->widget());
        nextEntry->showMessageEdit();
    }
    entry->deleteLater();
    updateLabels(this);
}

void MultiControlDialog::useAvailableBalance(SendCoinsEntry* entry)
{
    freeze = true;
    nPayAmount = 0;
    entry->setAmount(0);
    for (int i = 0; i < ui->entries->count(); ++i) {
        SendCoinsEntry* e = qobject_cast<SendCoinsEntry*>(ui->entries->itemAt(i)->widget());
        if (e && !e->isHidden()) {
            nPayAmount += e->getValue().amount;
        }
    }
    if (nAvaiableAmount > nPayAmount) {
        entry->setAmount(nAvaiableAmount - nPayAmount);
        nPayAmount = nAvaiableAmount;
    } else {
        entry->setAmount(0);
    }
    freeze = false;
    updateLabels(this);
}

void MultiControlDialog::updateAmounts()
{
    if (freeze)
        return;           // do not re-calc when freezed
    nPayAmount = 0;
    for (int i = 0; i < ui->entries->count(); ++i) {
        SendCoinsEntry* e = qobject_cast<SendCoinsEntry*>(ui->entries->itemAt(i)->widget());
        if (e && !e->isHidden()) {
            nPayAmount += e->getValue().amount;
        }
    }
    updateLabels(this);
}

void MultiControlDialog::hideEmptyChecked(int state)
{
    hideZero = (state == Qt::Checked);
    updateView();
}

// sub-methods for createTransactionButton

bool MultiControlDialog::setInputs(UniValue & inputs, UniValue & sourceAddresses)
{
    inputs = UniValue(UniValue::VARR);
    sourceAddresses = UniValue(UniValue::VARR);
    try {
        int nLab = ui->treeWidget->topLevelItemCount();
        for (int i=0; i<nLab; i++)
        {
            CMultiControlWidgetItem *itemWalletAddress = (CMultiControlWidgetItem*)ui->treeWidget->topLevelItem(i);
            int nChild = itemWalletAddress->childCount();
            for (int j=0; j<nChild; j++)
            {
                QTreeWidgetItem * itemOutput = (CMultiControlWidgetItem*)itemWalletAddress->child(j);
                if (itemOutput->checkState(COLUMN_CHECKBOX) == Qt::Checked)
                {
                    UniValue input = UniValue(UniValue::VOBJ);
                    std::string txid = itemOutput->text(COLUMN_TXHASH).toStdString();
                    uint64_t vout = itemOutput->text(COLUMN_VOUT_INDEX).toULongLong();
                    sourceAddresses.push_back(itemOutput->text(COLUMN_ADDRESS).toStdString());
                    input.pushKV("txid", txid);
                    input.pushKV("vout", vout);
                    inputs.push_back(input);
                }
            }
        }
        return true;
    } catch(...) {
        inputs = UniValue(UniValue::VARR);
        sourceAddresses = UniValue(UniValue::VARR);
        return false;
    }
}

bool MultiControlDialog::setOutputs(UniValue & outputs, QString & txMessage)
{
    outputs = UniValue(UniValue::VOBJ);
    txMessage = "";
    try {
        int nDest = ui->entries->count();
        for (int i = 0; i < nDest; ++i) {
            SendCoinsEntry* e = qobject_cast<SendCoinsEntry*>(ui->entries->itemAt(i)->widget());
            if (e && !e->isHidden()) {
                SendCoinsRecipient recipient = e->getValue();
                CAmount amount = recipient.amount;
                std::string address = recipient.address.toStdString();
                if (!IsValidDestinationString(address) || (amount==0))
                    throw std::invalid_argument( "invalid address or amount" );
                if (!txMessage.length()  && recipient.message.length())
                    txMessage = recipient.message;
                addToOutputs(outputs, address, amount);
            }
        }
        return true;
    } catch(...) {
        outputs = UniValue(UniValue::VOBJ);
        txMessage = "";
        return false;
    }
}

void MultiControlDialog::showError(QString msg)
{
    QMessageBox Msgbox;
    Msgbox.setText(msg);
    Msgbox.exec();
    sendButtonColor("red", "error");
}

void MultiControlDialog::addToOutputs(UniValue & outputs, const std::string & address, const CAmount & amount)
{
    double total = (outputs.exists(address)) ? outputs[address].get_real() : 0;
    total += SATOSHIVALUE * amount;
    // set amount precision to satoshies - baypass a double type bug
    QString tmpNum = QString::number(total, 'f', 8);
    UniValue tmpVal;
    tmpVal.setNumStr(tmpNum.toStdString());
    outputs.pushKV(address, tmpVal);
}


bool MultiControlDialog::setChange(UniValue & outputs, const UniValue & sourceAddresses)
{
    try {
        if (nChange > nPayFee)
        {
            std::string sourceAddress = sourceAddresses[0].get_str();
            addToOutputs(outputs, sourceAddress, nChange);
        }
        return true;
    } catch(...) {
        return false;
    }
}

bool MultiControlDialog::buildRawTransaction(CMutableTransaction & rawTx, const UniValue & inputs, const UniValue & outputs, const QString & txMessage)
{
    try {
        rawTx = ConstructTransaction(inputs, outputs, 0, false);
        if (txMessage.length())
        {
            rawTx.strTxComment = ValidateUnicodeString(txMessage.toStdString());
            rawTx.nVersion = (rawTx.strTxComment.length() > 0) ? 2 : 1;
        }
        return true;
    } catch(...) {
        return false;
    }
}

bool MultiControlDialog::signRawTransaction(std::string & rts)
{
    try {
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back(rts);
        UniValue result = signrawtransactionwithwallet(request);
        rts = result["hex"].get_str();
        if (result.exists("complete") && result["complete"].get_bool())
            sendButtonColor("green", "click to send");
        else
            sendButtonColor("orange", "need more signatures");
        return true;
    } catch(...) {
        return false;
    }
}

CAmount MultiControlDialog::calcAmount(unsigned int & nQuantity)
{
    CAmount nAmount = 0;
    nQuantity = 0;
    int nLab = ui->treeWidget->topLevelItemCount();
    for (int i=0; i<nLab; i++)
    {
        CMultiControlWidgetItem *itemWalletAddress = (CMultiControlWidgetItem*)ui->treeWidget->topLevelItem(i);
        int nChild = itemWalletAddress->childCount();
        for (int j=0; j<nChild; j++)
        {
            QTreeWidgetItem * itemOutput = (CMultiControlWidgetItem*)itemWalletAddress->child(j);
            if (itemOutput->checkState(COLUMN_CHECKBOX) == Qt::Checked)
            {
                QString s = itemOutput->text(COLUMN_AMOUNT);
                nAmount += std::round(s.toDouble() * COIN);
                nQuantity++;
            }
        }
    }
    return nAmount;
}

CAmount MultiControlDialog::calcFee(unsigned int & size)
{    
    UniValue inputs, outputs, sourceAddresses;
    QString txMessage;
    size = 0;
    CMutableTransaction rawTx;
    if (   setInputs(inputs, sourceAddresses)
        && ( setOutputs(outputs, txMessage) || setChange(outputs, sourceAddresses))
        && buildRawTransaction(rawTx, inputs, outputs, txMessage) )
    {
        std::string rts = EncodeHexTx(rawTx);
        size = rts.length()/2 + (1+outputs.size())*36 + 12 + txMessage.length();
        CAmount fee = size * 10;
        return (fee>MINFEE) ? fee : MINFEE;
    }
    else
        return MINFEE;
}

void MultiControlDialog::on_createTransactionButton_clicked()
{
    ui->copyTransactionButton->setEnabled(false);
    sendButtonColor("lightGray", "unsigned transaction");
    UniValue inputs, outputs, sourceAddresses;
    QString txMessage;
    if (!setInputs(inputs, sourceAddresses))
        return showError("Please select valid inputs.");
    if (!setOutputs(outputs, txMessage))
        return showError("Please enter valid recipients.");
    if (!setChange(outputs, sourceAddresses))
        return showError("Change output creation failed.");
    CMutableTransaction rawTx;
    if (!buildRawTransaction(rawTx, inputs, outputs, txMessage))
        return showError("Build transaction failed.");        
    std::string rts = EncodeHexTx(rawTx);
    if (ui->checkBoxSign->checkState() == Qt::Checked)
    {
        if (!signRawTransaction(rts))
            return showError("Sign transaction failed.");
    }
    ui->transactionTextEdit->setPlainText(QString::fromStdString(rts));
    ui->copyTransactionButton->setEnabled(true);
}

void MultiControlDialog::sendButtonColor(const QString & color, const QString & text)
{
    ui->sendTransactionButton->setStyleSheet(QString("QPushButton {background-color: ")+color+";}");
    ui->sendTransactionButton->setText(text);
    ui->sendTransactionButton->setEnabled(color == "green");
    ui->copyTransactionButton->setEnabled(color != "red");
}

void MultiControlDialog::on_sendTransactionButton_clicked()
{
    try {
        std::string rawTx = ui->transactionTextEdit->toPlainText().toStdString();
        JSONRPCRequest request;
        request.params = UniValue(UniValue::VARR);
        request.params.push_back(rawTx);
        UniValue rts = sendrawtransaction(request);
        ui->sendTransactionButton->setEnabled(false);
        QString answer = QString::fromStdString(rts.write());
        answer.replace(QString('"'), QString(""));
        ui->transactionTextEdit->setPlainText(answer);
    }
    catch(std::exception& e) {
        ui->transactionTextEdit->setPlainText(QString::fromStdString(e.what()));
    }
    catch(UniValue& e) {
        ui->transactionTextEdit->setPlainText(QString::fromStdString(e.write()));
    }
    catch(...) {
        ui->transactionTextEdit->setPlainText("Unknown error");
    }
}

void MultiControlDialog::on_copyTransactionButton_clicked()
{
    QApplication::clipboard()->setText(ui->transactionTextEdit->toPlainText());
}

///// odpad

