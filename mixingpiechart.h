#ifndef MIXINGPIECHART_H
#define MIXINGPIECHART_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

///
/// \brief The MixingPieChart class generates the pie chart for the cauldron
///
class MixingPieChart : public QWidget {
    Q_OBJECT

private:
    QPieSeries* series; /// series of colors for generating the pie chart
    QChart* chart;      /// the chart to display
    QChartView* view;   /// the view for the char

public:
    explicit MixingPieChart(QWidget* parent = nullptr);

public slots:
    ///
    /// \brief updatePieSeries Update the pie chart with the new color
    /// \param colors the colors to update it to
    /// \param totalColors the number of colors
    ///
    void updatePieSeries(const QMap<QString, int>& colors, int totalColors);
};

#endif // MIXINGPIECHART_H
