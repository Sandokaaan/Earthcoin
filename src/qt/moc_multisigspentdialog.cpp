/****************************************************************************
** Meta object code from reading C++ file 'multisigspentdialog.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qt/multisigspentdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multisigspentdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultisigSpentDialog_t {
    QByteArrayData data[42];
    char stringdata0[738];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultisigSpentDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultisigSpentDialog_t qt_meta_stringdata_MultisigSpentDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "MultisigSpentDialog"
QT_MOC_LITERAL(1, 20, 9), // "coinsSent"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 7), // "uint256"
QT_MOC_LITERAL(4, 39, 4), // "txid"
QT_MOC_LITERAL(5, 44, 7), // "message"
QT_MOC_LITERAL(6, 52, 5), // "title"
QT_MOC_LITERAL(7, 58, 5), // "style"
QT_MOC_LITERAL(8, 64, 5), // "clear"
QT_MOC_LITERAL(9, 70, 6), // "reject"
QT_MOC_LITERAL(10, 77, 6), // "accept"
QT_MOC_LITERAL(11, 84, 8), // "addEntry"
QT_MOC_LITERAL(12, 93, 15), // "SendCoinsEntry*"
QT_MOC_LITERAL(13, 109, 11), // "showMessage"
QT_MOC_LITERAL(14, 121, 19), // "updateTabsAndLabels"
QT_MOC_LITERAL(15, 141, 10), // "setBalance"
QT_MOC_LITERAL(16, 152, 26), // "interfaces::WalletBalances"
QT_MOC_LITERAL(17, 179, 8), // "balances"
QT_MOC_LITERAL(18, 188, 21), // "on_sendButton_clicked"
QT_MOC_LITERAL(19, 210, 26), // "on_buttonChooseFee_clicked"
QT_MOC_LITERAL(20, 237, 28), // "on_buttonMinimizeFee_clicked"
QT_MOC_LITERAL(21, 266, 22), // "on_closeButton_clicked"
QT_MOC_LITERAL(22, 289, 11), // "removeEntry"
QT_MOC_LITERAL(23, 301, 5), // "entry"
QT_MOC_LITERAL(24, 307, 19), // "useAvailableBalance"
QT_MOC_LITERAL(25, 327, 17), // "updateDisplayUnit"
QT_MOC_LITERAL(26, 345, 25), // "coinControlFeatureChanged"
QT_MOC_LITERAL(27, 371, 24), // "coinControlButtonClicked"
QT_MOC_LITERAL(28, 396, 24), // "coinControlChangeChecked"
QT_MOC_LITERAL(29, 421, 23), // "coinControlChangeEdited"
QT_MOC_LITERAL(30, 445, 23), // "coinControlUpdateLabels"
QT_MOC_LITERAL(31, 469, 28), // "coinControlClipboardQuantity"
QT_MOC_LITERAL(32, 498, 26), // "coinControlClipboardAmount"
QT_MOC_LITERAL(33, 525, 23), // "coinControlClipboardFee"
QT_MOC_LITERAL(34, 549, 28), // "coinControlClipboardAfterFee"
QT_MOC_LITERAL(35, 578, 25), // "coinControlClipboardBytes"
QT_MOC_LITERAL(36, 604, 29), // "coinControlClipboardLowOutput"
QT_MOC_LITERAL(37, 634, 26), // "coinControlClipboardChange"
QT_MOC_LITERAL(38, 661, 13), // "setMinimumFee"
QT_MOC_LITERAL(39, 675, 24), // "updateFeeSectionControls"
QT_MOC_LITERAL(40, 700, 17), // "updateMinFeeLabel"
QT_MOC_LITERAL(41, 718, 19) // "updateSmartFeeLabel"

    },
    "MultisigSpentDialog\0coinsSent\0\0uint256\0"
    "txid\0message\0title\0style\0clear\0reject\0"
    "accept\0addEntry\0SendCoinsEntry*\0"
    "showMessage\0updateTabsAndLabels\0"
    "setBalance\0interfaces::WalletBalances\0"
    "balances\0on_sendButton_clicked\0"
    "on_buttonChooseFee_clicked\0"
    "on_buttonMinimizeFee_clicked\0"
    "on_closeButton_clicked\0removeEntry\0"
    "entry\0useAvailableBalance\0updateDisplayUnit\0"
    "coinControlFeatureChanged\0"
    "coinControlButtonClicked\0"
    "coinControlChangeChecked\0"
    "coinControlChangeEdited\0coinControlUpdateLabels\0"
    "coinControlClipboardQuantity\0"
    "coinControlClipboardAmount\0"
    "coinControlClipboardFee\0"
    "coinControlClipboardAfterFee\0"
    "coinControlClipboardBytes\0"
    "coinControlClipboardLowOutput\0"
    "coinControlClipboardChange\0setMinimumFee\0"
    "updateFeeSectionControls\0updateMinFeeLabel\0"
    "updateSmartFeeLabel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultisigSpentDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  174,    2, 0x06 /* Public */,
       5,    3,  177,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  184,    2, 0x0a /* Public */,
       9,    0,  185,    2, 0x0a /* Public */,
      10,    0,  186,    2, 0x0a /* Public */,
      11,    1,  187,    2, 0x0a /* Public */,
      11,    0,  190,    2, 0x2a /* Public | MethodCloned */,
      14,    0,  191,    2, 0x0a /* Public */,
      15,    1,  192,    2, 0x0a /* Public */,
      18,    0,  195,    2, 0x08 /* Private */,
      19,    0,  196,    2, 0x08 /* Private */,
      20,    0,  197,    2, 0x08 /* Private */,
      21,    0,  198,    2, 0x08 /* Private */,
      22,    1,  199,    2, 0x08 /* Private */,
      24,    1,  202,    2, 0x08 /* Private */,
      25,    0,  205,    2, 0x08 /* Private */,
      26,    1,  206,    2, 0x08 /* Private */,
      27,    0,  209,    2, 0x08 /* Private */,
      28,    1,  210,    2, 0x08 /* Private */,
      29,    1,  213,    2, 0x08 /* Private */,
      30,    0,  216,    2, 0x08 /* Private */,
      31,    0,  217,    2, 0x08 /* Private */,
      32,    0,  218,    2, 0x08 /* Private */,
      33,    0,  219,    2, 0x08 /* Private */,
      34,    0,  220,    2, 0x08 /* Private */,
      35,    0,  221,    2, 0x08 /* Private */,
      36,    0,  222,    2, 0x08 /* Private */,
      37,    0,  223,    2, 0x08 /* Private */,
      38,    0,  224,    2, 0x08 /* Private */,
      39,    0,  225,    2, 0x08 /* Private */,
      40,    0,  226,    2, 0x08 /* Private */,
      41,    0,  227,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UInt,    6,    5,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 12, QMetaType::Bool,   13,
    0x80000000 | 12,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   23,
    QMetaType::Void, 0x80000000 | 12,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MultisigSpentDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultisigSpentDialog *_t = static_cast<MultisigSpentDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->coinsSent((*reinterpret_cast< const uint256(*)>(_a[1]))); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 2: _t->clear(); break;
        case 3: _t->reject(); break;
        case 4: _t->accept(); break;
        case 5: { SendCoinsEntry* _r = _t->addEntry((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< SendCoinsEntry**>(_a[0]) = _r; }  break;
        case 6: { SendCoinsEntry* _r = _t->addEntry();
            if (_a[0]) *reinterpret_cast< SendCoinsEntry**>(_a[0]) = _r; }  break;
        case 7: _t->updateTabsAndLabels(); break;
        case 8: _t->setBalance((*reinterpret_cast< const interfaces::WalletBalances(*)>(_a[1]))); break;
        case 9: _t->on_sendButton_clicked(); break;
        case 10: _t->on_buttonChooseFee_clicked(); break;
        case 11: _t->on_buttonMinimizeFee_clicked(); break;
        case 12: _t->on_closeButton_clicked(); break;
        case 13: _t->removeEntry((*reinterpret_cast< SendCoinsEntry*(*)>(_a[1]))); break;
        case 14: _t->useAvailableBalance((*reinterpret_cast< SendCoinsEntry*(*)>(_a[1]))); break;
        case 15: _t->updateDisplayUnit(); break;
        case 16: _t->coinControlFeatureChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->coinControlButtonClicked(); break;
        case 18: _t->coinControlChangeChecked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->coinControlChangeEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->coinControlUpdateLabels(); break;
        case 21: _t->coinControlClipboardQuantity(); break;
        case 22: _t->coinControlClipboardAmount(); break;
        case 23: _t->coinControlClipboardFee(); break;
        case 24: _t->coinControlClipboardAfterFee(); break;
        case 25: _t->coinControlClipboardBytes(); break;
        case 26: _t->coinControlClipboardLowOutput(); break;
        case 27: _t->coinControlClipboardChange(); break;
        case 28: _t->setMinimumFee(); break;
        case 29: _t->updateFeeSectionControls(); break;
        case 30: _t->updateMinFeeLabel(); break;
        case 31: _t->updateSmartFeeLabel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MultisigSpentDialog::*_t)(const uint256 & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultisigSpentDialog::coinsSent)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MultisigSpentDialog::*_t)(const QString & , const QString & , unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultisigSpentDialog::message)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MultisigSpentDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MultisigSpentDialog.data,
      qt_meta_data_MultisigSpentDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MultisigSpentDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultisigSpentDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MultisigSpentDialog.stringdata0))
        return static_cast<void*>(const_cast< MultisigSpentDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MultisigSpentDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void MultisigSpentDialog::coinsSent(const uint256 & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultisigSpentDialog::message(const QString & _t1, const QString & _t2, unsigned int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
