#ifndef MYCOLOR_H
#define MYCOLOR_H
#include<QColor>
#include<QString>

class MyColor
{
public:
    MyColor() = default;
    MyColor(const QString &color_str)
    {
        if(color_str == "white")
        {
            color = Qt::white;
        }
        else if(color_str == "black")
        {
            color = Qt::black;
        }
    }
    void set_color(QString str)
    {
        color = str == "white"? Qt::white : Qt::black;
    }
    void set_color(QColor color_)
    {
        color = color_;

    }
    /*
    void set_color(std::string str)
    {
        color = (str == "white"? Qt::white : Qt::black);
    }
    */
    QColor operator !()const
    {
        if(color == Qt::white)
        {
            return Qt::black;
        }
        else if(color == Qt::black)
        {
            return Qt::white;
        }
    }
    MyColor operator !()
    {
        QString str = (color == Qt::white ? "black":"white");
        return MyColor(str);
    }
    operator QString()const
    {
        QString str = (color == Qt::white? "white" : "black");
        return str;
    }
    operator QColor()const
    {
        return color;
    }
    bool operator ==(QColor arg)
    {
        return arg == color;
    }
    bool operator ==(QString arg)
    {
        QString str = (color == Qt::white)? "white" : "black";
        return str == arg;
    }
private:
    QColor color;
};

#endif // MYCOLOR_H
