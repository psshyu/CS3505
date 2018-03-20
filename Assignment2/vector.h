
/* Students that fail to comment their code will be unhappy, pointless students. */

class vector
{
  private:
    int x, y;
  
  public:
    vector();
    vector(const vector & other);
    vector(int x, int y);
   ~vector();

    vector & operator= (const vector & rhs);
  
    vector operator+ (const vector & rhs) const;

    

    void debug();
};
