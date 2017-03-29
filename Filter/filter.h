#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QIcon>
#include <QRunnable>
#include <functional>

class Filter;

class FilterWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    FilterWorker(Filter *filter, QImage image);
    void run() override;

signals:
    void imageReady(QImage);
    void progressChanged(int);

private:
    Filter *f;
    QImage image;
};

class Filter : public QObject
{
    Q_OBJECT
public:
    virtual ~Filter() {}
    virtual QImage applyFilter(QImage, std::function<void(int)>) = 0;
    virtual QIcon getIcon() = 0;
    virtual QString getName() = 0;

public slots:
    void request();

signals:
    void requested(Filter *f);
};

#endif // FILTER_H
