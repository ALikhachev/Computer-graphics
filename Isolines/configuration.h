#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QRgb>

class Configuration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(std::vector<QRgb> levels READ levels WRITE setLevels NOTIFY levelsChanged)
    Q_PROPERTY(QRgb isolinesColor READ isolinesColor WRITE setIsolinesColor NOTIFY isolinesColorChanged)
    Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate NOTIFY interpolateChanged)
public:
    explicit Configuration(QObject *parent = 0);
    int width() const;
    void setWidth(int);
    int height() const;
    void setHeight(int);
    std::vector<QRgb> levels() const;
    void setLevels(std::vector<QRgb> &);
    QRgb isolinesColor() const;
    void setIsolinesColor(QRgb);
    bool interpolate() const;
    void setInterpolate(bool);

signals:
    void widthChanged(int);
    void heightChanged(int);
    void levelsChanged(const std::vector<QRgb> &);
    void isolinesColorChanged(QRgb);
    void interpolateChanged(bool);

private:
    int _width;
    int _height;
    std::vector<QRgb> _levels;
    QRgb _isolines_color;
    bool _interpolate;
};

#endif // CONFIGURATION_H
