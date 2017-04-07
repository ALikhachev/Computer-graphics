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
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(double height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(double fMin READ fMin WRITE setFMin NOTIFY fMinChanged)
    Q_PROPERTY(double fMax READ fMax WRITE setFMax NOTIFY fMaxChanged)
    Q_PROPERTY(double fStep READ fStep WRITE setFStep NOTIFY fStepChanged)
    Q_PROPERTY(int horizontalCellCount READ horizontalCellCount WRITE setHorizontalCellCount NOTIFY horizontalCellCountChanged)
    Q_PROPERTY(int verticalCellCount READ verticalCellCount WRITE setVerticalCellCount NOTIFY verticalCellCountChanged)
    Q_PROPERTY(std::vector<QRgb> levels READ levels WRITE setLevels NOTIFY levelsChanged)
    Q_PROPERTY(QRgb isolinesColor READ isolinesColor WRITE setIsolinesColor NOTIFY isolinesColorChanged)
    Q_PROPERTY(bool interpolate READ interpolate WRITE setInterpolate NOTIFY interpolateChanged)
    Q_PROPERTY(bool showGrid READ showGrid WRITE setShowGrid NOTIFY showGridChanged)
    Q_PROPERTY(bool showEntries READ showEntries WRITE setShowEntries NOTIFY showEntriesChanged)
public:
    explicit Configuration(QObject *parent = 0);
    double startX() const;
    void setStartX(double);
    double startY() const;
    void setStartY(double);
    double width() const;
    void setWidth(double);
    double height() const;
    void setHeight(double);
    void setFMin(double);
    double fMin() const;
    void setFMax(double);
    double fMax() const;
    void setFStep(double);
    double fStep() const;
    int horizontalCellCount() const;
    void setHorizontalCellCount(int);
    int verticalCellCount() const;
    void setVerticalCellCount(int);
    std::vector<QRgb> levels() const;
    void setLevels(std::vector<QRgb> &);
    QRgb isolinesColor() const;
    void setIsolinesColor(QRgb);
    bool interpolate() const;
    void setInterpolate(bool);
    bool showGrid() const;
    void setShowGrid(bool);
    bool showIsolines() const;
    void setShowIsolines(bool);
    bool showEntries() const;
    void setShowEntries(bool);
    bool load(QTextStream &);
    void update();

signals:
    void startXChanged(double);
    void startYChanged(double);
    void widthChanged(double);
    void heightChanged(double);
    void fMinChanged(double);
    void fMaxChanged(double);
    void fStepChanged(double);
    void horizontalCellCountChanged(int);
    void verticalCellCountChanged(int);
    void levelsChanged(const std::vector<QRgb> &);
    void isolinesColorChanged(QRgb);
    void interpolateChanged(bool);
    void showGridChanged(bool);
    void showIsolinesChanged(bool);
    void showEntriesChanged(bool);
    void configurationUpdated();

private:
    double _start_x;
    double _start_y;
    double _width;
    double _height;
    double _f_min;
    double _f_max;
    double _f_step;
    int _horizontal_cell_count;
    int _vertical_cell_count;
    std::vector<QRgb> _levels;
    QRgb _isolines_color;
    bool _interpolate;
    bool _show_grid;
    bool _show_isolines;
    bool _show_cell_entries;
};

#endif // CONFIGURATION_H

