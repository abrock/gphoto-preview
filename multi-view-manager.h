#ifndef MULTIVIEWMANAGER_H
#define MULTIVIEWMANAGER_H

#include <QObject>

#include "manager.h"

class MultiViewManager : public QObject {
    Q_OBJECT
    std::vector<std::shared_ptr<Manager> > cams;

    double delay_ms = 0;
public:
    MultiViewManager();

    Q_INVOKABLE void captureAll();

    Q_INVOKABLE void setDelay(QString const& str);

    virtual ~MultiViewManager();
};

#endif // MULTIVIEWMANAGER_H
