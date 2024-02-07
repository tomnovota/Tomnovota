#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "tmatrix.h"
#include "genetic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent( QResizeEvent *event);
    void mousePressEvent( QMouseEvent *event);

private slots:
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_pushButton_clicked();
    void on_btnDalsi_clicked();
    void on_spinBox_valueChanged(int arg1);

    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;
    TMatrix *matrix;
    int pocetMest;
    int generace;
    TCrossoverMethod metodaKrizeni;
    QImage mapa;
    QPoint poziceMest[18];  // Ehm. Asi by to chtělo nějak lépe.
    int selectedMap;
    int mapW, mapH;
    int pocetKrokuNaKliknuti;

    void showState();
    void reset();
};
#endif // MAINWINDOW_H
