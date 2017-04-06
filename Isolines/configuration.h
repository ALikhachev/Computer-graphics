#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QRgb>
#include <QTextStream>

class Configuration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double startX READ startX WRITE setStartX NOTIFY startXChanged)
    Q_PROPERTY(double startY READ startY WRITE setStartY NOTIFY startYChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(double fMin READ fMin WRITE setFMin NOTIFY fMinChanged)
    Q_PROPERTY(double fMax READ fMax WRITE setFMax NOTIFY fMaxChanged)
    Q_PROPERTY(double fStep READ fStep WRITE setFStep NOTIFY fStepChanged)
    Q_PROPERTY(int cellWidth READ cellWidth WRITE setCellWidth NOTIFY cellWidthChanged)
    Q_PROPERTY(int cellHeight READ cellHeight WRITE setCellHeight NOTIFY cellHeightChanged)
    Q_PROPERTY(std::vector<QRgb> levels READ levels WRITE setLevels NOTIFY levelsChanged)
    Q_PROPERTY(QRgb isolinesColor READ isolinesColor WRITE setIsolinesColor NOTIFY isolinesColorChanged)
    Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate NOTIFY interpolateChanged)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
public:
    explicit Configuration(QObject *parent = 0);
    double startX() const;
    void setStartX(double);
    double startY() const;
    void setStartY(double);
    int width() const;
    void setWidth(int);
    int height() const;
    void setHeight(int);
    int cellWidth() const;
    void setFMin(double);
    double fMin() const;
    void setFMax(double);
    double fMax() const;
    void setFStep(double);
    double fStep() const;
    void setCellWidth(int);
    int cellHeight() const;
    void setCellHeight(int);
    std::vector<QRgb> levels() const;
    void setLevels(std::vector<QRgb> &);
    QRgb isolinesColor() const;
    void setIsolinesColor(QRgb);
    bool interpolate() const;
    void setInterpolate(bool);
    bool showGrid() const;
    void setShowGrid(bool);
    bool load(QTextStream &);

signals:
    void startXChanged(double);
    void startYChanged(double);
    void widthChanged(int);
    void heightChanged(int);
    void fMinChanged(double);
    void fMaxChanged(double);
    void fStepChanged(double);
    void cellWidthChanged(int);
    void cellHeightChanged(int);
    void levelsChanged(const std::vector<QRgb> &);
    void isolinesColorChanged(QRgb);
    void interpolateChanged(bool);
    void showGridChanged(bool);

private:
    double _start_x;
    double _start_y;
    int _width;
    int _height;
    double _f_min;
    double _f_max;
    double _f_step;
    int _cell_width;
    int _cell_height;
    std::vector<QRgb> _levels;
    QRgb _isolines_color;
    bool _interpolate;
    bool _show_grid;
};

#endif // CONFIGURATION_H

