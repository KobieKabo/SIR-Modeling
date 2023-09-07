// Jakob Long
// Jrl4725

#include "infect_lib.h"

int main()
{
    Person joe;
    int day = 1;
    for(day; day < 10; day++){
        
        joe.update();
        double bad_luck = (double) rand() / (double) RAND_MAX;
        if (bad_luck> .95)
        {   
            joe.infect(5);            
        }
        std::cout << "On day " << day << ", Joe is "
                  << joe.status_string() << std::endl;
        if (joe.is_stable())
        {
        break;
        }
    }
    
    return 0;
}