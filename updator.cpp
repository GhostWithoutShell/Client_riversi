#include "updator.h"
#include<QDebug>
Updator::Updator()
{

}
void Updator::operator ()(QTableWidget* table,QTableWidgetItem* item,const MyColor &color)
{
    if(item->background().color() == Qt::green)
    {
        item->setBackground(QBrush(color));
        size_t cols = table->columnCount();
        size_t rows = table->rowCount();
        size_t col = item->column();
        size_t row = item->row();
        std::tuple<int,int,int,int> params = std::make_tuple(rows,cols,row,col);
        vertical_update(params,table,color);
        horizontal_update(params,table,color);
        main_diag_update(params,table,color);
        add_diag_update(params,table,color);
    }
}
void Updator::vertical_update(const Tuple_params &params,QTableWidget* table,
                              const MyColor &color)
{
    int pos_vert1 = std::get<2>(params)+1;
    int pos_vert2 = std::get<2>(params)-1;
    while(pos_vert1 < std::get<0>(params)
          && table->item(pos_vert1,std::get<3>(params))->background().color() == (!color))
    {
        ++pos_vert1;
    }
    while(pos_vert2 >=0 &&
          table->item(pos_vert2,std::get<3>(params))->background().color() == (!color))
    {
        --pos_vert2;
    }
    if(pos_vert1 < std::get<1>(params)
       && table->item(pos_vert1,
                      std::get<3>(params))->background().color() == color)
    {
        for(int i = std::get<2>(params); i<pos_vert1; ++i)
        {
            table->item(i,std::get<3>(params))->setBackground(QBrush(color));
        }
    }
    if(pos_vert2 >=0
            && table->item(pos_vert2,std::get<3>(params))->background().color() == color)
    {
        for(int i = std::get<2>(params); i>pos_vert2; --i)
        {
            table->item(i,std::get<3>(params))->setBackground(QBrush(color));
        }
    }
}
void Updator::horizontal_update(const Tuple_params &params,QTableWidget *table,
                                const MyColor &color)
{
    int pos_hor1 = std::get<3>(params)+1;
    int pos_hor2 = std::get<3>(params)-1;
    while(pos_hor1 < std::get<0>(params)
          && table->item(std::get<2>(params),pos_hor1)->background().color() == !(color))
    {
        ++pos_hor1;
    }
    while(pos_hor2 >=0 &&
          table->item(std::get<2>(params),pos_hor2)->background().color() == !(color))
    {
        --pos_hor2;
    }
    if(pos_hor1 < std::get<0>(params) && table->item(std::get<2>(params),pos_hor1)->background().color() == color)
    {
        for(int i = std::get<3>(params); i<pos_hor1; ++i)
        {
            table->item(std::get<2>(params),i)->setBackground(QBrush(color));
        }
    }
    if(pos_hor2 >= 0
       && table->item(std::get<2>(params),pos_hor2)->background().color() == color)
    {
        for(int i = std::get<3>(params); i>pos_hor2; --i)
        {
            table->item(std::get<2>(params),i)->setBackground(QBrush(color));
        }
    }
}
void Updator::add_diag_update(const Tuple_params &params,QTableWidget *table,
                              const MyColor &color)
{
    std::pair<int,int> pos_diag_pair = std::make_pair(std::get<2>(params)+1,
                                                      std::get<3>(params)+1);
    std::pair<int,int> pos_diag_pair2 = std::make_pair(std::get<2>(params)-1,
                                                       std::get<3>(params)-1);
    while(pos_diag_pair.first < std::get<0>(params) && pos_diag_pair.second < std::get<0>(params) &&
          table->item(pos_diag_pair.first,pos_diag_pair.second)->background().color() == !(color))
    {
        pos_diag_pair.first++;
        pos_diag_pair.second++;
    }
    while(pos_diag_pair2.first >= 0 && pos_diag_pair2.second >= 0 &&
          table->item(pos_diag_pair2.first,pos_diag_pair2.second)->background().color() == !(color))
    {
        pos_diag_pair2.first--;
        pos_diag_pair2.second--;
    }
    if(pos_diag_pair.first < std::get<0>(params) && pos_diag_pair.second < std::get<0>(params) &&
       table->item(pos_diag_pair.first,pos_diag_pair.second)->background().color() == color)
    {
        for(int i = std::get<2>(params), j = std::get<3>(params); i< pos_diag_pair.first,j<pos_diag_pair.second;++i,++j)
        {
            table->item(i,j)->setBackground(QBrush(color));
        }
    }
    if(pos_diag_pair2.first >= 0 && pos_diag_pair2.second >=0 &&
       table->item(pos_diag_pair2.first,pos_diag_pair2.second)->background().color() == color)
    {
        for(int i = std::get<2>(params),j = std::get<3>(params); i > pos_diag_pair2.first,j > pos_diag_pair2.second;--i,--j)
        {
            table->item(i,j)->setBackground(QBrush(color));
        }
    }
}
void Updator::main_diag_update(const Tuple_params &params,QTableWidget* table,
                               const MyColor &color)
{
    std::pair<int,int> pos_add_diag1 = std::make_pair(std::get<2>(params)-1,
                                                      std::get<3>(params)+1);
    std::pair<int,int> pos_add_diag2 = std::make_pair(std::get<2>(params)+1,
                                                      std::get<3>(params)-1);

    while(pos_add_diag1.first >= 0 && pos_add_diag1.second < std::get<1>(params) &&
          table->item(pos_add_diag1.first,pos_add_diag1.second)->background() == !(color))
    {
        pos_add_diag1.first--;
        pos_add_diag1.second++;
    }
    while(pos_add_diag2.first < std::get<0>(params) && pos_add_diag2.second >= 0 &&
          table->item(pos_add_diag2.first,pos_add_diag2.second)->background() == !(color))
    {
        pos_add_diag2.first++;
        pos_add_diag2.second--;
    }
    if(pos_add_diag1.first >= 0 && pos_add_diag1.second < std::get<0>(params) &&
       table->item(pos_add_diag1.first,pos_add_diag1.second)->background().color() == color)
    {
        for(int i = std::get<2>(params), j = std::get<3>(params); i >=0,j <pos_add_diag1.second;--i,++j)
        {
            table->item(i,j)->setBackground(QBrush(color));
        }
    }
    if(pos_add_diag2.first < std::get<0>(params) && pos_add_diag2.second >= 0 &&
       table->item(pos_add_diag2.first,pos_add_diag2.second)->background().color() == color)
    {
        for(int i = std::get<2>(params), j = std::get<3>(params); i <pos_add_diag2.first ,j > pos_add_diag2.second;++i,--j)
        {
            table->item(i,j)->setBackground(QBrush(color));
        }
    }
}
int Updator::calc_sum_of_not_green(QTableWidget* table)const
{
    int sum = 0;
    for(int i = 0;i<table->rowCount();++i)
    {
        for(int j = 0;j<table->columnCount();++j)
        {
            if(table->item(i,j)->background().color() != Qt::green)
            {
                ++sum;
            }
        }
    }
    return sum;
}
QString Updator::get_winner(QTableWidget* table)const
{
    int score_winner = 0;
    if(calc_sum_of_not_green(table) == 64)
    {
        int num_black = 0;
        int num_white = 0;
        for(int i = 0;i<table->rowCount();++i)
        {
            for(int j = 0;j<table->columnCount();++j)
            {
                if(table->item(i,j)->background().color() == Qt::black)
                    ++num_black;
                else if(table->item(i,j)->background().color() == Qt::white)
                    ++num_white;
            }
        }
        std::string str;
        if(num_black > num_white)
        {
            score_winner = num_black;
            str += "Победили черные!";
            str += "\n Со счетом :";
            str += QString::number(score_winner).toStdString();
        }
        else if( num_white > num_black )
        {
            score_winner = num_white;
            str += "Победили белые!";
            str += "\n Со счетом :";
            str += QString::number(score_winner).toStdString();
        }
        else
        {
            str = "Победила дружба!";
        }
        return QString(str.c_str());
    }
    else
    {
        return "";
    }
}
