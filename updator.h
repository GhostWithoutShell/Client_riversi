#ifndef UPDATOR_H
#define UPDATOR_H
#include<QTableWidget>
#include<QTableWidgetItem>
#include"mycolor.h"

using Player_colors = std::pair<QColor,QColor>;
using Tuple_params = std::tuple<int,int,int,int>;
class Updator
{
public:
    Updator();
    void operator()(QTableWidget *table,QTableWidgetItem* item,
                    const MyColor& color);
    QString get_winner(QTableWidget* table)const;
protected:
    void vertical_update(const Tuple_params &params,QTableWidget* table,
                         const MyColor& color);
    void horizontal_update(const Tuple_params &params,QTableWidget* table,
                           const MyColor& color);
    void main_diag_update(const Tuple_params &params,QTableWidget* table,
                          const MyColor& color);
    void add_diag_update(const Tuple_params &params,QTableWidget* table,
                         const MyColor& color);
    int calc_sum_of_not_green(QTableWidget* table)const;
};

#endif // UPDATOR_H
