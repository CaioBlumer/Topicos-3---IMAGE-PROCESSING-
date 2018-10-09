#include "flood.hpp"

Flood::Flood(Mat input, int thr)
{
  setSource(input, thr);
}

Flood::Flood()
{

}

void Flood::setSource(Mat input, int threshold)
{
  m_Source = input.clone();
  m_numberofRegions = 0;
  m_thr = threshold;
  setMask();
}

void Flood::setMask()
{
  m_Mask = Mat::zeros(m_Source.size(), CV_8UC1);
}

int Flood::getNextPosition(Point pt)
{
  // UP

  if(pt.y > 0)
  {
    Point up = Point(pt.x, pt.y-1);
    if(m_Mask.at<uchar>( up ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( up );
      if(checkDistance(current, next_p)){
        return 1;
      }
    }
  }

  //left_up

  if(( pt.y > 0) && (pt.x> 0))
  {
    Point left_up = Point(pt.x-1, pt.y-1);
    if(m_Mask.at<uchar>( left_up ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( left_up );
      if(checkDistance(current, next_p)){
        return 2;
      }
    }
  }

  // LEFT

  if(pt.x > 0)
  {
    Point left = Point(pt.x-1, pt.y);
    if(m_Mask.at<uchar>( left ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( left );
      if(checkDistance(current, next_p)){
        return 3;
      }
    }
  }

  // left down

  if(( pt.x > 0) && ( pt.y < m_Source.rows - 1 ))
  {
    Point left_down = Point(pt.x-1, pt.y+1);
    if(m_Mask.at<uchar>( left_down ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( left_down );
      if(checkDistance(current, next_p)){
        return 4;
      }
    }
  }


  // DOWN

  if(pt.y < m_Source.rows-1)
  {
    Point down = Point(pt.x, pt.y+1);
    if(m_Mask.at<uchar>( down ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( down );
      if(checkDistance(current, next_p)){
        return 5;
      }
    }
  }

  // down_right

  if((pt.y < m_Source.rows-1)&&(pt.x < m_Source.cols-1))
  {
    Point down_right = Point(pt.x+1, pt.y+1);
    if(m_Mask.at<uchar>( down_right ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( down_right );
      if(checkDistance(current, next_p)){
        return 6;
      }
    }
  }

  //right

  if(pt.x < m_Source.cols-1)
  {
    Point right = Point(pt.x+1, pt.y);
    if(m_Mask.at<uchar>( right ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( right );
      if(checkDistance(current, next_p)){
        return 7;
      }
    }
  }
  //right up

  if((pt.x < m_Source.cols-1)&&(pt.y>0))
  {
    Point right_up = Point(pt.x+1, pt.y);
    if(m_Mask.at<uchar>( right_up ) == 0)
    {
      Vec3b current = m_Source.at<Vec3b>(pt);
      Vec3b next_p  = m_Source.at<Vec3b>( right_up );
      if(checkDistance(current, next_p)){
        return 8;
      }
    }
  }

  return 0;
}

int Flood::ExtractPartition(Point ponto, unsigned int index)
{
  vector<Point> stack;
  stack.push_back(ponto);
  int indexToMark = index;
  int area = 1;
  m_numberofRegions++;
  m_Mask.at<uchar>(Point(ponto.x, ponto.y)) = indexToMark;

  while (stack.size() > 0) {
    int position = getNextPosition(ponto);

    switch (position)
    {
      case 1:
        m_Mask.at<uchar>(Point(ponto.x, ponto.y-1)) = indexToMark;
        area++;
        ponto.y--;
        stack.push_back(ponto);
        break;
      case 2:
        m_Mask.at<uchar>(Point(ponto.x-1, ponto.y-1)) = indexToMark;
        area++;
        ponto.y--;
        ponto.x--;
        stack.push_back(ponto);
        break;
      case 3:
        m_Mask.at<uchar>(Point(ponto.x-1, ponto.y)) = indexToMark;
        area++;
        ponto.x--;
        stack.push_back(ponto);
        break;
      case 4:
        m_Mask.at<uchar>(Point(ponto.x-1, ponto.y+1)) = indexToMark;
        area++;
        ponto.y++;
        ponto.x--;
        stack.push_back(ponto);
        break;
      case 5:
        m_Mask.at<uchar>(Point(ponto.x, ponto.y+1)) = indexToMark;
        area++;
        ponto.y++;
        stack.push_back(ponto);
        break;
      case 6:
        m_Mask.at<uchar>(Point(ponto.x+1, ponto.y+1)) = indexToMark;
        area++;
        ponto.y++;
        ponto.x++;
        stack.push_back(ponto);
        break;
      case 7:
        m_Mask.at<uchar>(Point(ponto.x+1, ponto.y)) = indexToMark;
        area++;
        ponto.x++;
        stack.push_back(ponto);
        break;
      case 8:
        m_Mask.at<uchar>(Point(ponto.x+1, ponto.y-1)) = indexToMark;
        area++;
        ponto.y--;
        ponto.x++;
        stack.push_back(ponto);
        break;
      case 0:
        ponto = stack.at(stack.size()-1);
        stack.pop_back();

    }
  }
  return area;
}

bool Flood::checkDistance(Vec3b current, Vec3b next)
{
  float DE = sqrt ( pow((current[0]-next[0]),2) +  pow((current[1]-next[1]),2) + pow((current[2]-next[2]),2) );
  if( DE <= m_thr)
  {
    return true;
  }else
   {
    return false;
   }
}

Mat Flood::GetSegmentedImage()
{
  Mat result = Mat::zeros(m_Source.size(), m_Source.type());
  vector<Vec3b> cores;

  for (size_t i = 0; i < m_numberofRegions+10; i++)
  {
    cores.push_back(Vec3b(rand()%256,rand()%256,rand()%256));
  }
  for (size_t i = 0; i < m_Mask.rows; i++)
  {
    for (size_t j = 0; j < m_Mask.cols; j++)
    {
      if (m_Mask.at<uchar>(Point(j,i)) > 0) {
        result.at<Vec3b>(Point(j,i)) = cores.at(m_Mask.at<uchar>(Point(j,i)));
      }else
       {
         result.at<Vec3b>(Point(j,i)) = Vec3b(0,0,255);
       }
    }
  }
  return result;
}

Mat Flood::process()
{
  int k = 0;
  for (size_t i = 0; i < m_Source.rows; i++)
  {
    for (size_t j = 0; j < m_Source.cols; j++)
    {
      if (checkMask(Point(j,i)))
      {
        int area = ExtractPartition(Point(j,i), ++k);
        cout << "area: " << area <<endl;
      }
    }
  }
  return GetSegmentedImage();
}

bool Flood::checkMask(Point pt)
{
  if(m_Mask.at<uchar>(pt) > 0) return false;
  else return true;
}
