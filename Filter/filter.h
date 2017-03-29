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

class FilterSettings {
public:
    virtual ~FilterSettings() {}
};

class Filter : public QObject
{
    Q_OBJECT
public:
    Filter();
    Filter(FilterSettings *);
    virtual QImage applyFilter(QImage, std::function<void(int)>) = 0;
    virtual QIcon getIcon() = 0;
    virtual QString getName() = 0;
    virtual FilterSettings *getSettings() const;
    virtual void setSettings(FilterSettings *);

public slots:
    void request();

protected:
    FilterSettings *settings;

signals:
    void requested(Filter *f);
};

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

class NegativeFilter : public Filter
{
public:
    NegativeFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

class BlurFilter : public Filter
{
public:
    BlurFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

class ScaleFilterSettings : public FilterSettings {
public:
    float scale_factor = 2;
};

class ScaleFilter : public Filter
{
public:
    ScaleFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

#endif // FILTER_H
